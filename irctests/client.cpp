// ============================================================
//  client.cpp  —  A minimal TCP client that talks to server.cpp
//  Compile:  g++ -Wall -Wextra -o client client.cpp
//  Run:      ./client
//            (start the server first in another terminal)
// ============================================================

#include <iostream>
#include <cstring>       // memset, strlen
#include <unistd.h>      // close()
#include <sys/types.h>
#include <sys/socket.h>  // socket(), connect()
#include <netinet/in.h>  // sockaddr_in, htons()
#include <arpa/inet.h>   // inet_pton() — convert IP string to binary

// ── Constants ────────────────────────────────────────────────
static const char* SERVER_IP  = "127.0.0.1";  // localhost
static const int   SERVER_PORT = 4242;
static const int   BUFFER_LEN  = 1024;

int main()
{
    // ── STEP 1: socket() ────────────────────────────────────
    //
    //  Identical to the server: we create a TCP socket.
    //  At this point it's just a fd — not connected to anything.
    //
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        std::cerr << "socket() failed\n";
        return 1;
    }
    std::cout << "[1] socket() → fd=" << sock_fd << "\n";

    // ── STEP 2: connect() ───────────────────────────────────
    //
    //  We fill a sockaddr_in with the SERVER's address (not ours).
    //  connect() initiates the TCP 3-way handshake:
    //
    //    Client ──── SYN ────────────► Server
    //    Client ◄─── SYN-ACK ──────── Server
    //    Client ──── ACK ────────────► Server
    //
    //  After this, the connection is established and we can
    //  start sending data.  No bind() needed on the client —
    //  the OS assigns a random ephemeral port automatically.
    //
    //  inet_pton() converts "127.0.0.1" string into the 4-byte
    //  binary representation stored in sin_addr.
    //
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        std::cerr << "inet_pton() failed — invalid address\n";
        close(sock_fd);
        return 1;
    }

    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "connect() failed — is the server running?\n";
        close(sock_fd);
        return 1;
    }
    std::cout << "[2] connect() → connected to "
              << SERVER_IP << ":" << SERVER_PORT << "\n\n";

    // ── STEP 3: interactive send / recv loop ─────────────────
    //
    //  We read input from the user, send it to the server,
    //  and wait for the server's reply.
    //
    std::cout << "Type a message and press Enter (Ctrl+D to quit):\n";
    std::cout << "─────────────────────────────────────────────────\n";

    char buffer[BUFFER_LEN];
    std::string line;

    while (std::getline(std::cin, line))
    {
        if (line.empty())
            continue;

        // ── send() ──────────────────────────────────────────
        //  Sends the string to the server over TCP.
        //  Note: TCP is a stream protocol — it has no concept of
        //  "message boundaries".  If you send "hello", the server
        //  might recv it as "hel" + "lo" in two calls.  For this
        //  simple demo the messages are short enough that this
        //  doesn't happen, but in production you need a framing
        //  protocol (length prefix, newline delimiter, etc.).
        //
        ssize_t sent = send(sock_fd, line.c_str(), line.size(), 0);
        if (sent < 0) {
            std::cerr << "send() failed\n";
            break;
        }
        std::cout << "  → sent \"" << line << "\" (" << sent << " bytes)\n";

        // ── recv() ──────────────────────────────────────────
        //  Block until the server sends a reply.
        //
        memset(buffer, 0, BUFFER_LEN);
        ssize_t received = recv(sock_fd, buffer, BUFFER_LEN - 1, 0);
        if (received <= 0) {
            std::cout << "  server closed the connection.\n";
            break;
        }
        std::cout << "  ← recv \"" << buffer << "\" (" << received << " bytes)\n\n";
    }

    // ── STEP 4: close() ─────────────────────────────────────
    //
    //  Sends a TCP FIN to the server, then releases the fd.
    //
    close(sock_fd);
    std::cout << "\n[4] close() → connection closed. Bye!\n";
    return 0;
}
