// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "miner.h"

#include "amount.h"
#include "chain.h"
#include "chainparams.h"
#include "coins.h"
#include "consensus/consensus.h"
#include "consensus/merkle.h"
#include "consensus/validation.h"
#include "hash.h"
#include "main.h"
#include "net.h"
#include "policy/policy.h"
#include "pow.h"
#include "primitives/transaction.h"
#include "script/standard.h"
#include "timedata.h"
#include "txmempool.h"
#include "util.h"
#include "utilmoneystr.h"
#include "validationinterface.h"
#ifdef ENABLE_WALLET
#include "base58.h"
#include "script/sign.h"
#include "wallet/wallet.h"
#endif

#include <algorithm>
#include <boost/thread.hpp>
#include <boost/tuple/tuple.hpp>
#include <queue>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//
// BitcoinMiner
//

//
// Unconfirmed transactions in the memory pool often depend on other
// transactions in the memory pool. When we select transactions from the
// pool, we select by highest priority or fee rate, so we might consider
// transactions that depend on transactions that aren't yet in the block.

uint64_t nLastBlockTx = 0;
uint64_t nLastBlockSize = 0;
uint64_t nLastBlockWeight = 0;

class ScoreCompare
{
public:
    ScoreCompare() {}

    bool operator()(const CTxMemPool::txiter a, const CTxMemPool::txiter b)
    {
        return CompareTxMemPoolEntryByScore()(*b,*a); // Convert to less than
    }
};

int64_t UpdateTime(CBlockHeader* pblock, const Consensus::Params& consensusParams, const CBlockIndex* pindexPrev)
{
    int64_t nOldTime = pblock->nTime;
    int64_t nNewTime = std::max(pindexPrev->GetMedianTimePast()+1, GetAdjustedTime());

    if (nOldTime < nNewTime)
        pblock->nTime = nNewTime;

    // Updating time can change work required on testnet:
    if (consensusParams.fPowAllowMinDifficultyBlocks)
        pblock->nBits = GetNextWorkRequired(pindexPrev, pblock, consensusParams);

    return nNewTime - nOldTime;
}

BlockAssembler::BlockAssembler(const CChainParams& _chainparams)
    : chainparams(_chainparams)
{
    // Block resource limits
    // If neither -blockmaxsize or -blockmaxweight is given, limit to DEFAULT_BLOCK_MAX_*
    // If only one is given, only restrict the specified resource.
    // If both are given, restrict both.
    nBlockMaxWeight = DEFAULT_BLOCK_MAX_WEIGHT;
    nBlockMaxSize = DEFAULT_BLOCK_MAX_SIZE;
    bool fWeightSet = false;
    if (mapArgs.count("-blockmaxweight")) {
        nBlockMaxWeight = GetArg("-blockmaxweight", DEFAULT_BLOCK_MAX_WEIGHT);
        nBlockMaxSize = MAX_BLOCK_SERIALIZED_SIZE;
        fWeightSet = true;
    }
    if (mapArgs.count("-blockmaxsize")) {
        nBlockMaxSize = GetArg("-blockmaxsize", DEFAULT_BLOCK_MAX_SIZE);
        if (!fWeightSet) {
            nBlockMaxWeight = nBlockMaxSize * WITNESS_SCALE_FACTOR;
        }
    }

    // Limit weight to between 4K and MAX_BLOCK_WEIGHT-4K for sanity:
    nBlockMaxWeight = std::max((unsigned int)4000, std::min((unsigned int)(MAX_BLOCK_WEIGHT-4000), nBlockMaxWeight));
    // Limit size to between 1K and MAX_BLOCK_SERIALIZED_SIZE-1K for sanity:
    nBlockMaxSize = std::max((unsigned int)1000, std::min((unsigned int)(MAX_BLOCK_SERIALIZED_SIZE-1000), nBlockMaxSize));

    // Whether we need to account for byte usage (in addition to weight usage)
    fNeedSizeAccounting = (nBlockMaxSize < MAX_BLOCK_SERIALIZED_SIZE-1000);

    nMedianTimePast = 0;
    block_final_state = NO_BLOCK_FINAL_TX;
}

void BlockAssembler::resetBlock()
{
    inBlock.clear();

    // Reserve space for coinbase tx
    nBlockSize = 1000;
    nBlockWeight = 4000;
    nBlockSigOpsCost = 400;
    fIncludeWitness = false;

    // These counters do not include coinbase tx
    nBlockTx = 0;
    nFees = 0;

    lastFewTxs = 0;
    blockFinished = false;

    nMedianTimePast = 0;
    block_final_state = NO_BLOCK_FINAL_TX;
}

