#include "../includes/Server.hpp"
#include "../includes/ft_irc.hpp"


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
        sendReply(fd, ":irc.server " + std::string(ERR_NEEDMOREPARAMS) + " KICK :Not enough parameters\r\n");
        return;
    }

    channelName = cmd.getParams()[0];
    targetNick = cmd.getParams()[1];

    if (_channels.find(channelName) == _channels.end())
    {
        sendReply(fd, ":irc.server " + std::string(ERR_NOSUCHCHANNEL) + " " + channelName + " :No such channel\r\n");
        return;
    }

    channel = _channels[channelName];

    if (!channel->isMember(fd))
    {
        sendReply(fd, ":irc.server " + std::string(ERR_NOTONCHANNEL) + " " + channelName + " :You're not on that channel\r\n");
        return;
    }

    if (!channel->isOperator(sender))
    {
        sendReply(fd, ":irc.server " + std::string(ERR_CHANOPRIVSNEEDED) + " " + channelName + " :You're not channel operator\r\n");
        return;
    }

    target = getClientByNickname(targetNick);

    if (!target)
    {
        sendReply(fd, ":irc.server " + std::string(ERR_NOSUCHNICK) + " " + targetNick + " :No such nick\r\n");
        return;
    }

    if (!channel->isMember(target->getFD()))
    {
        sendReply(fd, ":irc.server " + std::string(ERR_USERNOTINCHANNEL) + " " + targetNick + " " + channelName + " :They aren't on that channel\r\n");
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
