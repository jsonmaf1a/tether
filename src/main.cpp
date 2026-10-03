#include "cli/CLI.hpp"
#include "client/TetherClient.hpp"
#include "core/PeerDiscovery.hpp"
#include <print>
#include <system_error>
#include <unistd.h>
#include <utility>

// TODO:
// - implement TetherError
// - persist PeerDiscovery state across CLI invocations through IPC messaging

int main(int argc, char *argv[])
{
    Args args{argc, argv};

    auto discovery = PeerDiscovery::create();
    if(!discovery)
    {
        std::println("Error creating PeerDiscovery instance: {}", discovery.error().message());
        return 1;
    }

    TetherClient client{std::move(*discovery)};

    CLI cli{client};
    auto result = cli.parse(args);

    if (!result)
    {
        std::println(stderr, "Error: {}", result.error().message);
        cli.printHelp(args.runtimePath());
        return 1;
    }

    if (result) cli.process(*result);

    return 0;
}
