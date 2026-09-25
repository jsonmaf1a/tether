#pragma once

#include <cstdint>
#include <netinet/in.h>

struct Peer
{
    std::uint32_t id;
    sockaddr_in address;
    std::uint16_t tcpPort;
};
