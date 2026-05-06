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
    public:
        Client(){};
        int getFD()
        {
            return client_fd;
        }
        void setFD(int fd)
        {
            client_fd = fd;
        };

        std::string & getIP()
        {
            return ip_addr;
        };

        void setIP(const std::string &ip)
        {
            ip_addr = ip;
        }
        /*
        IRC messages may arrive split or batched
        You must reconstruct full lines (\r\n)
        */





        // 5. Relationships
// list of joined channels (or pointers)
// maybe pointer/reference to server
}