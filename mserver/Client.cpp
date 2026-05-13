#include "Client.hpp"

Client::Client(int fd, const std::string &ip) : client_fd(fd), ip_addr(ip) {}

        int Client::getFD()
        {
            return client_fd;
        }
        
        void Client::setFD(int fd)
        {
            client_fd = fd;
        };

        std::string & Client::getIP()
        {
            return ip_addr;
        };

        void Client::setIP(const std::string &ip)
        {
            ip_addr = ip;
        }

        std::string &Client::bufferRef() {
            return _buffer;
        }

        void Client::appendBuffer(const std::string &chunk) {
            _buffer += chunk;
        }