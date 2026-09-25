#pragma once

#include "Socket.hpp"
#include <span>
#include <chrono>
#include <system_error>

class UDPSocket : public Socket
{
    public:
        static std::expected<UDPSocket, std::error_code> create()
        {
            const int fd = socket(AF_INET, SOCK_DGRAM, 0);

            if (fd == -1)
            {
                return std::unexpected(
                    std::error_code(errno, std::generic_category())
                );
            }

            return UDPSocket(fd);
        }

        std::expected<void, std::error_code> bind(uint16_t port);
        std::expected<void, std::error_code> enableBroadcast();
        std::expected<void, std::error_code> enableReuse();

        std::expected<ssize_t, std::error_code> send(std::span<const std::byte> payload, const sockaddr_in& dest);
        std::expected<ssize_t, std::error_code> receive(std::span<std::byte> payload, sockaddr_in& src);

        bool waitUntilReadable(std::chrono::milliseconds timeout);

    private:
        explicit UDPSocket(int fd) : Socket(fd) {}

        template<typename T>
        std::expected<void, std::error_code> setOption(int level, int option, const T& value);
};
