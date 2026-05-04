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
	std::string &bufferRef();
	bool isAuthenticated() const;
	void setAuthenticated(bool value);

	// Buffer helpers.
	void appendBuffer(const std::string &chunk);

private:
	std::string _buffer;
	bool        _authenticated;
};

#endif
