#!/usr/bin/env python3
# Copyright (c) 2015-2016 The Freicoin developers
# Copyright (c) 2010-2019 The Freicoin Developers
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
# Test ZMQ interface
#

from test_framework.test_framework import FreicoinTestFramework
from test_framework.util import *
import zmq
import struct

import http.client
import urllib.parse

class ZMQTest (FreicoinTestFramework):

    port = 28332

    def setup_nodes(self):
        self.zmqContext = zmq.Context()
        self.zmqSubSocket = self.zmqContext.socket(zmq.SUB)
        self.zmqSubSocket.setsockopt(zmq.SUBSCRIBE, b"hashblock")
        self.zmqSubSocket.setsockopt(zmq.SUBSCRIBE, b"hashtx")
        self.zmqSubSocket.connect("tcp://127.0.0.1:%i" % self.port)
        return start_nodes(4, self.options.tmpdir, extra_args=[
            ['-zmqpubhashtx=tcp://127.0.0.1:'+str(self.port), '-zmqpubhashblock=tcp://127.0.0.1:'+str(self.port)],
            [],
            [],
            []
            ])

    def run_test(self):
        self.sync_all()

        genhashes = self.nodes[0].generate(1)
        self.sync_all()

        print("listen...")
        msg = self.zmqSubSocket.recv_multipart()
        topic = msg[0]
        body = msg[1]

        msg = self.zmqSubSocket.recv_multipart()
        topic = msg[0]
        body = msg[1]
        blkhash = bytes_to_hex_str(body)

        assert_equal(genhashes[0], blkhash) #blockhash from generate must be equal to the hash received over zmq

        n = 10
        genhashes = self.nodes[1].generate(n)
        self.sync_all()

        zmqHashes = []
        for x in range(0,n*2):
            msg = self.zmqSubSocket.recv_multipart()
            topic = msg[0]
            body = msg[1]
            if topic == b"hashblock":
                zmqHashes.append(bytes_to_hex_str(body))

        for x in range(0,n):
            assert_equal(genhashes[x], zmqHashes[x]) #blockhash from generate must be equal to the hash received over zmq

        #test tx from a second node
        hashRPC = self.nodes[1].sendtoaddress(self.nodes[0].getnewaddress(), 1.0)
        self.sync_all()

        # now we should receive a zmq msg because the tx was broadcast
        msg = self.zmqSubSocket.recv_multipart()
        topic = msg[0]
        body = msg[1]
        hashZMQ = ""
        if topic == b"hashtx":
            hashZMQ = bytes_to_hex_str(body)

        assert_equal(hashRPC, hashZMQ) #blockhash from generate must be equal to the hash received over zmq


if __name__ == '__main__':
    ZMQTest ().main ()