#include "../includes/Server.hpp"

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