CBlockTemplate* BlockAssembler::CreateNewBlock(const CScript& scriptPubKeyIn)
{
    resetBlock();

    pblocktemplate.reset(new CBlockTemplate());

    if(!pblocktemplate.get())
        return NULL;
    pblock = &pblocktemplate->block; // pointer for convenience

    // Add dummy coinbase tx as first transaction
    pblock->vtx.push_back(CTransaction());
    pblocktemplate->vTxFees.push_back(-1); // updated at end
    pblocktemplate->vTxSigOpsCost.push_back(-1); // updated at end

    LOCK2(cs_main, mempool.cs);
    CBlockIndex* pindexPrev = chainActive.Tip();
    nHeight = pindexPrev->nHeight + 1;

    pblock->nVersion = ComputeBlockVersion(pindexPrev, chainparams.GetConsensus());
    // -regtest only: allow overriding block.nVersion with
    // -blockversion=N to test forking scenarios
    if (chainparams.MineBlocksOnDemand())
        pblock->nVersion = GetArg("-blockversion", pblock->nVersion);

    pblock->nTime = GetAdjustedTime();
    nMedianTimePast = pindexPrev->GetMedianTimePast();

    nLockTimeCutoff = (STANDARD_LOCKTIME_VERIFY_FLAGS & LOCKTIME_MEDIAN_TIME_PAST)
                       ? nMedianTimePast
                       : pblock->GetBlockTime();


    // Check if block-final tx rules are enforced. For the moment this
    // tracks just whether the soft-fork is active, but by the time we get
    // to transaction selection it will only be true if there is a
    // block-final transaction in this block template.
    if (VersionBitsState(pindexPrev, chainparams.GetConsensus(), Consensus::DEPLOYMENT_FINALTX, versionbitscache) == THRESHOLD_ACTIVE) {
        block_final_state = HAS_BLOCK_FINAL_TX;
    }

    // Check if this is the first block for which the block-final rules are
    // enforced, in which case all we need to do is add the initial
    // anyone-can-spend output.
    if ((block_final_state & HAS_BLOCK_FINAL_TX) && pindexPrev->pprev && (VersionBitsState(pindexPrev->pprev, chainparams.GetConsensus(), Consensus::DEPLOYMENT_FINALTX, versionbitscache) != THRESHOLD_ACTIVE)) {
        block_final_state = INITIAL_BLOCK_FINAL_TXOUT;
    }

    // Otherwise we will need to check if the prior block-final transaction
    // was a coinbase and if insufficient blocks have occured for it to mature.
    const CCoins* prev_final = NULL;
    if (block_final_state & HAS_BLOCK_FINAL_TX) {
        // Fetch the last block-final tx. The index, 0, is just a
        // placeholder and might not be the actual index we're looking for,
        // but that's okay. This call should never fail because the prior
        // block-final transaction was the last processed transaction (so
        // none of the outputs could have been spent) or a previously
        // immature coinbase.
        COutPoint prevout(pcoinsTip->GetFinalTx(), 0);
        prev_final = pcoinsTip->AccessCoins(prevout.hash);
        if (!prev_final) {
            // Should never happen
            return NULL;
        }
        // If it was a coinbase, meaning we're in the first 100 blocks after
        // activation, then we need to make sure it has matured, otherwise
        // we do nothing at all.
        if (prev_final->IsCoinBase() && (nHeight - prev_final->nHeight < COINBASE_MATURITY)) {
            // Still maturing. Nothing to do.
            block_final_state = NO_BLOCK_FINAL_TX;
        }
    }

    // Decide whether to include witness transactions
    // This is only needed in case the witness softfork activation is reverted
    // (which would require a very deep reorganization) or when
    // -promiscuousmempoolflags is used.
    // TODO: replace this with a call to main to assess validity of a mempool
    // transaction (which in most cases can be a no-op).
    fIncludeWitness = IsWitnessEnabled(pindexPrev, chainparams.GetConsensus());

    initFinalTx(prev_final ? *prev_final : CCoins());

    addPriorityTxs();
    addPackageTxs();

    nLastBlockTx = nBlockTx;
    nLastBlockSize = nBlockSize;
    nLastBlockWeight = nBlockWeight;

    // Create coinbase transaction.
    CMutableTransaction coinbaseTx;
    coinbaseTx.vin.resize(1);
    coinbaseTx.vin[0].prevout.SetNull();
    coinbaseTx.vout.resize(1);
    coinbaseTx.vout[0].scriptPubKey = scriptPubKeyIn;
    coinbaseTx.vout[0].nValue = nFees + GetBlockSubsidy(nHeight, chainparams.GetConsensus());
    if (block_final_state & INITIAL_BLOCK_FINAL_TXOUT) {
        coinbaseTx.vout.resize(2);
        coinbaseTx.vout[1].nValue = 0;
        coinbaseTx.vout[1].scriptPubKey = CScript() << OP_TRUE;
    }
    coinbaseTx.vin[0].scriptSig = CScript() << nHeight << OP_0;
    pblock->vtx[0] = coinbaseTx;
    pblocktemplate->vchCoinbaseCommitment = GenerateCoinbaseCommitment(*pblock, pindexPrev, chainparams.GetConsensus());
    pblocktemplate->vTxFees[0] = -nFees;

    // The miner needs to know whether the last transaction is a special
    // transaction, or not.
    pblocktemplate->has_block_final_tx = (block_final_state & HAS_BLOCK_FINAL_TX);

    uint64_t nSerializeSize = GetSerializeSize(*pblock, SER_NETWORK, PROTOCOL_VERSION);
    LogPrintf("CreateNewBlock(): total size: %u block weight: %u txs: %u fees: %ld sigops %d\n", nSerializeSize, GetBlockWeight(*pblock), nBlockTx, (block_final_state & HAS_BLOCK_FINAL_TX) ? nFees - pblocktemplate->vTxFees.back() : nFees, nBlockSigOpsCost);

    // Fill in header
    pblock->hashPrevBlock  = pindexPrev->GetBlockHash();
    UpdateTime(pblock, chainparams.GetConsensus(), pindexPrev);
    pblock->nBits          = GetNextWorkRequired(pindexPrev, pblock, chainparams.GetConsensus());
    pblock->nNonce         = 0;
    pblocktemplate->vTxSigOpsCost[0] = WITNESS_SCALE_FACTOR * GetLegacySigOpCount(pblock->vtx[0]);

    CValidationState state;
    if (!TestBlockValidity(state, chainparams, *pblock, pindexPrev, false, false)) {
        throw std::runtime_error(strprintf("%s: TestBlockValidity failed: %s", __func__, FormatStateMessage(state)));
    }

    return pblocktemplate.release();
}

