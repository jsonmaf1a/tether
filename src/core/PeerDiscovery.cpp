#include "PeerDiscovery.hpp"
#include "Message.hpp"
#include "core/Endpoint.hpp"
#include <netinet/in.h>
#include <print>
#include <random>

std::expected<void, std::error_code> PeerDiscovery::discover()
{
    auto start = std::chrono::steady_clock::now();

    while(true)
    {
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start);

        auto remaining = DISCOVERY_TIMEOUT - elapsed;

        if(remaining <= std::chrono::milliseconds::zero())
            break;

        if(!socket.waitUntilReadable(remaining))
            break;

        std::array<std::byte, MESSAGE_SIZE> payload{};

        sockaddr_in src{};
        auto res = socket.receive(payload, src);

        if(!res)
            return std::unexpected(res.error());

        if(*res != MESSAGE_SIZE)
            continue;

        auto msg = Message::fromBytes(payload);
        if(!msg)
            continue;

        if(msg->peerId == ownPeerId)
            continue;

        Endpoint peerAddress(src);
        Peer peer{.id = msg->peerId, .address = peerAddress, .tcpPort = msg->port};

        std::println("Discovered peer {} at {}:{}", peer.id.value, peerAddress.getIPString(),
                     peer.tcpPort);

        peers.insert_or_assign(peer.id.value, peer);
        removeExpiredPeers();
    }

    return {};
};

std::expected<void, std::error_code> PeerDiscovery::announceSelf(uint16_t tcpPort)
{
    Message msg = {
        .version = 1, // TODO: add versioning
        .type = MessageType::Announcement,
        .port = tcpPort,
        .peerId = ownPeerId,
    };

    auto res = socket.send(Message::toBytes(msg), broadcast);
    if(!res)
        return std::unexpected(res.error());

    std::println("Peer {} available to discovery", msg.peerId.value);

    return {};
};

void PeerDiscovery::removeExpiredPeers()
{
    auto now = std::chrono::steady_clock::now();

    for(auto it = peers.begin(); it != peers.end();)
    {
        if(now - it->second.lastSeen > PEER_TIMEOUT)
            it = peers.erase(it);
        else
            ++it;
    }
}

uint32_t PeerDiscovery::generatePeerId()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    return gen();
};
