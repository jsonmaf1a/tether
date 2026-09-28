#include "TCPSocket.hpp"

std::expected<void, std::error_code> TCPSocket::listen()
{
    if(::listen(this->fd, SOMAXCONN) == -1)
        return std::unexpected(std::error_code(errno, std::generic_category()));

    return {};
};

std::expected<void, std::error_code> TCPSocket::connect(const sockaddr_in &address)
{
    if(::connect(this->fd, reinterpret_cast<const sockaddr *>(&address), sizeof(address)) == -1)
        return std::unexpected(std::error_code(errno, std::generic_category()));

    return {};
};

std::expected<int, std::error_code> TCPSocket::accept(sockaddr_in &address)
{
    socklen_t addrLen = sizeof(address);

    int sock = ::accept(this->fd, reinterpret_cast<sockaddr *>(&address), &addrLen);

    if(sock == -1)
        return std::unexpected(std::error_code(errno, std::generic_category()));

    return sock;
};

std::expected<ssize_t, std::error_code> TCPSocket::send() { return {}; };

std::expected<ssize_t, std::error_code> TCPSocket::receive() { return {}; };
