#ifndef ROBOFORGE_SOCK_H
#define ROBOFORGE_SOCK_H
#include <iostream>
using namespace std;

#if __linux__

#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>

class Socket{
    int sock;
    fd_set read_fds;
public:
    Socket(string ip, int port);
    void sendMessage(string message);
    string getMessage();
    ~Socket();
};

#elif _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")

class Socket{
    SOCKET sock;
    fd_set read_fds;
public:
    Socket(string ip, int port);
    void sendMessage(string message);
    string getMessage();
    ~Socket();
};

#endif



#endif //ROBOFORGE_SOCK_H
