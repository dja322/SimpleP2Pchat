
#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#ifdef __cplusplus
extern "C" {
#endif

void encrypt_data(const char* input, char* output);
void decrypt_data(const char* input, char* output);

#ifdef __cplusplus
}
#endif

#endif // ENCRYPTION_H
