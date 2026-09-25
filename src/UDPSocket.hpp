#pragma once

#include "Socket.hpp"
#include <span>
#include <chrono>

class UDPSocket : public Socket
{
    public:
        UDPSocket(int socketFd = -1) : Socket(SocketType::UDP, socketFd){};

        void send(std::span<const std::byte> payload, const sockaddr_in& dest);
        ssize_t receive(std::span<std::byte> payload, sockaddr_in& src);
        bool waitUntilReadable(std::chrono::milliseconds timeout);

        void bind(uint16_t port);
        void enableBroadcast();
        void enableReuse();
};
