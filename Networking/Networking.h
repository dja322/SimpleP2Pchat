
#ifndef NETWORKING_H
#define NETWORKING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "../DataTypes.h"

void* receive_thread(void* arg);
int establish_connection(settings_t *settings);
void runConnection();


#endif // NETWORKING_H
