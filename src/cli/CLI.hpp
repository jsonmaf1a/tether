#pragma once

#include "cli/Command.hpp"
#include "client/TetherClient.hpp"
#include <expected>

enum ParseError {
    MissingCommand,
    UnknownCommand,
    MissingArgument,
    UnknownArgument,
    InvalidArgument
};

class Error {}; // TODO: implement a proper Error type

class CLI
{
    public:
        CLI(TetherClient &client) : client(client) {};

        std::expected<Command, ParseError> parse(int argc, char **argv);
        void process(const Command &command);


        void printHelp();
        void printVersion();

    private:
        TetherClient &client;

        void processCommand(const RunCommand&);
        void processCommand(const StatusCommand&);
        void processCommand(const ConnectCommand&);
        void processCommand(const PeersCommand&);
        void processCommand(const SendCommand&);
};
