#include "CLI.hpp"
#include "cli/Command.hpp"
#include <cstdint>
#include <expected>
#include <print>
#include <string>
#include <variant>

std::expected<Command, CLIError> CLI::parse(Args args)
{
    auto command = args.command();

    if(!command)
        return std::unexpected(
            CLIError{.kind = CLIError::Kind::MissingCommand, .message = "missing command"});

    if(*command == RunCommand::name)
    {
        auto tcpPortArg = args.at(0);
        if(!tcpPortArg.has_value())
            return std::unexpected(CLIError{.kind = CLIError::Kind::MissingArgument,
                                            .message = "run requires a TCP port"});

        uint16_t tcpPort = std::stoi(*tcpPortArg);
        if(!tcpPort)
            return std::unexpected(CLIError{.kind = CLIError::Kind::InvalidArgument,
                                            .message = "invalid TCP port provided"});

        return RunCommand{tcpPort};
    }

    if(*command == StatusCommand::name)
        return StatusCommand{};

    if(*command == PeersCommand::name)
        return PeersCommand{};

    if(*command == ConnectCommand::name)
    {
        auto address = args.at(0);

        if(!address.has_value())
            return std::unexpected(CLIError{.kind = CLIError::Kind::MissingArgument,
                                            .message = "connect requires an IP adress"});

        auto endpoint = Endpoint::parse(*address);

        if(!endpoint)
            return std::unexpected(CLIError{.kind = CLIError::Kind::InvalidArgument,
                                            .message = "invalid IP address provided"});

        return ConnectCommand{.address = *endpoint};
    }

    if(*command == SendCommand::name)
    {
        auto dest = args.at(0);
        auto message = args.at(1);

        if(!dest.has_value() || !message.has_value())
            return std::unexpected(CLIError{.kind = CLIError::Kind::MissingArgument,
                                            .message = "send requires a destination peer ID"});

        auto peer = PeerID::parse(*dest);

        if(!peer)
            return std::unexpected(CLIError{.kind = CLIError::Kind::InvalidArgument,
                                            .message = "invalid peer ID provided"});

        return SendCommand{.dest = *peer, .message = std::string(*message)};
    }

    if(*command == HelpCommand::name)
        return HelpCommand{};

    if(*command == VersionCommand::name)
        return VersionCommand{};

    return std::unexpected(
        CLIError{.kind = CLIError::Kind::UnknownCommand, .message = "unknown command"});
}

void CLI::process(const Command &command)
{
    std::visit([this](const auto &c) { return processCommand(c); }, command);
}

void CLI::processCommand(const RunCommand &command) { auto res = client.run(command.tcpPort); }

void CLI::processCommand(const StatusCommand &) {}

void CLI::processCommand(const ConnectCommand &command) {}

void CLI::processCommand(const PeersCommand &) {}

void CLI::processCommand(const SendCommand &command) {}

void CLI::processCommand(const HelpCommand &) { return printHelp(); }

void CLI::processCommand(const VersionCommand &) { return printVersion(); }

void CLI::printHelp(const std::string_view app) const
{
    std::println("Usage:");
    std::println("  {} run <port>           Runs peer discovery in the background and starts a TCP listener on the specified port", app);
    std::println("  {} connect <ip:port>       Initiates a TCP connection", app);
    std::println("  {} send <peer> <message>   Sends a message to the specified peer", app);
    std::println("  {} status                  Prints current connection status", app);
    std::println("  {} peers                   Prints discovered peers available to connect", app);
    std::println("  {} help                    Prints this help message", app);
    std::println("  {} version                 Prints version", app);
}

void CLI::printVersion() const
{
    std::println("version not implemented"); // TODO: implement CLI::printVersion
}
