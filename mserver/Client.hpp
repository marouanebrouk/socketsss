#include <iostream>

class Client
{
    private:
        int fd_socket;
        std::string ip_host;
        int port;
        std::string recv_buffer;
        std::string send_buffer;
        bool is_operator;
        bool passed_authentication;
        bool disconnected;
    public:
        void nickname () 
        void username ()
        void realname ()
        void registered ()
        /*
        IRC messages may arrive split or batched
        You must reconstruct full lines (\r\n)
        */





        // 5. Relationships
// list of joined channels (or pointers)
// maybe pointer/reference to server
}