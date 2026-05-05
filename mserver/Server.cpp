#include "Server.hpp"

    Server::Server(int port, std::string password ) : _serverFd(-1), _port(port), _password(password)
    {
    };

    Server::~Server()
    {};

    void Server::setupSocket()
    {
        this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
        if (this->_serverFd < 0)
            throw std::runtime_error("failed to create server socket");
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(this->_port);
        addr.sin_addr.s_addr = INADDR_ANY;
        int a = 1;
        setsockopt(this->_serverFd,SOL_SOCKET, SO_REUSEADDR, &a, sizeof(a));

        if(bind(this->_serverFd,(struct sockaddr *)&addr,sizeof(addr)) < 0)
            throw std::runtime_error("could not bind the serverFD");
            
        if (listen(_serverFd, 5) < 0)
            throw std::runtime_error("could not make the socket listen");
        std::cout << "waiting for client connections"  << std::endl;

        socklen_t len = sizeof(addr);
        char recvbuffer[10];
        while (true)
        {
            std::memset(recvbuffer,0,sizeof(recvbuffer));
            int fd = accept(this->_serverFd, (struct sockaddr *)&addr,&len);
            if (fd < 0)
                throw std::runtime_error("accept failed");
            std::cout << "client connected at " << fd << std::endl;
            char buffer[100];
            sprintf(buffer,"hello from server\n");
            int bytes_sent = send(fd, buffer, std::strlen(buffer),0);
            if (bytes_sent <= 0)
                throw std::runtime_error("send failed");

            // std::cout << "aaaaaaaa1" << std::endl;
        	// fcntl(fd, F_SETFL, O_NONBLOCK); // makes the fd passed non blocking;
            if (recv(fd,recvbuffer,sizeof(recvbuffer),0) < 0) //blocking
                throw std::runtime_error("error receiving from client");

            // std::cout << "aaaaaaaa2" << std::endl;
            for(int i = 0; recvbuffer[i] ;i++)
                std::cout << i << " : " << recvbuffer[i] << std::endl;

            if (std::strcmp(recvbuffer,"PASS") == 0)
            {
                std::cout << "correct command from " << fd << std::endl;
            }
        }
    };