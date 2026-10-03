#pragma once

#include "Peer.hpp"
#include "PeerID.hpp"
#include "UDPSocket.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <expected>
#include <netinet/in.h>
#include <ranges>
#include <sys/socket.h>
#include <system_error>
#include <unordered_map>
#include <vector>

using namespace std::chrono_literals;

constexpr auto DISCOVERY_TIMEOUT = 2500ms;
constexpr auto PEER_TIMEOUT = 10s;

constexpr uint16_t BROADCAST_PORT = 6767;

class PeerDiscovery
{
    public:
        static std::expected<PeerDiscovery, std::error_code> create()
        {
            return UDPSocket::create().and_then(
                [](UDPSocket socket) -> std::expected<PeerDiscovery, std::error_code> {
                    if(auto result = socket.enableReuse(); !result)
                    {
                        return std::unexpected(result.error());
                    }
                    if(auto result = socket.enableBroadcast(); !result)
                    {
                        return std::unexpected(result.error());
                    }
                    if(auto result = socket.bind(BROADCAST_PORT); !result)
                    {
                        return std::unexpected(result.error());
                    }

                    sockaddr_in broadcast{};
                    broadcast.sin_family = AF_INET;
                    broadcast.sin_port = htons(BROADCAST_PORT);
                    broadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);

                    return PeerDiscovery(std::move(socket), broadcast);
                });
        }

        std::expected<void, std::error_code> discover();
        std::expected<void, std::error_code> announceSelf(uint16_t tcpPort);

        const std::vector<Peer> getPeers()
        {
            return peers | std::ranges::views::values | std::ranges::to<std::vector>();
        };

    private:
        explicit PeerDiscovery(UDPSocket socket, sockaddr_in broadcast)
            : socket(std::move(socket)), broadcast(broadcast), ownPeerId(generatePeerId())
        {}

        UDPSocket socket;
        sockaddr_in broadcast{};
        std::unordered_map<std::uint32_t, Peer> peers;
        PeerID ownPeerId;

        uint32_t generatePeerId();
        void removeExpiredPeers();
};
