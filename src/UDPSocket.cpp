#include "UDPSocket.hpp"
#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>
#include <poll.h>

void UDPSocket::send(std::span<const std::byte> payload, const sockaddr_in &dest)
{
    auto sent = ::sendto(
        this->fd,
        payload.data(),
        payload.size_bytes(),
        0,
        reinterpret_cast<const sockaddr*>(&dest),
        sizeof(dest)
    );

    if(sent == -1) {
        perror("Error sending data over UDP socket");
        exit(EXIT_FAILURE);
    }
};

ssize_t UDPSocket::receive(std::span<std::byte> payload, sockaddr_in &src)
{
    socklen_t srcLen = sizeof(src);
    auto received = ::recvfrom(
        this->fd,
        payload.data(),
        payload.size_bytes(),
        0,
        reinterpret_cast<sockaddr*>(&src),
        &srcLen
    );

    if(received == -1) {
        perror("Error receiving data over UDP socket");
        exit(EXIT_FAILURE);
    }

    return received;
};

bool UDPSocket::waitUntilReadable(std::chrono::milliseconds timeout)
{
    pollfd pfd{};
    pfd.fd = fd;
    pfd.events = POLLIN;

    int timeoutMs = timeout.count();
    return poll(&pfd, 1, timeoutMs) > 0;
}

void UDPSocket::bind(uint16_t port)
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

void UDPSocket::enableBroadcast()
{
    int enable = 1;
    if (::setsockopt(
            this->fd,
            SOL_SOCKET,
            SO_BROADCAST,
            &enable,
            sizeof(enable)
        ) == -1)
    {
        perror("Error enabling UDP broadcast");
        exit(EXIT_FAILURE);
    }
};

void UDPSocket::enableReuse()
{
    int rAddr = 1;
    int rPort = 1;

    if (setsockopt(
        fd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &rAddr,
        sizeof(rAddr)
    ) == -1)
    {
        perror("Error enabling addr reuse");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(
        fd,
        SOL_SOCKET,
        SO_REUSEPORT,
        &rPort,
        sizeof(rPort)
    ) == -1)
    {
        perror("Error enabling port reuse");
        exit(EXIT_FAILURE);
    }
}
