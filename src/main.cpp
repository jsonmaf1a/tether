#include "cli/CLI.hpp"
#include "client/TetherClient.hpp"
#include "core/PeerDiscovery.hpp"
#include <cstdint>
#include <print>
#include <system_error>
#include <unistd.h>
#include <utility>

using namespace std::chrono_literals;

constexpr uint16_t BROADCAST_PORT = 6767;

int main(int argc, char *argv[])
{
    auto discovery = PeerDiscovery::create(BROADCAST_PORT);
    if(!discovery)
    {
        std::println("Error creating PeerDiscovery instance: {}", discovery.error().message());
        return 1;
    }

    TetherClient client{std::move(*discovery)};

    CLI cli{client};
    auto result = cli.parse(argc, argv);
    if (result) cli.process(*result);

    return 0;
}
