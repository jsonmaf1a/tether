#pragma once

#include <string>
struct CLIError
{
    public:
        enum class Kind
        {
            MissingCommand,
            UnknownCommand,
            MissingArgument,
            InvalidArgument
        };

        Kind kind;
        std::string message;
};
