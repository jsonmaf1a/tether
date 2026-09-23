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
        .version = 1,
        .type = PeerDiscoveryMessageType::Req,
        .port = port,
        .peerId = generatePeerId(),
    };
    socket.send(serializeMsg(msg), broadcast);

}

PeerDiscoveryMessage PeerDiscovery::parseMsg(std::span<std::byte> payload) {
    if (payload.size_bytes() < BUFFER_SIZE) {
        // TODO: handle error
        std::println("Invalid message");
    }

    uint8_t version;
    std::memcpy(&version, payload.data(), sizeof(version));

    uint8_t type;
    std::memcpy(&type, payload.data() + 1, sizeof(type));

    uint16_t port;
    std::memcpy(&port, payload.data() + 2, sizeof(port));
    port = ntohs(port);

    uint32_t peerId;
    std::memcpy(&peerId, payload.data() + 4, sizeof(peerId));
    peerId = ntohl(peerId);

    return {version, type, port, peerId};
};

std::array<std::byte, BUFFER_SIZE> PeerDiscovery::serializeMsg(const PeerDiscoveryMessage& msg) {
    std::array<std::byte, BUFFER_SIZE> payload{};

    std::memcpy(payload.data(), &msg.version, sizeof(msg.version));
    std::memcpy(payload.data() + 1, &msg.type,    sizeof(msg.type));

    uint16_t port = htons(msg.port);
    std::memcpy(payload.data() + 2, &port, sizeof(port));

    uint32_t peerId = htonl(msg.peerId);
    std::memcpy(payload.data() + 4, &peerId, sizeof(peerId));

    return payload;
}

uint32_t PeerDiscovery::generatePeerId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    return gen();
};
