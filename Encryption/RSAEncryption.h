// rsa_demo.h — header for RSA demo

#ifndef RSA_ENCRYPTION_H
#define RSA_ENCRYPTION_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#ifndef USE_OPENSSL
/******************************
 *   U64 (no OpenSSL) path
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

#else /* USE_OPENSSL defined */
/******************************
 *   OpenSSL (BIGNUM) path
 *****************************/
#include <openssl/bn.h>

typedef struct {
    BIGNUM *n, *e, *d;
} RSAKeysBN;

RSAKeysBN generate_keys_bn(int prime_bits);

int compute_block_size_bn(const BIGNUM *n);

void encrypt_blocks_bn(const unsigned char *in, int len,
                       BIGNUM **out, int *out_len,
                       const RSAKeysBN *keys);

void decrypt_blocks_bn(BIGNUM **in, int in_len,
                       unsigned char *out, int *out_len,
                       int original_len, const RSAKeysBN *keys);

void free_blocks_bn(BIGNUM **arr, int len);

void print_bn_hex(const BIGNUM *x);

#endif /* USE_OPENSSL */

int demo_encryption(int argc, char **argv);

#endif /* RSA_ENCRYPTION_H */
