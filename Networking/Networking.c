
#include "Networking.h"
#include "../UtilityFiles/UtilityFunctions.h"
#include "../UtilityFiles/StringUtils.h"
#include "../UserActivity/UserSettings.h"
#include "../Encryption/RSAEncryption.h"

#define PORT 5000
#define BUFFER_SIZE 1024

int sockfd;

void *receive_thread(void* arg) {
    bool usernameKnown = false;
    char username[BUFFER_SIZE];
    unsigned char buffer[BUFFER_SIZE];
    unsigned long long recvBuffer[BUFFER_SIZE];
    int recvLen = 0;
    KeyPair *kp = (KeyPair*)arg;
    RSAKeys *ownKeys = kp->own;
    RSAKeys *otherKeys = kp->other;

    ssize_t len = recv(sockfd, username, sizeof(username) - 1, 0);
    if (len > 0) {
        username[len] = '\0';
        usernameKnown = true;
    }
    len = recv(sockfd, &otherKeys->e, sizeof(otherKeys->e), 0);
    len = recv(sockfd, &otherKeys->n, sizeof(otherKeys->n), 0);

    while (1) {
        len = recv(sockfd, recvBuffer, sizeof(recvBuffer) - 1, 0);
        if (len <= 0) {
            printf("\nConnection closed.\n");
            exit(0);
        }
        
        decrypt_blocks_u64(recvBuffer, len, buffer, &recvLen, BUFFER_SIZE, ownKeys);

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


    RSAKeys keys = generate_keys_u64(32);
    RSAKeys otherKeys = {0,0,0};
    // Allocate a KeyPair on the heap so it survives after the function scope
    KeyPair *kp = malloc(sizeof(KeyPair));
    kp->own = &keys;
    kp->other = &otherKeys;

    printf("Connection established. You can start chatting.\n");
    pthread_create(&recv_thread, NULL, receive_thread, (void*)kp);
    sleep(1); // Give the thread time to start
    send(sockfd, settings->username, strlen(settings->username), 0);
    send(sockfd, &keys.e, sizeof(keys.e), 0);
    send(sockfd, &keys.n, sizeof(keys.n), 0);

    // unsigned long long sendBuffer[BUFFER_SIZE];
    int sendBufferLen = 0;

    printf("Data received from client:\n");
    printf("Own public key (e): %llu\n", keys.e);
    printf("Own public key (n): %llu\n", keys.n);

    while (1) {
        printf("> ");
    if (!fgets(buffer, BUFFER_SIZE, stdin)) {
        break; // EOF or error
    }

    if (strncmp(buffer, "exit", 4) == 0)
        break;

    // Compute block size safely for allocation
    int blk = compute_block_size_u64(otherKeys.n);
    int maxBlocks = (strlen(buffer) + blk - 1) / blk;

    // Allocate send buffer to exactly the needed size
    unsigned long long sendBuffer[maxBlocks];

    encrypt_blocks_u64(buffer, strlen(buffer),
                       sendBuffer, &sendBufferLen, &otherKeys);

    if (sendBufferLen > 0) {
        printf("Encrypted Message (first block): %llu\n", sendBuffer[0]);

        // Send only the valid portion of the buffer
        send(sockfd, sendBuffer, sendBufferLen * sizeof(unsigned long long), 0);
    } else {
        printf("Nothing to encrypt.\n");
    }
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