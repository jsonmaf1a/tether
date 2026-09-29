#pragma once

#include <cstdint>

struct PeerID
{
    uint32_t value;
    bool operator==(const PeerID &) const = default;
};
