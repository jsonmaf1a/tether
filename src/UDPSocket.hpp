#pragma once

#include "Socket.hpp"
#include <span>

class UDPSocket : public Socket
{
    public:
        UDPSocket(uint16_t port, int socketFd = -1) : Socket(SocketType::UDP, socketFd)
        {
            bind(port);
            enableBroadcast();
        };

        void send(std::span<const std::byte> payload, const sockaddr_in& dest);
        ssize_t receive(std::span<std::byte> payload, sockaddr_in& src);

    private:
        void bind(uint16_t port);
        void enableBroadcast();
};
