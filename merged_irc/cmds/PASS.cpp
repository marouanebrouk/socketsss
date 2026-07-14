#include "../includes/Server.hpp"
#include "../includes/ft_irc.hpp"

void Server::PASS_cmd(int fd, const Command &cmd)
{
    Client *client;

    client = _clients[fd];

    if (client->isRegistered())
    {
        sendReply(fd, ":irc.server " + std::string(ERR_ALREADYREGISTRED) + " :You may not reregister\r\n");
        return;
    }

    if (cmd.getParams().size() < 1)
    {
        sendReply(fd, ":irc.server " + std::string(ERR_NEEDMOREPARAMS) + " PASS :Not enough parameters\r\n");
        return;
    }

    if (cmd.getParams()[0] != _password)
    {
        sendReply(fd, ":irc.server " + std::string(ERR_PASSWDMISMATCH) + " :Password incorrect\r\n");
        return;
    }

    client->setAuthorized(true);

    if(registerClient(client))
        std::cout << "PASS executed for fd=" << fd << std::endl;
}
