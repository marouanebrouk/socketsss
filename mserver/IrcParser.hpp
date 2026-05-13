#ifndef IRCPARSER_HPP
#define IRCPARSER_HPP

#include <string>
#include "IrcMessage.hpp"

class IrcParser {
public:
	// Parse a single IRC line (no trailing "\r\n").
	// Returns true if a command was found, false for empty/whitespace-only lines.
	static bool parseLine(const std::string &line, Command &out);

private:
	IrcParser();
	IrcParser(const IrcParser &);
	IrcParser &operator=(const IrcParser &);

	static void skipSpaces(const std::string &s, size_t &i);
	static std::string readToken(const std::string &s, size_t &i);
};

#endif
