#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <poll.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

class Client
{
    private:
        int client_fd;
        std::string ip_addr;
        // recv() is stream-based: accumulate until we can extract full "\r\n" lines.
		std::string	_recvBuffer;
        std::string	_nickname;
        std::string	_realname;
        std::string	_username;
        std::string _hostname;
        bool        _isAuth;
        bool        _isRegistered;
        bool        _isOperator;
    public:
        Client(){};
        Client(int fd, const std::string &ip);
        int getFD();
        std::string & getIP();
        std::string &getNick();
        std::string &getRealName();
        std::string &getUser();
        std::string &getHost();
        bool isAuthorized() ;
        bool isRegistered() ;
        bool isOperator() ;

        void setFD(int fd);
        void setIP(const std::string &ip);
        void setNick(const std::string &nick);
        void setRealName(const std::string &realname);
        void setUser(const std::string &username);
        void setHost(const std::string &hostname);
        void setAuthorized(bool isAuth);
        void setRegistered(bool isReg);
        void setOperator(bool isOp);

        // Buffer access/manipulation
        std::string &bufferRef();
        void appendBuffer(const std::string &chunk);
        /*
        IRC messages may arrive split or batched
        You must reconstruct full lines (\r\n)
        */





        // 5. Relationships
// list of joined channels (or pointers)
// maybe pointer/reference to server
};

#endif