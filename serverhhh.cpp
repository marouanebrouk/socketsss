#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read

int main() {
  // Create a socket (IPv4, TCP)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Listen to port 9999 on any address
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(9999); // htons is necessary to convert a number to
                                   // network byte order
  if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
    std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Start listening. Hold at most 10 connections in the queue
  if (listen(sockfd, 10) < 0) {
    std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Grab a connection from the queue
  auto addrlen = sizeof(sockaddr);
  int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
  if (connection < 0) {
    std::cout << "Failed to grab connection. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Read from the connection
  char buffer[100];
  auto bytesRead = read(connection, buffer, 100);
  std::cout << "The message was: " << buffer;

  // Send a message to the connection
  std::string response = "Good talking to you\n";
  send(connection, response.c_str(), response.size(), 0);

  // Close the connections
  close(connection);
  close(sockfd);
}

















/*

An IRC server handles multiple clients without blocking by using non-blocking I/O multiplexing (typically with select(), poll(), or epoll() on Linux). This allows a single server process to monitor thousands of client socket connections simultaneously, only handling them when they have data to read or write.Here is how the non-blocking architecture works:1. Core PrinciplesNon-Blocking Sockets: All client sockets are set to non-blocking mode (using fcntl with O_NONBLOCK).I/O Multiplexing (select/poll/epoll): Instead of one thread per user, the server uses a single event loop to wait for activity on any client socket.Event-Driven: The server waits for the OS to tell it which client has data ready (read/write), rather than asking each client if they have data (which would block).2. The Non-Blocking ProcessAccepting Clients: The server socket is in non-blocking mode. When accept() is called, it returns immediately if no new clients are pending, rather than stopping the server.Monitoring (epoll/poll): The server registers all connected client file descriptors (FDs) with epoll or poll. The server pauses (blocks) only on the epoll_wait() call.Handling Events: When one or more clients send data, epoll_wait() returns, providing a list of only those active clients.Reading/Writing: The server processes the input from these specific clients. If a read() or write() operation cannot be completed immediately, it returns EAGAIN or EWOULDBLOCK, allowing the server to move on to other clients.3. Key Components for High Concurrencyepoll (Linux-specific): More efficient than select() or poll(). While select checks all 1024+ connections sequentially (\(O(N)\)), epoll notifies the server instantly of only the active connections (\(O(1)\)).Message Buffering: The server uses buffers to store partial messages from clients, preventing one client’s slow connection from slowing down others.Single-Threaded Loop: Many IRCds (like ircd-hybrid) use a single-threaded loop to avoid the overhead of context switching between threads, relying entirely on asynchronous I/O.4. Handling Blocking OperationsDNS Lookups: To prevent blocking, servers often use asynchronous DNS libraries (like udns or c-ares) to resolve client IPs, rather than the standard blocking gethostbyname

*/