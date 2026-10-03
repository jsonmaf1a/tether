#include "TetherClient.hpp"
#include <cstdint>
#include <expected>

std::expected<void, std::error_code> TetherClient::run(uint16_t tcpPort)
{
    while(true)
    {
        // TODO:
        // - handle transient network errors (e.g. interface reconnect) with retry instead of
        // terminating
        // - start TCP listening on tcpPort

        if(auto res = pd.announceSelf(tcpPort); !res)
        {
            std::println("Error while announcing self: {}", res.error().message());
            break;
        }

        if(auto res = pd.discover(); !res)
        {
            std::println("Error while discovering: {}", res.error().message());
            break;
        }
    }

    return {};
};

std::vector<Peer> TetherClient::peers() {
    return pd.getPeers();
};

std::expected<void, std::error_code> TetherClient::connect(const Peer &peer) {
    // TODO: implement TetherClient::connect()
    return {};
};
