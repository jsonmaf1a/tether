#include "PeerDiscovery.hpp"

constexpr uint16_t PORT = 6767;

int main()
{
    PeerDiscovery discovery {PORT};
    discovery.write();
    sleep(5);
    discovery.read();

    return 0;
}
