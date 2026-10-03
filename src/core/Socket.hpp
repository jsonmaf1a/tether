#pragma once

#include <arpa/inet.h>
#include <expected>
#include <print>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>

enum SocketType
{
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM
};

class Socket
{
    public:
        Socket(const Socket &) = delete;
        Socket &operator=(const Socket &) = delete;

        Socket(Socket &&other) noexcept : fd(std::exchange(other.fd, -1)) {}

        Socket &operator=(Socket &&other) noexcept
        {
            if(this != &other)
            {
                close();
                fd = std::exchange(other.fd, -1);
            }

            return *this;
        }
        virtual ~Socket() { close(); }

        static std::expected<Socket, std::error_code> create(SocketType type)
        {
            const int fd = socket(AF_INET, type, 0);

            if(fd == -1)
            {
                return std::unexpected(std::error_code(errno, std::generic_category()));
            }

            return Socket(fd);
        }

        static Socket fromFd(int fd) { return Socket(fd); }

        std::expected<void, std::error_code> bind(uint16_t port);
        void close();

    protected:
        explicit Socket(int socketFd) : fd(socketFd)
        {
#ifdef TETHER_DEBUG
            std::println("Socket {} initialized successfully", fd);
#endif
        }

        int fd = -1;
};