bool BlockAssembler::isStillDependent(CTxMemPool::txiter iter)
{
    BOOST_FOREACH(CTxMemPool::txiter parent, mempool.GetMemPoolParents(iter))
    {
        if (!inBlock.count(parent)) {
            return true;
        }
    }
    return false;
}

void BlockAssembler::onlyUnconfirmed(CTxMemPool::setEntries& testSet)
{
    for (CTxMemPool::setEntries::iterator iit = testSet.begin(); iit != testSet.end(); ) {
        // Only test txs not already in the block
        if (inBlock.count(*iit)) {
            testSet.erase(iit++);
        }
        else {
            iit++;
        }
    }
}

bool BlockAssembler::TestPackage(uint64_t packageSize, int64_t packageSigOpsCost)
{
    // TODO: switch to weight-based accounting for packages instead of vsize-based accounting.
    if (nBlockWeight + WITNESS_SCALE_FACTOR * packageSize >= nBlockMaxWeight)
        return false;
    if (nBlockSigOpsCost + packageSigOpsCost >= MAX_BLOCK_SIGOPS_COST)
        return false;
    return true;
}

// Perform transaction-level checks before adding to block:
// - transaction finality (locktime)
// - premature witness (in case segwit transactions are added to mempool before
//   segwit activation)
// - serialized size (in case -blockmaxsize is in use)
bool BlockAssembler::TestPackageTransactions(const CTxMemPool::setEntries& package)
{
    uint64_t nPotentialBlockSize = nBlockSize; // only used with fNeedSizeAccounting
    BOOST_FOREACH (const CTxMemPool::txiter it, package) {
        if (!IsFinalTx(it->GetTx(), nHeight, nLockTimeCutoff))
            return false;
        if (!fIncludeWitness && !it->GetTx().wit.IsNull())
            return false;
        if (fNeedSizeAccounting) {
            uint64_t nTxSize = ::GetSerializeSize(it->GetTx(), SER_NETWORK, PROTOCOL_VERSION);
            if (nPotentialBlockSize + nTxSize >= nBlockMaxSize) {
                return false;
            }
            nPotentialBlockSize += nTxSize;
        }
    }
    return true;
}

