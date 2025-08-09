#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h> // for _beginthread

#pragma comment(lib, "ws2_32.lib")

#define PORT 5000
#define BUFFER_SIZE 1024

SOCKET sock;

void receive_thread(void* arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        int len = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (len <= 0) {
            printf("Connection closed.\n");
            exit(0);
        }
        buffer[len] = '\0';
        printf("\n[Friend]: %s\n> ", buffer);
        fflush(stdout);
    }
}

int establish_connection() {
    WSADATA wsa;
    struct sockaddr_in server, client;
    char buffer[BUFFER_SIZE];
    char mode;
    int client_len = sizeof(client);

    printf("Starting Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    printf("Run as server (s) or client (c)? ");
    scanf(" %c", &mode);
    getchar(); // Clear newline

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (mode == 's') {
        server.sin_addr.s_addr = INADDR_ANY;

        if (bind(sock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
            printf("Bind failed with error code: %d\n", WSAGetLastError());
            return 1;
        }

        listen(sock, 1);
        printf("Waiting for incoming connection on port %d...\n", PORT);

        SOCKET new_sock = accept(sock, (struct sockaddr*)&client, &client_len);
        if (new_sock == INVALID_SOCKET) {
            printf("Accept failed: %d\n", WSAGetLastError());
            return 1;
        }

        closesocket(sock); // use the new socket from now on
        sock = new_sock;
        printf("Connected to client.\n");

    } else if (mode == 'c') {
        char ip[100];
        printf("Enter server IP: ");
        scanf("%s", ip);
        getchar();

        server.sin_addr.s_addr = inet_addr(ip);

        if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
            printf("Connection failed.\n");
            return 1;
        }

        printf("Connected to server.\n");
    } else {
        printf("Invalid mode.\n");
        return 1;
    }

    _beginthread(receive_thread, 0, NULL);

    while (1) {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        if (strncmp(buffer, "exit", 4) == 0)
            break;
        send(sock, buffer, strlen(buffer), 0);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
