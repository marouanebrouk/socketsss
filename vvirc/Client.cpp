// ── Client.cpp ──
#include "Client.hpp"

// Construct a client with its socket fd.
Client::Client(int fd)
	: _fd(fd), _nick(""), _user(""), _buffer(""), _authenticated(false),
	  _registered(false) {}

// Destroy client.
Client::~Client() {}

// Get the socket fd.
int Client::getFd() const { return _fd; }

// Get the nickname.
const std::string &Client::getNick() const { return _nick; }

// Get the username.
const std::string &Client::getUser() const { return _user; }

// Get the buffer.
const std::string &Client::getBuffer() const { return _buffer; }

// Get a mutable buffer reference.
std::string &Client::bufferRef() { return _buffer; }

// Check authentication.
bool Client::isAuthenticated() const { return _authenticated; }

// Check registration.
bool Client::isRegistered() const { return _registered; }

// Set the nickname.
void Client::setNick(const std::string &nick) { _nick = nick; }

// Set the username.
void Client::setUser(const std::string &user) { _user = user; }

// Set authenticated flag.
void Client::setAuthenticated(bool value) { _authenticated = value; }

// Set registered flag.
void Client::setRegistered(bool value) { _registered = value; }

// Append to buffer.
void Client::appendBuffer(const std::string &chunk) { _buffer += chunk; }

// Clear buffer.
void Client::clearBuffer() { _buffer.clear(); }
