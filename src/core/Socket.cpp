#include "Socket.hpp"

void Socket::close()
{
    if(fd != -1)
    {
        ::close(fd);
        fd = -1;
    }
}

std::expected<void, std::error_code> Socket::bind(uint16_t port)
{
    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(::bind(this->fd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) == -1)
        return std::unexpected(std::error_code(errno, std::generic_category()));

    return {};
};
