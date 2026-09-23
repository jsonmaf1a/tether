#include "PeerDiscovery.hpp"
#include "Peer.hpp"
#include "Socket.hpp"
#include <cstdint>
#include <cstring>
#include <print>
#include <random>
#include <sys/socket.h>

std::vector<Peer> PeerDiscovery::discover() {
    return {};
};

void PeerDiscovery::read() {
    std::array<std::byte, BUFFER_SIZE> payload{};
    socket.receive(payload, broadcast);

    auto msg = parseMsg(payload);

    std::println("version: {}\ntype: {}\nport: {}\npeer id: {}",msg.version, msg.type,msg.port,msg.peerId);
}

void PeerDiscovery::write() {
    PeerDiscoveryMessage msg = {
        .version = 1, // TODO: add versioning
        .type = PeerDiscoveryMessageType::Req,
        .port = port,
        .peerId = generatePeerId(),
    };
    socket.send(serializeMsg(msg), broadcast);

}

PeerDiscoveryMessage PeerDiscovery::parseMsg(std::span<std::byte> payload) {
    std::size_t offset = 0;

    if (payload.size_bytes() < BUFFER_SIZE) {
        // TODO: handle error
        std::println("Invalid message");
    }

    uint8_t version;
    std::memcpy(&version, payload.data() + offset, sizeof(version));
    offset += sizeof(version);

    uint8_t type;
    std::memcpy(&type, payload.data() + offset, sizeof(type));
    offset += sizeof(type);

    uint16_t port;
    std::memcpy(&port, payload.data() + offset, sizeof(port));
    port = ntohs(port);
    offset += sizeof(port);

    uint32_t peerId;
    std::memcpy(&peerId, payload.data() + offset, sizeof(peerId));
    peerId = ntohl(peerId);

    return {version, type, port, peerId};
};

std::array<std::byte, BUFFER_SIZE> PeerDiscovery::serializeMsg(const PeerDiscoveryMessage& msg) {
    std::size_t offset = 0;
    std::array<std::byte, BUFFER_SIZE> payload{};

    std::memcpy(payload.data() + offset, &msg.version, sizeof(msg.version));
    offset += sizeof(msg.version);

    std::memcpy(payload.data() + offset, &msg.type,    sizeof(msg.type));
    offset += sizeof(msg.type);

    uint16_t port = htons(msg.port);
    std::memcpy(payload.data() + offset, &port, sizeof(port));
    offset += sizeof(msg.port);

    uint32_t peerId = htonl(msg.peerId);
    std::memcpy(payload.data() + offset, &peerId, sizeof(peerId));

    return payload;
}

uint32_t PeerDiscovery::generatePeerId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    return gen();
};
