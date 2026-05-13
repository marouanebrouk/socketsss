#ifndef IRCMESSAGE_HPP
#define IRCMESSAGE_HPP

#include <string>
#include <vector>

// Represents one IRC message without the trailing CRLF.
// Wire format: [:prefix] COMMAND [params] [:trailing]
class Command {
public:
	Command();
	Command(const Command &other);
	Command &operator=(const Command &other);
	~Command();

	const std::string &getPrefix() const;
	const std::string &getCommand() const;
	const std::vector<std::string> &getParams() const;

	void setPrefix(const std::string &prefix);
	void setCommand(const std::string &command);
	void addParam(const std::string &param);
	void clear();

private:
	std::string              _prefix;
	std::string              _command;
	std::vector<std::string> _params;
};

#endif
