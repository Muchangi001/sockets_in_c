#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PORT 8080
#define BUFFER_SIZE 256

int main() {
    int s_sock, n_sock;
    
    // creating server socket
    s_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (s_sock == -1) {
        perror("Error : Failed to create server socket\n");
        exit(EXIT_FAILURE);
   }

    // creating address for the socket
    struct sockaddr_in s_addr;
    socklen_t addrlen = sizeof(s_addr);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    s_addr.sin_addr.s_addr = INADDR_ANY; // allow the socket to allow incoming connections from any interfaces

    // binding the socket to address
    if (bind(s_sock, (struct sockaddr *)&s_addr, addrlen) < 0) {
        perror("Error : Failed to bind server socket to address\n");
        exit(EXIT_FAILURE);
    }

    // listening for incoming connections
    if (listen(s_sock, 3) == -1) {
        perror("Error : Failed to listen for incoming connections\n");
        exit(EXIT_FAILURE);
    }

    // accepting connection
    n_sock = accept(s_sock, (struct sockaddr *)&s_addr, &addrlen);
    if (n_sock == -1) {
        perror("Error : Unable to accept connections\n");
        exit(EXIT_FAILURE);
    }

    char s_data[BUFFER_SIZE], c_data[BUFFER_SIZE];
    strcpy(s_data, "Hello from server\n");

    // sending data to client
    ssize_t bytes_sent = send(n_sock, s_data, strlen(s_data), 0);
    if (bytes_sent == -1) {
        perror("Error : Failed to send data to client\n");
        exit(EXIT_FAILURE);
    }

    // receiving data from client
    ssize_t bytes_read = recv(n_sock, c_data, BUFFER_SIZE, 0);
    if (bytes_read == -1) {
        perror("Error : Failed to read data from client\n");
        exit(EXIT_FAILURE);
    }
    printf("Client message : %s\n", c_data);

    // closing the connection
    close(s_sock);

    return 0;
}