#include "IrcParser.hpp"

void IrcParser::skipSpaces(const std::string &s, size_t &i) {
	while (i < s.size() && (s[i] == ' ' || s[i] == '\t'))
		++i;
}

std::string IrcParser::readToken(const std::string &s, size_t &i) {
	size_t start = i;
	while (i < s.size() && s[i] != ' ' && s[i] != '\t')
		++i;
	return s.substr(start, i - start);
}

bool IrcParser::parseLine(const std::string &line, Command &out) {
	out.clear();

	size_t i = 0;
	skipSpaces(line, i);
	if (i >= line.size())
		return false;

	if (line[i] == ':') {
		++i;
		size_t start = i;
		while (i < line.size() && line[i] != ' ' && line[i] != '\t')
			++i;
		out.setPrefix(line.substr(start, i - start));
		skipSpaces(line, i);
	}

	if (i >= line.size())
		return false;
	out.setCommand(readToken(line, i));
	if (out.getCommand().empty())
		return false;

	while (i < line.size()) {
		skipSpaces(line, i);
		if (i >= line.size())
			break;
		if (line[i] == ':') {
			++i;
			out.addParam(line.substr(i));
			break;
		}
		out.addParam(readToken(line, i));
	}
	return true;
}
