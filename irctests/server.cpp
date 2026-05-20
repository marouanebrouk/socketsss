// ============================================================
//  server.cpp  —  A minimal TCP echo server
//  Compile:  g++ -Wall -Wextra -o server server.cpp
//  Run:      ./server
// ============================================================

#include <iostream>
#include <cstring>       // memset, strlen
#include <cstdlib>       // exit
#include <unistd.h>      // close(), read(), write()
#include <sys/types.h>   // types used by socket APIs
#include <sys/socket.h>  // socket(), bind(), listen(), accept()
#include <netinet/in.h>  // sockaddr_in, INADDR_ANY, htons()
#include <arpa/inet.h>   // inet_ntoa() — convert IP to string

// ── Constants ────────────────────────────────────────────────
static const int PORT       = 4242;
static const int BACKLOG    = 5;     // max pending connections queue
static const int BUFFER_LEN = 1024;

int main()
{
    // ── STEP 1: socket() ────────────────────────────────────
    //
    //  socket(domain, type, protocol)
    //    AF_INET   = IPv4 address family
    //    SOCK_STREAM = reliable, ordered, connection-based (TCP)
    //    0         = let the OS pick the right protocol (TCP here)
    //
    //  Returns a file descriptor — an integer that the kernel uses
    //  to identify this socket.  Treat it like an open file.
    //
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "socket() failed\n";
        return 1;
    }
    std::cout << "[1] socket() → fd=" << server_fd << "\n";

    // ── OPTIONAL: SO_REUSEADDR ───────────────────────────────
    //
    //  Without this, if your server crashes and you restart it
    //  within ~60 s, bind() will fail with "Address already in use"
    //  because the kernel holds the port in TIME_WAIT.
    //  SO_REUSEADDR tells the kernel: let us reuse it immediately.
    //
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt() failed\n";
        close(server_fd);
        return 1;
    }

    // ── STEP 2: bind() ──────────────────────────────────────
    //
    //  We fill a sockaddr_in struct with:
    //    sin_family = AF_INET (same as in socket())
    //    sin_port   = port number, converted to network byte order
    //                 (big-endian) with htons()
    //    sin_addr   = which local IP to listen on.
    //                 INADDR_ANY means "all interfaces" (0.0.0.0)
    //
    //  bind() attaches this address to our socket fd.
    //
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));          // zero out padding bytes
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(PORT);      // host-to-network short
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind() failed\n";
        close(server_fd);
        return 1;
    }
    std::cout << "[2] bind()   → port " << PORT << "\n";

    // ── STEP 3: listen() ────────────────────────────────────
    //
    //  Puts the socket into passive mode — it will now accept
    //  incoming connections.  The BACKLOG argument sets the size
    //  of the kernel's queue of pending (not yet accept()-ed)
    //  connections.  Connections beyond that limit are dropped.
    //
    if (listen(server_fd, BACKLOG) < 0) {
        std::cerr << "listen() failed\n";
        close(server_fd);
        return 1;
    }
    std::cout << "[3] listen() → waiting on port " << PORT << " ...\n\n";

    // ── STEP 4: accept() — main server loop ─────────────────
    //
    //  accept() BLOCKS here until a client calls connect().
    //  When a client arrives:
    //    • the kernel completes the TCP 3-way handshake
    //    • accept() returns a NEW file descriptor for that
    //      specific client connection.
    //    • server_fd stays open and keeps listening for more.
    //    • client_addr is filled with the remote IP and port.
    //
    while (true)
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(server_fd,
                               (struct sockaddr*)&client_addr,
                               &client_len);
        if (client_fd < 0) {
            std::cerr << "accept() failed\n";
            continue;   // don't die on one bad connection
        }

        // Print who connected
        std::cout << "[4] accept() → client fd=" << client_fd
                  << "  from " << inet_ntoa(client_addr.sin_addr)
                  << ":" << ntohs(client_addr.sin_port) << "\n";

        // ── STEP 5: recv() / send() ─────────────────────────
        //
        //  recv(fd, buffer, len, flags)
        //    Reads up to `len` bytes from the socket into `buffer`.
        //    Returns the number of bytes actually received,
        //    or 0 if the client closed the connection,
        //    or -1 on error.
        //
        //  send(fd, buffer, len, flags)
        //    Writes `len` bytes from `buffer` to the socket.
        //    Returns the number of bytes actually sent.
        //    WARNING: it can send LESS than you asked — always
        //    check the return value and loop in production code.
        //
        char buffer[BUFFER_LEN];
        while (true)
        {
            memset(buffer, 0, BUFFER_LEN);
            ssize_t bytes = recv(client_fd, buffer, BUFFER_LEN - 1, 0);

            if (bytes <= 0) {
                // 0 = client disconnected cleanly, -1 = error
                std::cout << "[5] client disconnected (fd=" << client_fd << ")\n\n";
                break;
            }

            std::cout << "[5] recv  ← \"" << buffer << "\" (" << bytes << " bytes)\n";

            // Echo it back (with a prefix so client can see it's our reply)
            std::string reply = "ECHO: ";
            reply += buffer;
            send(client_fd, reply.c_str(), reply.size(), 0);
            std::cout << "[5] send  → \"" << reply << "\"\n";
        }

        // ── STEP 6: close() ─────────────────────────────────
        //
        //  close() releases the file descriptor and triggers a
        //  TCP FIN to the other side, telling them we're done.
        //
        close(client_fd);
        std::cout << "[6] close() → client fd=" << client_fd << " closed\n";
        std::cout << "Waiting for next connection...\n\n";
    }

    close(server_fd);
    return 0;
}
