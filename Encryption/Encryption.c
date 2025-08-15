
#include "Encryption.h"
//char numbers 32 - 126

// Simple modular exponentiation
static unsigned long long modexp(unsigned long long base, unsigned long long exp, unsigned long long mod) 
{
    unsigned long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Miller–Rabin primality test for small unsigned long long numbers
static int is_probably_prime(unsigned long long n, int k) 
{
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;

    unsigned long long d = n - 1;
    int r = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        r++;
    }

    for (int i = 0; i < k; i++) {
        unsigned long long a = 2 + rand() % (n - 3);
        unsigned long long x = modexp(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int cont = 0;
        for (int j = 0; j < r - 1; j++) {
            x = (x * x) % n;
            if (x == n - 1) { cont = 1; break; }
        }
        if (!cont) return 0;
    }
    return 1;
}

// Euclidean GCD
static unsigned long long gcd(unsigned long long a, unsigned long long b) 
{
    while (b != 0) {
        unsigned long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Extended Euclidean for modular inverse
static unsigned long long modinv(unsigned long long e, unsigned long long phi) {
    long long t = 0, newt = 1;
    long long r = (long long)phi, newr = (long long)e;

    while (newr != 0) {
        long long q = r / newr;
        long long tmp = newt;
        newt = t - q * newt;
        t = tmp;
        tmp = newr;
        newr = r - q * newr;
        r = tmp;
    }

    if (r != 1) return (unsigned long long)-1; // not invertible
    if (t < 0) t += (long long)phi;
    return (unsigned long long)t;
}

// Max number of bytes b such that 256^b <= n-1
static int compute_block_size(unsigned long long n) 
{
    int b = 0;
    unsigned long long t = 1;         // = 256^b
    while (t <= (n - 1) / 256ULL) {   // next multiply still keeps 256^(b+1) <= n-1
        t *= 256ULL;
        b++;
    }
    if (b < 1) b = 1;
    return b;
}

// Generate a random unsigned long long with full 64-bit randomness
static unsigned long long rand_ull(void) 
{
    unsigned long long r = 0;
    int bits = 0;
    while (bits < 64) {
        r = (r << 15) ^ (rand() & 0x7FFF); // 15 bits from each rand()
        bits += 15;
    }
    return r;
}

// Generate a random number with 'bits' significant bits
static unsigned long long rand_bits(int bits) 
{
    if (bits <= 0 || bits > 64) return 0;
    unsigned long long mask = (bits == 64) ? ~0ULL : ((1ULL << bits) - 1);
    unsigned long long num;
    do {
        num = rand_ull() & mask;
    } while (num < (1ULL << (bits - 1))); // ensure MSB set
    return num;
}


// Generate RSA keys with prime size in bits
RSAKeys generate_keys_bits(int prime_bits) 
{
    RSAKeys keys = {0};
    srand((unsigned int)time(NULL));

    unsigned long long p = 0, q = 0;

    // Generate random primes of desired bit size
    do { p = rand_bits(prime_bits); } while (!is_probably_prime(p, 50));
    do { q = rand_bits(prime_bits); } while (!is_probably_prime(q, 50) || q == p);

    char* fmt = "Generated primes (%d-bit):\n p = %llu\n q = %llu\n";
    printf(fmt, prime_bits, p, q);

    keys.n = p * q;
    unsigned long long phi = (p - 1) * (q - 1);

    // Public exponent
    keys.e = 65537;
    if (gcd(keys.e, phi) != 1) {
        keys.e = 3;
        while (gcd(keys.e, phi) != 1) keys.e += 2;
    }

    // Private exponent
    keys.d = modinv(keys.e, phi);

    return keys;
}

void encrypt_data_blocks(const unsigned char *in, int len,
                         unsigned long long *out, int *out_len, RSAKeys keys) {
    const int block_size = compute_block_size(keys.n);
    int pos = 0, outpos = 0;

    while (pos < len) {
        int chunk = (len - pos < block_size) ? (len - pos) : block_size;
        unsigned long long m = 0;
        for (int i = 0; i < chunk; i++) {        // pack MSB first
            m = (m << 8) | in[pos + i];
        }
        out[outpos++] = modexp(m, keys.e, keys.n);
        pos += chunk;
    }
    *out_len = outpos;
}

void decrypt_data_blocks(const unsigned long long *in, int in_len,
                         unsigned char *out, int *out_len,
                         int original_len, RSAKeys keys) {
    const int block_size = compute_block_size(keys.n);
    int outpos = 0;
    int remaining = original_len;

    for (int i = 0; i < in_len; i++) {
        unsigned long long m = modexp(in[i], keys.d, keys.n);
        int chunk = (remaining < block_size) ? remaining : block_size;

        // extract exactly 'chunk' bytes, big‑endian
        for (int k = chunk - 1; k >= 0; k--) {
            out[outpos + k] = (unsigned char)(m & 0xFF);
            m >>= 8;
        }
        outpos += chunk;
        remaining -= chunk;
    }

    *out_len = outpos;
    out[outpos] = '\0';
}

int main() {
    RSAKeys keys = generate_keys_bits(16);
    char* P_fmt = "Public Key: (n: %llu, e: %llu)\n";
    char* S_fmt = "Private Key: (n: %llu, d: %llu)\n";
    printf("RAND_MAX = %d\n", RAND_MAX);
    printf(P_fmt, keys.n, keys.e);
    printf(S_fmt, keys.n, keys.d);

    const unsigned char* message = (unsigned char*) "Hello, RSA!";
    int msg_len = strlen((const char*)message);
    
    unsigned long long encrypted[128] = {0};   // store encrypted integers
    unsigned char decrypted[128] = {0};
    
    int enc_len = 0, dec_len = 0;
    encrypt_data_blocks(message, msg_len, encrypted, &enc_len, keys);
    
    printf("Encrypted blocks: ");
    char *fmt = "%llu ";
    for (int i = 0; i < enc_len; i++) {
        printf(fmt, encrypted[i]);
    }
    printf("\n");
    
    decrypt_data_blocks(encrypted, enc_len, decrypted, &dec_len, msg_len, keys);

    printf("Decrypted: %s\n", decrypted);

    return 0;
}