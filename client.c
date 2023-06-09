#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[MAX_BUFFER_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Set up server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr)) <= 0) {
        perror("Invalid address");
        exit(1);
    }

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    printf("Connected to server\n");

    // Send message to server
    printf("Enter message: ");
    fgets(buffer, sizeof(buffer), stdin);
    if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
        perror("Error sending message");
        exit(1);
    }

    // Receive echoed message from server
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        perror("Error receiving message");
        exit(1);
    }

    printf("Received echoed message: %s", buffer);

    // Close socket
    close(client_socket);

    return 0;
}