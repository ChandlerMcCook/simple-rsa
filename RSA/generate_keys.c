#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define E_VALUE 3

typedef struct {
    unsigned int prime1;
    unsigned int prime2;
} PrimePair;

typedef struct {
    long unsigned int de;
    long long unsigned int n;
} Key;

typedef struct {
    Key priv;
    Key pub;
} Keys;

int isPrime(int num) {
    for (int i=2; i<num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }

    return 1;
}

PrimePair generatePrimes() {
    unsigned int p1, p2;
    srand(time(NULL));  

    do {
        p1 = rand();
    } while (!isPrime(p1));

    do {
        p2 = rand();
    } while (!isPrime(p2));

    PrimePair primes = {p1, p2};
    return primes;
}

long unsigned int computeE(long unsigned int phi) {
    unsigned int e = E_VALUE;
    while (e < phi) {
        if (phi % e != 0) {
            return e;
        }
        e += 2; 
    }
    return -1; // Should not reach here
}

long unsigned int computeD(long long unsigned int phi, int e) {
    unsigned long int d = 3;
    while ((d * e) % phi != 1) {
        d++;
    }
    return d;
}

Keys generateKeys() {
    PrimePair primes = generatePrimes();
    long long unsigned int n = primes.prime1 * primes.prime2; 
    long long unsigned int phi = (long long unsigned int) (primes.prime1 - 1) * (primes.prime2 - 1);   
    long unsigned int e = computeE(phi);
    long unsigned int d = computeD(phi, e);
    Key privateKey = {d, n};
    Key publicKey = {e, n};

    Keys keys = {privateKey, publicKey};

    return keys;
}

long long unsigned int modExp(long long unsigned int base, long unsigned int exp, long long unsigned int mod) {
    long long unsigned int result = 1;
    base = base % mod; 

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1; 
        base = (base * base) % mod; 
    }
    return result;
}

void rsa(FILE* file, Key key, const char* outputFileName, int encrypt) {
    if (file == NULL) {
        printf("Encrypt error: please provide file\n");
        return;
    }

    FILE* outfile = fopen(outputFileName, "w");

    if (encrypt) {
        char c;
        while ((c = fgetc(file)) != EOF) {
            long long unsigned int encryptedVal = modExp(c, key.de, key.n);
            fprintf(outfile, "%llu ", encryptedVal);
        }
    } else {
        long long unsigned int encryptedVal;
        while ((fscanf(file, "%llu", &encryptedVal)) == 1) {
            char decrypted = (char) modExp(encryptedVal, key.de, key.n); 
            fputc(decrypted, outfile);
        }
    }

    fclose(outfile);
}