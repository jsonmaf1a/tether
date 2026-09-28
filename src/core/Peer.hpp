#pragma once

#include <chrono>
#include <cstdint>
#include <netinet/in.h>

struct Peer
{
    std::uint32_t id;
    sockaddr_in address;
    std::uint16_t tcpPort;
    std::chrono::steady_clock::time_point lastSeen = std::chrono::steady_clock::now();
};
