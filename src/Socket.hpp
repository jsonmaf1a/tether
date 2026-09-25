#pragma once

#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
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
        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        Socket(Socket&& other) noexcept
            : fd(std::exchange(other.fd, -1))
        {
        }

        Socket& operator=(Socket&& other) noexcept
        {
            if (this != &other) {
                close();
                fd = std::exchange(other.fd, -1);
            }

            return *this;
        }

        void close()
        {
            if(fd != -1)
            {
                ::close(fd);
                fd = -1;
            }
        };

    protected:
        int fd = -1;

        Socket(SocketType type, int socketFd = -1)
        {
            if(socketFd == -1)
            {
                std::println("Initializing socket");

                socketFd = socket(AF_INET, type, 0);
                if(socketFd == -1)
                {
                    perror("Error creating socket");
                    exit(EXIT_FAILURE);
                }
            }


            this->fd = socketFd;
            std::println("Socket {} initialized successfully", fd);
        };
};
