#include "PeerDiscovery.hpp"
#include "Message.hpp"
#include "Peer.hpp"
#include <cstdint>
#include <print>
#include <random>
#include <sys/socket.h>

std::vector<Peer> PeerDiscovery::discover() { return {}; };

void PeerDiscovery::read()
{
    std::array<std::byte, MESSAGE_SIZE> payload{};
    socket.receive(payload, broadcast);

    auto msg = Message::fromBytes(payload);

    std::println("version: {}\ntype: {}\nport: {}\npeer id: {}", msg.version, msg.type, msg.port, msg.peerId);
}

void PeerDiscovery::write()
{
    Message msg = {
        .version = 1, // TODO: add versioning
        .type = MessageType::Announcement,
        .port = port,
        .peerId = generatePeerId(),
    };

    socket.send(Message::toBytes(msg), broadcast);
}

uint32_t PeerDiscovery::generatePeerId()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    return gen();
};
