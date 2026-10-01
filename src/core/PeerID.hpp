#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

struct PeerID
{
    uint32_t value;
    bool operator==(const PeerID &) const = default;

    static std::optional<PeerID> parse(std::string_view value);
};
