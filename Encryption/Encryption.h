
#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct {
    long long n;
    long long e;
    long long d;
} RSAKeys;


static long long modexp(long long base, long long exp, long long mod); 
static int is_probably_prime(long long n, int k); 
static long long gcd(long long a, long long b); 
static long long modinv(long long e, long long phi);
RSAKeys generate_keys();
void encrypt_data(const char* input, long long* output, size_t* len, RSAKeys keys);
void decrypt_data(const long long* input, size_t len, char* output, RSAKeys keys);


#endif // ENCRYPTION_H
