
#include "Encryption.h"
//char numbers 32 - 126

// Simple modular exponentiation
static long long modexp(long long base, long long exp, long long mod) 
{
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Miller–Rabin primality test for small long long numbers
static int is_probably_prime(long long n, int k) 
{
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;

    long long d = n - 1;
    int r = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        r++;
    }

    for (int i = 0; i < k; i++) {
        long long a = 2 + rand() % (n - 3);
        long long x = modexp(a, d, n);
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
static long long gcd(long long a, long long b) 
{
    while (b != 0) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Extended Euclidean for modular inverse
static long long modinv(long long e, long long phi) 
{
    long long t = 0, newt = 1;
    long long r = phi, newr = e;
    while (newr != 0) {
        long long quotient = r / newr;
        long long temp = newt;
        newt = t - quotient * newt;
        t = temp;
        temp = newr;
        newr = r - quotient * newr;
        r = temp;
    }
    if (r > 1) return -1;
    if (t < 0) t += phi;
    return t;
}

// Generate RSA keys (toy-sized)
RSAKeys generate_keys() 
{
    RSAKeys keys = {0};
    srand((unsigned int)time(NULL));

    long long p = 0, q = 0;

    // Generate random 16-bit-ish primes (DEMO ONLY)
    while (!is_probably_prime(p, 5)) p = (rand() % 32768) + 32768; // ~15–16 bits
    while (!is_probably_prime(q, 5) || q == p) q = (rand() % 32768) + 32768;

    printf("Generated primes: p = %lld, q = %lld\n", p, q);

    keys.n = p * q;
    long long phi = (p - 1) * (q - 1);

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

void encrypt_data(const char* input, long long* output, int* len, RSAKeys keys) {
    int n = strlen(input);
    for (int i = 0; i < n; i++) {
        output[i] = modexp((long long)(unsigned char)input[i], keys.e, keys.n);
    }
    *len = n;
}

void decrypt_data(const long long* input, int len, char* output, RSAKeys keys) {
    for (int i = 0; i < len; i++) {
        output[i] = (char)modexp(input[i], keys.d, keys.n);
    }
    output[len] = '\0';
}

int main() {
    RSAKeys keys = generate_keys();
    printf("Public Key: (n: %lld, e: %lld)\n", keys.n, keys.e);
    printf("Private Key: (n: %lld, d: %lld)\n", keys.n, keys.d);

    const char* message = "Hello, RSA!";
    int msg_len = strlen(message);

    long long encrypted[128] = {0};   // store encrypted integers
    char decrypted[128] = {0};
    int encrypted_len = 0;

    // Encrypt the message
    for (int i = 0; i < msg_len; i++) {
        encrypted[i] = modexp((long long)(unsigned char)message[i], keys.e, keys.n);
    }
    encrypted_len = msg_len;

    printf("Encrypted: ");
    for (int i = 0; i < encrypted_len; i++) {
        printf("%lld ", encrypted[i]);
    }
    printf("\n");

    // Decrypt the message
    for (int i = 0; i < encrypted_len; i++) {
        decrypted[i] = (char)modexp(encrypted[i], keys.d, keys.n);
    }
    decrypted[encrypted_len] = '\0';

    printf("Decrypted: %s\n", decrypted);

    return 0;
}