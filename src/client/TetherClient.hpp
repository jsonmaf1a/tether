#pragma once

#include "core/Peer.hpp"
#include "core/PeerDiscovery.hpp"
#include <expected>
#include <system_error>
#include <vector>

class TetherClient
{
    public:
        TetherClient(PeerDiscovery pd) : pd(std::move(pd)) {};

        std::expected<void, std::error_code> run(uint16_t tcpPort);
        std::vector<Peer> peers();
        std::expected<void, std::error_code> connect(const Peer &peer);

    private:
        PeerDiscovery pd;
};
