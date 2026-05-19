#include "Server.hpp"


void Server::DebugClientInfo(int fd)
{
    std::map<int, Client *>::iterator it = _clients.find(fd);
    if (it != _clients.end())
    {
        Client *client = it->second;
        std::ostringstream oss;
        oss << "DEBUG: Client fd=" << fd << " IP=" << client->getIP() << " NICK=" << client->getNick() << " IsAuth=" <<
        (client->isAuthorized() ? "true" : "false") << " RealName=" << client->getRealName() << " User=" << client->getUser()
        << " host=" << client->getHost() << " isReg=" << (client->isRegistered() ? "true" : "false") << std::endl;
        std::cout << oss.str();
        send(fd, oss.str().c_str(), oss.str().length(), 0);
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




void Server::JOIN_cmd(int fd, const Command &cmd)
{
    if (cmd.getParams().size() < 1)
    {
        std::cerr << "JOIN command missing channel name parameter" << std::endl;
        return;
    }
    const std::string &channelName = cmd.getParams()[0];
    std::map<int, Client *>::iterator it = _clients.find(fd);
    if (it != _clients.end())
    {
        Client *client = it->second;
        // For now, just log the join attempt. You can add actual channel management later.
        //here i need to create a channel if it doesn't exist
        if (_channels.find(channelName) == _channels.end())
            _channels[channelName] = new Channel();
        Channel *channel = _channels.find(channelName)->second;
        if (!channel->isMember(client->getFD()))
            channel->addMember(client);

        if (channel->getMembers().size() == 1)
            channel->addOperator(client);
        std::ostringstream oss;
        oss << "Client <" << fd << "> joined channel " << channelName << "\r\n";
        send(fd, oss.str().c_str(), oss.str().length(), 0);
        std::cout << oss.str();
    }
}

void Server::NICK_cmd(int fd, const Command &cmd)
{
    if (cmd.getParams().size() < 1)
    {
        std::cerr << "NICK command missing nickname parameter\r\n" << std::endl;
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
        oss << "nickname changed to " << nick << "\r\n";
        send(fd, oss.str().c_str(), oss.str().length(), 0);
        std::cout << oss.str();
    }
}




void Server::USER_cmd(int fd, const Command &cmd)
{
    if (cmd.getParams().size() < 3)
    {
        std::cerr << "USER command missing parameters" << std::endl;
        return;
    }
    const std::string &username = cmd.getParams()[0];
    const std::string &hostname = cmd.getParams()[1];
    const std::string &server = cmd.getParams()[2];
    const std::string &realname = cmd.getParams()[3];
    std::map<int, Client *>::iterator it = _clients.find(fd);
    if (it != _clients.end())
    {
        Client *client = it->second;
        client->setUser(username);
        client->setHost(hostname);
        client->setServer(server);
        client->setRealName(realname);
        // example of a user command :  USER <username> <hostname> <realname> <server>
        //buffer this instead of output directly
        std::ostringstream oss;
        oss << "Client <" << fd << "> set USERNAME to " << username << ", HOSTNAME to " << hostname
            << ", REALNAME to " << realname << ", SERVER to " << server << "\r\n";

        client->setRegistered(true);
        send(fd, oss.str().c_str(), oss.str().length(), 0);
        std::cout << oss.str();
        send(fd, ":IRC 001 :Welcome to IRC\r\n", 39, 0);
    }
}

void Server::DebugChannelInfo(const std::string &channelName)
{
    std::map<std::string, Channel *>::iterator it = _channels.find(channelName);
    if (it != _channels.end())
    {
        Channel *channel = it->second;
        std::cout << "Channel: " << channelName << std::endl;
        std::cout << "Members: ";
        for (std::map<int, Client *>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); it++)
        {
            std::cout << it->first << " ";
        }
        std::cout << std::endl;

        std::cout << "Operators: ";
        for (std::set<int>::const_iterator it = channel->getOperators().begin(); it != channel->getOperators().end(); it++)
        {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Channel " << channelName << " not found." << std::endl;
    }
}

void Server::PART_cmd(int fd, const Command &cmd)
{
    if (cmd.getParams().size() < 1)
    {
        std::cerr << "PART command missing channel parameter" << std::endl;
        return;
    }
    const std::string &channelName = cmd.getParams()[0];
    std::map<int, Client *>::iterator it = _clients.find(fd);
    if (it != _clients.end())
    {
        Client *client = it->second;
        Channel *channel = _channels[channelName];
        if (channel)
        {
            channel->removeMember(client->getFD());
            channel->removeOperator(client);
            std::string oss;
            std::stringstream ss;
            ss << "Client <" << fd << "> left channel " << channelName << "\r\n";
            oss = ss.str();
            send(fd, oss.c_str(), oss.length(), 0);
            std::cout << oss;
        }
        if (channel->getMembers().size() == 0)
        {
            _channels.erase(channelName);
        }
        if (!channel)
            std::cerr << "Channel " << channelName << " not found" << std::endl;
    }
    else
        std::cerr << "Client <" << fd << "> not found" << std::endl;
}

void Server::CAP_cmd(int fd, const Command &cmd)
{
    if (cmd.getParams()[0] == "LS")
    {
        std::string str = "CAP * LS :\r\n";
        send(fd, str.c_str(), str.length(), 0);
        std::cout << "Sent CAP * LS :" << std::endl;
    }
}

void Server::command_dispatcher(int fd, const Command &cmd)
{
    if (cmd.getCommand() == "INFO")
    {
        DebugClientInfo(fd);
    }
    else if (cmd.getCommand() == "PING")
    {
        std::string pong = "PONG :" + cmd.getParams()[0] + "\r\n";
        send(fd, pong.c_str(), pong.length(), 0);
        std::cout << "Sent PONG :" << cmd.getParams()[0] << std::endl;
    }
    else if (cmd.getCommand() == "DEBUGCHANNEL")
    {
        DebugChannelInfo(cmd.getParams()[0]);
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
        JOIN_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "QUIT")
    {
        QUIT_cmd(fd);
    }
    else if (cmd.getCommand() == "PART")
    {
        PART_cmd(fd, cmd);
    }
    else if (cmd.getCommand() == "CAP")
    {
        CAP_cmd(fd, cmd);
    }
    else
        std::cout << "Unknown command '" << cmd.getCommand() << "' from fd=" << fd << std::endl;
}