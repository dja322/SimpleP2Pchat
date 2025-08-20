
#include "Networking.h"
#include "../UtilityFiles/UtilityFunctions.h"
#include "../UtilityFiles/StringUtils.h"
#include "../UserActivity/UserSettings.h"

#define PORT 5000
#define BUFFER_SIZE 1024

int sockfd;

void *receive_thread(void* arg) {
    bool usernameKnown = false;
    char username[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    ssize_t len = recv(sockfd, username, sizeof(username) - 1, 0);
    if (len > 0) {
        username[len] = '\0';
        usernameKnown = true;
    }

    while (1) {
        len = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (len <= 0) {
            printf("\nConnection closed.\n");
            exit(0);
        }
        buffer[len] = '\0';
        printf("\n[%s]: %s\n> ", usernameKnown ? username : "Unknown", buffer);
        fflush(stdout);
    }
    return NULL;
}

int establish_connection(settings_t *settings) {
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

    printf("Connection established. You can start chatting.\n");
    pthread_create(&recv_thread, NULL, receive_thread, NULL);
    sleep(1); // Give the thread time to start
    send(sockfd, settings->username, strlen(settings->username), 0);

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

void runConnection()
{
    int choice = 0;

    fflush(stdout);

    settings_t userSettings;
    if (loadSettings(&userSettings, "settings.dat") == 0) {
        printf("Failed to load settings. Please check your settings file.\n");
        return;
    }

    while (1)
    {
        printf("1. Set up connection\n");
        printf("2. Back to Main Menu\n");
        printf("Enter your choice (1-2): ");
        choice = getSingleDigitNumericalInput();

        switch (choice)
        {
            case 1:
                printf("Setting up networking...\n");
                establish_connection(&userSettings);
                break;
            case 2:
                printf("Returning to main menu...\n");
                return;
            default:
                printf("Invalid choice. Please select a valid option.\n");
                break;
        }
    }
}