bool BlockAssembler::TestForBlock(CTxMemPool::txiter iter)
{
    if (nBlockWeight + iter->GetTxWeight() >= nBlockMaxWeight) {
        // If the block is so close to full that no more txs will fit
        // or if we've tried more than 50 times to fill remaining space
        // then flag that the block is finished
        if (nBlockWeight >  nBlockMaxWeight - 400 || lastFewTxs > 50) {
             blockFinished = true;
             return false;
        }
        // Once we're within 4000 weight of a full block, only look at 50 more txs
        // to try to fill the remaining space.
        if (nBlockWeight > nBlockMaxWeight - 4000) {
            lastFewTxs++;
        }
        return false;
    }

    if (fNeedSizeAccounting) {
        if (nBlockSize + ::GetSerializeSize(iter->GetTx(), SER_NETWORK, PROTOCOL_VERSION) >= nBlockMaxSize) {
            if (nBlockSize >  nBlockMaxSize - 100 || lastFewTxs > 50) {
                 blockFinished = true;
                 return false;
            }
            if (nBlockSize > nBlockMaxSize - 1000) {
                lastFewTxs++;
            }
            return false;
        }
    }

    if (nBlockSigOpsCost + iter->GetSigOpCost() >= MAX_BLOCK_SIGOPS_COST) {
        // If the block has room for no more sig ops then
        // flag that the block is finished
        if (nBlockSigOpsCost > MAX_BLOCK_SIGOPS_COST - 8) {
            blockFinished = true;
            return false;
        }
        // Otherwise attempt to find another tx with fewer sigops
        // to put in the block.
        return false;
    }

    // Must check that lock times are still valid
    // This can be removed once MTP is always enforced
    // as long as reorgs keep the mempool consistent.
    if (!IsFinalTx(iter->GetTx(), nHeight, nLockTimeCutoff))
        return false;

    return true;
}

void BlockAssembler::AddToBlock(CTxMemPool::txiter iter)
{
    // If we have a block-final transaction, insert just
    // before the end, so the block-final tx remains last.
    pblock->vtx.insert(pblock->vtx.end() - !!(block_final_state & HAS_BLOCK_FINAL_TX), iter->GetTx());
    pblocktemplate->vTxFees.insert(pblocktemplate->vTxFees.end() - !!(block_final_state & HAS_BLOCK_FINAL_TX), iter->GetFee());
    pblocktemplate->vTxSigOpsCost.insert(pblocktemplate->vTxSigOpsCost.end() - !!(block_final_state & HAS_BLOCK_FINAL_TX), iter->GetSigOpCost());
    if (fNeedSizeAccounting) {
        nBlockSize += ::GetSerializeSize(iter->GetTx(), SER_NETWORK, PROTOCOL_VERSION);
    }
    nBlockWeight += iter->GetTxWeight();
    ++nBlockTx;
    nBlockSigOpsCost += iter->GetSigOpCost();
    nFees += iter->GetFee();
    inBlock.insert(iter);

    bool fPrintPriority = GetBoolArg("-printpriority", DEFAULT_PRINTPRIORITY);
    if (fPrintPriority) {
        double dPriority = iter->GetPriority(nHeight);
        CAmount dummy;
        mempool.ApplyDeltas(iter->GetTx().GetHash(), dPriority, dummy);
        LogPrintf("priority %.1f fee %s txid %s\n",
                  dPriority,
                  CFeeRate(iter->GetModifiedFee(), iter->GetTxSize()).ToString(),
                  iter->GetTx().GetHash().ToString());
    }
}

void BlockAssembler::UpdatePackagesForAdded(const CTxMemPool::setEntries& alreadyAdded,
        indexed_modified_transaction_set &mapModifiedTx)
{
    BOOST_FOREACH(const CTxMemPool::txiter it, alreadyAdded) {
        CTxMemPool::setEntries descendants;
        mempool.CalculateDescendants(it, descendants);
        // Insert all descendants (not yet in block) into the modified set
        BOOST_FOREACH(CTxMemPool::txiter desc, descendants) {
            if (alreadyAdded.count(desc))
                continue;
            modtxiter mit = mapModifiedTx.find(desc);
            if (mit == mapModifiedTx.end()) {
                CTxMemPoolModifiedEntry modEntry(desc);
                modEntry.nSizeWithAncestors -= it->GetTxSize();
                modEntry.nModFeesWithAncestors -= it->GetModifiedFee();
                modEntry.nSigOpCostWithAncestors -= it->GetSigOpCost();
                mapModifiedTx.insert(modEntry);
            } else {
                mapModifiedTx.modify(mit, update_for_parent_inclusion(it));
            }
        }
    }
}

