#include "PeerDiscovery.hpp"
#include <cstdint>
#include <unistd.h>
#include <utility>

using namespace std::chrono_literals;

// TODO:
// - replace perror/exit with std::expected-based error handling
// - cli

constexpr uint16_t BROADCAST_PORT = 6767;
constexpr auto DISCOVERY_TIMEOUT = 2500ms;

int main(int argc, char *argv[])
{
    // temporary
    std::string tcpPortValue;
    if (argc > 1) {
        std::string_view tcpPortArg = argv[1];

        if(tcpPortArg == "--port" && argv[2]) {
            tcpPortValue = argv[2];
        } else return 1;
    }

    uint16_t tcpPort = static_cast<uint16_t>(std::stoi(tcpPortValue));

    PeerDiscovery discovery(BROADCAST_PORT);
    bool isRunning = true;

    while(isRunning) {
            discovery.announceSelf(tcpPort);
            discovery.discover(DISCOVERY_TIMEOUT);
    }

    std::unreachable();
}
