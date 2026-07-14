#include "../includes/Server.hpp"
#include "../includes/ft_irc.hpp"

void Server::NICK_cmd(int fd, const Command &cmd)
{
    Client *client;
    std::string oldNick;
    std::string newNick;
    std::string msg;

    client = _clients[fd];

    if (cmd.getParams().size() < 1)
    {
        sendReply(fd, ":irc.server " + std::string(ERR_NONICKNAMEGIVEN) + " :No nickname given\r\n");
        return;
    }
    newNick = cmd.getParams()[0];
    if (!isValidNick(newNick))
    {
        sendReply(fd, ":irc.server " + std::string(ERR_ERRONEUSNICKNAME) + " " + newNick + " :Erroneous nickname\r\n");
        return;
    }

    if (nickExists(newNick) && client->getNick() != newNick)
    {
        sendReply(fd, ":irc.server " + std::string(ERR_NICKNAMEINUSE) + " " + newNick + " :Nickname already in use\r\n");
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