// Skip entries in mapTx that are already in a block or are present
// in mapModifiedTx (which implies that the mapTx ancestor state is
// stale due to ancestor inclusion in the block)
// Also skip transactions that we've already failed to add. This can happen if
// we consider a transaction in mapModifiedTx and it fails: we can then
// potentially consider it again while walking mapTx.  It's currently
// guaranteed to fail again, but as a belt-and-suspenders check we put it in
// failedTx and avoid re-evaluation, since the re-evaluation would be using
// cached size/sigops/fee values that are not actually correct.
bool BlockAssembler::SkipMapTxEntry(CTxMemPool::txiter it, indexed_modified_transaction_set &mapModifiedTx, CTxMemPool::setEntries &failedTx)
{
    assert (it != mempool.mapTx.end());
    if (mapModifiedTx.count(it) || inBlock.count(it) || failedTx.count(it))
        return true;
    return false;
}

void BlockAssembler::SortForBlock(const CTxMemPool::setEntries& package, CTxMemPool::txiter entry, std::vector<CTxMemPool::txiter>& sortedEntries)
{
    // Sort package by ancestor count
    // If a transaction A depends on transaction B, then A's ancestor count
    // must be greater than B's.  So this is sufficient to validly order the
    // transactions for block inclusion.
    sortedEntries.clear();
    sortedEntries.insert(sortedEntries.begin(), package.begin(), package.end());
    std::sort(sortedEntries.begin(), sortedEntries.end(), CompareTxIterByAncestorCount());
}

void BlockAssembler::initFinalTx(const CCoins& prev_final)
{
    // Create block-final tx
    CMutableTransaction txFinal;
    txFinal.nVersion = 2;
    txFinal.nLockTime = static_cast<uint32_t>(nMedianTimePast);

    // Block-final transactions are only created from prior block-final outputs
    // after we have reached the final state of activation.
    if (block_final_state & HAS_BLOCK_FINAL_TX) {
        // Add all outputs from the prior block-final transaction.  We do
        // nothing here to prevent selected transactions from spending these
        // same outputs out from underneath us; we depend insted on mempool
        // protections that prevent such transactions from being considered in
        // the first place.
        COutPoint prevout(pcoinsTip->GetFinalTx(), 0);
        for (; prevout.n < prev_final.vout.size(); ++prevout.n) {
            if (IsTriviallySpendable(prev_final, prevout, MANDATORY_SCRIPT_VERIFY_FLAGS|SCRIPT_VERIFY_WITNESS|SCRIPT_VERIFY_CLEANSTACK)) {
                txFinal.vin.push_back(CTxIn(prevout, CScript(), CTxIn::SEQUENCE_FINAL));
            }
        }
    }

#ifdef ENABLE_WALLET
    else {
        if (!pwalletMain) {
            LogPrintf("No wallet; unable to fetch outputs for block-final transaction.\n");
            return;
        }

        LOCK(pwalletMain->cs_wallet);

        CBitcoinAddress minesweepaddr; // FIXME
        CBitcoinAddress carryforward;

        // Get a vector of available outputs from the wallet.  This should not
        // include any outputs spent in this block, because outputs in the
        // mempool are excluded (and the transactions of the block were pulled
        // from the mempool).
        std::vector<COutput> outputs;
        pwalletMain->AvailableCoins(outputs, false, nullptr, true);
        if (outputs.empty()) {
            LogPrintf("No available wallet outputs for block-final transaction.\n");
            return;
        }

        // Gather inputs
        CAmount totalin = 0;
        std::vector<CCoins> coins;
        for (const COutput& out : outputs) {
            if (!out.tx || out.nDepth<=0) {
                // Do not use unconfirmed outputs because we don't know if they
                // will be included in the block or not.
                continue;
            }
            txFinal.vin.emplace_back(COutPoint(out.tx->GetHash(), out.i));
            totalin += out.tx->vout[out.i].nValue;
            coins.emplace_back(*out.tx, out.nDepth);
            if (!minesweepaddr.IsValid()) {
                // If we're not sweeping the wallet, then we only need one.
                break;
            }
        }
        // Optional: sweep outputs
        if (minesweepaddr.IsValid()) {
            // The block-final transaction already includes all confirmed wallet
            // outputs.  So we just have to add an output to the minesweeper
            // address claiming the funds.
        }
        // Optional: fixed carry-forward addr
        if (carryforward.IsValid()) {
            // If a carry-forward address is specified, we make sure the tx
            // includes an output to that address, to enable future blocks to be
            // mined.
        }
        // Default: send funds to reserve address
        if (totalin || !carryforward.IsValid()) {
            boost::shared_ptr<CReserveScript> reserve;
            GetMainSignals().ScriptForMining(reserve);
            if (!reserve) {
                LogPrintf("Keypool ran out while reserving script for block-final transaction, please call keypoolrefill\n");
                return;
            }
            if (reserve->reserveScript.empty()) {
                LogPrintf("No coinbase script available for block-final transaction (merge mining requires a wallet!)\n");
                return;
            }
            txFinal.vout.emplace_back(totalin, reserve->reserveScript);
            totalin = 0;
        }
    }
#endif // ENABLE_WALLET

    // We should have input(s) for the block-final transaction either from the
    // prior block-final transaction, or from the wallet.  If not, we cannot
    // create a block-final transaction because any non-coinbase transaction
    // must have valid inputs.
    if (txFinal.vin.empty()) {
        LogPrintf("Unable to create block-final transaction due to lack of inputs.\n");
        return;
    }

    // Add commitment and sign block-final transaction.
    if (!UpdateBlockFinalTransaction(txFinal, uint256())) {
        LogPrintf("Error signing block-final transaction; cannot use invalid transaction.\n");
        return;
    }

    // Add block-final transaction to block template.
    pblock->vtx.push_back(txFinal);
    ++nBlockTx;

    // Record the fees forwarded by the block-final transaction to the coinbase.
    CAmount nTxFees = pcoinsTip->GetValueIn(pblock->vtx.back())
                    - pblock->vtx.back().GetValueOut();
    pblocktemplate->vTxFees.push_back(nTxFees);
    nFees += nTxFees;

    // The block-final transaction contributes to aggregate limits:
    // the number of sigops is tracked...
    int64_t nTxSigOpsCost = GetTransactionSigOpCost(txFinal, pcoinsTip, STANDARD_SCRIPT_VERIFY_FLAGS);
    pblocktemplate->vTxSigOpsCost.push_back(nTxSigOpsCost);
    nBlockSigOpsCost += nTxSigOpsCost;

    // ...the size is not:
    nBlockWeight += GetTransactionWeight(txFinal);
    nBlockSize += ::GetSerializeSize(txFinal, SER_NETWORK, PROTOCOL_VERSION);
}

