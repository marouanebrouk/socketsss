#include "../Server.hpp"

void Server::PASS_cmd(int fd, const Command &cmd)
{
    Client *client;

    client = _clients[fd];

    if (client->isRegistered())
    {
        sendReply(fd, ":irc.server 462 :You may not reregister\r\n");
        return;
    }

    if (cmd.getParams().size() < 1)
    {
        sendReply(fd, ":irc.server 461 PASS :Not enough parameters\r\n");
        return;
    }

    if (cmd.getParams()[0] != _password)
    {
        sendReply(fd, ":irc.server 464 :Password incorrect\r\n");
        return;
    }

    client->setAuthorized(true);

    if(registerClient(client))
        std::cout << "PASS executed for fd=" << fd << std::endl;
}