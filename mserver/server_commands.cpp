#include "Server.hpp"




void Server::PASS_cmd(int fd, const Command &cmd)
{
    Client *client;

    client = _clients[fd];

    if (client->isRegistered())
    {
        sendReply(fd, ":irc.server 462 :You may not reregister\r\n");
        return;
    }

    if (cmd.getParams().size() < 1)
    {
        sendReply(fd, ":irc.server 461 PASS :Not enough parameters\r\n");
        return;
    }

    if (cmd.getParams()[0] != _password)
    {
        sendReply(fd, ":irc.server 464 :Password incorrect\r\n");
        return;
    }

    client->setAuthorized(true);

    if(registerClient(client))
        std::cout << "PASS executed for fd=" << fd << std::endl;
}
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

void Server::JOIN_cmd(int fd, const Command &cmd)
{
    Client      *client;
    Channel     *channel;
    std::string channelName;
    std::string joinMsg;

    client = _clients[fd];

    if (cmd.getParams().size() < 1)
    {
        sendReply(fd, ":irc.server 461 JOIN :Not enough parameters\r\n");
        return;
    }

    channelName = cmd.getParams()[0];

    if (!isValidChannelName(channelName))
    {
        sendReply(fd, ":irc.server 403 " + channelName + " :No such channel\r\n");
        return;
    }

    if (_channels.find(channelName) == _channels.end())
        _channels[channelName] = new Channel(channelName);

    channel = _channels[channelName];

    if (channel->isMember(fd))
        return;

    /* Invite-only channel */
    if (channel->isInviteOnly())
    {
        if (!channel->isInvited(client->getFD()))
        {
            sendReply(fd, ":irc.server 473 " + channelName + " :Cannot join channel (+i)\r\n");
            return;
        }
        channel->removeInvite(client->getFD());
    }
    
    if (channel->hasKey())
    { 
        if (cmd.getParams().size() < 2 || cmd.getParams()[1] != channel->getKey()) 
        { 
            sendReply(fd, ":irc.server 475 " + channelName + " :Cannot join channel (+k)\r\n");
             return; 
        } 
    }

    if (channel->hasLimit() && channel->getMembers().size() >= channel->getUserLimit())
    { 
        sendReply(fd, ":irc.server 471 " + channelName + " :Cannot join channel (+l)\r\n");
        return; 
    }
    channel->addMember(client);

    if (channel->getMembers().size() == 1)
        channel->addOperator(client);

    joinMsg = ":" + client->getNick() + "!" + client->getUser() + "@localhost JOIN " + channelName + "\r\n";

    sendToChannel(channel, joinMsg);

    if (channel->getTopic().empty())
        sendReply(fd, ":irc.server 331 " + client->getNick() + " " + channelName + " :No topic is set\r\n");
    else
        sendReply(fd, ":irc.server 332 " + client->getNick() + " " + channelName + " :" + channel->getTopic() + "\r\n");
    sendNamesList(client, channel);
    std::cout << "JOIN executed for fd=" << fd << " to channel " << channelName << std::endl;
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
        sendReply(fd, ":irc.server 431 :No nickname given\r\n");
        return;
    }
    newNick = cmd.getParams()[0];
    if (!isValidNick(newNick))
    {
        sendReply(fd, ":irc.server 432 " + newNick + " :Erroneous nickname\r\n");
        return;
    }

    if (nickExists(newNick) && client->getNick() != newNick)
    {
        sendReply(fd, ":irc.server 433 " + newNick + " :Nickname already in use\r\n");
        return;
    }
    oldNick = client->getNick();
    client->setNick(newNick);
    if (!oldNick.empty())
    {
        msg = ":" + oldNick + " NICK :" + newNick + "\r\n";
        sendReply(fd, msg);
    }

    std::cout << "NICK executed for fd=" << fd << " new nick: " << newNick << std::endl;
    if (registerClient(client))
        std::cout << "Client fd=" << fd << " registered successfully" << std::endl;
}

