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


// Process a complete line (stub).
void Server::processLine(int fd, const std::string &line) {
	Client *client = _clients[fd];
	std::vector<std::string> parts;
	std::string token;
	for (size_t i = 0; i < line.size(); ++i) {
		if (line[i] == ' ') {
			if (!token.empty()) {
				parts.push_back(token);
				token.clear();
			}
		} else {
			token += line[i];
		}
	}
	if (!token.empty()) {
		parts.push_back(token);
	}
	if (parts.empty()) {
		return;
	}

	std::string cmd = parts[0];
	if (cmd == "PASS") {
		if (parts.size() < 2) {
			sendTo(fd, "464 :Password mismatch");
			disconnectClient(fd);
			return;
		}
		if (parts[1] != _password) {
			sendTo(fd, "464 :Password mismatch");
			disconnectClient(fd);
			return;
		}
		client->setAuthenticated(true);
		return;
	}

	if (!client->isAuthenticated()) {
		sendTo(fd, "464 :Password mismatch");
		disconnectClient(fd);
		return;
	}

	if (cmd == "SALAM") {
		if (parts.size() >= 2) {
			sendTo(fd, "Wa3lik salam " + parts[1]);
		} else {
			sendTo(fd, "Wa3lik salam");
		}
		return;
	}

	if (cmd == "QUIT") {
		disconnectClient(fd);
		return;
	}
}

// Send a message to a client.
void Server::sendTo(int fd, const std::string &msg) {
	std::string wire = msg + "\r\n";
	send(fd, wire.c_str(), wire.size(), 0);
}