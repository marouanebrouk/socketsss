#include "../includes/Server.hpp"
#include <cerrno>

Server::Server(int port , std::string password) :
        _port(port) , _password(password) , _cmds(NULL) {}

bool Server::F_signal = false;

void Server::SignalHandler(int SignalNum)
{
        (void)SignalNum;
        F_signal = true;
}

void Server::acceptconnetion()
{
        memset(&addr_client ,0 ,sizeof(addr_client));
        socklen_t len = sizeof(addr_client);
        int Fd_user = accept(_serverSocket,reinterpret_cast<sockaddr*>(&addr_client), &len);
        if(Fd_user == -1)
            {std::cout << "accept fail()" << std::endl; return ;}
        if(fcntl(Fd_user, F_SETFL , O_NONBLOCK) == -1)
              {std::cout << "Client Socket Non BLocked" << std ::endl; return; }
        new_user.fd = Fd_user;
        new_user.events = POLLIN;
        new_user.revents = 0;
        Client* user = new Client(Fd_user, inet_ntoa(addr_client.sin_addr));
        _clients[Fd_user] = user;
        fds.push_back(new_user);
        std::cout << "Client [" << Fd_user << "]" << " connected" << std::endl; 
}

void Server::RemoveClient(int fd)
{
        for(unsigned int i = 0 ; i < fds.size() ; i++)
                if(fds[i].fd == fd) {fds.erase(fds.begin() + i); break;}
        if (_clients.find(fd) != _clients.end()) {
                delete _clients[fd];
                _clients.erase(fd);
        }
        close(fd);
}

Command* Server::parsecmd(std::string cmd)
{
   Command *_cmd = new Command();
   int flag = 0;
   if(cmd.find(" ") == std::string::npos && cmd.find(":") == std::string::npos) 
        {_cmd->_setcmd(cmd); return _cmd;}
   size_t pos = cmd.find(":");
   if(pos != std::string::npos) {
       std::string subtrailing = cmd.substr(pos + 1);
       if(!subtrailing.empty()) {_cmd->_settrailing(subtrailing); cmd.erase(pos);}
  }
  if(!cmd.empty())
      {pos = cmd.find(" ");  _cmd->_setcmd(cmd.substr(0,pos)); cmd.erase(0,pos + 1);}
  while(true)
  {
       pos = cmd.find(" ");
       if(pos != std::string::npos) flag = 1;
       if(pos == std::string::npos) flag = 0;
       if(!flag) { if(!cmd.empty()) _cmd->_setparams(cmd); return _cmd;}
       std::string subcmd = cmd.substr(0,pos);
       if(!subcmd.empty()) _cmd->_setparams(subcmd);
       cmd.erase(0,pos + 1);
  }
  return NULL;
}

void Server::displayMessage(int fd, const Command &cmd)
{
    // Parser is separated from execution; keep this as the dispatch point.
    // For now, just log what we parsed.



    //[IRC] fd=4 cmd='DEBUG' <<<<<< you want this

    //do this \/

    std::cout << "[IRC] fd=" << fd;
    if (!cmd.getPrefix().empty())
        std::cout << " prefix='" << cmd.getPrefix() << "'";
    std::cout << " cmd='" << cmd.getCommand() << "'";
    const std::vector<std::string> &params = cmd.getParams();
    for (size_t i = 0; i < params.size(); ++i)
        std::cout << " param[" << i << "]='" << params[i] << "'";
    std::cout << " trailing is :"<< cmd._gettrailing() << std::endl;
    std::cout << std::endl;

    // Here you can add code to handle the command, e.g., by calling a method
    // on the Client object associated with the fd.
}
void Server::handleClient(int fd)
{
        char buff[1024];
        memset(buff, 0, sizeof(buff));
        int bytes = recv(fd, buff , sizeof(buff) - 1, 0);
        if(bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) return ;
        if(bytes <= 0) {std::cout << "Client [" << fd << "]" << " Disconnected" << std::endl; RemoveClient(fd);}
        else
        {
                Client* user = _clients[fd];
                user->appendBuffer(buff);
                if(user->bufferRef().find("\r\n") == std::string::npos) return ;
                else
                {
                        size_t pos = user->bufferRef().find("\r\n");
                        std::string cmd = user->bufferRef().substr(0,pos);
                        user->bufferRef().erase(0,pos + 2);
                        _cmds = parsecmd(cmd);
                        if (_cmds) {
                                try{
                                        displayMessage(fd, *_cmds);
                                        command_dispatcher(fd, *_cmds);
                                }
                                catch(std::runtime_error &e)
                                {
                                        std::cout << e.what() << std::endl;
                                }
                                delete _cmds;
                        }
                }
        }

}
void Server::_setServerSocket()
{
        int flag = 1;
        _serverSocket = socket(AF_INET, SOCK_STREAM , 0);
        if(_serverSocket == -1)
                throw (std::runtime_error("Error : failed to create Socket"));
        if(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR , &flag, sizeof(flag)) == -1)
                throw(std::runtime_error("Error : setsockopt cannot change the settings Socket "));
        if(fcntl(_serverSocket, F_SETFL , O_NONBLOCK) == -1)
                throw(std::runtime_error("Error : ServerSocket Non BLocked"));
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(_port);
        addr.sin_addr.s_addr = INADDR_ANY;
        if(bind(_serverSocket, reinterpret_cast<sockaddr*>(&addr),sizeof(addr)) == -1)
                throw (std::runtime_error("Error : cannot bind the socket"));
        if(listen(_serverSocket, SOMAXCONN) == -1)
                throw (std::runtime_error("Error : Socket cannot listen"));
        new_user.fd = _serverSocket;
        new_user.events = POLLIN;
        new_user.revents = 0;
        fds.push_back(new_user);
}
void Server::StartServer()
{
        signal(SIGINT, SignalHandler);
        signal(SIGQUIT, SignalHandler);
        this->_setServerSocket();
        while(!F_signal)
        {
            if((poll(fds.data(),fds.size(), -1)) == -1 && (!F_signal))
                throw(std::runtime_error("poll() failed"));
            for(size_t i = 0 ; i < fds.size() ; i++)
            {
                if(fds[i].revents & POLLIN)
                {
                    if(fds[i].fd == _serverSocket)
                        acceptconnetion();
                    else
                        handleClient(fds[i].fd);
                }
            }
        }
}

bool is_valid(std::string s)
{
    for(unsigned int i = 0;  i < s.size() ; ++i)
    {
        if(std::isalpha(s[i]))
            return true;
    }
    return false;
}
