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

class Server
{
    private:
        int _serverFd;
        int _port;
        std::string _password;
    
    public:
        Server(int port, std::string password);
        ~Server();

        void setupSocket();
};