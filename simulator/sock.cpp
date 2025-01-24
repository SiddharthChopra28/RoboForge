#include "sock.h"
#include "fstream"
#if __linux__

Socket::Socket(string ip, int port){

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        throw runtime_error("Failed to create socket"); //error
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr);

    while (1) {
        if (connect(sock, (sockaddr*)&serverAddress, sizeof(serverAddress)) != -1){
            break;
        }

    }

    fcntl(sock, F_SETFL, O_NONBLOCK);

}


string Socket::getMessage() {
    char buffer[1024] = {0};
    FD_ZERO(&read_fds);
    FD_SET(sock, &read_fds);

    struct timeval timeout = {0, 100000};  // 0.1 seconds
    int result = select(sock + 1, &read_fds, nullptr, nullptr, &timeout);


    if (result > 0 && FD_ISSET(sock, &read_fds)) {
        int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            return string(buffer);
        }

    }
    return "NODATA";
}


void Socket::sendMessage(string message){
    send(sock, message.c_str(), message.size(), 0);

}


#elif _WIN32

Socket::Socket(string ip, int port){
    // returns 0 if success, 1 if failure
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw runtime_error("WSAStartup failed");
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        throw runtime_error("Socket creation failed");
        WSACleanup();
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        throw runtime_error("Connection failed");
        closesocket(sock);
        WSACleanup();
    }
    u_long mode = 1; // 1 enables non-blocking mode, 0 disables it
    ioctlsocket(sock, FIONBIO, &mode);
}

Socket::~Socket(){
    closesocket(sock);
    WSACleanup();
}

void Socket::sendMessage(string message){
    send(sock, message.c_str(), message.size(), 0);
}
string Socket::getMessage(){
    char buffer[1024];
    FD_ZERO(&read_fds);
    FD_SET(sock, &read_fds);

    timeval timeout = {0, 100000};  // 0.1 seconds
    int result = select(0, &read_fds, nullptr, nullptr, &timeout);
    if (result > 0 && FD_ISSET(sock, &read_fds)) {
        int bsytesReceived = recv(sock, buffer, sizeof(buffer)-1, 0);
        if (bytesReceived>0){
            return string(buffer);
        }
    }
    return "NODATA";


}


#endif


