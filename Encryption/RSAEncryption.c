// rsa_demo.c — RSA demo with block mode
// Build:
//   U64 mode:    gcc RSAOpenSSLEncryption.c -o encryption_test
//   OpenSSL:     gcc RSAOpenSSLEncryption.c -o encryption_test -DUSE_OPENSSL -lcrypto
//
// Usage: ./encryption_test <prime_bits> "message"

#include "RSAEncryption.h"

#ifndef USE_OPENSSL
/******************************
 *   U64 (no OpenSSL) path
 *****************************/

// (implementations of helpers remain static, only public functions are declared in .h)

static unsigned long long rand_ull(void) {
    unsigned long long r = 0;
    int bits = 0;
    while (bits < 64) {
        r = (r << 15) ^ (rand() & 0x7FFFu);
        bits += 15;
    }
    return r;
}

static unsigned long long rand_bits(int bits) {
    if (bits <= 0 || bits > 64) return 0;
    unsigned long long mask = (bits == 64) ? ~0ULL : ((1ULL << bits) - 1);
    unsigned long long num;
    do {
        num = rand_ull() & mask;
    } while (num < (1ULL << (bits - 1)));
    if ((num & 1ULL) == 0) num |= 1ULL;
    return num;
}

static unsigned long long gcd_u64(unsigned long long a, unsigned long long b) {
    while (b) { unsigned long long t = b; b = a % b; a = t; }
    return a;
}

static unsigned long long modinv_u64(unsigned long long e, unsigned long long phi) {
    long long t = 0, newt = 1;
    long long r = (long long)phi, newr = (long long)e;
    while (newr != 0) {
        long long q = r / newr;
        long long tmp = newt; newt = t - q * newt; t = tmp;
        tmp = newr; newr = r - q * newr; r = tmp;
    }
    if (r != 1) return (unsigned long long)-1;
    if (t < 0) t += (long long)phi;
    return (unsigned long long)t;
}

#if defined(__SIZEOF_INT128__)
static inline unsigned long long mul_mod_u64(unsigned long long a,
                                             unsigned long long b,
                                             unsigned long long mod) {
    __uint128_t t = ( __uint128_t)a * b;
    t %= mod;
    return (unsigned long long)t;
}
#else
static unsigned long long mul_mod_u64(unsigned long long a,
                                      unsigned long long b,
                                      unsigned long long mod) {
    unsigned long long res = 0;
    a %= mod;
    while (b) {
        if (b & 1ULL) { res += a; if (res >= mod) res -= mod; }
        a <<= 1; if (a >= mod) a -= mod;
        b >>= 1;
    }
    return res;
}
#endif

static unsigned long long modexp_u64(unsigned long long base,
                                     unsigned long long exp,
                                     unsigned long long mod) {
    unsigned long long result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) result = mul_mod_u64(result, base, mod);
        base = mul_mod_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static int is_probably_prime_u64(unsigned long long n, int k) {
    if (n < 2) return 0;
    if ((n & 1ULL) == 0) return n == 2;
    static const int small_primes[] = {3,5,7,11,13,17,19,23,29,31,37,0};
    for (int i=0; small_primes[i]; ++i) {
        if ((unsigned)small_primes[i] >= n) break;
        if (n % (unsigned)small_primes[i] == 0) return 0;
    }
    unsigned long long d = n - 1;
    int r = 0;
    while ((d & 1ULL) == 0) { d >>= 1; r++; }

    for (int i = 0; i < k; i++) {
        unsigned long long a = 2 + (rand_ull() % (n - 3));
        unsigned long long x = modexp_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int witness = 1;
        for (int j = 0; j < r - 1; j++) {
            x = mul_mod_u64(x, x, n);
            if (x == n - 1) { witness = 0; break; }
        }
        if (witness) return 0;
    }
    return 1;
}

int compute_block_size_u64(unsigned long long n) {
    int b = 0;
    unsigned long long t = 1;
    while (t <= (n - 1) / 256ULL) { t *= 256ULL; b++; }
    if (b < 1) b = 1;
    return b;
}

