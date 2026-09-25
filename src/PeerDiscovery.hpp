#pragma once

#include "Peer.hpp"
#include "UDPSocket.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <expected>
#include <netinet/in.h>
#include <sys/socket.h>
#include <system_error>
#include <unordered_map>
#include <chrono>

using namespace std::chrono_literals;

constexpr auto PEER_TIMEOUT = 10s;

class PeerDiscovery
{
    public:
        static std::expected<PeerDiscovery, std::error_code> create(uint16_t port) {
            return UDPSocket::create()
                .and_then([port](UDPSocket socket) -> std::expected<PeerDiscovery, std::error_code> {
                    if (auto r = socket.enableReuse(); !r) {
                        return std::unexpected(r.error());
                    }
                    if (auto r = socket.enableBroadcast(); !r) {
                        return std::unexpected(r.error());
                    }
                    if (auto r = socket.bind(port); !r) {
                        return std::unexpected(r.error());
                    }

                    sockaddr_in broadcast{};
                    broadcast.sin_family = AF_INET;
                    broadcast.sin_port = htons(port);
                    broadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);

                    return PeerDiscovery(std::move(socket), broadcast);
                });

        }

        std::expected<void, std::error_code> discover(std::chrono::milliseconds timeout);
        std::expected<void, std::error_code> announceSelf(uint16_t tcpPort);

    private:
        explicit PeerDiscovery(UDPSocket socket, sockaddr_in broadcast) : socket(std::move(socket)), broadcast(broadcast) {}

        UDPSocket socket;
        sockaddr_in broadcast{};
        std::unordered_map<std::uint32_t, Peer> peers;
        uint32_t ownPeerId;

        uint32_t generatePeerId();
        void removeExpiredPeers();
};
