#include "UDPSocket.hpp"
#include <netinet/in.h>
#include <sys/socket.h>

void UDPSocket::send(std::span<const std::byte> payload, const sockaddr_in &dest) {
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

ssize_t UDPSocket::receive(std::span<std::byte> payload, sockaddr_in &src) {
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
        perror("Error sending data over UDP socket");
        exit(EXIT_FAILURE);
    }

    return received;
};
