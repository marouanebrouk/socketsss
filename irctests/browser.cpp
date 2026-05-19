#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <cstdlib>
#include <cstring>

#define MAX_LINE 4086
#define PORT 80


/*
Client socket (linux)5


Create socket
connect socket to outside connection
send and receive data
disconnect

*/



int main(int ac, char *av[]) // ./a.out 172.217.20.238
{
    int socketfd, n, sendbytes;
    struct sockaddr_in serveraddr;
    char sendline[MAX_LINE];
    char receivedline[MAX_LINE];

    if (ac != 2) //   ./prgram <server ip_address>
        throw std::runtime_error("argument number uncorrect");

    if ((socketfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
        throw std::runtime_error("failed to create socket");
    
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    // for big and little endian correction thing we use htons
    serveraddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, av[1], &serveraddr.sin_addr) <= 0)
        throw std::runtime_error("couldn't convert the ip address to binary representaion and store it sin.addr param");
        
    if (connect(socketfd, (struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0)
        throw std::runtime_error("couldn't connect to the socket");
        
        sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
        sendbytes = strlen(sendline);
        
    if (write(socketfd, sendline,sendbytes) != sendbytes)
        throw std::runtime_error("couldn't write in socket");

    while((n = read(socketfd,receivedline,MAX_LINE - 1)) > 0)
    {
        std::cout << receivedline << std::endl;
        memset(receivedline,0,MAX_LINE);
    }
    if (n < 0)
        throw std::runtime_error("error happened during reading, or no reading happened at all hahahha");
    exit(0);
}

// chatgpt socket chat (claude too maybe beneficial)+ video femme algerian + irc medium + geeksforgeeks also a good catch ...;















/*

SOCKET definition

In C++, a socket is an endpoint for communication between two processes over a network. 


Core Concepts
Identification: A socket is defined by a combination of an IP address (to find the machine) and a port number (to find the specific application).
Representation: In your code, a socket is handled as a file descriptor (on Linux) . which is essentially an integer used to refer to the communication channel.

Types of Sockets:
Stream Sockets (TCP): Provide reliable, connection-oriented, and ordered data delivery.
Datagram Sockets (UDP): Provide faster but unreliable, connectionless communication where packets may arrive out of order or not at all. 

Typical Lifecycle
To establish a connection, the C++ socket programming workflow generally follows these stages: 
Creation: Use socket() to create a new socket descriptor.
Naming (Server-side): Use bind() to assign a specific IP and port to the socket.
Listening: Use listen() to wait for incoming connections.
Connecting (Client-side): Use connect() to initiate a connection with a server.
Data Exchange: Use send() and recv() (or read() and write()) to transfer data.
Closing: Use close() (on Linux) or closesocket() (on Windows) to terminate the connection

*/












/*

STREAM SOCKET vs DATAGRAM SOCKET


Stream sockets (

) provide reliable, ordered, connection-oriented data transmission using TCP, ideal for file transfers or web browsing. Datagram sockets (

) are connectionless, faster, but unreliable, using UDP for real-time applications like streaming, where occasional packet loss is acceptable. 
Stack Overflow
Stack Overflow
 +4
Here is a breakdown of the differences based on: 
Stack Overflow
Stack Overflow
 +3
Protocol: Stream sockets use TCP, whereas Datagram sockets use UDP.
Connection: Stream is connection-oriented (requires a handshake), while Datagram is connectionless (sends packets independently).
Reliability: Stream ensures data is delivered, ordered, and error-checked. Datagram does not guarantee delivery, order, or integrity.
Data Boundaries: Stream handles data as a continuous, ordered byte stream without boundaries. Datagram preserves message boundaries.
Speed: Datagram sockets are generally faster and lower overhead due to the lack of error checking and connection management.
Use Cases:
Stream Socket: HTTP, FTP, SSH, Email (applications needing high reliability).
Datagram Socket: VoIP, video streaming, DNS, gaming (applications prioritizing speed over data integrity). 
Summary Table of Differences
| Feature | Stream Socket (

) | Datagram Socket (

) |
| :--- | :--- | :--- |
| Protocol | TCP | UDP |
| Connection | Connection-oriented | Connectionless |
| Reliability | Guaranteed delivery | No guaranteed delivery |
| Ordering | In-order | May arrive out-of-order |
| Speed | Slower | Faster |
| Packet Flow | Continuous stream | Discrete messages |
| Usage | Web browsing (HTTP), File transfer (FTP) | Real-time audio/video, DNS |
*/

