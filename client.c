#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PORT 8080
#define BUFFER_SIZE 256

int main() {
    int c_sock;

    // creating client socket
    c_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(c_sock == -1) {
        perror("Error : Failed to create client socket\n");
        exit(EXIT_FAILURE);
        
    }

    // creating server address
    struct sockaddr_in s_addr;
    socklen_t addrlen = sizeof(s_addr);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT); // converts 16 bit interger to network bytes

    // converting text ip address to binary network format
    if (inet_pton(AF_INET, "127.0.0.1", &s_addr.sin_addr) == 0 || inet_pton(AF_INET, "127.0.0.1", &s_addr.sin_addr) == -1) {
        perror("Error : Failed to convert ip address to network format\n");
        exit(EXIT_FAILURE);
    }

    // connecting to server
    if (connect(c_sock, (struct sockaddr *)&s_addr, addrlen) == -1) {
        perror("Error : Failed to connect client to server\n");
        exit(EXIT_FAILURE);
    }

    char s_data[BUFFER_SIZE], c_data[BUFFER_SIZE];
    strcpy(c_data, "Hello from client\n");

    // sending data to server
    ssize_t bytes_sent = send(c_sock, c_data, strlen(c_data), 0);
    if (bytes_sent == -1) {
        perror("Error : Failed to send data to server\n");
        exit(EXIT_FAILURE);
    }

    // receiving data from server
    ssize_t bytes_read = recv(c_sock, s_data, BUFFER_SIZE, 0);
    if (bytes_read == -1) {
        perror("Error : Failed to read data from server\n");
        exit(EXIT_FAILURE);
    }
    printf("Server message : %s\n", s_data);

    // closing the connection
    close(c_sock);

    return 0;
}
