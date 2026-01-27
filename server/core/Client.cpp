#include "Client.h"
#include <cstddef>
#include <unistd.h>
#include <sys/socket.h>
using namespace std;

Client::Client(int fd) : socketFd(fd) {}

Client::~Client() {
    closeConnection();
}

int Client::getFd() const {
    return socketFd;
}

/*void Client::appendToBuffer(const char* data, size_t len) {
    recvBuffer.append(data, len);
}

string& Client::getBuffer() {
    return recvBuffer;
}

void Client::clearBuffer() {
    recvBuffer.clear();
}*/

bool Client::sendRaw(const std::string& data) {
    ssize_t totalSent = 0;
    ssize_t len = data.size();

    while (totalSent < len) {
        ssize_t sent = send(
            socketFd,
            data.c_str() + totalSent,
            len - totalSent,
            MSG_NOSIGNAL
        );
        if (sent <= 0) return false;
        totalSent += sent;
    }
    return true;
}

bool Client::sendText(const std::string& text) {
    return sendRaw(text + "\n");
}

bool Client::receiveData() {
    char buffer[1024];

    ssize_t bytes = recv(socketFd, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        return false; // client disconnected or error
    }

    recvBuffer.append(buffer, bytes);
    return true;
}

std::vector<std::string> Client::popMessages() {
    std::vector<std::string> messages;
    size_t pos;

    while ((pos = recvBuffer.find('\n')) != std::string::npos) {
        std::string msg = recvBuffer.substr(0, pos);
        messages.push_back(msg);
        recvBuffer.erase(0, pos + 1);
    }

    return messages;
}


void Client::closeConnection() {
    if (socketFd >= 0) {
        close(socketFd);
        socketFd = -1;
    }
}

