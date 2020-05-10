// Copyright (c) 2020 The Freicoin Developers
//
// This program is free software: you can redistribute it and/or
// modify it under the conjunctive terms of BOTH version 3 of the GNU
// Affero General Public License as published by the Free Software
// Foundation AND the MIT/X11 software license.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Affero General Public License and the MIT/X11 software license for
// more details.
//
// You should have received a copy of both licenses along with this
// program.  If not, see <https://www.gnu.org/licenses/> and
// <http://www.opensource.org/licenses/mit-license.php>

#include "stratum.h"

#include "chainparams.h"
#include "httpserver.h"
#include "netbase.h"
#include "util.h"

#include <string>
#include <vector>

#include <boost/foreach.hpp>

#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <errno.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

struct StratumClient
{
    evconnlistener* m_listener;
    evutil_socket_t m_socket;
    bufferevent* m_bev;
    CService m_from;

    CService GetPeer() const
      { return m_from; }

    StratumClient() : m_listener(0), m_socket(0), m_bev(0) { }
    StratumClient(evconnlistener* listener, evutil_socket_t socket, bufferevent* bev, CService from) : m_listener(listener), m_socket(socket), m_bev(bev), m_from(from) { }
};

//! List of subnets to allow stratum connections from
static std::vector<CSubNet> stratum_allow_subnets;

//! Bound stratum listening sockets
static std::map<evconnlistener*, CService> bound_listeners;

//! Active miners connected to us
static std::map<bufferevent*, StratumClient> subscriptions;

/** Callback to read from a stratum connection. */
static void stratum_read_cb(bufferevent *bev, void *ctx)
{
    evconnlistener *listener = (evconnlistener*)ctx;
    // Lookup the client record for this connection
    if (!subscriptions.count(bev)) {
        LogPrint("stratum", "Received read notification for unknown stratum connection 0x%x\n", (size_t)bev);
        return;
    }
    StratumClient& client = subscriptions[bev];
    LogPrint("stratum", "Received data from stratum connection %s\n", client.GetPeer().ToString());
}

/** Callback to handle unrecoverable errors in a stratum link. */
static void stratum_event_cb(bufferevent *bev, short what, void *ctx)
{
    evconnlistener *listener = (evconnlistener*)ctx;
    // Fetch the return address for this connection, for the debug log.
    std::string from("UNKNOWN");
    if (!subscriptions.count(bev)) {
        LogPrint("stratum", "Received event notification for unknown stratum connection 0x%x\n", (size_t)bev);
        return;
    } else {
        from = subscriptions[bev].GetPeer().ToString();
    }
    // Report the reason why we are closing the connection.
    if (what & BEV_EVENT_ERROR) {
        LogPrint("stratum", "Error detected on stratum connection from %s\n", from);
    }
    if (what & BEV_EVENT_EOF) {
        LogPrint("stratum", "Remote disconnect received on stratum connection from %s\n", from);
    }
    // Remove the connection from our records, and tell libevent to
    // disconnect and free its resources.
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        LogPrint("stratum", "Closing stratum connection from %s\n", from);
        subscriptions.erase(bev);
        if (bev) {
            bufferevent_free(bev);
            bev = NULL;
        }
    }
}

/** Callback to accept a stratum connection. */
static void stratum_accept_conn_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *address, int socklen, void *ctx)
{
    // Parse the return address
    CService from;
    from.SetSockAddr(address);
    // Early address-based allow check
    if (!ClientAllowed(stratum_allow_subnets, from)) {
        evconnlistener_free(listener);
        LogPrint("stratum", "Rejected connection from disallowed subnet: %s\n", from.ToString());
        return;
    }
    // Should be the same as EventBase(), but let's get it the
    // official way.
    event_base *base = evconnlistener_get_base(listener);
    // Create a buffer for sending/receiving from this connection.
    bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    // Disable Nagle's algorithm, so that TCP packets are sent
    // immediately, even if it results in a small packet.
    int one = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    // Setup the read and event callbacks to handle receiving requests
    // from the miner and error handling.  A write callback isn't
    // needed because we're not sending enough data to fill buffers.
    bufferevent_setcb(bev, stratum_read_cb, NULL, stratum_event_cb, (void*)listener);
    // Enable bidirectional communication on the connection.
    bufferevent_enable(bev, EV_READ|EV_WRITE);
    // Record the connection state
    subscriptions[bev] = StratumClient(listener, fd, bev, from);
    // Log the connection.
    LogPrint("stratum", "Accepted stratum connection from %s\n", from.ToString());
}

