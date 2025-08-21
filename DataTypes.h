#ifndef DATA_TYPES_H
#define DATA_TYPES_H

typedef struct settings_struct {
    char username[50];
    char password[50];
    char server_ip[15];
    int server_port;
} settings_t;

typedef struct contact_struct {
    char name[100];
    char address[100];
} contact_t;

#endif // DATA_TYPES_H