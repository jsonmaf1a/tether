#include "UDPSocket.hpp"
#include <cstdint>
#include <expected>
#include <netinet/in.h>
#include <sys/socket.h>
#include <poll.h>
#include <sys/types.h>
#include <system_error>

std::expected<ssize_t, std::error_code> UDPSocket::send(std::span<const std::byte> payload, const sockaddr_in &dest)
{
    auto sent = ::sendto(
        this->fd,
        payload.data(),
        payload.size_bytes(),
        0,
        reinterpret_cast<const sockaddr*>(&dest),
        sizeof(dest)
    );

    if(sent == -1)
        return std::unexpected(std::error_code(errno, std::generic_category()));

    return sent;
};

std::expected<ssize_t, std::error_code> UDPSocket::receive(std::span<std::byte> payload, sockaddr_in &src)
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

    if(received == -1)
        return std::unexpected(std::error_code(errno, std::generic_category()));

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

std::expected<void, std::error_code> UDPSocket::bind(uint16_t port)
{
    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(::bind(this->fd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
        return std::unexpected(std::error_code(errno, std::generic_category()));

    return {};
};

std::expected<void, std::error_code> UDPSocket::enableBroadcast()
{
    return setOption(SOL_SOCKET, SO_BROADCAST, 1);
};

std::expected<void, std::error_code> UDPSocket::enableReuse()
{
    return setOption(SOL_SOCKET, SO_REUSEADDR, 1)
        .and_then([this] {
            return setOption(SOL_SOCKET, SO_REUSEPORT, 1);
        });
}

template<typename T>
std::expected<void, std::error_code> UDPSocket::setOption(int level, int option, const T& value)
{
    if (::setsockopt(fd, level, option, &value, sizeof(value)) == -1)
        return std::unexpected(std::error_code(errno, std::generic_category()));

    return {};
}
