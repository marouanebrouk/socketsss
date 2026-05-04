#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <vector>
#include <poll.h>

class Client;

class Server {
public:
	Server(int port, const std::string &password);
	~Server();

	void run();

private:
	// Disable copy.
	Server(const Server &other);
	Server &operator=(const Server &other);

	void setupSocket();
	// Accept a new client 
	void acceptClient();
	// Handle a client fd 
	void handleClient(int fd);
	void disconnectClient(int fd);
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
