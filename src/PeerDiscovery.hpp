#pragma once

#include "Peer.hpp"
#include "UDPSocket.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>

constexpr int DISCOVERY_TIMEOUT = 500;

class PeerDiscovery
{
    public:
        PeerDiscovery(uint16_t port) : socket(port), port(port)
        {
            broadcast.sin_family = AF_INET;
            broadcast.sin_port = htons(port);
            broadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        };

        std::vector<Peer> discover();
        void read();
        void write();

    private:
        UDPSocket socket;
        uint16_t port;
        sockaddr_in broadcast;

        uint32_t generatePeerId();
};
