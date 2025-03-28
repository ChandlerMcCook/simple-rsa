#include <stdlib.h>
#include <time.h>

#define E_VALUE 3

typedef struct {
    unsigned int prime1;
    unsigned int prime2;
} PrimePair;

typedef struct {
    unsigned int de;
    long n;
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

    do {
        p1 = rand();
    } while (!isPrime(p1));

    do {
        p2 = rand();
    } while (!isPrime(p2));

    PrimePair primes = {p1, p2};
    return primes;
}

int computeD(long phi) {
    unsigned int d = 3;
    while ((d * E_VALUE) % phi != 1) {
        d++;
    }
    return d;
}

Keys generateKeys() {
    PrimePair primes = generatePrimes();
    long n = primes.prime1 * primes.prime2; 
    long phi = (primes.prime1 - 1) * (primes.prime2 - 1);   
    unsigned int d = computeD(phi);
    // int d = 3;
    Key privateKey = {d, n};
    Key publicKey = {E_VALUE, n};

    printf("Phi: %d\n", phi);
    printf("n: %d\n", n);
    printf("Prime 1: %d\n", primes.prime1);
    printf("Prime 2: %d\n", primes.prime2);

    Keys keys = {privateKey, publicKey};

    return keys;
}

char encryptChar(char plaintext, Key key) {
    return (plaintext * key.de) % key.n;
}

void encryptFile(FILE* file, Key key) {
    if (file == NULL) {
        printf("Encrypt error: please provide file\n");
        return;
    }

    FILE* outfile;
    outfile = fopen("ciphertext.txt", "w");
    char content[1000];
    int buffSize = sizeof(content);

    while(fgets(content, sizeof(content), file)) {
        for (int i=0; i<buffSize; i++) {
            if (content[i] == '\0') {
                break;
            }
            content[i] = encryptChar(content[i], key);
        }

        fputs(content, outfile);
    }
}