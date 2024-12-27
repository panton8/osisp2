//
// Created by panton8 on 2/25/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8888
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

typedef struct {
    int socket;
    struct sockaddr_in address;
    int addr_len;
} Client;

Client clients[MAX_CLIENTS];
int active_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    char buffer[BUFFER_SIZE] = {0};
    char join_message[100];
    snprintf(join_message, sizeof(join_message), "User %d joined the chat.\n", active_clients);
    for (int i = 0; i < active_clients; i++) {
        send(clients[i].socket, join_message, strlen(join_message), 0);
    }

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        if (recv(client_socket, buffer, BUFFER_SIZE, 0) <= 0) {
            printf("Client disconnected\n");
            close(client_socket);

            for (int i = 0; i < active_clients; i++) {
                if (clients[i].socket == client_socket) {
                    for (int j = i; j < active_clients - 1; j++) {
                        clients[j] = clients[j + 1];
                    }
                    active_clients--;
                    break;
                }
            }


            char leave_message[100];
            snprintf(leave_message, sizeof(leave_message), "User %d left the chat.\n", active_clients + 1);
            for (int i = 0; i < active_clients; i++) {
                send(clients[i].socket, leave_message, strlen(leave_message), 0);
            }

            break;
        }

        for (int i = 0; i < active_clients; i++) {
            if (clients[i].socket != client_socket) {
                send(clients[i].socket, buffer, strlen(buffer), 0);
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int opt = 1;
    int addrlen = sizeof(server_addr);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);


    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {

        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        clients[active_clients].socket = client_socket;
        clients[active_clients].address = client_addr;
        clients[active_clients].addr_len = addrlen;
        active_clients++;

        printf("New client connected. Total clients: %d\n", active_clients);

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, (void *)&client_socket) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

