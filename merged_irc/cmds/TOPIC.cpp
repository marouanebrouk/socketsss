#include "../includes/Server.hpp"

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
