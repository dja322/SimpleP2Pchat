
#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct {
    unsigned long long n;
    unsigned long long e;
    unsigned long long d;
} RSAKeys;


static unsigned long long modexp(unsigned long long base, unsigned long long exp, unsigned long long mod); 
static int is_probably_prime(unsigned long long n, int k); 
static unsigned long long gcd(unsigned long long a, unsigned long long b); 
static unsigned long long modinv(unsigned long long e, unsigned long long phi);
static int compute_block_size(unsigned long long n);
static unsigned long long rand_ull(void);
static unsigned long long rand_bits(int bits);
RSAKeys generate_keys_bits(int prime_bits);
void encrypt_data_blocks(const unsigned char *in, int len,
                         unsigned long long *out, int *out_len, RSAKeys keys);
void decrypt_data_blocks(const unsigned long long *in, int in_len,
                         unsigned char *out, int *out_len,
                         int original_len, RSAKeys keys);

#endif // ENCRYPTION_H
