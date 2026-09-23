#pragma once

#include "Socket.hpp"
#include <span>

class UDPSocket : public Socket
{
    public:
        UDPSocket(uint16_t port, int socketFd = -1) : Socket(SocketType::UDP, socketFd)
        {
            sockaddr_in address{};

            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(port);

            if(::bind(this->fd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
            {
                perror("Socket bind failed");
                exit(EXIT_FAILURE);
            }
        };

        void send(std::span<const std::byte> payload, const sockaddr_in& dest);
        ssize_t receive(std::span<std::byte> payload, sockaddr_in& src);
};