void Server::USER_cmd(int fd, const Command &cmd)
{
    Client *client;

    client = _clients[fd];

    if (client->isRegistered())
    {
        sendReply(fd, ":irc.server 462 :You may not reregister\r\n");
        return;
    }

    if (cmd.getParams().size() < 4)
    {
        sendReply(fd, ":irc.server 461 USER :Not enough parameters\r\n");
        return;
    }

    client->setUser(cmd.getParams()[0]);
    // client->setHost(cmd.getParams()[1]);    those two parameters are ignored in most irc servers, so i will ignore them too
    // client->setServer(cmd.getParams()[2]);
    client->setRealName(cmd.getParams()[3]);

    if (registerClient(client))
    {
        std::cout << "USER executed for fd=" << fd << std::endl;
    }
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

void Server::PART_cmd(int fd, const Command &cmd)
{
    Client *client;
    Channel *channel;
    std::string channelName;
    std::string msg;

    client = _clients[fd];

    if (cmd.getParams().size() < 1)
    {
        sendReply(fd,":irc.server 461 PART :Not enough parameters\r\n");
        return;
    }

    channelName = cmd.getParams()[0];

    if (_channels.find(channelName) == _channels.end())
    {
        sendReply(fd, ":irc.server 403 " + channelName + " :No such channel\r\n");
        return;
    }

    channel = _channels[channelName];

    if (!channel->isMember(fd))
    {
        sendReply(fd, ":irc.server 442 " + channelName + " :You're not on that channel\r\n");
        return;
    }

    msg = ":" + client->getNick() + " PART " + channelName + "\r\n";

    sendToChannel(channel, msg, fd);

    channel->removeMember(fd);
    channel->removeOperator(client);

    if (channel->getMembers().empty())
    {
        delete channel;
        _channels.erase(channelName);
    }
    std::cout << "PART executed for fd=" << fd << " from channel " << channelName << std::endl;
}

void Server::CAP_cmd(int fd, const Command &cmd)
{
    if (cmd.getParams()[0] == "LS")
    {
        std::string str = "CAP * LS :\r\n";
        send(fd, str.c_str(), str.length(), 0);
        // std::cout << "Sent CAP * LS :" << std::endl;
    }
    std::cout << "CAP executed for fd=" << fd << std::endl;
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



void Server::PRIVMSG_cmd(int fd, const Command &cmd)
{
    Client      *sender;
    Client      *targetClient;
    Channel     *channel;

    std::string target;
    std::string message;
    std::string fullMsg;

    sender = _clients[fd];
    if (cmd.getParams().size() < 1)
    {
        sendReply(fd, ":irc.server 411 :No recipient given (PRIVMSG)\r\n");
        return;
    }
    if (cmd.getParams().size() < 2)
    {
        sendReply(fd, ":irc.server 412 :No text to send\r\n");
        return;
    }

    target = cmd.getParams()[0];
    message = cmd.getParams()[1];

    /*
    ** IRC message format
    */
    fullMsg = ":" + sender->getNick() + "!" + sender->getUser() + "@localhost PRIVMSG " + target + " :" + message + "\r\n";

    /*
    ** CHANNEL MESSAGE
    */
    if (target[0] == '#')
    {
        if (_channels.find(target) == _channels.end())
        {
            sendReply(fd, ":irc.server 403 " + target + " :No such channel\r\n");
            return;
        }

        channel = _channels[target];
        if (!channel->isMember(fd))
        {
            sendReply(fd, ":irc.server 404 " + target + " :Cannot send to channel\r\n");
            return;
        }
        //Broadcast to everyone except sender
        sendToChannel(channel, fullMsg, fd);
    }
    else
    {
        /*
        ** PRIVATE MESSAGE TO USER
        */
        targetClient = getClientByNickname(target);
        if (!targetClient)
        {
            sendReply(fd, ":irc.server 401 " + target + " :No such nick\r\n");
            return;
        }

        sendReply(targetClient->getFD(), fullMsg);
    }
}


void Server::TOPIC_cmd(int fd, const Command &cmd)
{
    Client      *client;
    Channel     *channel;
    std::string channelName;
    std::string msg;

    client = _clients[fd];

    if (cmd.getParams().size() < 1)
    {
        sendReply(fd,":irc.server 461 TOPIC :Not enough parameters\r\n");
        return;
    }

    channelName = cmd.getParams()[0];

    if (_channels.find(channelName) == _channels.end())
    {
        sendReply(fd,":irc.server 403 " + channelName + " :No such channel\r\n");
        return;
    }

    channel = _channels[channelName];

    if (!channel->isMember(fd))
    {
        sendReply(fd,":irc.server 442 " + channelName + " :You're not on that channel\r\n");
        return;
    }

    /*
    ** READ TOPIC
    */
    if (cmd.getParams().size() == 1)
    {
        if (channel->getTopic().empty())
        {
            sendReply(fd,":irc.server 331 "+ client->getNick()+ " " + channelName + " :No topic is set\r\n");
        }
        else
        {
            sendReply(fd,":irc.server 332 "+ client->getNick()+ " "+ channelName+ " :" + channel->getTopic() + "\r\n");
        }
        return;
    }

    /*
    ** SET TOPIC
    */
    if (channel->isTopicRestricted() && !channel->isOperator(client))
    { 
        sendReply(fd, ":irc.server 482 " + channelName + " :You're not channel operator\r\n");
        return; 
    }
    channel->setTopic(cmd.getParams()[1]);

    msg = ":" + client->getNick() + "!" + client->getUser() + "@localhost TOPIC " + channelName + " :" + channel->getTopic() + "\r\n";

    sendToChannel(channel, msg);
}




void Server::INVITE_cmd(int fd, const Command &cmd)
{
    Client      *sender;
    Client      *target;
    Channel     *channel;

    std::string nick;
    std::string channelName;
    std::string inviteMsg;

    sender = _clients[fd];

    if (cmd.getParams().size() < 2)
    {
        sendReply(fd, ":irc.server 461 INVITE :Not enough parameters\r\n");
        return;
    }

    nick = cmd.getParams()[0];
    channelName = cmd.getParams()[1];

    target = getClientByNickname(nick);

    if (!target)
    {
        sendReply(fd, ":irc.server 401 " + nick + " :No such nick\r\n");
        return;
    }

    if (_channels.find(channelName) == _channels.end())
    {
        sendReply(fd, ":irc.server 403 " + channelName + " :No such channel\r\n");
        return;
    }

    channel = _channels[channelName];

    if (!channel->isMember(fd))
    {
        sendReply(fd, ":irc.server 442 " + channelName + " :You're not on that channel\r\n");
        return;
    }

    if (!channel->isOperator(sender))
    {
        sendReply(fd, ":irc.server 482 " + channelName + " :You're not channel operator\r\n");
        return;
    }

    if (channel->isMember(target->getFD()))
    {
        sendReply(fd, ":irc.server 443 " + nick + " " + channelName + " :is already on channel\r\n");
        return;
    }

    channel->inviteClient(target);

    sendReply(fd, ":irc.server 341 " + sender->getNick() + " " + nick + " " + channelName + "\r\n");

    inviteMsg = ":" + sender->getNick() + "!" + sender->getUser() + "@localhost INVITE " + nick + " :" + channelName + "\r\n";

    sendReply(target->getFD(), inviteMsg);
}



void Server::KICK_cmd(int fd, const Command &cmd)
{
    Client      *sender;
    Client      *target;
    Channel     *channel;

    std::string channelName;
    std::string targetNick;
    std::string reason;
    std::string kickMsg;

    sender = _clients[fd];

    if (cmd.getParams().size() < 2)
    {
        sendReply(fd, ":irc.server 461 KICK :Not enough parameters\r\n");
        return;
    }

    channelName = cmd.getParams()[0];
    targetNick = cmd.getParams()[1];

    if (_channels.find(channelName) == _channels.end())
    {
        sendReply(fd, ":irc.server 403 " + channelName + " :No such channel\r\n");
        return;
    }

    channel = _channels[channelName];

    if (!channel->isMember(fd))
    {
        sendReply(fd, ":irc.server 442 " + channelName + " :You're not on that channel\r\n");
        return;
    }

    if (!channel->isOperator(sender))
    {
        sendReply(fd, ":irc.server 482 " + channelName + " :You're not channel operator\r\n");
        return;
    }

    target = getClientByNickname(targetNick);

    if (!target)
    {
        sendReply(fd, ":irc.server 401 " + targetNick + " :No such nick\r\n");
        return;
    }

    if (!channel->isMember(target->getFD()))
    {
        sendReply(fd, ":irc.server 441 " + targetNick + " " + channelName + " :They aren't on that channel\r\n");
        return;
    }

    if (cmd.getParams().size() >= 3)
        reason = cmd.getParams()[2];
    else
        reason = sender->getNick();

    kickMsg = ":" + sender->getNick() + "!" + sender->getUser() + "@localhost KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";

    sendToChannel(channel, kickMsg,fd);

    channel->removeMember(target->getFD());
    channel->removeOperator(target);

    if (channel->getMembers().empty())
    {
        delete channel;
        _channels.erase(channelName);
    }
}


void Server::MODE_cmd(int fd, const Command &cmd)
{
    Client      *sender;
    Channel     *channel;
    std::string channelName;
    std::string mode;

    
    if (cmd.getParams().size() < 2)
    {
        sendReply(fd, ":irc.server 461 MODE :Not enough parameters\r\n");
        return;
    }
    sender = _clients[fd];
    channelName = cmd.getParams()[0];
    mode = cmd.getParams()[1];

    if (_channels.find(channelName) == _channels.end())
    {
        sendReply(fd, ":irc.server 403 " + channelName + " :No such channel\r\n");
        return;
    }
    channel = _channels[channelName];
    if (!channel->isMember(fd))
    {
        sendReply(fd, ":irc.server 442 " + channelName + " :You're not on that channel\r\n");
        return;
    }
    if (!channel->isOperator(sender))
    {
        sendReply(fd, ":irc.server 482 " + channelName + " :You're not channel operator\r\n");
        return;
    }

    if (mode == "+l")
    {
        size_t limit;
        if (cmd.getParams().size() < 3)
        {
            sendReply(fd, ":irc.server 461 MODE :Not enough parameters\r\n");
            return;
        }
        limit = std::atoi(cmd.getParams()[2].c_str());
        if (limit == 0)
        {
            sendReply(fd, ":irc.server 461 MODE :Invalid limit\r\n");
            return;
        }
        channel->setUserLimit(limit);
    }
    else if (mode == "-l")
        channel->removeUserLimit();
    else if (mode == "+o" || mode == "-o")
    {
        Client *target;
        if (cmd.getParams().size() < 3)
        {
            sendReply(fd, ":irc.server 461 MODE :Not enough parameters\r\n");
            return;
        }
        target = getClientByNickname(cmd.getParams()[2]);
        if (!target)
        {
            sendReply(fd, ":irc.server 401 " + cmd.getParams()[2] + " :No such nick\r\n");
            return;
        }
        if (!channel->isMember(target->getFD()))
        {
            sendReply(fd, ":irc.server 441 " + target->getNick() + " " + channelName + " :They aren't on that channel\r\n");
            return;
        }
        if (mode == "+o")
            channel->addOperator(target);
        else
            channel->removeOperator(target);
    }
    else if(mode == "+k")
    {
        if (cmd.getParams().size() < 3)
        {
            sendReply(fd, ":irc.server 461 MODE :Not enough parameters\r\n");
            return;
        }
        channel->setKey(cmd.getParams()[2]);
    }
    else if(mode == "-k")
        channel->removeKey();
    else if (mode == "+t")
        channel->setTopicRestricted(true);
    else if (mode == "-t")
        channel->setTopicRestricted(false);
    else if (mode == "+i")
        channel->setInviteOnly(true);
    else if (mode == "-i")
        channel->setInviteOnly(false);

    std::string msg;
    msg = ":" + sender->getNick() + "!" + sender->getUser() + "@localhost MODE " + channelName + " " + mode;
    if ((mode == "+o" || mode == "-o") && cmd.getParams().size() >= 3)
        msg += " " + cmd.getParams()[2];
    msg += "\r\n";
    sendToChannel(channel, msg);
};


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
    // else if (cmd.getCommand() == "PART" || cmd.getCommand() == "part")
    // {
    //     PART_cmd(fd, cmd);
    // }
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
    else if (cmd.getCommand() == "KICK" || cmd.getCommand() == "kick")
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