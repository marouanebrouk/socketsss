#include "Server.hpp"

    Server::Server(int port, std::string password ) : _port(port), _password(password) ,_serverFd(-1)
    {
    };

    Server::~Server()
    {};


    void Server::getallServerData()
    {
        std::cout << this->_port << std::endl;
        std::cout << this->_serverFd << std::endl;
        std::cout << this->_password << std::endl;
    }


    void Server::receiveClientData(int fd)
    {
        char buffer[1024];
        std::memset(&buffer,0,sizeof(buffer));
        size_t bytes_r = recv(fd,buffer,sizeof(buffer),0);
        if (bytes_r < 0)
        {
            throw std::runtime_error("recv function failed");
            close(fd);
        }
        std::cout << "Client <" << fd << "> Data: "  << buffer;
		//here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
    }

    void Server::handleNewClient()
    {
        struct sockaddr_in client_sock;
        socklen_t len = sizeof(client_sock);
        int clientFd = accept(_serverFd, (struct sockaddr_in *)&client_sock, len);
        if (clientFd == -1)
            throw std::runtime_error("accept failed");
        
        /* i should do something here till tomorrow okkkkk*/
        // allocation to new client; creation of new client;



        std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;
        std::cout << "client connected at fd : " << clientFd << std::endl;
    }

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
        std::cout << "waiting for clients"  << std::endl;

        struct pollfd pfd;
        pfd.fd = _serverFd;
        pfd.events = POLLIN;
        pfd.revents = 0;
        _pollfds.push_back(pfd);
        this->getallServerData();

        //----//---- end_of_function setupSocket function.

        //run the poll loop that handles multiple clients;
        Server::ignitServer()
        {

        while (true)
        {
            if (poll(&_pollfds[0],_pollfds.size(),-1) == -1)
                throw std::runtime_error("poll failed");
            for (int i = 0; i < _pollfds.size(); i++)
            {
                if ((_pollfds[i].revents & POLLIN) && _pollfds[i].fd == _serverFd)
                    handleNewClient();
                else
                    receiveClientData();
            }

            // basically its the end of the pool loop (handle new client || accept new conenction)
            //------------------------------- STOP


            //hello
            // socklen_t len = sizeof(addr);
            // char recvbuffer[100];
            // std::memset(recvbuffer ,0 ,sizeof(recvbuffer));
            // int fd = accept(this->_serverFd, (struct sockaddr *)&addr,&len);
            // if (fd < 0)
            //     throw std::runtime_error("accept failed");
            // std::cout << "client connected at " << fd << std::endl;
            // char buffer[100];
            // sprintf(buffer,"hello from server\n");
            // int bytes_sent = send(fd, buffer, std::strlen(buffer),0);
            // if (bytes_sent <= 0)
            //     throw std::runtime_error("send failed");

            // // std::cout << "aaaaaaaa1" << std::endl;
        	// // fcntl(fd, F_SETFL, O_NONBLOCK); // makes the fd passed non blocking;
            // if (recv(fd,recvbuffer,sizeof(recvbuffer),0) < 0) //blocking
            //     throw std::runtime_error("error receiving from client");

            // std::cout << "msg from fd " << fd << ":" << recvbuffer << std::endl;
            // // for(int i = 0; recvbuffer[i] ;i++)
            //     // std::cout << i << " : " << recvbuffer[i] << std::endl;
            // if (std::strcmp(recvbuffer,"PASS") == 0)
            // {
            //     std::cout << "correct command from " << fd << std::endl;
            // }
        }
        }
    };

