#include "PeerDiscovery.hpp"
#include <cstdint>
#include <print>
#include <system_error>
#include <unistd.h>
#include <utility>

using namespace std::chrono_literals;

// TODO:
// - cli

constexpr uint16_t BROADCAST_PORT = 6767;
constexpr auto DISCOVERY_TIMEOUT = 2500ms;

int main(int argc, char *argv[])
{
    // temporary
    std::string tcpPortValue;
    if (argc > 1)
    {
        std::string_view tcpPortArg = argv[1];

        if(tcpPortArg == "--port" && argv[2]) {
            tcpPortValue = argv[2];
        } else return 1;
    }

    uint16_t tcpPort = static_cast<uint16_t>(std::stoi(tcpPortValue));

    auto discovery = PeerDiscovery::create(BROADCAST_PORT);
    if(!discovery)
    {
        std::println("Error creating PeerDiscovery instance: {}", discovery.error().message());
        return 1;
    }

    bool isRunning = true;
    while(isRunning)
    {
        // TODO: handle transient network errors (e.g. interface reconnect) with retry instead of terminating

        if(auto res = discovery->announceSelf(tcpPort); !res)
        {
            std::println("Error while announcing self: {}", res.error().message());
            return 1;
        }

        if(auto res = discovery->discover(DISCOVERY_TIMEOUT); !res)
        {
            std::println("Error while discovering: {}", res.error().message());
            return 1;
        }
    }

    std::unreachable();
}
