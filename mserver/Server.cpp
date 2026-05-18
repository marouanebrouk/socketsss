#include "Server.hpp"
#include "Client.hpp"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include "IrcParser.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password), _serverFd(-1) {}

Server::~Server() {}

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
    std::map<int, Client *>::iterator it = _clients.find(fd);
    if (it != _clients.end())
    {
        delete it->second;
        _clients.erase(it);
    }
}

void Server::receiveClientData(int fd)
{
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    // recv returns ssize_t (signed)
    ssize_t bytes_r = recv(fd, buffer, sizeof(buffer), 0);
    if (bytes_r == 0)
    {
        // orderly shutdown by peer which means client has disconnected.
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


    std::map<int, Client *>::iterator it = _clients.find(fd);
    if (it == _clients.end() || !it->second)
    {
        std::cout << "Client not found or invalid" << std::endl;
        return;
    }
    Client *client = it->second;
    client->appendBuffer(std::string(buffer, bytes_r));
    std::string &buf = client->bufferRef();
    size_t pos = 0;
    while ((pos = buf.find("\r\n")) != std::string::npos)
    {
        std::string line = buf.substr(0, pos);
        buf.erase(0, pos + 2);
        processLine(fd, line);
    }
}

void Server::processLine(int fd, const std::string &line)
{
    Command cmd;
    if (!IrcParser::parseLine(line, cmd))
        return;
    // displayMessage(fd, cmd);
    command_dispatcher(fd, cmd);
}



void Server::displayMessage(int fd, const Command &cmd)
{
    // Parser is separated from execution; keep this as the dispatch point.
    // For now, just log what we parsed.



    //[IRC] fd=4 cmd='DEBUG' <<<<<< you want this

    //do this \/

    std::cout << "[IRC] fd=" << fd;
    if (!cmd.getPrefix().empty())
        std::cout << " prefix='" << cmd.getPrefix() << "'";
    std::cout << " cmd='" << cmd.getCommand() << "'";
    const std::vector<std::string> &params = cmd.getParams();
    for (size_t i = 0; i < params.size(); ++i)
        std::cout << " param[" << i << "]='" << params[i] << "'";
    std::cout << std::endl;

    // Here you can add code to handle the command, e.g., by calling a method
    // on the Client object associated with the fd.
}

void Server::handleNewClient()
{
    struct sockaddr_in client_sock;
    std::memset(&client_sock, 0, sizeof(client_sock));
    socklen_t len = sizeof(client_sock);
    int clientFd = accept(_serverFd, (struct sockaddr *)&client_sock, &len);
    if (clientFd == -1)
    {
        // transient failures shouldn't kill the server
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
            return;
        throw std::runtime_error("accept failed");
    }
    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
    {
        close(clientFd);
        return;
    }

    Client *clienttttt = new Client();
    std::memset((void *)clienttttt, 0, sizeof(Client));
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

    int yes = 1;
    if (setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
    {
        close(this->_serverFd);
        throw std::runtime_error("setsockopt failed");
    }

    // set server socket non-blocking for poll-driven accept
    if (fcntl(this->_serverFd, F_SETFL, O_NONBLOCK) < 0)
    {
        close(this->_serverFd);
        throw std::runtime_error("fcntl failed");
    }

    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(this->_serverFd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("could not bind the serverFD");

    if (listen(_serverFd, 5) < 0)
        throw std::runtime_error("could not make the socket listen");
    std::cout << "waiting for clients." << std::endl;

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
        if (_pollfds.empty())
            usleep(100000);
        else
        {
            int ret = poll(&_pollfds[0], _pollfds.size(), -1);
            if (ret == -1)
            {
                if (errno != EINTR)
                    throw std::runtime_error("poll failed");
            }
            else
            {
                for (size_t i = 0; i < _pollfds.size(); ++i)
                {
                    short revents = _pollfds[i].revents;
                    int fd = _pollfds[i].fd;
                    if (revents & (POLLERR | POLLHUP | POLLNVAL))
                    {
                        clear_client(fd);
                        close(fd);
                        --i;
                    }
                    else if (revents & POLLIN)
                    {
                        if (fd == _serverFd)
                            handleNewClient();
                        else
                            receiveClientData(fd);
                    }
                }
            }
        }
    }
}




/*
Socket setup (Server::setupSocket)

Created server socket and added checking of setsockopt.
Set the server socket to non-blocking with fcntl(..., O_NONBLOCK).
Zero-initialized sockaddr_in and performed bind + listen.
Added the server fd to _pollfds for POLLIN.
Accepting new clients (Server::handleNewClient)

Accept now tolerates transient errors (EAGAIN, EWOULDBLOCK, EINTR) and returns instead of throwing for those.
Sets accepted client socket to non-blocking.
Creates a Client object, stores it in _clients, and adds its fd to _pollfds with events = POLLIN.
Removed noisy "New client handling function" debug prints.
Receiving data (Server::receiveClientData)

Use ssize_t bytes_r = recv(...) (signed) and handle:
bytes_r == 0: peer closed — call clear_client(fd), close fd.
bytes_r < 0: if EAGAIN/EWOULDBLOCK then ignore; otherwise remove client and close fd.
bytes_r > 0: print exactly the received bytes with std::string(buffer, bytes_r).
Removed an extra log line that printed before receiving data (this stopped repeated empty prints).
Client cleanup (Server::clear_client)

Erase pollfd entry for that fd.
If a Client* exists in _clients, delete it and erase the map entry to avoid leaks.
Poll loop (Server::ignitServer)

If _pollfds is empty, sleep briefly and continue (avoids polling an empty array).
Poll now checks for EINTR and continues instead of throwing.
For each pollfd, if revents includes POLLERR | POLLHUP | POLLNVAL -> remove client and its pollfd.
Only call receiveClientData(fd) when revents & POLLIN is set.
This prevents trying to read from fds that had no readable data and avoids the interleaved/empty prints.
Why this fixes the interleaved/empty prints

Previously code called receiveClientData for every pollfd regardless of revents, which caused spurious recv attempts and noisy prints (including empty strings). Now recv is only called when the OS indicated the socket is readable (POLLIN). Non-blocking sockets + correct recv handling prevent blocking or returning inconsistent/empty output.
Deleting Client* when clearing avoids stale references that could cause confusing logs.

*/