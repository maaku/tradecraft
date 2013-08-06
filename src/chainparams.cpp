// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2011-2021 The Freicoin Developers
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of version 3 of the GNU Affero General Public License as published
// by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
// details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "streams.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

const std::string hexGenesisTx = "02000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d01044554656c6567726170682032372f4a756e2f3230313220426172636c61797320686974207769746820c2a33239306d2066696e65206f766572204c69626f7220666978696e67ffffffff08893428ed05000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac010000000000000023205029d180e0c5ed798d877b1ada99772986c1422ca932c41b2d0400000000000075000100000000000000fd530103202020754d31014d6574616c73207765726520616e20696d706c696369746c7920616275736976652061677265656d656e742e0a4d6f6465726e2022706170657222206973206120666c6177656420746f6f6c2c2069747320656e67696e656572696e672069732061206e657374206f66206c6565636865732e0a546865206f6c64206d6f6e6579206973206f62736f6c6574652e0a4c65742074686520696e646976696475616c206d6f6e6574697a65206974732063726564697420776974686f75742063617274656c20696e7465726d65646961726965732e0a4769766520757320612072656e742d6c657373206361736820736f2077652063616e206265206672656520666f72207468652066697273742074696d652e0a4c65742074686973206265207468652061776169746564206461776e2e7576a9140ef0f9d19a653023554146a866238b8822bc84df88ac0100000000000000fa082020202020202020754cd4224c65742075732063616c63756c6174652c20776974686f757420667572746865722061646f2c20696e206f7264657220746f207365652077686f2069732072696768742e22202d2d476f747466726965642057696c68656c6d204c6569626e697a0acebec2b4efbda5e28880efbda560efbc89e38080e38080e38080e3808020206e0aefbfa3e38080e38080e380802020efbcbce38080e380802020efbc882045efbc8920676f6f64206a6f622c206d61616b75210aefbe8ce38080e38080e3808020202fe383bd20e383bd5fefbc8fefbc8f7576a914c26be5ec809aa4bf6b30aa89823cff7cedc3679a88ac01000000000000005f06202020202020753c4963682077c3bc6e736368652046726569636f696e207669656c204572666f6c67207a756d204e75747a656e206465722039392050726f7a656e74217576a9142939acd60037281a708eb11e4e9eda452c029eca88ac0100000000000000980d20202020202020202020202020754c6d225468652076616c7565206f662061206d616e2073686f756c64206265207365656e20696e207768617420686520676976657320616e64206e6f7420696e20776861742068652069732061626c6520746f20726563656976652e22202d2d416c626572742045696e737465696e7576a914f9ca5caab4bda4dc28b5556aa79a2eec0447f0bf88ac0100000000000000800c202020202020202020202020754c5622416e2061726d79206f66207072696e6369706c65732063616e2070656e65747261746520776865726520616e2061726d79206f6620736f6c64696572732063616e6e6f742e22202d2d54686f6d6173205061696e657576a91408f320cbb41a1ae25b794f6175f96080681989f388accc60948c0b0000001976a91485e54144c4020a65fa0a8fdbac8bba75dbc2fd0088ac0000000000000000";

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion)
{
    CMutableTransaction txNew;
    CDataStream stream(ParseHex(hexGenesisTx), SER_NETWORK, PROTOCOL_VERSION);
    stream >> txNew;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.bitcoin_mode = false;
        consensus.nSubsidyHalvingInterval = 0; // never
        consensus.perpetual_subsidy = 9536743164; // 95.367,431,64fc
        consensus.equilibrium_height = 161280; // three years
        consensus.equilibrium_monetary_base = 10000000000000000LL; // 100,000,000.0000,0000fc
        consensus.initial_excess_subsidy = 15916928404LL; // 1519.1692,8404fc
        consensus.truncate_inputs_activation_height = 158425;
        consensus.alu_activation_height = 229376;
        consensus.BIP34Height = 227931;
        consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8");
        consensus.BIP66Height = 158425; // 0000000000000799b28bbc61b9a93770af898ffc621174e70480656f0382a020
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // OriginalTargetTimespan() / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68 and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_LOCKTIME].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_LOCKTIME].nStartTime = 1462060800; // May 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_LOCKTIME].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1479168000; // November 15th, 2016.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1510704000; // November 15th, 2017.

        // Deployment of block-final miner commitment transaction.
        consensus.vDeployments[Consensus::DEPLOYMENT_FINALTX].bit = 12;
        consensus.vDeployments[Consensus::DEPLOYMENT_FINALTX].nStartTime = 1599004800; // September 2, 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_FINALTX].nTimeout = 1719878400; // July 2, 2024

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000262149e6218a5b60cef");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000000000000114100284febd7d76aadf7522062dabf611c73f4f9b44db72c35"); //302400

        consensus.original_adjust_interval = 2016; // two weeks
        consensus.filtered_adjust_interval = 9; // 1.5 hrs
        consensus.diff_adjust_threshold = 28336;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xbe;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0xd9;
        nDefaultPort = 8639;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1356123600, 278229610, 0x1d00ffff, 1);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000000005b1e3d23ecfd2dd4a6e1a35238aa0392c0a8528c40df52376d7efe2c"));
        assert(genesis.hashMerkleRoot == uint256S("0xf53b1baa971ea40be88cf51288aabd700dfec96c486bf7155a53a4919af4c8bd"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.push_back(CDNSSeedData("bitcoin.sipa.be", "seed.bitcoin.sipa.be", true)); // Pieter Wuille, only supports x1, x5, x9, and xd
        vSeeds.push_back(CDNSSeedData("bluematt.me", "dnsseed.bluematt.me", true)); // Matt Corallo, only supports x9
        vSeeds.push_back(CDNSSeedData("dashjr.org", "dnsseed.freicoin.dashjr.org")); // Luke Dashjr
        vSeeds.push_back(CDNSSeedData("freicoinstats.com", "seed.freicoinstats.com", true)); // Christian Decker, supports x1 - xf
        vSeeds.push_back(CDNSSeedData("freicoin.jonasschnelli.ch", "seed.freicoin.jonasschnelli.ch", true)); // Jonas Schnelli, only supports x1, x5, x9, and xd

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (10080, uint256S("0x00000000003ff9c4b806639ec4376cc9acafcdded0e18e9dbcc2fc42e8e72331"))
            (20160, uint256S("0x0000000000009708ba48a52599295db8b9ec5d29148561e6ac850af765026528"))
            (28336, uint256S("0x000000000000cc374a984c0deec9aed6fff764918e2cfd4be6670dd4d5292ccb")) // Difficulty adjustment hard-fork activation
            (30240, uint256S("0x0000000000004acbe1ed430d4a70d8a9ac62daa849e0bc708da52eeba8f39afc"))
            (40320, uint256S("0x0000000000008ad31a52a3e749bd5c477aa3da18cc0acd3e3d944b5edc58e7bd"))
            (50400, uint256S("0x0000000000000e2e3686f1eb852222ffff33a403947478bea143ed88c81fdd87"))
            (60480, uint256S("0x000000000000029a0d1df882d1ddd15387855d5f904127c25359f8bdc6425928"))
            (70560, uint256S("0x00000000000002b41cead9ce01c519a56998db8a715aae518f4b72403d6dc95a"))
            (80640, uint256S("0x00000000000001c20353e3df80d35c8348bc07940d5e08d4740372ef45a4474f"))
            (90720, uint256S("0x00000000000006c884dfe4e81504fd8eaf9d7d770a04dbdafb2cbf5ad7ab64c6"))
            (100800, uint256S("0x00000000000004dc5badc155de4d07b4c09b9f3ecfdfdaf71576f3d2be192ea3"))
            (110880, uint256S("0x0000000000000ef59288c01fcef9c26b0457bc93ca106d06bb10cd5dfad7fca9"))
            (120960, uint256S("0x00000000000002968c68497ec2a7ec6b5030202dbf874126a65e437f53c03bea"))
            (131040, uint256S("0x0000000000000bf11095c39e143ed02508132e48e040db791a0e7ed73378e7ed"))
            (141120, uint256S("0x000000000000016331fe98568de3673c7c983f10d4ceab0f75d928acc0378001"))
            (151200, uint256S("0x000000000000047df778aaa84d03cf2d8f9b51ef530a7d3708bfd6a9e0dd5d41"))
            (161280, uint256S("0x00000000000021b3611f18840adf738c4a0c8de1479f53721c29a899620a4064"))
            (171360, uint256S("0x00000000000037920bd0a1f13c579ca7c6ade2ef56b19027dd4408c292e5882f"))
            (181440, uint256S("0x00000000000001d49e7ad75303c6217d6205cd51d5c1cc494427418385976d44"))
            (191520, uint256S("0x000000000000034be18ec2f1ca59bbc70d54a9cb10fc7230122297c037f441ee"))
            (201600, uint256S("0x00000000000004bb0cc14b70f9fd72900a6839731892d959764dd89615a5535a"))
            (211680, uint256S("0x00000000000000e1156dafc83bc94c1508fbaa2ec1b1440aeceac7dfc0944664"))
            (221760, uint256S("0x00000000000000a7ca764843bedea1e8c7eb2e22390aca9d133caafcd0842ea1"))
            (231840, uint256S("0x000000000000000d1e7c399c42e260076f541b1d41bb805af46994ce896befe7"))
            (241920, uint256S("0x000000000000007f4809ec08659c88598624743896e8620d4a7ebb36ede698f9"))
            (252000, uint256S("0x00000000000000437687524302491d9aead11eb0090a5c451a4dbe6f85d4fbe1"))
            (262080, uint256S("0x000000000000001332e59516a8156b56de7f7ca804238402732f7de4470da1a0"))
            (272160, uint256S("0x000000000000002781d74d59a2e0edaf3b14b5435d8de67c1ed7b547e5f67752"))
            (282240, uint256S("0x00000000000000b852854b82afcff8caf86fc2f392b9e4a4814bf47977813fc1"))
            (292320, uint256S("0x000000000000140206e6fe913172634efa63c3928b0305052bfe4078f1a636fd"))
            (302400, uint256S("0x000000000000114100284febd7d76aadf7522062dabf611c73f4f9b44db72c35"))
        };

        chainTxData = ChainTxData{
            // Data as of block 000000000000114100284febd7d76aadf7522062dabf611c73f4f9b44db72c35 (height 302400).
            1613065771, // * UNIX timestamp of last known number of transactions
            1004030,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.002253125 // * estimated number of transactions per second after that timestamp
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.bitcoin_mode = false;
        consensus.nSubsidyHalvingInterval = 0; // never
        consensus.perpetual_subsidy = 9536743164; // 95.367,431,64fc
        consensus.equilibrium_height = 161280; // three years
        consensus.equilibrium_monetary_base = 10000000000000000LL; // 100,000,000.0000,0000fc
        consensus.initial_excess_subsidy = 15916928404LL; // 1519.1692,8404fc
        consensus.truncate_inputs_activation_height = 1;
        consensus.alu_activation_height = 1;
        consensus.BIP34Height = 21111;
        consensus.BIP34Hash = uint256S("0x0000000023b3a96d3484e5abb3755c413e7d41500f8e2a5c3f0dd01299cd8ef8");
        consensus.BIP66Height = 1; // 0000000000002076358270b88c18cce6a0886c870e6167776e40d167bd01b49f
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // OriginalTargetTimespan() / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68 and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_LOCKTIME].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_LOCKTIME].nStartTime = 1456790400; // March 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_LOCKTIME].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1462060800; // May 1st 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1493596800; // May 1st 2017

        // Deployment of block-final miner commitment transaction.
        consensus.vDeployments[Consensus::DEPLOYMENT_FINALTX].bit = 12;
        consensus.vDeployments[Consensus::DEPLOYMENT_FINALTX].nStartTime = 1599004800; // September 2, 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_FINALTX].nTimeout = 1719878400; // July 2, 2024

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000100010001");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000000a52504ffe3420a43bd385ef24f81838921a903460b235d95f37cd65e"); //0

        consensus.original_adjust_interval = 2016; // two weeks
        consensus.filtered_adjust_interval = 9; // 1.5 hrs
        consensus.diff_adjust_threshold = 2016;

        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        nDefaultPort = 18639;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1356123600, 3098244593U, 0x1d00ffff, 1);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000000a52504ffe3420a43bd385ef24f81838921a903460b235d95f37cd65e"));
        assert(genesis.hashMerkleRoot == uint256S("0xf53b1baa971ea40be88cf51288aabd700dfec96c486bf7155a53a4919af4c8bd"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("testnetfreicoin.jonasschnelli.ch", "testnet-seed.freicoin.jonasschnelli.ch", true));
        vSeeds.push_back(CDNSSeedData("petertodd.org", "seed.tfrc.petertodd.org", true));
        vSeeds.push_back(CDNSSeedData("bluematt.me", "testnet-seed.bluematt.me"));
        vSeeds.push_back(CDNSSeedData("freicoin.schildbach.de", "testnet-seed.freicoin.schildbach.de"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (2016, uint256S("0x0000000000001e891fcab0d810f81795497da3ac799ef8c179ec8e839ccc001b")),
        };

        chainTxData = ChainTxData{
            // Data as of block 0x0000000000001e891fcab0d810f81795497da3ac799ef8c179ec8e839ccc001b (height 2016)
            1594881439,
            2017,
            196
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.bitcoin_mode = false;
        consensus.nSubsidyHalvingInterval = 150;
        consensus.perpetual_subsidy = 5000000000; // 50.000,000,00fc
        consensus.equilibrium_height = 1; // disable
        consensus.equilibrium_monetary_base = 0;
        consensus.initial_excess_subsidy = 0;
        consensus.truncate_inputs_activation_height = 1;
        consensus.alu_activation_height = 1;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_LOCKTIME].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_LOCKTIME].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_LOCKTIME].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_FINALTX].bit = 12;
        consensus.vDeployments[Consensus::DEPLOYMENT_FINALTX].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_FINALTX].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        consensus.original_adjust_interval = 2016; // two weeks
        consensus.filtered_adjust_interval = 9; // 1.5 hrs
        consensus.diff_adjust_threshold = std::numeric_limits<int64_t>::max();

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 28639;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1356123600, 1, 0x207fffff, 1);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x67756db06265141574ff8e7c3f97ebd57c443791e0ca27ee8b03758d6056edb8"));
        assert(genesis.hashMerkleRoot == uint256S("0xf53b1baa971ea40be88cf51288aabd700dfec96c486bf7155a53a4919af4c8bd"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x67756db06265141574ff8e7c3f97ebd57c443791e0ca27ee8b03758d6056edb8"))
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
 
