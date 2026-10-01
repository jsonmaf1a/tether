#include "CLI.hpp"
#include "cli/Args.hpp"
#include <expected>
#include <print>
#include <variant>

std::expected<Command, ParseError> CLI::parse(int argc, char **argv)
{
    Args args{argc, argv};

    auto command = args.command();

    if(!command)
        return std::unexpected(ParseError::MissingCommand);

    if(*command == RunCommand::name)
        return RunCommand{};

    if(*command == StatusCommand::name)
        return StatusCommand{};

    if(*command == PeersCommand::name)
        return PeersCommand{};

    if(*command == ConnectCommand::name)
    {
        auto address = args.at(0);

        if(!address)
            return std::unexpected(ParseError::MissingArgument);

        auto endpoint = Endpoint::parse(address);

        if(!endpoint)
            return std::unexpected(ParseError::InvalidArgument);

        return ConnectCommand{.address = *endpoint};
    }

    if(*command == SendCommand::name)
    {
        auto dest = args.at(0);
        auto message = args.at(1);

        if(!dest || !message)
            return std::unexpected(ParseError::MissingArgument);

        auto peer = PeerID::parse(dest);

        if(!peer)
            return std::unexpected(ParseError::InvalidArgument);

        return SendCommand{.dest = *peer, .message = std::string(message)};
    }

    return std::unexpected(ParseError::UnknownCommand);
}

void CLI::process(const Command &command)
{
    std::visit([this](const auto &c) { return processCommand(c); }, command);
}

void CLI::processCommand(const RunCommand &) {
    auto res = client.run();
}

void CLI::processCommand(const StatusCommand &) {}

void CLI::processCommand(const ConnectCommand &command) {}

void CLI::processCommand(const PeersCommand &) {}

void CLI::processCommand(const SendCommand &command) {}

void CLI::printHelp()
{
    std::println("help string"); // TODO: implement CLI::printHelp
}
