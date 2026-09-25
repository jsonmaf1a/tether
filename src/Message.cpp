#include "Message.hpp"
#include <cstring>
#include <expected>
#include <netinet/in.h>

std::expected<Message, std::error_code> Message::fromBytes(std::span<std::byte> bytes)
{
    std::size_t offset = 0;

    if (bytes.size_bytes() < MESSAGE_SIZE)
        return std::unexpected(std::make_error_code(std::errc::bad_message));

    uint8_t version;
    std::memcpy(&version, bytes.data() + offset, sizeof(version));
    offset += sizeof(version);

    uint8_t type;
    std::memcpy(&type, bytes.data() + offset, sizeof(type));
    offset += sizeof(type);

    uint16_t port;
    std::memcpy(&port, bytes.data() + offset, sizeof(port));
    port = ntohs(port);
    offset += sizeof(port);

    uint32_t peerId;
    std::memcpy(&peerId, bytes.data() + offset, sizeof(peerId));
    peerId = ntohl(peerId);

    return Message{version, type, port, peerId};
};

std::array<std::byte, MESSAGE_SIZE> Message::toBytes(const Message &msg)
{
    std::size_t offset = 0;
    std::array<std::byte, MESSAGE_SIZE> payload{};

    std::memcpy(payload.data() + offset, &msg.version, sizeof(msg.version));
    offset += sizeof(msg.version);

    std::memcpy(payload.data() + offset, &msg.type, sizeof(msg.type));
    offset += sizeof(msg.type);

    uint16_t port = htons(msg.port);
    std::memcpy(payload.data() + offset, &port, sizeof(port));
    offset += sizeof(msg.port);

    uint32_t peerId = htonl(msg.peerId);
    std::memcpy(payload.data() + offset, &peerId, sizeof(peerId));

    return payload;
}
