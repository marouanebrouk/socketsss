#include "Server.hpp"


void Server::DebugClientInfo(int fd)
{
    std::map<int, Client *>::iterator it = _clients.find(fd);
    if (it != _clients.end())
    {
        Client *client = it->second;
        std::ostringstream oss;
        oss << "DEBUG: Client fd=" << fd << " IP=" << client->getIP() << " NICK=" << client->getNick() << " IsAuth=" <<
        (client->isAuthorized() ? "true" : "false") << " RealName=" << client->getRealName() << " User=" << client->getUser()
        << " host=" << client->getHost() << " isReg=" << (client->isRegistered() ? "true" : "false") << std::endl;
        std::cout << oss.str();
        send(fd, oss.str().c_str(), oss.str().length(), 0);
    }
}



bool Server::isValidNick(const std::string &nick)
{
    size_t i;

    if (nick.empty())
        return (false);

    i = 0;
    while (i < nick.length())
    {
        if (!std::isalnum(nick[i])
            && nick[i] != '_'
            && nick[i] != '-')
            return (false);
        ++i;
    }
    return (true);
}

bool Server::isValidChannelName(const std::string &name)
{
    if (name.empty())
        return (false);

    if (name[0] != '#')
        return (false);

    return (true);
}

bool Server::nickExists(const std::string &nick)
{
    std::map<int, Client *>::iterator it;

    it = _clients.begin();
    while (it != _clients.end())
    {
        if (it->second->getNick() == nick)
            return (true);
        ++it;
    }
    return (false);
}


void Server::sendReply(int fd, const std::string &msg)
{
    send(fd, msg.c_str(), msg.size(), 0);
}

void Server::sendToChannel(Channel *channel,const std::string &msg,int excludeFd) //: exclude fd as argument
{
    std::map<int, Client *>::const_iterator it;

    it = channel->getMembers().begin();

    while (it != channel->getMembers().end())
    {
        if (it->first != excludeFd)
            sendReply(it->first, msg);
        ++it;
    }
}



bool Server::isAllowedBeforeRegister(const std::string &cmd)
{
    return (
        cmd == "CAP" ||
        cmd == "PASS" ||
        cmd == "NICK" ||
        cmd == "USER" ||
        cmd == "QUIT" ||
        cmd == "PING" ||
        cmd == "INFO"
    );
}