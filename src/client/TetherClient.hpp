#pragma once

#include "core/Peer.hpp"
#include <cstdint>
#include <vector>

// TODO:
// - implement methods
class TetherClient
{
    public:
        std::vector<Peer> discover(uint16_t port);
        std::vector<Peer> peers();
        void connect(const Peer &peer);
        // Status status();
};