/** Setup the stratum connection listening services */
bool StratumBindAddresses(event_base* base)
{
    int defaultPort = GetArg("-stratumport", BaseParams().StratumPort());
    std::vector<std::pair<std::string, uint16_t> > endpoints;

    // Determine what addresses to bind to
    if (!InitEndpointList("stratum", defaultPort, endpoints))
        return false;

    // Bind each addresses
    for (std::vector<std::pair<std::string, uint16_t> >::iterator i = endpoints.begin(); i != endpoints.end(); ++i) {
        LogPrint("stratum", "Binding stratum on address %s port %i\n", i->first, i->second);
        // Use CService to translate string -> sockaddr
        CService socket(CNetAddr(i->first), i->second);
        union {
            sockaddr     ipv4;
            sockaddr_in6 ipv6;
        } addr;
        socklen_t len = sizeof(addr);
        socket.GetSockAddr((sockaddr*)&addr, &len);
        // Setup an event listener for the endpoint
        evconnlistener *listener = evconnlistener_new_bind(base, stratum_accept_conn_cb, NULL, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1, (sockaddr*)&addr, len);
        // Only record successful binds
        if (listener) {
            bound_listeners[listener] = socket;
        } else {
            LogPrintf("Binding stratum on address %s port %i failed. (Reason: %d, '%s')\n", i->first, i->second, errno, evutil_socket_error_to_string(errno));
        }
    }

    return !bound_listeners.empty();
}

/** Configure the stratum server */
bool InitStratumServer()
{
    if (!InitSubnetAllowList("stratum", stratum_allow_subnets)) {
        LogPrint("stratum", "Unable to bind stratum server to an endpoint.\n");
        return false;
    }

    std::string strAllowed;
    BOOST_FOREACH (const CSubNet& subnet, stratum_allow_subnets)
        strAllowed += subnet.ToString() + " ";
    LogPrint("stratum", "Allowing stratum connections from: %s\n", strAllowed);

    event_base* base = EventBase();
    if (!base) {
        LogPrint("stratum", "No event_base object, cannot setup stratum server.\n");
        return false;
    }

    if (!StratumBindAddresses(base)) {
        LogPrintf("Unable to bind any endpoint for stratum server\n");
    } else {
        LogPrint("stratum", "Initialized stratum server\n");
    }

    return true;
}

/** Interrupt the stratum server connections */
void InterruptStratumServer()
{
    // Stop listening for connections on stratum sockets
    typedef std::pair<evconnlistener*, CService> binding_type;
    BOOST_FOREACH (binding_type binding, bound_listeners) {
        LogPrint("stratum", "Interrupting stratum service on %s\n", binding.second.ToString());
        evconnlistener_disable(binding.first);
    }
}

/** Cleanup stratum server network connections and free resources. */
void StopStratumServer()
{
    /* Tear-down active connections. */
    typedef std::pair<bufferevent*, StratumClient> subscription_type;
    BOOST_FOREACH (subscription_type subscription, subscriptions) {
        LogPrint("stratum", "Closing stratum server connection to %s due to process termination\n", subscription.second.GetPeer().ToString());
        bufferevent_free(subscription.first);
    }
    subscriptions.clear();
    /* Un-bind our listeners from their network interfaces. */
    typedef std::pair<evconnlistener*, CService> binding_type;
    BOOST_FOREACH (binding_type binding, bound_listeners) {
        LogPrint("stratum", "Removing stratum server binding on %s\n", binding.second.ToString());
        evconnlistener_free(binding.first);
    }
    bound_listeners.clear();
}

// End of File