RSAKeys generate_keys_u64(int prime_bits) {
    RSAKeys keys = {0};
    unsigned long long p = 0, q = 0;
    do { p = rand_bits(prime_bits); } while (!is_probably_prime_u64(p, 10));
    do { q = rand_bits(prime_bits); } while (!is_probably_prime_u64(q, 10) || q == p);

    unsigned long long n = p * q;
    unsigned long long phi = (p - 1) * (q - 1);

    unsigned long long e = 65537ULL;
    if (gcd_u64(e, phi) != 1ULL) {
        e = 3;
        while (gcd_u64(e, phi) != 1ULL) e += 2ULL;
    }
    unsigned long long d = modinv_u64(e, phi);

    keys.n = n; keys.e = e; keys.d = d;
    return keys;
}

void encrypt_blocks_u64(const char *in, int len,
                        unsigned long long *out, int *out_len,
                        const RSAKeys *keys) {
    printf("Encrypting blocks...\n");
    int blk = compute_block_size_u64(keys->n);
    printf("Block size: %d\n", blk);
    int pos = 0, op = 0;
    while (pos < len) {
        int chunk = (len - pos < blk) ? (len - pos) : blk;
        unsigned long long m = 0;
        for (int i = 0; i < chunk; i++) m = (m << 8) | in[pos + i];
        out[op++] = modexp_u64(m, keys->e, keys->n);
        printf("op: %llu\n", out[op - 1]);
        pos += chunk;
    }
    *out_len = op;
}

void decrypt_blocks_u64(const unsigned long long *in, int in_len,
                        unsigned char *out, int *out_len,
                        int original_len, const RSAKeys *keys) {
    int blk = compute_block_size_u64(keys->n);
    int pos = 0, remaining = original_len;
    for (int i = 0; i < in_len; i++) {
        int chunk = (remaining < blk) ? remaining : blk;
        unsigned long long m = modexp_u64(in[i], keys->d, keys->n);
        for (int k = chunk - 1; k >= 0; k--) {
            out[pos + k] = (unsigned char)(m & 0xFFu);
            m >>= 8;
        }
        pos += chunk;
        remaining -= chunk;
    }
    *out_len = pos;
    out[pos] = '\0';
}

#else /* USE_OPENSSL defined */
/******************************
 *   OpenSSL (BIGNUM) path
 *****************************/

RSAKeysBN generate_keys_bn(int prime_bits) {
    RSAKeysBN keys = { BN_new(), BN_new(), BN_new() };
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *p = BN_new(), *q = BN_new(), *phi = BN_new();
    BIGNUM *p1 = BN_new(), *q1 = BN_new(), *g = BN_new();

    BN_generate_prime_ex(p, prime_bits, 0, NULL, NULL, NULL);
    BN_generate_prime_ex(q, prime_bits, 0, NULL, NULL, NULL);

    keys.n = BN_new();
    BN_mul(keys.n, p, q, ctx);

    p1 = BN_dup(p); q1 = BN_dup(q);
    BN_sub_word(p1, 1); BN_sub_word(q1, 1);
    BN_mul(phi, p1, q1, ctx);

    BN_set_word(keys.e, 65537);
    BN_gcd(g, keys.e, phi, ctx);
    if (!BN_is_one(g)) {
        BN_set_word(keys.e, 3);
        while (1) {
            BN_gcd(g, keys.e, phi, ctx);
            if (BN_is_one(g)) break;
            BN_add_word(keys.e, 2);
        }
    }

    BN_mod_inverse(keys.d, keys.e, phi, ctx);

    BN_free(p); BN_free(q); BN_free(phi);
    BN_free(p1); BN_free(q1); BN_free(g);
    BN_CTX_free(ctx);
    return keys;
}

int compute_block_size_bn(const BIGNUM *n) {
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *nm1 = BN_dup(n);
    BN_sub_word(nm1, 1);
    int bits = BN_num_bits(nm1);
    int b = bits / 8;
    if (b < 1) b = 1;
    BN_free(nm1);
    BN_CTX_free(ctx);
    return b;
}

