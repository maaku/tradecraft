#!/usr/bin/env python3
# Copyright (c) 2016 The Freicoin developers
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
# address.py
#
# This file encodes and decodes BASE58 P2PKH and P2SH addresses
#

from .script import hash256, hash160, sha256, CScript, OP_0
from .util import bytes_to_hex_str, hex_str_to_bytes

chars = '123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz'

def byte_to_base58(b, version):
    result = ''
    str = bytes_to_hex_str(b)
    str = bytes_to_hex_str(chr(version).encode('latin-1')) + str
    checksum = bytes_to_hex_str(hash256(hex_str_to_bytes(str)))
    str += checksum[:8]
    value = int('0x'+str,0)
    while value > 0:
        result = chars[value % 58] + result
        value //= 58
    while (str[:2] == '00'):
        result = chars[0] + result
        str = str[2:]
    return result

# TODO: def base58_decode

def keyhash_to_p2pkh(hash, main = False):
    assert (len(hash) == 20)
    version = 0 if main else 111
    return byte_to_base58(hash, version)

def scripthash_to_p2sh(hash, main = False):
    assert (len(hash) == 20)
    version = 5 if main else 196
    return byte_to_base58(hash, version)

def key_to_p2pkh(key, main = False):
    key = check_key(key)
    return keyhash_to_p2pkh(hash160(key), main)

def script_to_p2sh(script, main = False):
    script = check_script(script)
    return scripthash_to_p2sh(hash160(script), main)

def key_to_p2sh_p2wpk(key, main = False):
    key = check_key(key)
    p2shscript = CScript([OP_0, ripemd160(hash256(bytes([0]) + CScript([key, CScriptOp(OP_CHECKSIG)])))])
    return script_to_p2sh(p2shscript, main)

def script_to_p2sh_p2wsh(script, main = False):
    script = check_script(script)
    p2shscript = CScript([OP_0, sha256(script)])
    return script_to_p2sh(p2shscript, main)

def check_key(key):
    if (type(key) is str):
        key = hex_str_to_bytes(key) # Assuming this is hex string
    if (type(key) is bytes and (len(key) == 33 or len(key) == 65)):
        return key
    assert(False)

def check_script(script):
    if (type(script) is str):
        script = hex_str_to_bytes(script) # Assuming this is hex string
    if (type(script) is bytes or type(script) is CScript):
        return script
    assert(False)