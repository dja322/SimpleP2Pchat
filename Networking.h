
#ifndef NETWORKING_H
#define NETWORKING_H

#ifdef __cplusplus
extern "C" {
#endif

void receive_thread(void* arg);
int establish_connection();

#ifdef __cplusplus
}
#endif

#endif // NETWORKING_H
