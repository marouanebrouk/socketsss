# ── Makefile for socket learning project ──────────────────────
#  make          → builds both server and client
#  make server   → builds only server
#  make client   → builds only client
#  make clean    → removes compiled binaries

CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17

all: server client

server: server.cpp
	$(CXX) $(CXXFLAGS) -o server server.cpp

client: client.cpp
	$(CXX) $(CXXFLAGS) -o client client.cpp

clean:
	rm -f server client

.PHONY: all clean
