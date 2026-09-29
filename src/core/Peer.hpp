#pragma once

#include "PeerID.hpp"
#include "core/Endpoint.hpp"
#include <chrono>
#include <cstdint>
#include <netinet/in.h>

struct Peer
{
    PeerID id;
    Endpoint address;
    std::uint16_t tcpPort;
    std::chrono::steady_clock::time_point lastSeen = std::chrono::steady_clock::now();
};
