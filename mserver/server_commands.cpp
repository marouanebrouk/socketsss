#include "Server.hpp"


void Server::DebugClientInfo(int fd)
{
    std::map<int, Client *>::iterator it = _clients.find(fd);
    if (it != _clients.end())
    {
        Client *client = it->second;
        std::cout << "DEBUG: Client fd=" << fd << " IP=" << client->getIP() << " NICK=" << client->getNick() << " IsAuth=" <<
        (client->isAuthorized() ? "true" : "false") << " RealName=" << client->getRealName() << " User=" << client->getUser()
        << " host=" << client->getHost() << " isReg=" << (client->isRegistered() ? "true" : "false") << " isOperator=" <<
        (client->isOperator() ? "true" : "false") << std::endl;
    }
}











void Server::PASS_cmd(int fd, const Command &cmd)
{
    if (cmd.getParams().size() < 1)
    {
        std::cerr << "PASS command missing password parameter" << std::endl;
        return;
    }
    const std::string &password = cmd.getParams()[0];
    if (password == _password)
    {
        std::ostringstream oss;
        oss << "authenticated successfully" << std::endl;
        send(fd, oss.str().c_str(), oss.str().length(), 0);
        std::cout << oss.str();
        std::map<int, Client *>::iterator it = _clients.find(fd);
        if (it != _clients.end())
        {
            Client *client = it->second;
            client->setAuthorized(true);
        }
    }
    else
    {
        std::cerr << "Client <" << fd << "> failed authentication" << std::endl;
        std::cout << "try again" << std::endl;
        return;
    }
}










void Server::QUIT_cmd(int fd)
{
    std::map<int, Client *>::iterator it = _clients.find(fd);
    if (it != _clients.end())
    {
        std::cout << "Client <" << fd << "> has quit" << std::endl;
        send(fd, "Goodbye!\r\n", 10, 0);
        clear_client(fd);
        close(fd);
    }
}











void Server::NICK_cmd(int fd, const Command &cmd)
{
    if (cmd.getParams().size() < 1)
    {
        std::cerr << "NICK command missing nickname parameter" << std::endl;
        return;
    }
    const std::string &nick = cmd.getParams()[0];
    std::map<int, Client *>::iterator it = _clients.find(fd);
    if (it != _clients.end())
    {
        Client *client = it->second;
        client->setNick(nick);

        //buffer this instead of output directly
        std::ostringstream oss;
        oss << "Client <" << fd << "> changed nickname to " << nick << std::endl;
        send(fd, oss.str().c_str(), oss.str().length(), 0);
        std::cout << oss.str();
    }
}












void Server::USER_cmd(int fd, const Command &cmd)
{
    if (cmd.getParams().size() < 4)
    {
        std::cerr << "USER command missing parameters" << std::endl;
        return;
    }
    const std::string &username = cmd.getParams()[0];
    const std::string &hostname = cmd.getParams()[1];
    const std::string &realname = cmd.getParams()[2];
    std::map<int, Client *>::iterator it = _clients.find(fd);
    if (it != _clients.end())
    {
        Client *client = it->second;
        client->setUser(username);
        client->setHost(hostname);
        client->setRealName(realname);
        // example of a user command :  USER <username> <hostname> <realname>
        //buffer this instead of output directly
        std::ostringstream oss;
        oss << "Client <" << fd << "> set USER to " << username << ", HOST to " << hostname
            << ", REALNAME to " << realname << std::endl;

        client->setRegistered(true);
        send(fd, oss.str().c_str(), oss.str().length(), 0);
        std::cout << oss.str();
    }
}




void Server::command_dispatcher(int fd, const Command &cmd)
{
    if (cmd.getCommand() == "DEBUG")
    {
        DebugClientInfo(fd);
    }
    else if (cmd.getCommand() == "PASS")
    {
        PASS_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "NICK")
    {
        NICK_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "USER")
    {
        USER_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "JOIN")
    {
        // JOIN_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "QUIT")
    {
        QUIT_cmd(fd);
    }
    else
        std::cout << "Unknown command '" << cmd.getCommand() << "' from fd=" << fd << std::endl;
}