#include "PeerID.hpp"

std::optional<PeerID>
PeerID::parse(std::string_view value)
{
    if (value.empty())
        return std::nullopt;

    uint32_t id = 0;

    for (char c : value) {
        if (c < '0' || c > '9')
            return std::nullopt;

        const auto digit = static_cast<uint32_t>(c - '0');

        if (id > (UINT32_MAX - digit) / 10)
            return std::nullopt;

        id = id * 10 + digit;
    }

    return PeerID{.value = id};
}
