#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int sockfd;

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        ssize_t len = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (len <= 0) {
            printf("\nConnection closed.\n");
            exit(0);
        }
        buffer[len] = '\0';
        printf("\n[Friend]: %s\n> ", buffer);
        fflush(stdout);
    }
    return NULL;
}

int main() {
    struct sockaddr_in addr;
    pthread_t recv_thread;
    char buffer[BUFFER_SIZE];
    char mode;

    printf("Run as server (s) or client (c)? ");
    scanf(" %c", &mode);
    getchar(); // Clear newline

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if (mode == 's') {
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("Bind failed");
            exit(1);
        }

        listen(sockfd, 1);
        printf("Waiting for a connection on port %d...\n", PORT);

        socklen_t addrlen = sizeof(addr);
        int new_sock = accept(sockfd, (struct sockaddr *)&addr, &addrlen);
        if (new_sock < 0) {
            perror("Accept failed");
            exit(1);
        }

        close(sockfd); // use new_sock from now on
        sockfd = new_sock;
        printf("Connected to client.\n");

    } else if (mode == 'c') {
        char ip[100];
        printf("Enter server IP: ");
        scanf("%s", ip);
        getchar(); // clear newline

        if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
            perror("Invalid address");
            exit(1);
        }

        if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("Connection failed");
            exit(1);
        }

        printf("Connected to server.\n");
    } else {
        printf("Invalid mode.\n");
        exit(1);
    }

    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    while (1) {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        if (strncmp(buffer, "exit", 4) == 0)
            break;
        send(sockfd, buffer, strlen(buffer), 0);
    }

    close(sockfd);
    return 0;
}
