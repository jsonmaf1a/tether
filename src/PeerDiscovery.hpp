#pragma once

#include "Peer.hpp"
#include "UDPSocket.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unordered_map>
#include <chrono>

using namespace std::chrono_literals;

constexpr auto PEER_TIMEOUT = 10s;

class PeerDiscovery
{
    public:
        PeerDiscovery(uint16_t port) : port(port)
        {
            socket.enableReuse();
            socket.enableBroadcast();
            socket.bind(port);

            broadcast.sin_family = AF_INET;
            broadcast.sin_port = htons(port);
            broadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);

            ownPeerId = generatePeerId();
        };

        void discover(std::chrono::milliseconds timeout);
        void announceSelf(uint16_t tcpPort);

    private:
        UDPSocket socket;
        uint16_t port;
        sockaddr_in broadcast;
        std::unordered_map<std::uint32_t, Peer> peers;
        uint32_t ownPeerId;

        uint32_t generatePeerId();
        void removeExpiredPeers();
};
