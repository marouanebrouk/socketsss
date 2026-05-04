// ── Server.cpp ──
#include "Server.hpp"
#include "Client.hpp"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// Construct server and start listening.
Server::Server(int port, const std::string &password)
	: _serverFd(-1), _port(port), _password(password) {
	setupSocket();
}

// Destroy server and close fds.
Server::~Server() {
	for (std::map<int, Client*>::iterator it = _clients.begin();
		 it != _clients.end(); ++it) {
		close(it->first);
		delete it->second;
	}
	_clients.clear();
	if (_serverFd >= 0) {
		close(_serverFd);
	}
}

// Main loop using poll().
void Server::run() {
	while (true) {
		int ret = poll(&_pollfds[0], _pollfds.size(), -1);
		if (ret < 0) {
			if (errno == EINTR) {
				continue;
			}
			throw std::runtime_error("poll failed");
		}
		for (size_t i = 0; i < _pollfds.size(); ++i) {
			int fd = _pollfds[i].fd;
			short revents = _pollfds[i].revents;
			if (revents & (POLLERR | POLLHUP | POLLNVAL)) {
				disconnectClient(fd);
				continue;
			}
			if ((revents & POLLIN) && fd == _serverFd) {
				acceptClient();
			} else if (revents & POLLIN) {
				handleClient(fd);
			}
		}
	}
}

// Setup server socket.
void Server::setupSocket() {
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd < 0) {
		throw std::runtime_error("socket failed");
	}
	fcntl(_serverFd, F_SETFL, O_NONBLOCK);
	int yes = 1;
	setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);
	if (bind(_serverFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		close(_serverFd);
		throw std::runtime_error("bind failed");
	}
	if (listen(_serverFd, 10) < 0) {
		close(_serverFd);
		throw std::runtime_error("listen failed");
	}
	struct pollfd pfd;
	pfd.fd = _serverFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollfds.push_back(pfd);
}

// Accept a new client (stub).
void Server::acceptClient() {
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int clientFd = accept(_serverFd, (struct sockaddr*)&addr, &len);
	if (clientFd < 0) {
		// if (errno == EAGAIN || errno == EWOULDBLOCK) {
		// 	return;
		// }
		return;
	}
	fcntl(clientFd, F_SETFL, O_NONBLOCK);
	struct pollfd pfd;
	pfd.fd = clientFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollfds.push_back(pfd);
	_clients[clientFd] = new Client(clientFd);
}

// Handle a client fd (stub).
void Server::handleClient(int fd) {
	char buffer[512];
	ssize_t n = recv(fd, buffer, sizeof(buffer), 0);
	if (n <= 0) {
		// if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
		// 	return;
		// }
		disconnectClient(fd);
		return;
	}
	Client *client = _clients[fd];
	client->appendBuffer(std::string(buffer, n));

	std::string &buf = client->bufferRef();
	size_t pos = 0;
	while ((pos = buf.find("\r\n")) != std::string::npos) {
		std::string line = buf.substr(0, pos);
		buf.erase(0, pos + 2);
		processLine(fd, line);
	}
}

// Disconnect a client (stub).
void Server::disconnectClient(int fd) {
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it != _clients.end()) {
		close(fd);
		delete it->second;
		_clients.erase(it);
	}
	for (std::vector<struct pollfd>::iterator pit = _pollfds.begin();
		 pit != _pollfds.end(); ++pit) {
		if (pit->fd == fd) {
			_pollfds.erase(pit);
			break;
		}
	}
}


