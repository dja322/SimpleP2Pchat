// rsa_demo.h — header for RSA demo

#ifndef RSA_ENCRYPTION_H
#define RSA_ENCRYPTION_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/******************************
 *   U64 path
 *****************************/
typedef struct {
    unsigned long long n, e, d;
} RSAKeys;

typedef struct {
    RSAKeys *own;
    RSAKeys *other;
} KeyPair;

RSAKeys generate_keys_u64(int prime_bits);

int compute_block_size_u64(unsigned long long n);

void encrypt_blocks_u64(const char *in, int len,
                        unsigned long long *out, int *out_len,
                        const RSAKeys *keys);

void decrypt_blocks_u64(const unsigned long long *in, int in_len,
                        unsigned char *out, int *out_len,
                        const RSAKeys *keys);

int demo_encryption(int argc, char **argv);

#endif /* RSA_ENCRYPTION_H */
