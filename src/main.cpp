#include "PeerDiscovery.hpp"
#include <cstdint>
#include <print>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

constexpr uint16_t PORT = 6767;

int main()
{
    PeerDiscovery discovery {PORT};
    discovery.write();
    sleep(5);
    discovery.read();

    return 0;
}
