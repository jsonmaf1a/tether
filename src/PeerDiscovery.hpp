#pragma once

#include "Peer.hpp"
#include "UDPSocket.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
#include <chrono>

class PeerDiscovery
{
    public:
        PeerDiscovery(uint16_t port) : socket(port), port(port)
        {
            broadcast.sin_family = AF_INET;
            broadcast.sin_port = htons(port);
            broadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);

            ownPeerId = generatePeerId();
        };

        void discover(std::chrono::milliseconds timeout);
        void announceSelf();

    private:
        UDPSocket socket;
        uint16_t port;
        sockaddr_in broadcast;
        std::vector<Peer> peers;
        uint32_t ownPeerId;

        uint32_t generatePeerId();
};