// This transaction selection algorithm orders the mempool based
// on feerate of a transaction including all unconfirmed ancestors.
// Since we don't remove transactions from the mempool as we select them
// for block inclusion, we need an alternate method of updating the feerate
// of a transaction with its not-yet-selected ancestors as we go.
// This is accomplished by walking the in-mempool descendants of selected
// transactions and storing a temporary modified state in mapModifiedTxs.
// Each time through the loop, we compare the best transaction in
// mapModifiedTxs with the next transaction in the mempool to decide what
// transaction package to work on next.
void BlockAssembler::addPackageTxs()
{
    // mapModifiedTx will store sorted packages after they are modified
    // because some of their txs are already in the block
    indexed_modified_transaction_set mapModifiedTx;
    // Keep track of entries that failed inclusion, to avoid duplicate work
    CTxMemPool::setEntries failedTx;

    // Start by adding all descendants of previously added txs to mapModifiedTx
    // and modifying them for their already included ancestors
    UpdatePackagesForAdded(inBlock, mapModifiedTx);

    CTxMemPool::indexed_transaction_set::index<ancestor_score>::type::iterator mi = mempool.mapTx.get<ancestor_score>().begin();
    CTxMemPool::txiter iter;
    while (mi != mempool.mapTx.get<ancestor_score>().end() || !mapModifiedTx.empty())
    {
        // First try to find a new transaction in mapTx to evaluate.
        if (mi != mempool.mapTx.get<ancestor_score>().end() &&
                SkipMapTxEntry(mempool.mapTx.project<0>(mi), mapModifiedTx, failedTx)) {
            ++mi;
            continue;
        }

        // Now that mi is not stale, determine which transaction to evaluate:
        // the next entry from mapTx, or the best from mapModifiedTx?
        bool fUsingModified = false;

        modtxscoreiter modit = mapModifiedTx.get<ancestor_score>().begin();
        if (mi == mempool.mapTx.get<ancestor_score>().end()) {
            // We're out of entries in mapTx; use the entry from mapModifiedTx
            iter = modit->iter;
            fUsingModified = true;
        } else {
            // Try to compare the mapTx entry to the mapModifiedTx entry
            iter = mempool.mapTx.project<0>(mi);
            if (modit != mapModifiedTx.get<ancestor_score>().end() &&
                    CompareModifiedEntry()(*modit, CTxMemPoolModifiedEntry(iter))) {
                // The best entry in mapModifiedTx has higher score
                // than the one from mapTx.
                // Switch which transaction (package) to consider
                iter = modit->iter;
                fUsingModified = true;
            } else {
                // Either no entry in mapModifiedTx, or it's worse than mapTx.
                // Increment mi for the next loop iteration.
                ++mi;
            }
        }

        // We skip mapTx entries that are inBlock, and mapModifiedTx shouldn't
        // contain anything that is inBlock.
        assert(!inBlock.count(iter));

        uint64_t packageSize = iter->GetSizeWithAncestors();
        CAmount packageFees = iter->GetModFeesWithAncestors();
        int64_t packageSigOpsCost = iter->GetSigOpCostWithAncestors();
        if (fUsingModified) {
            packageSize = modit->nSizeWithAncestors;
            packageFees = modit->nModFeesWithAncestors;
            packageSigOpsCost = modit->nSigOpCostWithAncestors;
        }

        if (packageFees < ::minRelayTxFee.GetFee(packageSize)) {
            // Everything else we might consider has a lower fee rate
            return;
        }

        if (!TestPackage(packageSize, packageSigOpsCost)) {
            if (fUsingModified) {
                // Since we always look at the best entry in mapModifiedTx,
                // we must erase failed entries so that we can consider the
                // next best entry on the next loop iteration
                mapModifiedTx.get<ancestor_score>().erase(modit);
                failedTx.insert(iter);
            }
            continue;
        }

        CTxMemPool::setEntries ancestors;
        uint64_t nNoLimit = std::numeric_limits<uint64_t>::max();
        std::string dummy;
        mempool.CalculateMemPoolAncestors(*iter, ancestors, nNoLimit, nNoLimit, nNoLimit, nNoLimit, dummy, false);

        onlyUnconfirmed(ancestors);
        ancestors.insert(iter);

        // Test if all tx's are Final
        if (!TestPackageTransactions(ancestors)) {
            if (fUsingModified) {
                mapModifiedTx.get<ancestor_score>().erase(modit);
                failedTx.insert(iter);
            }
            continue;
        }

        // Package can be added. Sort the entries in a valid order.
        vector<CTxMemPool::txiter> sortedEntries;
        SortForBlock(ancestors, iter, sortedEntries);

        for (size_t i=0; i<sortedEntries.size(); ++i) {
            AddToBlock(sortedEntries[i]);
            // Erase from the modified set, if present
            mapModifiedTx.erase(sortedEntries[i]);
        }

        // Update transactions that depend on each of these
        UpdatePackagesForAdded(ancestors, mapModifiedTx);
    }
}

