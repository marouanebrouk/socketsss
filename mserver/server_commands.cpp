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

void Server::sendToChannel(Channel *channel,const std::string &msg) //: exlude fd as argument
{
    std::map<int, Client *>::const_iterator it;

    it = channel->getMembers().begin();

    while (it != channel->getMembers().end())
    {
        // if (it->first != excludeFd)
            sendReply(it->first, msg);
        ++it;
    }
}


void Server::PASS_cmd(int fd, const Command &cmd)
{
    Client *client;

    client = _clients[fd];

    if (client->isRegistered())
    {
        sendReply(fd,
            ":irc.server 462 :You may not reregister\r\n");
        return;
    }

    if (cmd.getParams().size() < 1)
    {
        sendReply(fd,
            ":irc.server 461 PASS :Not enough parameters\r\n");
        return;
    }

    if (cmd.getParams()[0] != _password)
    {
        sendReply(fd,
            ":irc.server 464 :Password incorrect\r\n");
        return;
    }

    client->setAuthorized(true);

    registerClient(client);
}
// quit command should remove the client from all channels and then clear the client data and close the connection



void Server::JOIN_cmd(int fd, const Command &cmd)
{
    Client *client;
    Channel *channel;
    std::string channelName;
    std::string joinMsg;

    client = _clients[fd];

    if (cmd.getParams().size() < 1)
    {
        sendReply(fd,
            ":irc.server 461 JOIN :Not enough parameters\r\n");
        return;
    }

    channelName = cmd.getParams()[0];

    if (!isValidChannelName(channelName))
    {
        sendReply(fd,
            ":irc.server 403 "
            + channelName
            + " :No such channel\r\n");
        return;
    }

    if (_channels.find(channelName) == _channels.end())
        _channels[channelName] = new Channel(channelName);

    channel = _channels[channelName];

    if (channel->isMember(fd))
        return;

    channel->addMember(client);

    if (channel->getMembers().size() == 1)
        channel->addOperator(client);

    joinMsg = ":"
        + client->getNick()
        + "!"
        + client->getUser()
        + "@localhost JOIN "
        + channelName
        + "\r\n";

    sendToChannel(channel, joinMsg);
}


void Server::NICK_cmd(int fd, const Command &cmd)
{
    Client *client;
    std::string oldNick;
    std::string newNick;
    std::string msg;

    client = _clients[fd];

    if (cmd.getParams().size() < 1)
    {
        sendReply(fd,
            ":irc.server 431 :No nickname given\r\n");
        return;
    }

    newNick = cmd.getParams()[0];

    if (!isValidNick(newNick))
    {
        sendReply(fd,
            ":irc.server 432 "
            + newNick
            + " :Erroneous nickname\r\n");
        return;
    }

    if (nickExists(newNick)
        && client->getNick() != newNick)
    {
        sendReply(fd,
            ":irc.server 433 "
            + newNick
            + " :Nickname already in use\r\n");
        return;
    }

    oldNick = client->getNick();

    client->setNick(newNick);

    if (!oldNick.empty())
    {
        msg = ":" + oldNick
            + " NICK :" + newNick + "\r\n";

        sendReply(fd, msg);
    }

    registerClient(client);
}

void Server::USER_cmd(int fd, const Command &cmd)
{
    Client *client;

    client = _clients[fd];

    if (client->isRegistered())
    {
        sendReply(fd,
            ":irc.server 462 :You may not reregister\r\n");
        return;
    }

    if (cmd.getParams().size() < 4)
    {
        sendReply(fd,
            ":irc.server 461 USER :Not enough parameters\r\n");
        return;
    }

    client->setUser(cmd.getParams()[0]);
    // client->setHost(cmd.getParams()[1]);    those two parameters are ignored in most irc servers, so i will ignore them too
    // client->setServer(cmd.getParams()[2]);
    client->setRealName(cmd.getParams()[3]);

    registerClient(client);
}

