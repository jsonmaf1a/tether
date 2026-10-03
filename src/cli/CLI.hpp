#pragma once

#include "App.hpp"
#include "cli/Args.hpp"
#include "cli/Command.hpp"
#include "cli/Error.hpp"
#include "client/TetherClient.hpp"
#include <expected>

class CLI
{
    public:
        CLI(TetherClient &client) : client(client) {};

        std::expected<Command, CLIError> parse(Args args);
        void process(const Command &command);

        void printHelp(const std::string_view app = App::name) const;
        void printVersion() const;

    private:
        TetherClient &client;

        void processCommand(const RunCommand &);
        void processCommand(const StatusCommand &);
        void processCommand(const ConnectCommand &);
        void processCommand(const PeersCommand &);
        void processCommand(const SendCommand &);
        void processCommand(const HelpCommand &);
        void processCommand(const VersionCommand &);
};