void BlockAssembler::addPriorityTxs()
{
    // How much of the block should be dedicated to high-priority transactions,
    // included regardless of the fees they pay
    unsigned int nBlockPrioritySize = GetArg("-blockprioritysize", DEFAULT_BLOCK_PRIORITY_SIZE);
    nBlockPrioritySize = std::min(nBlockMaxSize, nBlockPrioritySize);

    if (nBlockPrioritySize == 0) {
        return;
    }

    bool fSizeAccounting = fNeedSizeAccounting;
    fNeedSizeAccounting = true;

    // This vector will be sorted into a priority queue:
    vector<TxCoinAgePriority> vecPriority;
    TxCoinAgePriorityCompare pricomparer;
    std::map<CTxMemPool::txiter, double, CTxMemPool::CompareIteratorByHash> waitPriMap;
    typedef std::map<CTxMemPool::txiter, double, CTxMemPool::CompareIteratorByHash>::iterator waitPriIter;
    double actualPriority = -1;

    vecPriority.reserve(mempool.mapTx.size());
    for (CTxMemPool::indexed_transaction_set::iterator mi = mempool.mapTx.begin();
         mi != mempool.mapTx.end(); ++mi)
    {
        double dPriority = mi->GetPriority(nHeight);
        CAmount dummy;
        mempool.ApplyDeltas(mi->GetTx().GetHash(), dPriority, dummy);
        vecPriority.push_back(TxCoinAgePriority(dPriority, mi));
    }
    std::make_heap(vecPriority.begin(), vecPriority.end(), pricomparer);

    CTxMemPool::txiter iter;
    while (!vecPriority.empty() && !blockFinished) { // add a tx from priority queue to fill the blockprioritysize
        iter = vecPriority.front().second;
        actualPriority = vecPriority.front().first;
        std::pop_heap(vecPriority.begin(), vecPriority.end(), pricomparer);
        vecPriority.pop_back();

        // If tx already in block, skip
        if (inBlock.count(iter)) {
            assert(false); // shouldn't happen for priority txs
            continue;
        }

        // cannot accept witness transactions into a non-witness block
        if (!fIncludeWitness && !iter->GetTx().wit.IsNull())
            continue;

        // If tx is dependent on other mempool txs which haven't yet been included
        // then put it in the waitSet
        if (isStillDependent(iter)) {
            waitPriMap.insert(std::make_pair(iter, actualPriority));
            continue;
        }

        // If this tx fits in the block add it, otherwise keep looping
        if (TestForBlock(iter)) {
            AddToBlock(iter);

            // If now that this txs is added we've surpassed our desired priority size
            // or have dropped below the AllowFreeThreshold, then we're done adding priority txs
            if (nBlockSize >= nBlockPrioritySize || !AllowFree(actualPriority)) {
                break;
            }

            // This tx was successfully added, so
            // add transactions that depend on this one to the priority queue to try again
            BOOST_FOREACH(CTxMemPool::txiter child, mempool.GetMemPoolChildren(iter))
            {
                waitPriIter wpiter = waitPriMap.find(child);
                if (wpiter != waitPriMap.end()) {
                    vecPriority.push_back(TxCoinAgePriority(wpiter->second,child));
                    std::push_heap(vecPriority.begin(), vecPriority.end(), pricomparer);
                    waitPriMap.erase(wpiter);
                }
            }
        }
    }
    fNeedSizeAccounting = fSizeAccounting;
}