void Server::DebugChannelInfo(const std::string &channelName)
{
    std::map<std::string, Channel *>::iterator it = _channels.find(channelName);
    if (it != _channels.end())
    {
        Channel *channel = it->second;
        std::cout << "Channel: " << channelName << std::endl;
        std::cout << "Members: ";
        for (std::map<int, Client *>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); it++)
        {
            std::cout << it->first << " ";
        }
        std::cout << std::endl;

        std::cout << "Operators: ";
        for (std::set<int>::const_iterator it = channel->getOperators().begin(); it != channel->getOperators().end(); it++)
        {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Channel " << channelName << " not found." << std::endl;
    }
}

void Server::PART_cmd(int fd, const Command &cmd)
{
    Client *client;
    Channel *channel;
    std::string channelName;
    std::string msg;

    client = _clients[fd];

    if (cmd.getParams().size() < 1)
    {
        sendReply(fd,
            ":irc.server 461 PART :Not enough parameters\r\n");
        return;
    }

    channelName = cmd.getParams()[0];

    if (_channels.find(channelName) == _channels.end())
    {
        sendReply(fd,
            ":irc.server 403 "
            + channelName
            + " :No such channel\r\n");
        return;
    }

    channel = _channels[channelName];

    if (!channel->isMember(fd))
    {
        sendReply(fd,
            ":irc.server 442 "
            + channelName
            + " :You're not on that channel\r\n");
        return;
    }

    msg = ":"
        + client->getNick()
        + " PART "
        + channelName
        + "\r\n";

    sendToChannel(channel, msg);

    channel->removeMember(fd);
    channel->removeOperator(client);

    if (channel->getMembers().empty())
    {
        delete channel;
        _channels.erase(channelName);
    }
}

void Server::CAP_cmd(int fd, const Command &cmd)
{
    if (cmd.getParams()[0] == "LS")
    {
        std::string str = "CAP * LS :\r\n";
        send(fd, str.c_str(), str.length(), 0);
        std::cout << "Sent CAP * LS :" << std::endl;
    }
}

bool Server::canRegister(Client *client)
{
    if (!client->isAuthorized())
        return (false);

    if (client->getNick().empty())
        return (false);

    if (client->getUser().empty())
        return (false);

    return (true);
};


void Server::registerClient(Client *client)
{
    std::string welcome;

    if (client->isRegistered())
        return;

    if (!canRegister(client))
        return;

    client->setRegistered(true);

    welcome = ":irc.server 001 "
        + client->getNick()
        + " :Welcome to the IRC server\r\n";

    sendReply(client->getFD(), welcome);
}


bool Server::isAllowedBeforeRegister(const std::string &cmd)
{
    return (
        cmd == "CAP" ||
        cmd == "PASS" ||
        cmd == "NICK" ||
        cmd == "USER" ||
        cmd == "QUIT" ||
        cmd == "PING"
    );
}

void Server::command_dispatcher(int fd, const Command &cmd)
{
    Client *client = _clients[fd];

    if (!client->isRegistered())
    {
        if (!isAllowedBeforeRegister(cmd.getCommand()))
        {
            std::string err = ":irc.server 451 * :You have not registered\r\n";
            send(fd, err.c_str(), err.size(), 0);
            return;
        }
    }

    if (cmd.getCommand() == "INFO")
    {
        DebugClientInfo(fd);
    }
    else if (cmd.getCommand() == "PING")
    {
        std::string pong = "PONG :" + cmd.getParams()[0] + "\r\n";
        send(fd, pong.c_str(), pong.length(), 0);
        std::cout << "Sent PONG :" << cmd.getParams()[0] << std::endl;
    }
    else if (cmd.getCommand() == "DEBUGCHANNEL")
    {
        DebugChannelInfo(cmd.getParams()[0]);
    }
    else if (cmd.getCommand() == "PASS")
    {
        PASS_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "NICK")
    {
        NICK_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "USER")
    {
        USER_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "JOIN")
    {
        JOIN_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "PART")
    {
        PART_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "CAP")
    {
        CAP_cmd(fd, cmd);
    }
    else
        std::cout << "Unknown command '" << cmd.getCommand() << "' from fd=" << fd << std::endl;
}