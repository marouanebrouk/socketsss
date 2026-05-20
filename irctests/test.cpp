    #include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <iostream>



int main(void)
{
    int sockfd;
    int client_fd;
    struct sockaddr_in addr;
    char *msg;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        return (1);

int yes = 1;  
    // setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(tv)); //-> set the socket option (SO_REUSEADDR) to reuse the address

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        return (1);

    if (listen(sockfd, 5) < 0)
        return (1);
    
    std::cout << "before accept :" << std::endl;
    client_fd = accept(sockfd, NULL, NULL);
    if (client_fd < 0)
        return (1);
    std::cout << "after accept :" << std::endl;

    msg = "Hello from server\n";
    send(client_fd, msg, strlen(msg), 0);

    close(client_fd);
    close(sockfd);
    return (0);
}