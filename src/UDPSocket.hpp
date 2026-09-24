#pragma once

#include "Socket.hpp"
#include <span>
#include <chrono>

class UDPSocket : public Socket
{
    public:
        UDPSocket(uint16_t port, int socketFd = -1) : Socket(SocketType::UDP, socketFd)
        {
            enableReuse();
            enableBroadcast();
            bind(port);
        };

        void send(std::span<const std::byte> payload, const sockaddr_in& dest);
        ssize_t receive(std::span<std::byte> payload, sockaddr_in& src);
        bool waitUntilReadable(std::chrono::milliseconds timeout);

    private:
        void bind(uint16_t port);
        void enableBroadcast();
        void enableReuse();
};
