#include "../includes/Command.hpp"

void Command::_setcmd(std::string cmd ) {_cmd = cmd;}
void Command::_setparams(std::string param) { _parametres.push_back(param);}
std::string Command::_getcmd() { return _cmd;}
void Command::_settrailing(std::string trailing)  { _trailing = trailing;}
std::string Command::_gettrailing() const { return _trailing;}
std::vector<std::string> Command::_getparametres() { return _parametres;}
size_t  Command::_getsize() { return _parametres.size();}

std::string Command::getCommand() const {
    return _cmd;
}

std::vector<std::string> Command::getParams() const {
    std::vector<std::string> params = _parametres;
    if (!_trailing.empty()) {
        params.push_back(_trailing);
    }
    return params;
}

std::string Command::getPrefix() const {
    return ""; // anas Command doesn't parse prefix, so return empty string
}
