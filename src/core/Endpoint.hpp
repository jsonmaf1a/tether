#pragma once

#include <arpa/inet.h>
#include <cstdint>
#include <netinet/in.h>
#include <string>

class Endpoint
{
    public:
        Endpoint(const sockaddr_in &addr) : ip(addr.sin_addr), port(ntohs(addr.sin_port)) {};

        in_addr ip;
        uint16_t port;

        std::string getIPString()
        {
            char s[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &ip, s, INET_ADDRSTRLEN);
            return s;
        };
};
