
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <iostream>
#include <chrono>

using namespace std::chrono;


void send_msg(int sockfd)
{
    const uint64_t MSG_SIZE = 64 << 10;

    char* msg = new char[MSG_SIZE];  // 1GB message
    memset(msg, 42, MSG_SIZE);
    msg[0] = 't';
    msg[1] = 'e';
    msg[2] = 's';
    msg[3] = 't';
    char recv[4];
    std::cout << "created message\n";

    uint64_t sent_bytes = 0;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < 1024; i++) {
        // sent_bytes += write(sockfd, msg, MSG_SIZE);
        sent_bytes += send(sockfd, msg, MSG_SIZE, 0);

        // send()
        // printf("%d ", i);
        
        // read(sockfd, recv, MSG_SIZE);
        // printf("receive : %s\n", recv);
    }
    auto end = high_resolution_clock::now();

    double elapsed = duration_cast< duration<double> >(end - start).count();
    std::cout << "Transfer time: " << elapsed << "s, sent " << (sent_bytes / 1024) << "KB \n";
    printf("Bandwdith %.3lf MB/s\n", sent_bytes / 1024 / 1024 / elapsed);
}

int main()
{
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
 
    // assign IP, PORT
    serv_addr.sin_family = AF_INET;
    // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // loopback 
    serv_addr.sin_addr.s_addr = inet_addr("132.239.17.136");  // another computer in CSE
    serv_addr.sin_port = htons(8080);
 
    // connect the client socket to server socket
    if (connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "failed to connect\n";
        return -1;
    } else {
        std::cout << "connected\n";
    }
 
    send_msg(socket_fd);

    close(socket_fd);
}