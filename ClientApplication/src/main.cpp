#include "UtilsLib/Client.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Test : public Client{
    public:
    void sendToServer(){
        getSocket();
        const char *req = "Hello server";
        write(getSocket(), req, strlen(req));
    }
};

int main() {
    Test client;
    client.openSocket();
    client.connectToServer("127.0.0.1");
    client.sendToServer();
    return 0;
}