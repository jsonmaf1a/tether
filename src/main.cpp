#include "PeerDiscovery.hpp"
#include <chrono>
#include <unistd.h>

// TODO: replace perror/exit with std::expected-based error handling

constexpr uint16_t PORT = 6767;
constexpr std::chrono::milliseconds DISCOVERY_TIMEOUT { 500 };

int main()
{
    PeerDiscovery discovery(PORT);
    bool isRunning = true;

    while(isRunning) {
            discovery.announceSelf();
            discovery.discover(DISCOVERY_TIMEOUT);
    }

    return 0;
}
