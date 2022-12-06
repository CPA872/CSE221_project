#include <iostream>
#include <stdio.h>
#include <chrono>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> 
#include <immintrin.h>

#define MSG_SIZE 64 << 10  // 1GB message
   
// Driver function

void return_msg(int connection_fd) {
    char* msg = new char[MSG_SIZE];
    char ack[4] = "ack";

    while (true) {
        // bzero(msg, MSG_SIZE);
        recv(connection_fd, msg, MSG_SIZE, MSG_WAITALL);
        // printf("From client: %s\n", msg);
        // bzero(msg, 1);

        // write(connection_fd, ack, sizeof(msg));
        // std::cout << "sent\n";
    }
}

int main()
{
    // steps: 
    // create a socket
    // bind the socket to ip and port
    // have socket to listen
    // wait for connection
    // close listening socket

    int socket_fd, connection_fd;
    struct sockaddr_in serv_addr, cli_addr;

    socklen_t len = sizeof(cli_addr);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
        std::cout << "failed to create socket\n";
        return -1;
    } else {
        std::cout << "successfully created a socket\n";
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(8080);  // use port 8080 
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "ERROR on binding\n";
        return -2;
    } else {
        std::cout << "binding complete\n";
    }

    if (listen(socket_fd, 5) < 0) {
        std::cout << "listen failed\n";
        return -4;
    } else {
        std::cout << "listening\n";
    }

    connection_fd = accept(socket_fd, (struct sockaddr *) &cli_addr, &len);
    if (connection_fd < 0) {
        std::cout << "failed to accept\n";
        return -3;
    } else {
        std::cout << "server accept\n";
    }

    return_msg(connection_fd);
    close(socket_fd);

    return 0;
}