#ifndef SERVER_HPP
#define SERVER_HPP


#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <vector>
#include <poll.h>
#include <signal.h>
#include <map>
#include <set>
#include <sstream>
#include "Client.hpp"
#include "Channel.hpp"
#include <arpa/inet.h>
#include "Command.hpp"

class Channel;

class Server
{
    private:

        int   _serverSocket;
        int _port;
        std::string  _password;
        struct sockaddr_in  addr;
        struct sockaddr_in  addr_client;
        struct pollfd new_user;
        std::map<int, Client*> _clients;
        std::map<std::string, Channel*> _channels;
        std::vector<pollfd> fds;
        Command *_cmds;
        static bool F_signal;
        
        typedef void (Server::*CommandHandler)(int, const Command&);
        std::map<std::string, CommandHandler> _commandTable;
        void initCommandTable();
        
    private:
        Server();
        
        void         _setServerSocket();
        int           _getServerSocket() const;
        std::string   _getPassword() const;
        int           _getPort() const;
        static void   SignalHandler(int Signalnum);
        void          acceptconnetion();
        void          handleClient(int fd);
        void          RemoveClient(int fd);
        Command        *parsecmd(std::string cmd);

    public:
        Server(int port , std::string pass);
        ~Server();
        void StartServer();

        void displayMessage(int fd, const Command &cmd);
        // Command handling from marouane
        void command_dispatcher(int fd, const Command &cmd);
        void sendNamesList(Client *client, Channel *channel);
        void DebugChannelInfo(const std::string &channelName);
        void DebugClientInfo(int fd);

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
        void PING_cmd(int fd, const Command &cmd);
        void INFO_cmd(int fd, const Command &cmd);
        void DEBUGCHANNEL_cmd(int fd, const Command &cmd);

};

bool is_valid(std::string s);
#endif  