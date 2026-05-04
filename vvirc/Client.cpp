#include "Client.hpp"

// Construct a client with its socket fd.
Client::Client(int fd)
	: _buffer(""), _authenticated(false) {
	(void)fd;
}


Client::~Client() {}

std::string &Client::bufferRef() { return _buffer; }

bool Client::isAuthenticated() const { return _authenticated; }

void Client::setAuthenticated(bool value) { _authenticated = value; }


void Client::appendBuffer(const std::string &chunk) { _buffer += chunk; }

