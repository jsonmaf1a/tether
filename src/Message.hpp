#pragma once

#include <cstdint>
#include <span>

constexpr int MESSAGE_SIZE = 8;

struct Message
{
    uint8_t version;
    uint8_t type;
    uint16_t port;
    uint32_t peerId;

    static Message fromBytes(std::span<std::byte> bytes);
    static std::array<std::byte, MESSAGE_SIZE> toBytes(const Message &msg);
};

enum MessageType : uint8_t
{
    Announcement = 0,
};
