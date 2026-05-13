#include "Server.hpp"
#include "Client.hpp"
#include <cerrno>

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


    void Server::clear_client(int fd)
    {
        for (size_t i = 0; i < _pollfds.size(); i++)
        {
            if (_pollfds[i].fd == fd)
            {
                _pollfds.erase(_pollfds.begin() + i);
                break;
            }
        }
        // delete the Client object (if present) to avoid memory leak
        std::map<int, Client*>::iterator it = _clients.find(fd);
        if (it != _clients.end()) {
            delete it->second;
            _clients.erase(it);
        }
    }

    void Server::receiveClientData(int fd)
    {
        std::cout << "receiving Client messages" << std::endl;
        char buffer[1024];
        std::memset(&buffer,0,sizeof(buffer));
        // recv returns ssize_t (signed)
        ssize_t bytes_r = recv(fd, buffer, sizeof(buffer), 0);
        if (bytes_r == 0)
        {
            // orderly shutdown by peer
            std::cout << "Client <" << fd << "> Disconnected" << std::endl;
            clear_client(fd);
            close(fd);
            return;
        }
        if (bytes_r < 0)
        {
            // no data available on non-blocking socket
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return;
            std::cerr << "recv error on fd=" << fd << " errno=" << errno << std::endl;
            clear_client(fd);
            close(fd);
            return;
        }
        // bytes_r > 0: print exactly the received data
        std::cout << "Client <" << fd << "> Data: " << std::string(buffer, bytes_r) << std::endl;

        //here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
    }

    void Server::handleNewClient()
    {
        std::cout << "New client handling function" << std::endl;
        struct sockaddr_in client_sock;
        std::memset(&client_sock, 0, sizeof(client_sock));
        socklen_t len = sizeof(client_sock);
        int clientFd = accept(_serverFd, (struct sockaddr *)&client_sock, &len);
        if (clientFd == -1) {
            // transient failures shouldn't kill the server
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
                return;
            throw std::runtime_error("accept failed");
        }
        if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
		{
            std::cout << "fcntl() failed" << std::endl;
            return;
        }

        Client *clienttttt = new Client();
        clienttttt->setFD(clientFd);
        clienttttt->setIP(inet_ntoa(client_sock.sin_addr));
        _clients[clientFd] = clienttttt;
        struct pollfd pfd;
        pfd.fd = clientFd;
        pfd.events = POLLIN;
        pfd.revents = 0;
        _pollfds.push_back(pfd);
        std::cout << "client connected at fd : " << clientFd << std::endl;
    }

    void Server::setupSocket()
    {
        this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
        if (this->_serverFd < 0)
            throw std::runtime_error("failed to create server socket");
        struct sockaddr_in addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(this->_port);
        addr.sin_addr.s_addr = INADDR_ANY;
        int a = 1;
        setsockopt(this->_serverFd,SOL_SOCKET, SO_REUSEADDR, &a, sizeof(a));
        if(bind(this->_serverFd,(struct sockaddr *)&addr,sizeof(addr)) < 0)
            throw std::runtime_error("could not bind the serverFD");
            
        if (listen(_serverFd, 5) < 0)
            throw std::runtime_error("could not make the socket listen");
        std::cout << "waiting for clients."  << std::endl;

        struct pollfd pfd;
        pfd.fd = _serverFd;
        pfd.events = POLLIN;
        pfd.revents = 0;
        _pollfds.push_back(pfd);
        this->getallServerData();

    }

        void Server::ignitServer()
        {
            while (true)
            {
                if (poll(&_pollfds[0],_pollfds.size(),-1) == -1)
                    throw std::runtime_error("poll failed");
                for (size_t i = 0; i < _pollfds.size(); i++)
                {
                    if ((_pollfds[i].revents & POLLIN) && _pollfds[i].fd == _serverFd)
                        handleNewClient();
                    else
                        receiveClientData(_pollfds[i].fd);
                }
            }
        }

