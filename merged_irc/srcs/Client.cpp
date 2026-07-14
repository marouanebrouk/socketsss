#include "../includes/Client.hpp"

Client::Client() : client_fd(-1), ip_addr(""), _recvBuffer(""), _nickname(""), _realname(""), _username(""), _hostname(""), _server(""), _isAuth(false), _isRegistered(false) {}

Client::Client(int fd, const std::string &ip) : client_fd(fd), ip_addr(ip), _recvBuffer(""), _nickname(""), _realname(""), _username(""), _hostname(""), _server(""), _isAuth(false), _isRegistered(false) {}

        int Client::getFD()
        {
            return client_fd;
        }
        
        void Client::setFD(int fd)
        {
            client_fd = fd;
        };

        std::string & Client::getIP()
        {
            return ip_addr;
        };

        void Client::setIP(const std::string &ip)
        {
            ip_addr = ip;
        }

        std::string &Client::bufferRef() {
            return _recvBuffer;
        }

        void Client::appendBuffer(const std::string &chunk) {
            _recvBuffer += chunk;
        }
        void Client::setNick(const std::string &nick) {
            _nickname = nick;
        }
        void Client::setRealName(const std::string &realname) {
            _realname = realname;
        }
        void Client::setUser(const std::string &username) {
            _username = username;
        }
        void Client::setHost(const std::string &hostname) {
            _hostname = hostname;
        }
        // getters
        std::string &Client::getNick() {
            return _nickname;
        }
        std::string &Client::getRealName() {
            return _realname;
        }
        std::string &Client::getUser() {
            return _username;
        }
        std::string &Client::getHost() {
            return _hostname;
        }
        std::string &Client::getServer() {
            return _server;
        }

        void Client::setServer(const std::string &server) {
            _server = server;
        }


        bool Client::isAuthorized()  {
            return _isAuth;
        }
        bool Client::isRegistered()  {
            return _isRegistered;
        }
        void Client::setAuthorized(bool isAuth) {
            _isAuth = isAuth;
        }
        void Client::setRegistered(bool isReg) {
            _isRegistered = isReg;
        }
