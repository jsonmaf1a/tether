#pragma once

#include <cstdint>
#include <span>
#include <expected>
#include <system_error>

constexpr int MESSAGE_SIZE = 8;

struct Message
{
    uint8_t version;
    uint8_t type;
    uint16_t port;
    uint32_t peerId;

    static std::expected<Message, std::error_code> fromBytes(std::span<std::byte> bytes);
    static std::array<std::byte, MESSAGE_SIZE> toBytes(const Message &msg);
};

enum MessageType : uint8_t
{
    Announcement = 0,
};
