#include "../includes/Server.hpp"


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
