#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <string_view>

class Args
{
    public:
        Args(int count, char **values) : count(count), values(values) {};

        std::string_view runtimePath() const { return values[0]; };

        std::optional<std::string_view> command() const
        {
            if(count < 2)
                return std::nullopt;

            return values[1];
        };

        std::span<char *> arguments() const
        {
            return {values + 2, static_cast<std::size_t>(count - 2)};
        };

        char *at(size_t index) const { return arguments()[index]; };

    private:
        int count;
        char **values;
};
