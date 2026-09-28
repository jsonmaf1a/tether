#pragma once

#include "Socket.hpp"
#include <system_error>

class TCPSocket : public Socket
{
    public:
        static std::expected<TCPSocket, std::error_code> create()
        {
            const int fd = socket(AF_INET, SocketType::TCP, 0);

            if(fd == -1)
            {
                return std::unexpected(std::error_code(errno, std::generic_category()));
            }

            return TCPSocket(fd);
        }

        std::expected<void, std::error_code> listen();
        std::expected<void, std::error_code> connect(const sockaddr_in &address);
        std::expected<int, std::error_code> accept(sockaddr_in &address);

        std::expected<ssize_t, std::error_code> send();
        std::expected<ssize_t, std::error_code> receive();

    private:
        explicit TCPSocket(int fd) : Socket(fd) {}
};
