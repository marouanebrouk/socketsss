#include "../includes/Server.hpp"

void Server::CAP_cmd(int fd, const Command &cmd)
{
    if (cmd.getParams()[0] == "LS")
    {
        std::string str = "CAP * LS :\r\n";
        send(fd, str.c_str(), str.length(), 0);
        // std::cout << "Sent CAP * LS :" << std::endl;
    }
    std::cout << "CAP executed for fd=" << fd << std::endl;
}
