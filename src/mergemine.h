// Copyright (c) 2020 The Freicoin Developers
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0.  If a copy of the MPL was not distributed with this file, you can
// obtain one at <https://mozilla.org/MPL/2.0/>.

#ifndef BITCOIN_MERGEMINE_H
#define BITCOIN_MERGEMINE_H

/** Configure the merge-mining subsystem.  This involved setting up some global
 ** state and spawning initialization and management threads. */
bool InitMergeMining();

/** Interrupt any active network connections. */
void InterruptMergeMining();

/** Cleanup network connections made by the merge-mining subsystem, free
 ** associated resources, and cleanup global state. */
void StopMergeMining();

#endif // BITCOIN_MERGEMINE_H

// End of File
