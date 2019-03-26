<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="en">
<context>
    <name>freicoin</name>
    <message>
        <location filename="../freicoinstrings.cpp" line="+307"/>
        <source>Options:</source>
        <translation>Options:</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Specify data directory</source>
        <translation>Specify data directory</translation>
    </message>
    <message>
        <location line="-90"/>
        <source>Connect to a node to retrieve peer addresses, and disconnect</source>
        <translation>Connect to a node to retrieve peer addresses, and disconnect</translation>
    </message>
    <message>
        <location line="+93"/>
        <source>Specify your own public address</source>
        <translation>Specify your own public address</translation>
    </message>
    <message>
        <location line="-110"/>
        <source>Accept command line and JSON-RPC commands</source>
        <translation>Accept command line and JSON-RPC commands</translation>
    </message>
    <message>
        <location line="-128"/>
        <source>If &lt;category&gt; is not supplied or if &lt;category&gt; = 1, output all debugging information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Prune configured below the minimum of %d MiB.  Please use a higher number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Prune: last wallet synchronisation goes beyond pruned data. You need to -reindex (download the whole blockchain again in case of pruned node)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Reduce storage requirements by pruning (deleting) old blocks. This mode is incompatible with -txindex and -rescan. Warning: Reverting this setting requires re-downloading the entire blockchain. (default: 0 = disable pruning blocks, &gt;%u = target size in MiB to use for block files)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Rescans are not possible in pruned mode. You will need to use -reindex which will download the whole blockchain again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+124"/>
        <source>Error: A fatal internal error occurred, see debug.log for details</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Fee (in %s/kB) to add to transactions you send (default: %s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Pruning blockstore...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Run in the background as a daemon and accept commands</source>
        <translation>Run in the background as a daemon and accept commands</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Unable to start HTTP server. See debug log for details.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-126"/>
        <source>Accept connections from outside (default: 1 if no -proxy or -connect)</source>
        <translation>Accept connections from outside (default: 1 if no -proxy or -connect)</translation>
    </message>
    <message>
        <location line="-218"/>
        <source>Freicoin</source>
        <translation type="unfinished">Freicoin</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The %s developers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>-fallbackfee is set very high! This is the transaction fee you may pay when fee estimates are not available.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>A fee rate (in %s/kB) that will be used when fee estimation has insufficient data (default: %s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Accept relayed transactions received from whitelisted peers even when not relaying transactions (default: %d)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Allow Stratum work requests from specified source. Valid for &lt;ip&gt; are a single IP (e.g. 1.2.3.4), a network/netmask (e.g. 1.2.3.4/255.255.255.0) or a network/CIDR (e.g. 1.2.3.4/24). This option can be specified multiple times</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Bind to given address and always listen on it. Use [host]:port notation for IPv6</source>
        <translation>Bind to given address and always listen on it. Use [host]:port notation for IPv6</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Bind to given address to listen for Stratum work requests. Use [host]:port notation for IPv6. This option can be specified multiple times (default: bind to all interfaces)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Cannot obtain a lock on data directory %s. %s is probably already running.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Delete all wallet transactions and only recover those parts of the blockchain through -rescan on startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Equivalent bytes per sigop in transactions for relay and mining (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Error loading %s: You can&apos;t enable HD on a already existing non-HD wallet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Error reading %s! All keys read correctly, but transaction data or address book entries might be missing or incorrect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Execute command when a wallet transaction changes (%s in cmd is replaced by TxID)</source>
        <translation>Execute command when a wallet transaction changes (%s in cmd is replaced by TxID)</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Maximum allowed median peer time offset adjustment. Local perspective of time may be influenced by peers forward or backward by this amount. (default: %u seconds)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Maximum total fees (in %s) to use in a single wallet transaction or raw transaction; setting this too low may abort large transactions (default: %s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Please check that your computer&apos;s date and time are correct! If your clock is wrong, %s will not work properly.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Please contribute if you find %s useful. Visit %s for further information about the software.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Set the number of script verification threads (%u to %d, 0 = auto, &lt;0 = leave that many cores free, default: %d)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>The block database contains a block which appears to be from the future. This may be due to your computer&apos;s date and time being set incorrectly. Only rebuild the block database if you are sure that your computer&apos;s date and time are correct</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>This is a pre-release test build - use at your own risk - do not use for mining or merchant applications</source>
        <translation>This is a pre-release test build - use at your own risk - do not use for mining or merchant applications</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Unable to rewind the database to a pre-fork state. You will need to redownload the blockchain</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Use UPnP to map the listening port (default: 1 when listening and no -proxy)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Wallet will not create transactions that violate mempool chain limits (default: %u</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Warning: The network does not appear to fully agree! Some miners appear to be experiencing issues.</source>
        <translation>Warning: The network does not appear to fully agree! Some miners appear to be experiencing issues.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Warning: We do not appear to fully agree with our peers! You may need to upgrade, or other nodes may need to upgrade.</source>
        <translation>Warning: We do not appear to fully agree with our peers! You may need to upgrade, or other nodes may need to upgrade.</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>You need to rebuild the database using -reindex-chainstate to change -txindex</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%s corrupt, salvage failed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>-maxmempool must be at least %d MB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;category&gt; can be:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Append comment to the user agent string</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Attempt to recover private keys from a corrupt wallet on startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Bitcoin unit test compatibility mode only allowed on regtest.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Block creation options:</source>
        <translation>Block creation options:</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Cannot resolve -%s address: &apos;%s&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Change index out of range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Connect only to the specified node(s)</source>
        <translation>Connect only to the specified node(s)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Connection options:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Copyright (C) %i-%i</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Corrupted block database detected</source>
        <translation>Corrupted block database detected</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Database corruption likely.  Try restarting with `-reindex=1`.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Debugging/Testing options:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Do not load the wallet and disable wallet RPC calls</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Do you want to rebuild the block database now?</source>
        <translation>Do you want to rebuild the block database now?</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Enable publish hash block in &lt;address&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Enable publish hash transaction in &lt;address&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Enable publish raw block in &lt;address&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Enable publish raw transaction in &lt;address&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Enable stratum server (default: off)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Error initializing block database</source>
        <translation>Error initializing block database</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Error initializing wallet database environment %s!</source>
        <translation>Error initializing wallet database environment %s!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Error loading %s</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Error loading %s: Wallet corrupted</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Error loading %s: Wallet requires newer version of %s</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Error loading %s: You can&apos;t disable HD on a already existing HD wallet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Error loading block database</source>
        <translation>Error loading block database</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Error opening block database</source>
        <translation>Error opening block database</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Error: Disk space is low!</source>
        <translation>Error: Disk space is low!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Failed to listen on any port. Use -listen=0 if you want this.</source>
        <translation>Failed to listen on any port. Use -listen=0 if you want this.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Importing...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Incorrect or no genesis block found. Wrong datadir for network?</source>
        <translation>Incorrect or no genesis block found. Wrong datadir for network?</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Initialization sanity check failed. %s is shutting down.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Invalid -onion address: &apos;%s&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Invalid amount for -%s=&lt;amount&gt;: &apos;%s&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Invalid amount for -fallbackfee=&lt;amount&gt;: &apos;%s&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Keep the transaction memory pool below &lt;n&gt; megabytes (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Keypool ran out, please call keypoolrefill first</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Listen for Stratum work requests on &lt;port&gt; (default: %u or testnet: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Loading banlist...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Location of the auth cookie (default: data dir)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Not enough file descriptors available.</source>
        <translation>Not enough file descriptors available.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Only connect to nodes in network &lt;net&gt; (ipv4, ipv6 or onion)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Print this help message and exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Print version and exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Prune cannot be configured with a negative value.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Prune mode is incompatible with -txindex.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Rebuild chain state and block index from the blk*.dat files on disk</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Rebuild chain state from the currently indexed blocks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Rewinding blocks...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Set database cache size in megabytes (%d to %d, default: %d)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Set maximum BIP141 block weight (default: %d)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Set maximum block size in bytes (default: %d)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Specify wallet file (within data directory)</source>
        <translation>Specify wallet file (within data directory)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Starting network threads...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Stratum server options:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The source code is available from %s.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Transaction has too long of a mempool chain</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Unable to bind to %s on this computer. %s is probably already running.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Unsupported argument -benchmark ignored, use -debug=bench.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unsupported argument -debugnet ignored, use -debug=net.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unsupported argument -tor found, use -onion.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Use UPnP to map the listening port (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>User Agent comment (%s) contains unsafe characters.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Verifying blocks...</source>
        <translation>Verifying blocks...</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Verifying wallet...</source>
        <translation>Verifying wallet...</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Wallet %s resides outside data directory %s</source>
        <translation>Wallet %s resides outside data directory %s</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Wallet debugging/testing options:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Wallet needed to be rewritten: restart %s to complete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Wallet options:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-338"/>
        <source>Allow JSON-RPC connections from specified source. Valid for &lt;ip&gt; are a single IP (e.g. 1.2.3.4), a network/netmask (e.g. 1.2.3.4/255.255.255.0) or a network/CIDR (e.g. 1.2.3.4/24). This option can be specified multiple times</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Bind to given address and whitelist peers connecting to it. Use [host]:port notation for IPv6</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Bind to given address to listen for JSON-RPC connections. Use [host]:port notation for IPv6. This option can be specified multiple times (default: bind to all interfaces)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Create new files with system default permissions, instead of umask 077 (only effective with disabled wallet functionality)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Discover own IP addresses (default: 1 when listening and no -externalip or -proxy)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Distributed under the conjunctive terms of BOTH the GNU Affero General Purpose License v3.0 (&lt;https://www.gnu.org/licenses/agpl-3.0.en.html&gt;) and the MIT software license (&lt;https://www.opensource.org/licenses/mit-license.php&gt;).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Error: Listening for incoming connections failed (listen returned error %s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Execute command when a relevant alert is received or we see a really long fork (%s in cmd is replaced by message)</source>
        <translation>Execute command when a relevant alert is received or we see a really long fork (%s in cmd is replaced by message)</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Fees (in %s/kB) smaller than this are considered zero fee for relaying, mining and transaction creation (default: %s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Force relay of transactions from whitelisted peers even if they violate local relay policy (default: %d)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>If paytxfee is not set, include enough fee so transactions begin confirmation on average within n blocks (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Invalid amount for -maxtxfee=&lt;amount&gt;: &apos;%s&apos; (must be at least the minrelay fee of %s to prevent stuck transactions)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Query for peer addresses via DNS lookup, if low on addresses (default: 1 unless -connect)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Randomize credentials for every proxy connection. This enables Tor stream isolation (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Set maximum size of high-priority/low-fee transactions in bytes (default: %d)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Sets the serialization of raw transaction or block hex returned in non-verbose mode, non-segwit(0) or segwit(1) (default: %d)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>The transaction amount is too small to send after the fee has been deducted</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>This product includes software developed by the OpenSSL Project for use in the OpenSSL Toolkit &lt;https://www.openssl.org/&gt; and cryptographic software written by Eric Young and UPnP software written by Thomas Bernard.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Use hierarchical deterministic key generation (HD) after BIP32. Only has effect during wallet creation/first start</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Whitelist peers connecting from the given IP address (e.g. 1.2.3.4) or CIDR notated network (e.g. 1.2.3.0/24). Can be specified multiple times.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Whitelisted peers cannot be DoS banned and their transactions are always relayed, even if they are already in the mempool, useful e.g. for a gateway</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>You need to rebuild the database using -reindex to go back to unpruned mode.  This will redownload the entire blockchain</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>(default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Accept public REST requests (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Automatically create Tor hidden service (default: %d)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Connect through SOCKS5 proxy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Error reading from database, shutting down.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Imports blocks from external blk000??.dat file on startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Information</source>
        <translation>Information</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Invalid amount for -paytxfee=&lt;amount&gt;: &apos;%s&apos; (must be at least %s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Invalid netmask specified in -whitelist: &apos;%s&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Keep at most &lt;n&gt; unconnectable transactions in memory (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Need to specify a port with -whitebind: &apos;%s&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Node relay options:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>RPC server options:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Reducing -maxconnections from %d to %d, because of system limitations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Rescan the block chain for missing wallet transactions on startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Send trace/debug info to console instead of debug.log file</source>
        <translation>Send trace/debug info to console instead of debug.log file</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Send transactions as zero-fee transactions if possible (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Show all debugging options (usage: --help -help-debug)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Shrink debug.log file on client startup (default: 1 when no -debug)</source>
        <translation>Shrink debug.log file on client startup (default: 1 when no -debug)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Signing transaction failed</source>
        <translation>Signing transaction failed</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>The transaction amount is too small to pay the fee</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This is experimental software.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Tor control port password (default: empty)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Tor control port to use if onion listening enabled (default: %s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Transaction amount too small</source>
        <translation>Transaction amount too small</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Transaction amounts must be positive</source>
        <translation>Transaction amounts must be positive</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Transaction too large for fee policy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Transaction too large</source>
        <translation>Transaction too large</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to bind to %s on this computer (bind returned error %s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Upgrade wallet to latest format on startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Username for JSON-RPC connections</source>
        <translation>Username for JSON-RPC connections</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Warning</source>
        <translation>Warning</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Warning: unknown new rules activated (versionbit %i)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Whether to operate in a blocks only mode (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Zapping all transactions from wallet...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>ZeroMQ notification options:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-67"/>
        <source>Password for JSON-RPC connections</source>
        <translation>Password for JSON-RPC connections</translation>
    </message>
    <message>
        <location line="-221"/>
        <source>Execute command when the best block changes (%s in cmd is replaced by block hash)</source>
        <translation>Execute command when the best block changes (%s in cmd is replaced by block hash)</translation>
    </message>
    <message>
        <location line="+146"/>
        <source>Allow DNS lookups for -addnode, -seednode and -connect</source>
        <translation>Allow DNS lookups for -addnode, -seednode and -connect</translation>
    </message>
    <message>
        <location line="+61"/>
        <source>Loading addresses...</source>
        <translation>Loading addresses...</translation>
    </message>
    <message>
        <location line="-280"/>
        <source>(1 = keep tx meta data e.g. account owner and payment request information, 2 = drop tx meta data)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>-maxtxfee is set very high! Fees this large could be paid on a single transaction.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>-paytxfee is set very high! This is the transaction fee you will pay if you send a transaction.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+47"/>
        <source>Do not keep transactions in the mempool longer than &lt;n&gt; hours (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Fees (in %s/kB) smaller than this are considered zero fee for transaction creation (default: %s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>How thorough the block verification of -checkblocks is (0-4, default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Maintain a full transaction index, used by the getrawtransaction rpc call (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Number of seconds to keep misbehaving peers from reconnecting (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Output debugging information (default: %u, supplying &lt;category&gt; is optional)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Support filtering of blocks and transaction with bloom filters (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Total length of network version string (%i) exceeds maximum length (%i). Reduce the number or size of uacomments.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Tries to keep outbound traffic under the given target (in MiB per 24h), 0 = no limit (default: %d)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unsupported argument -socks found. Setting SOCKS version isn&apos;t possible anymore, only SOCKS5 proxies are supported.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Unsupported argument -whitelistalwaysrelay ignored, use -whitelistrelay and/or -whitelistforcerelay.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Use separate SOCKS5 proxy to reach peers via Tor hidden services (default: %s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Username and hashed password for JSON-RPC connections. The field &lt;userpw&gt; comes in the format: &lt;USERNAME&gt;:&lt;SALT&gt;$&lt;HASH&gt;. A canonical python script is included in share/rpcuser. This option can be specified multiple times</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Warning: Wallet file corrupt, data salvaged! Original %s saved as %s in %s; if your balance or transactions are incorrect you should restore from a backup.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>(default: %s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Always query for peer addresses via DNS lookup (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+40"/>
        <source>How many blocks to check at startup (default: %u, 0 = all)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Include IP addresses in debug output (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Invalid -proxy address: &apos;%s&apos;</source>
        <translation>Invalid -proxy address: &apos;%s&apos;</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Listen for JSON-RPC connections on &lt;port&gt; (default: %u or testnet: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Listen for connections on &lt;port&gt; (default: %u or testnet: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Maintain at most &lt;n&gt; connections to peers (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Make the wallet broadcast transactions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Maximum per-connection receive buffer, &lt;n&gt;*1000 bytes (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Maximum per-connection send buffer, &lt;n&gt;*1000 bytes (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Prepend debug output with timestamp (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Relay non-P2SH multisig (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Set key pool size to &lt;n&gt; (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Set the number of threads to service RPC calls (default: %d)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Specify configuration file (default: %s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Specify connection timeout in milliseconds (minimum: 1, default: %d)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Specify pid file (default: %s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Spend unconfirmed change when sending transactions (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Threshold for disconnecting misbehaving peers (default: %u)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Unknown network specified in -onlynet: &apos;%s&apos;</source>
        <translation>Unknown network specified in -onlynet: &apos;%s&apos;</translation>
    </message>
    <message>
        <location line="-76"/>
        <source>Insufficient funds</source>
        <translation>Insufficient funds</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Loading block index...</source>
        <translation>Loading block index...</translation>
    </message>
    <message>
        <location line="-64"/>
        <source>Add a node to connect to and attempt to keep the connection open</source>
        <translation>Add a node to connect to and attempt to keep the connection open</translation>
    </message>
    <message>
        <location line="+65"/>
        <source>Loading wallet...</source>
        <translation>Loading wallet...</translation>
    </message>
    <message>
        <location line="-57"/>
        <source>Cannot downgrade wallet</source>
        <translation>Cannot downgrade wallet</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Cannot write default address</source>
        <translation>Cannot write default address</translation>
    </message>
    <message>
        <location line="+79"/>
        <source>Rescanning...</source>
        <translation>Rescanning...</translation>
    </message>
    <message>
        <location line="-67"/>
        <source>Done loading</source>
        <translation>Done loading</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Error</source>
        <translation>Error</translation>
    </message>
</context>
</TS>
