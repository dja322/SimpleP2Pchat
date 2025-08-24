// rsa_demo.c — RSA demo with block mode
// Build:
//   U64 mode:    gcc RSAOpenSSLEncryption.c -o encryption_test
//
// Usage: ./encryption_test <prime_bits> "message"

#include "RSAEncryption.h"

/******************************
 *   U64 path
 *****************************/

// (implementations of helpers remain static, only public functions are declared in .h)

static unsigned long long rand_ull(void) {
    srand(time(NULL));
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
    int blk = compute_block_size_u64(keys->n);
    int pos = 0, op = 0;

    // Always include the null terminator in the encryption stream
    int total = len + 1;  

    while (pos < total) {
        int chunk = (total - pos < blk) ? (total - pos) : blk;
        unsigned long long m = 0;

        // Pack chunk bytes into integer
        for (int i = 0; i < chunk; i++) {
            m = (m << 8) | (unsigned char)in[pos + i];
        }

        // If the chunk is smaller than blk, pad with zeroes
        for (int i = chunk; i < blk; i++) {
            m = (m << 8);
        }

        out[op++] = modexp_u64(m, keys->e, keys->n);
        pos += chunk;
    }

    *out_len = op;
}

void decrypt_blocks_u64(const unsigned long long *in, int in_len,
                        unsigned char *out, int *out_len,
                        const RSAKeys *keys) {
    int blk = compute_block_size_u64(keys->n);
    int pos = 0;

    for (int i = 0; i < in_len; i++) {
        unsigned long long m = modexp_u64(in[i], keys->d, keys->n);

        // Unpack exactly blk bytes
        for (int k = blk - 1; k >= 0; k--) {
            out[pos + k] = (unsigned char)(m & 0xFFu);
            m >>= 8;
        }
        pos += blk;
    }

    // Find actual string length by stopping at null terminator
    int actualLen = 0;
    while (actualLen < pos && out[actualLen] != '\0') {
        actualLen++;
    }

    *out_len = actualLen;
    out[actualLen] = '\0';  // ensure null-terminated
}


// Used for demoing encryption made from main
int demo_encryption(int argc, char **argv) {
    int prime_bits = (argc >= 2) ? atoi(argv[1]) : 20;
    const char *msg = (argc >= 3) ? argv[2] : "Hello, RSA!";
    srand((unsigned)time(NULL));

    if (prime_bits > 32) {
        printf("[U64 build] prime_bits=%d is too large. "
               "Use -DUSE_OPENSSL or pick <=32.\n", prime_bits);
        return 1;
    }

    RSAKeys keys = generate_keys_u64(prime_bits);
    printf("U64 keys: n=%llu, e=%llu, d=%llu\n", keys.n, keys.e, keys.d);

    const char *message = (const char*)msg;
    int msg_len = (int)strlen((const char*)message);

    unsigned long long ct[1024]; int ct_len = 0;
    encrypt_blocks_u64(message, msg_len, ct, &ct_len, &keys);

    printf("Ciphertext blocks (u64):\n");
    for (int i = 0; i < ct_len; i++) printf("%llu ", ct[i]);
    printf("\n");

    unsigned char pt[2048]; int pt_len = 0;
    decrypt_blocks_u64(ct, ct_len, pt, &pt_len, &keys);
    printf("Decrypted: %s\n", pt);

    return 0;
}
