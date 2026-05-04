// ── Server.hpp ──
#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <vector>
#include <poll.h>

class Client;

class Server {
public:
	// Construct server and start listening.
	Server(int port, const std::string &password);
	// Destroy server and close fds.
	~Server();

	// Main loop (will be filled later).
	void run();

private:
	// Disable copy.
	Server(const Server &other);
	Server &operator=(const Server &other);

	// Setup server socket.
	void setupSocket();
	// Accept a new client (stub for now).
	void acceptClient();
	// Handle a client fd (stub for now).
	void handleClient(int fd);
	// Disconnect a client (stub for now).
	void disconnectClient(int fd);
	// Process a complete line (stub for now).
	void processLine(int fd, const std::string &line);
	// Send a message to a client.
	void sendTo(int fd, const std::string &msg);

	int         _serverFd;
	int         _port;
	std::string _password;
	std::vector<struct pollfd> _pollfds;
	std::map<int, Client*> _clients;
};

#endif
