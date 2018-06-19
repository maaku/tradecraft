#!/usr/bin/env python3
# Copyright (c) 2014-2016 The Freicoin developers
# Copyright (c) 2010-2021 The Freicoin Developers
#
# This program is free software: you can redistribute it and/or modify
# it under the conjunctive terms of BOTH version 3 of the GNU Affero
# General Public License as published by the Free Software Foundation
# AND the MIT/X11 software license.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Affero General Public License and the MIT/X11 software license for
# more details.
#
# You should have received a copy of both licenses along with this
# program.  If not, see <https://www.gnu.org/licenses/> and
# <http://www.opensource.org/licenses/mit-license.php>

#
# Test re-org scenarios with a mempool that contains transactions
# that spend (directly or indirectly) coinbase transactions.
#

from test_framework.test_framework import FreicoinTestFramework
from test_framework.util import *

# Create one-input, one-output, no-fee transaction:
class MempoolCoinbaseTest(FreicoinTestFramework):
    def __init__(self):
        super().__init__()
        self.num_nodes = 2
        self.setup_clean_chain = False

    alert_filename = None  # Set by setup_network

    def setup_network(self):
        args = ["-checkmempool", "-debug=mempool"]
        self.nodes = []
        self.nodes.append(start_node(0, self.options.tmpdir, args))
        self.nodes.append(start_node(1, self.options.tmpdir, args))
        connect_nodes(self.nodes[1], 0)
        self.is_network_split = False
        self.sync_all()

    def run_test(self):
        start_count = self.nodes[0].getblockcount()

        # Mine three blocks. After this, nodes[0] blocks
        # 101, 102, and 103 are spend-able.
        new_blocks = self.nodes[1].generate(4)
        self.sync_all()

        node0_address = self.nodes[0].getnewaddress()
        node1_address = self.nodes[1].getnewaddress()

        # Three scenarios for re-orging coinbase spends in the memory pool:
        # 1. Direct coinbase spend  :  spend_101
        # 2. Indirect (coinbase spend in chain, child in mempool) : spend_102 and spend_102_1
        # 3. Indirect (coinbase and child both in chain) : spend_103 and spend_103_1
        # Use invalidatblock to make all of the above coinbase spends invalid (immature coinbase),
        # and make sure the mempool code behaves correctly.
        b = [ self.nodes[0].getblockhash(n) for n in range(101, 105) ]
        coinbase_txids = [ self.nodes[0].getblock(h)['tx'][0] for h in b ]
        spend_101_raw = create_tx(self.nodes[0], coinbase_txids[1], node1_address, 49.99)
        spend_102_raw = create_tx(self.nodes[0], coinbase_txids[2], node0_address, 49.99)
        spend_103_raw = create_tx(self.nodes[0], coinbase_txids[3], node0_address, 49.99)

        # Create a block-height-locked transaction which will be invalid after reorg
        timelock_tx = self.nodes[0].createrawtransaction([{"txid": coinbase_txids[0], "vout": 0}], {node0_address: 49.99})
        # Set the time lock
        timelock_tx = timelock_tx.replace("ffffffff", "feffffff", 1)
        timelock_tx = timelock_tx[:-16] + hex(self.nodes[0].getblockcount() + 2)[2:] + "000000" + timelock_tx[-8:]
        timelock_tx = self.nodes[0].signrawtransaction(timelock_tx)["hex"]
        assert_raises(JSONRPCException, self.nodes[0].sendrawtransaction, timelock_tx)

        # Broadcast and mine spend_102 and 103:
        spend_102_id = self.nodes[0].sendrawtransaction(spend_102_raw)
        spend_103_id = self.nodes[0].sendrawtransaction(spend_103_raw)
        self.nodes[0].generate(1)
        assert_raises(JSONRPCException, self.nodes[0].sendrawtransaction, timelock_tx)

        # Create 102_1 and 103_1:
        spend_102_1_raw = create_tx(self.nodes[0], spend_102_id, node1_address, 49.98)
        spend_103_1_raw = create_tx(self.nodes[0], spend_103_id, node1_address, 49.98)

        # Broadcast and mine 103_1:
        spend_103_1_id = self.nodes[0].sendrawtransaction(spend_103_1_raw)
        last_block = self.nodes[0].generate(1)
        timelock_tx_id = self.nodes[0].sendrawtransaction(timelock_tx)

        # ... now put spend_101 and spend_102_1 in memory pools:
        spend_101_id = self.nodes[0].sendrawtransaction(spend_101_raw)
        spend_102_1_id = self.nodes[0].sendrawtransaction(spend_102_1_raw)

        self.sync_all()

        assert_equal(set(self.nodes[0].getrawmempool()), {spend_101_id, spend_102_1_id, timelock_tx_id})

        for node in self.nodes:
            node.invalidateblock(last_block[0])
        assert_equal(set(self.nodes[0].getrawmempool()), {spend_101_id, spend_102_1_id, spend_103_1_id})

        # Use invalidateblock to re-org back and make all those coinbase spends
        # immature/invalid:
        for node in self.nodes:
            node.invalidateblock(new_blocks[0])

        self.sync_all()

        # mempool should be empty.
        assert_equal(set(self.nodes[0].getrawmempool()), set())

if __name__ == '__main__':
    MempoolCoinbaseTest().main()
