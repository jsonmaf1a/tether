#pragma once

#include <arpa/inet.h>
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

constexpr std::size_t BUFFER_SIZE = 8;

enum SocketType
{
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM
};

class Socket
{
    public:
        ~Socket() {
            close();
        }

        std::array<std::byte, BUFFER_SIZE> buffer;

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
                socketFd = socket(AF_INET, type, 0); if(socketFd == -1)
                {
                    perror("Error creating socket");
                    exit(EXIT_FAILURE);
                }
            }

            this->fd = socketFd;
        };
};
