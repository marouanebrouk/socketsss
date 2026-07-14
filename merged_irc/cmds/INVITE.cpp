#include "../includes/Server.hpp"
#include "../includes/ft_irc.hpp"


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
        sendReply(fd, ":irc.server " + std::string(ERR_NEEDMOREPARAMS) + " INVITE :Not enough parameters\r\n");
        return;
    }

    nick = cmd.getParams()[0];
    channelName = cmd.getParams()[1];

    target = getClientByNickname(nick);

    if (!target)
    {
        sendReply(fd, ":irc.server " + std::string(ERR_NOSUCHNICK) + " " + nick + " :No such nick\r\n");
        return;
    }

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

    if (channel->isMember(target->getFD()))
    {
        sendReply(fd, ":irc.server " + std::string(ERR_USERONCHANNEL) + " " + nick + " " + channelName + " :is already on channel\r\n");
        return;
    }

    channel->inviteClient(target);

    sendReply(fd, ":irc.server " + std::string(RPL_INVITING) + " " + sender->getNick() + " " + nick + " " + channelName + "\r\n");

    inviteMsg = ":" + sender->getNick() + "!" + sender->getUser() + "@localhost INVITE " + nick + " :" + channelName + "\r\n";

    sendReply(target->getFD(), inviteMsg);
}
