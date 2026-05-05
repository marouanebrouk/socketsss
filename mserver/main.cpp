#include "Server.hpp"

int main(int ac, char *av[])
{
    if (ac != 3)
    {
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return 1;
    }
    int port = std::atoi(av[1]);
    if (port <= 0)
		std::cerr << "Invalid port" << std::endl;
    try
    {
        Server server(port, av[1]);
        server.setupSocket();
        // server.run();

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    


}