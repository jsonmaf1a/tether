#pragma once

#include "Peer.hpp"
#include "UDPSocket.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>

struct PeerDiscoveryMessage {
    uint8_t version;
    uint8_t type;
    uint16_t port;
    uint32_t peerId;
};

enum PeerDiscoveryMessageType {
    Req = 0,
    Res = 1
};

constexpr int DISCOVERY_TIMEOUT = 500;

class PeerDiscovery {
    public:
        PeerDiscovery(uint16_t port): socket(port), port(port) {
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

        PeerDiscoveryMessage parseMsg(std::span<std::byte> payload);
        std::array<std::byte, BUFFER_SIZE> serializeMsg(const PeerDiscoveryMessage& msg);
        uint32_t generatePeerId();
};