void encrypt_blocks_bn(const unsigned char *in, int len,
                       BIGNUM **out, int *out_len,
                       const RSAKeysBN *keys) {
    BN_CTX *ctx = BN_CTX_new();
    int blk = compute_block_size_bn(keys->n);
    int pos = 0, op = 0;

    while (pos < len) {
        int chunk = (len - pos < blk) ? (len - pos) : blk;
        BIGNUM *m = BN_new(), *c = BN_new();
        BN_bin2bn(in + pos, chunk, m);
        BN_mod_exp(c, m, keys->e, keys->n, ctx);
        out[op++] = c;
        BN_free(m);
        pos += chunk;
    }
    *out_len = op;
    BN_CTX_free(ctx);
}

void decrypt_blocks_bn(BIGNUM **in, int in_len,
                       unsigned char *out, int *out_len,
                       int original_len, const RSAKeysBN *keys) {
    BN_CTX *ctx = BN_CTX_new();
    int blk = compute_block_size_bn(keys->n);
    int pos = 0, remaining = original_len;

    for (int i = 0; i < in_len; i++) {
        int chunk = (remaining < blk) ? remaining : blk;
        BIGNUM *m = BN_new();
        BN_mod_exp(m, in[i], keys->d, keys->n, ctx);
        BN_bn2binpad(m, out + pos, chunk);
        pos += chunk;
        remaining -= chunk;
        BN_free(m);
    }
    *out_len = pos;
    out[pos] = '\0';
    BN_CTX_free(ctx);
}

void free_blocks_bn(BIGNUM **arr, int len) {
    for (int i = 0; i < len; i++) BN_free(arr[i]);
}

void print_bn_hex(const BIGNUM *x) {
    char *hex = BN_bn2hex(x);
    printf("%s", hex);
    OPENSSL_free(hex);
}
#endif /* USE_OPENSSL */


// Used for demoing encryption made from main
int demo_encryption(int argc, char **argv) {
    int prime_bits = (argc >= 2) ? atoi(argv[1]) : 20;
    const char *msg = (argc >= 3) ? argv[2] : "Hello, RSA!";
    srand((unsigned)time(NULL));

#ifndef USE_OPENSSL
    if (prime_bits > 32) {
        printf("[U64 build] prime_bits=%d is too large. "
               "Use -DUSE_OPENSSL or pick <=32.\n", prime_bits);
        return 1;
    }

    RSAKeys keys = generate_keys_u64(prime_bits);
    printf("U64 keys: n=%llu, e=%llu, d=%llu\n", keys.n, keys.e, keys.d);

    const unsigned char *message = (const unsigned char*)msg;
    int msg_len = (int)strlen((const char*)message);

    unsigned long long ct[1024]; int ct_len = 0;
    encrypt_blocks_u64(message, msg_len, ct, &ct_len, &keys);

    printf("Ciphertext blocks (u64):\n");
    for (int i = 0; i < ct_len; i++) printf("%llu ", ct[i]);
    printf("\n");

    unsigned char pt[2048]; int pt_len = 0;
    decrypt_blocks_u64(ct, ct_len, pt, &pt_len, msg_len, &keys);
    printf("Decrypted: %s\n", pt);

#else
    RSAKeysBN keys = generate_keys_bn(prime_bits);

    printf("OpenSSL keys:\n  n(bits)=%d\n", BN_num_bits(keys.n));
    printf("  n = "); print_bn_hex(keys.n); printf("\n");
    printf("  e = "); print_bn_hex(keys.e); printf("\n");
    printf("  d = "); print_bn_hex(keys.d); printf("\n");

    const unsigned char *message = (const unsigned char*)msg;
    int msg_len = (int)strlen((const char*)message);

    BIGNUM *ct[1024]; int ct_len = 0;
    encrypt_blocks_bn(message, msg_len, ct, &ct_len, &keys);

    printf("Ciphertext blocks (hex):\n");
    for (int i = 0; i < ct_len; i++) {
        print_bn_hex(ct[i]);
        printf(" ");
    }
    printf("\n");

    unsigned char pt[4096]; int pt_len = 0;
    decrypt_blocks_bn(ct, ct_len, pt, &pt_len, msg_len, &keys);
    printf("Decrypted: %s\n", pt);

    free_blocks_bn(ct, ct_len);
    BN_free(keys.n); BN_free(keys.e); BN_free(keys.d);
#endif

    return 0;
}
