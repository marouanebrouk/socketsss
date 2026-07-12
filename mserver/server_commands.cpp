#include "Server.hpp"




// quit command should remove the client from all channels and then clear the client data and close the connection


void Server::sendNamesList(Client *client, Channel *channel)
{
    std::map<int, Client *>::const_iterator it;
    std::string names;
    std::string reply;

    it = channel->getMembers().begin();
    while (it != channel->getMembers().end())
    {
        if (channel->isOperator(it->second))
            names += "@";

        names += it->second->getNick();

        ++it;
        if (it != channel->getMembers().end())
            names += " ";
    }

    reply = ":irc.server 353 " + client->getNick() + " = " + channel->getName() + " :" + names + "\r\n";

    sendReply(client->getFD(), reply);

    reply = ":irc.server 366 " + client->getNick() + " " + channel->getName() + " :End of /NAMES list.\r\n";

    sendReply(client->getFD(), reply);
}





void Server::DebugChannelInfo(const std::string &channelName)
{
    std::map<std::string, Channel *>::iterator it = _channels.find(channelName);
    if (it != _channels.end())
    {
        Channel *channel = it->second;
        std::cout << "hasKey: " << channel->hasKey() << std::endl;
        std::cout << "hasLimit: " << channel->hasLimit() << std::endl;
        std::cout << "isInviteOnly: " << channel->isInviteOnly() << std::endl;
        std::cout << "isTopicRestricted: " << channel->isTopicRestricted() << std::endl;
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



bool Server::canRegister(Client *client)
{
    if (!client->isAuthorized())
        return (false);
    if (client->getNick().empty())
    {
        std::cout << "Client fd=" << client->getFD() << " cannot register: nickname is empty" << std::endl;
        return (false);
    }
    if (client->getUser().empty())
    {
        std::cout << "Client fd=" << client->getFD() << " cannot register: username is empty" << std::endl;
        return (false);
    }
    return (true);
};


bool Server::registerClient(Client *client)
{
    std::string welcome;

    if (client->isRegistered())
        return false;
    if (!canRegister(client))
        return false;
    client->setRegistered(true);
    welcome = ":irc.server 001 " + client->getNick() + " :Welcome to the IRC server\r\n";
    sendReply(client->getFD(), welcome);
    std::cout << welcome;
    return true;
}

Client *Server::getClientByNickname(const std::string &nick)
{
    std::map<int, Client *>::iterator it;

    it = _clients.begin();

    while (it != _clients.end())
    {
        if (it->second->getNick() == nick)
            return (it->second);
        ++it;
    }
    return (NULL);
}











void Server::command_dispatcher(int fd, const Command &cmd)
{
    Client *client = _clients[fd];

    if (!client->isRegistered())
    {
        std::cout << "Client fd=" << fd << " is not registered. Command: " << cmd.getCommand() << std::endl;
        if (!isAllowedBeforeRegister(cmd.getCommand()))
        {
            std::string err = ":irc.server 451 * :You have not registered\r\n";
            std::cout << "Sent 451 * :You have not registered to fd=" << fd << std::endl;
            send(fd, err.c_str(), err.size(), 0);
            return;
        }
    }

    if (cmd.getCommand() == "INFO" || cmd.getCommand() == "info")
    {
        DebugClientInfo(fd);
    }
    else if (cmd.getCommand() == "PING" || cmd.getCommand() == "ping")
    {
        if (cmd.getParams().empty())
            return;
        std::string pong = "PONG :" + cmd.getParams()[0] + "\r\n";
        send(fd, pong.c_str(), pong.length(), 0);
        std::cout << "Sent PONG :" << cmd.getParams()[0] << std::endl;
    }
    else if (cmd.getCommand() == "DEBUGCHANNEL" || cmd.getCommand() == "debugchannel")
    {
        if(!cmd.getParams().empty())
            DebugChannelInfo(cmd.getParams()[0]);
    }
    else if (cmd.getCommand() == "PASS" || cmd.getCommand() == "pass")
    {
        PASS_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "NICK" || cmd.getCommand() == "nick")
    {
        NICK_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "USER" || cmd.getCommand() == "user")
    {
        USER_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "JOIN" || cmd.getCommand() == "join")
    {
        JOIN_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "PART" || cmd.getCommand() == "part")
    {
        PART_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "CAP" || cmd.getCommand() == "cap")
    {
        CAP_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "TOPIC" || cmd.getCommand() == "topic")
    {
        TOPIC_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "INVITE" || cmd.getCommand() == "invite")
        INVITE_cmd(fd, cmd);
    else if (cmd.getCommand() == "PRIVMSG" || cmd.getCommand() == "privmsg")
    {
    //using nc client
    // PRIVMSG nickname :hello
    // PRIVMSG #channel :hello everyone
    PRIVMSG_cmd(fd, cmd);
    }
    else if ((cmd.getCommand())== "KICK" || cmd.getCommand() == "kick")
    {
        KICK_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "MODE" || cmd.getCommand() == "mode")
    {
        MODE_cmd(fd, cmd);
    }
    else
    {
        std::cout << "Unknown command '" << cmd.getCommand() << "' from fd=" << fd << std::endl;
        sendReply(fd, ":irc.server 421 " + cmd.getCommand() + " :Unknown command\r\n");
    }
}