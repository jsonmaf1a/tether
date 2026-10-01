#include "TetherClient.hpp"
#include <expected>

// TODO: pass tcp port from cli
std::expected<void, std::error_code> TetherClient::run()
{
    while(true)
    {
        // TODO: handle transient network errors (e.g. interface reconnect) with retry instead of
        // terminating

        if(auto res = pd.announceSelf(1818); !res)
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
