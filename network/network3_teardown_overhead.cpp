
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> 
#include <sys/socket.h>
#include <unistd.h> 
#include <iostream>
#include <chrono>
#include <x86intrin.h>

using namespace std::chrono;

#define MSG_SIZE 1

// void send_msg(int sockfd)
// {
//     auto start = high_resolution_clock::now();
//     char msg[MSG_SIZE] = {'A'};
//     int i = 0;
//     while (i < 10000) {
//         write(sockfd, msg, MSG_SIZE);
//         // bzero(msg, 1);
//         read(sockfd, msg, MSG_SIZE);
//         // printf("receive : %s\n", msg);

//         i++;
//     }
//     auto end = high_resolution_clock::now();
//     double elapsed = duration_cast< duration<double> >(end - start).count();
//     double average = elapsed / 10000.0;
//     std::cout << "One RTT: " << average << "s\n";
// }
 
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
    int connect_ret = connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    if (connect_ret != 0) {
        std::cout << "non zero return from connect\n";
        return -1;
    } else {
        std::cout << "connected\n";
    }

    struct linger optval;

    optval.l_onoff = 1;
    optval.l_linger = 10;

    // Set the SO_LINGER option on the socket.
    setsockopt(socket_fd, SOL_SOCKET, SO_LINGER, &optval, sizeof(optval));

    uint64_t start = __rdtsc();
    close(socket_fd);
    uint64_t end = __rdtsc();

    std::cout << "close() took " << end - start << " cycles ";
    std::cout << (end - start) * (1.0 / 2.6e6) << " ms\n";
}