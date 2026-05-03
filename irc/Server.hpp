#include <iostream>

class Server
{
    private:
        int server_fd;
        std::vector<int> pollfds;
        // map (fd → Client* mapping)
        // map (CLients)
        // map (channels: (channel name-> channel object))
        int port;
        int password;
        std::string server_name;
        bool disconnected;
        // 5. Internal helpers
        // buffer sizes, limits, etc.
    public:

}