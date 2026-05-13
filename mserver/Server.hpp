#ifndef SERVER_HPP
#define SERVER_HPP

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
#include "Client.hpp"

#include "IrcMessage.hpp"

class Server
{
    private:
        int _port;
        std::string _password;
        int _serverFd;
	    std::map<int, Client*> _clients;
        std::vector<struct pollfd> _pollfds;

    public:
        Server(int port, std::string password);
        void setupSocket();
        void getallServerData();
        void handleNewClient();
        void receiveClientData(int fd);
        void ignitServer();
        void clear_client(int fd);
        void processLine(int fd, const std::string &line);
    void dispatchMessage(int fd, const Command &cmd);

        ~Server();
};


#endif