#ifndef SERVER_HPP
#define SERVER_HPP
#include <cctype>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <poll.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <sstream>

#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include "Client.hpp"
#include "Channel.hpp"
#include "IrcCommand.hpp"

class Channel;
class Server
{
    private:
        int _port;
        std::string _password;
        int _serverFd;
	    std::map<int, Client*> _clients;
	    std::map<std::string, Channel*> _channels;
        std::vector<struct pollfd> _pollfds;

    public:
        Server(int port, std::string password);
        void setupSocket();
        void getallServerData();
        void handleNewClient();
        void receiveClientData(int fd);
        void ignitServer();
        void clear_client(int fd);
        void processLine(int fd, const std::string &line);
        //debugging methods
        void DebugClientInfo(int fd);
        void displayMessage(int fd, const Command &cmd);
        void DebugChannelInfo(const std::string &channelName);
        
    void sendNamesList(Client *client, Channel *channel);



    //commands
    void command_dispatcher(int fd, const Command &cmd);

    bool canRegister(Client *client);
    bool registerClient(Client *client);
    bool isAllowedBeforeRegister(const std::string &cmd);

    void sendReply(int fd, const std::string &msg);
    bool nickExists(const std::string &nick);
    bool isValidChannelName(const std::string &name);
    bool isValidNick(const std::string &nick);
    void sendToChannel(Channel *channel,const std::string &msg,int excludeFd);
    void sendToChannel(Channel *channel,const std::string &msg);
    Client *getClientByNickname(const std::string &nick);


    void PASS_cmd(int fd, const Command &cmd);
    void NICK_cmd(int fd, const Command &cmd);
    void USER_cmd(int fd, const Command &cmd);
    void JOIN_cmd(int fd, const Command &cmd);
    void PART_cmd(int fd, const Command &cmd);
    void CAP_cmd(int fd, const Command &cmd);
    void TOPIC_cmd(int fd, const Command &cmd);
    void INVITE_cmd(int fd, const Command &cmd);
    void KICK_cmd(int fd, const Command &cmd);


    void PRIVMSG_cmd(int fd, const Command &cmd);
    void MODE_cmd(int fd, const Command &cmd);



    ~Server();
};


#endif