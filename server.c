#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 8888

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[MAX_BUFFER_SIZE];
    const char *serverIP = argv[1];
    const char *service = argv[2];
    struct addrinfo hints, *res;
    int sockfd;

       memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // IPv4 oder IPv6
    hints.ai_socktype = SOCK_STREAM;

    // IP-Adresse und Portnummer auflösen
    if (getaddrinfo(serverIP, service, &hints, &res) != 0) {
        fprintf(stderr, "Konnte Hostnamen oder Dienst nicht auflösen.\n");
        return 1;
    }
       // IP-Adresse in Hostnamen umwandeln
    if (getaddrinfo(serverIP, NULL, &hints, &res) != 0) {
        fprintf(stderr, "Konnte Hostnamen nicht auflösen.\n");
        return 1;
    }

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Set up server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to server address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Error binding");
        exit(1);
    }

    // Listen for connections
    if (listen(server_socket, 5) < 0) {
        perror("Error listening");
        exit(1);
    }

    printf("Server listening on port %d\n", SERVER_PORT);

    while (1) {
        socklen_t client_address_length = sizeof(client_address);

        // Accept connection from client
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket < 0) {
            perror("Error accepting connection");
            exit(1);
        }

        printf("Client connected\n");

        // Receive and echo message
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received < 0) {
            perror("Error receiving message");
            exit(1);
        }

        printf("Received message: %s", buffer);

        // Send message back to client
        if (send(client_socket, buffer, bytes_received, 0) < 0) {
            perror("Error sending message");
            exit(1);
        }

        // Close client socket
        close(client_socket);
    }

    // Close server socket
    close(server_socket);

    return 0;
}