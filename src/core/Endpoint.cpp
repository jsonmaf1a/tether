#include "Endpoint.hpp"

std::optional<Endpoint> Endpoint::parse(std::string_view value)
{
    const auto separator = value.find(':');

    if (separator == std::string_view::npos)
        return std::nullopt;

    const auto ip_string = value.substr(0, separator);
    const auto port_string = value.substr(separator + 1);

    if (ip_string.empty() || port_string.empty())
        return std::nullopt;

    unsigned int port = 0;

    for (char c : port_string) {
        if (c < '0' || c > '9')
            return std::nullopt;

        port = port * 10 + (c - '0');

        if (port > UINT16_MAX)
            return std::nullopt;
    }

    in_addr ip{};

    const std::string ip_copy{ip_string};

    if (inet_pton(AF_INET, ip_copy.c_str(), &ip) != 1)
        return std::nullopt;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr = ip;
    addr.sin_port = htons(static_cast<uint16_t>(port));

    return Endpoint{addr};
}
