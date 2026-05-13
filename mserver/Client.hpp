#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <poll.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

class Client
{
    private:
        int client_fd;
        std::string ip_addr;
        // recv() is stream-based: accumulate until we can extract full "\r\n" lines.
        std::string _buffer;
    public:
        Client(){};
        Client(int fd, const std::string &ip);
        int getFD();
        void setFD(int fd);

        std::string & getIP();

        void setIP(const std::string &ip);

        // Buffer access/manipulation
        std::string &bufferRef();
        void appendBuffer(const std::string &chunk);
        /*
        IRC messages may arrive split or batched
        You must reconstruct full lines (\r\n)
        */





        // 5. Relationships
// list of joined channels (or pointers)
// maybe pointer/reference to server
};

#endif