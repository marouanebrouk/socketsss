#include "../includes/Server.hpp"

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
