// ── Client.hpp ──
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
public:
	// Construct a client with its socket fd.
	Client(int fd);
	// Destroy client.
	~Client();

	// Getters.
	int getFd() const;
	const std::string &getNick() const;
	const std::string &getUser() const;
	const std::string &getBuffer() const;
	std::string &bufferRef();
	bool isAuthenticated() const;
	bool isRegistered() const;

	// Setters.
	void setNick(const std::string &nick);
	void setUser(const std::string &user);
	void setAuthenticated(bool value);
	void setRegistered(bool value);

	// Buffer helpers.
	void appendBuffer(const std::string &chunk);
	void clearBuffer();

private:
	int         _fd;
	std::string _nick;
	std::string _user;
	std::string _buffer;
	bool        _authenticated;
	bool        _registered;
};

#endif
