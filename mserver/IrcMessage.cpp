#include "IrcMessage.hpp"

Command::Command() : _prefix(""), _command(""), _params() {}

Command::Command(const Command &other)
	: _prefix(other._prefix), _command(other._command), _params(other._params) {}

Command &Command::operator=(const Command &other) {
	if (this != &other) {
		_prefix = other._prefix;
		_command = other._command;
		_params = other._params;
	}
	return *this;
}

Command::~Command() {}

const std::string &Command::getPrefix() const { return _prefix; }
const std::string &Command::getCommand() const { return _command; }
const std::vector<std::string> &Command::getParams() const { return _params; }

void Command::setPrefix(const std::string &prefix) { _prefix = prefix; }
void Command::setCommand(const std::string &command) { _command = command; }

void Command::addParam(const std::string &param) { _params.push_back(param); }

void Command::clear() {
	_prefix.clear();
	_command.clear();
	_params.clear();
}
