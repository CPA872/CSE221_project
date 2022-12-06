
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
#include <x86intrin.h>

using namespace std::chrono;

#define MSG_SIZE 1

void send_msg(int sockfd)
{
    // auto start = high_resolution_clock::now();
    char msg[MSG_SIZE] = {'A'};
    int i = 0;

    uint64_t start = __rdtsc();
    while (i < 10000) {
        write(sockfd, msg, MSG_SIZE);
        // bzero(msg, 1);
        read(sockfd, msg, MSG_SIZE);
        // printf("receive : %s\n", msg);

        i++;
    }
    uint64_t end = __rdtsc();
    uint64_t elapsed = end - start;
    double average = elapsed * (1.0 / 2.6e9) / 10;

    // auto end = high_resolution_clock::now();
    // double elapsed = duration_cast< duration<double> >(end - start).count();
    // double average = elapsed / 10000.0;
    std::cout << "One RTT: " << average << " ms, " << elapsed << "cycles\n";
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
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // loopback 
    // serv_addr.sin_addr.s_addr = inet_addr("132.239.17.136");  // another computer in CSE
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