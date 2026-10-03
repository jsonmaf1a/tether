#pragma once

#include "core/Endpoint.hpp"
#include "core/PeerID.hpp"
#include <cstdint>
#include <string_view>
#include <variant>

struct RunCommand
{
    static constexpr std::string_view name = "run";
    uint16_t tcpPort;
};

struct StatusCommand
{
    static constexpr std::string_view name = "status";
};

struct ConnectCommand
{
    static constexpr std::string_view name = "connect";
    Endpoint address;
};

struct PeersCommand
{
    static constexpr std::string_view name = "peers";
};

struct SendCommand
{
    static constexpr std::string_view name = "send";
    PeerID dest;
    std::string message;
};

struct HelpCommand
{
    static constexpr std::string_view name = "help";
};

struct VersionCommand
{
    static constexpr std::string_view name = "version";
};

using Command = std::variant<RunCommand, StatusCommand, ConnectCommand, PeersCommand, SendCommand, HelpCommand, VersionCommand>;