void IncrementExtraNonce(CBlock* pblock, const CBlockIndex* pindexPrev, unsigned int& nExtraNonce)
{
    // Update nExtraNonce
    static uint256 hashPrevBlock;
    if (hashPrevBlock != pblock->hashPrevBlock)
    {
        nExtraNonce = 0;
        hashPrevBlock = pblock->hashPrevBlock;
    }
    ++nExtraNonce;
    unsigned int nHeight = pindexPrev->nHeight+1; // Height first in coinbase required for block.version=2
    CMutableTransaction txCoinbase(pblock->vtx[0]);
    txCoinbase.vin[0].scriptSig = (CScript() << nHeight << CScriptNum(nExtraNonce)) + COINBASE_FLAGS;
    assert(txCoinbase.vin[0].scriptSig.size() <= 100);

    pblock->vtx[0] = txCoinbase;
    pblock->hashMerkleRoot = BlockMerkleRoot(*pblock);
}

bool UpdateBlockFinalTransaction(CMutableTransaction &ret, const uint256& hash)
{
    CMutableTransaction mtx(ret);

    // Generate new commitment
    std::vector<unsigned char> new_commitment(36, 0x00);
    std::copy(hash.begin(), hash.end(),
              new_commitment.begin());
    new_commitment[32] = 0x4b;
    new_commitment[33] = 0x4a;
    new_commitment[34] = 0x49;
    new_commitment[35] = 0x48;

    // Find & update commitment
    if (!mtx.vout.empty() && mtx.vout.back().scriptPubKey.size() == 37 && mtx.vout.back().scriptPubKey[0] == 36 && mtx.vout.back().scriptPubKey[33] == 0x4b && mtx.vout.back().scriptPubKey[34] == 0x4a && mtx.vout.back().scriptPubKey[35] == 0x49 && mtx.vout.back().scriptPubKey[36] == 0x48) {
        mtx.vout.back().scriptPubKey = CScript() << new_commitment;
    } else {
        mtx.vout.emplace_back(0, CScript() << new_commitment);
    }

    LOCK(cs_main);

    // Sign transaction
    CTransaction tx(mtx);
    ScriptError serror = SCRIPT_ERR_OK;
    for (size_t i = 0; i < tx.vin.size(); ++i) {
        CTxIn& txin = mtx.vin[i];
        const CCoins* coin = pcoinsTip->AccessCoins(txin.prevout.hash);
        if (!coin) {
            LogPrintf("Unable to find UTXO for block-final transaction input hash %s; unable to sign block-final transaction.\n", txin.prevout.hash.ToString());
            return false;
        }
        const CScript& prevPubKey = coin->vout[txin.prevout.n].scriptPubKey;
        const CAmount& amount = coin->vout[txin.prevout.n].nValue;
        SignatureData sigdata;
        ProduceSignature(MutableTransactionSignatureCreator(pwalletMain, &mtx, i, amount, SIGHASH_NONE), prevPubKey, sigdata);
        UpdateTransaction(mtx, i, sigdata);
        ScriptError serror = SCRIPT_ERR_OK;
        if (!VerifyScript(txin.scriptSig, prevPubKey, mtx.wit.vtxinwit.size() > i ? &mtx.wit.vtxinwit[i].scriptWitness : NULL, STANDARD_SCRIPT_VERIFY_FLAGS, TransactionSignatureChecker(&tx, i, amount), &serror)) {
            LogPrintf("error creating signature for wallet input to block-final transaction: %s", ScriptErrorString(serror));
            return false;
        }
    }

    ret = mtx;
    return true;
}
