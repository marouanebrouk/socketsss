#include "../Server.hpp"

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
        if (limit <= 0)
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
    else
    {
        sendReply(fd, ":irc.server 472 " + mode + " :is unknown mode char to me for " + channelName + "\r\n");
        return;
    }

    std::string msg;
    msg = ":" + sender->getNick() + "!" + sender->getUser() + "@localhost MODE " + channelName + " " + mode;
    if ((mode == "+o" || mode == "-o") && cmd.getParams().size() >= 3)
        msg += " " + cmd.getParams()[2];
    msg += "\r\n";
    sendToChannel(channel, msg);
};
