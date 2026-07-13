#include "../includes/Server.hpp"

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



    msg = ":" + client->getNick() + "!" + client->getUser() + "@localhost PART " + channelName;
    if (cmd.getParams().size() >= 2)
        msg += " :" + cmd.getParams()[1];

    msg += "\r\n";

    sendToChannel(channel, msg);
    channel->removeMember(fd);
    channel->removeOperator(client);
    channel->removeInvite(fd);
    if (channel->getMembers().empty())
    {
        delete channel;
        _channels.erase(channelName);
    }
    std::cout << "PART executed for fd=" << fd << " from channel " << channelName << std::endl;
}
