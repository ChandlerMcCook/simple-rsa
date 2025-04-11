#include <stdio.h>
#include <time.h>

#define E_VALUE 3

typedef struct {
    unsigned int prime1;
    unsigned int prime2;
} PrimePair;

typedef struct {
    unsigned int de;
    long unsigned int n;
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

int computeD(long unsigned int phi) {
    unsigned int d = 3;
    while ((d * E_VALUE) % phi != 1) {
        d++;
    }
    return d;
}

Keys generateKeys() {
    PrimePair primes = generatePrimes();
    long unsigned int n = primes.prime1 * primes.prime2; 
    long unsigned int phi = (primes.prime1 - 1) * (primes.prime2 - 1);   
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

char modChar(char plaintext, Key key) {
    unsigned int result = 1;
    unsigned int base = plaintext;
    unsigned int power = key.de;
    unsigned long int mod = key.n;
    
    while (power > 0) {
        if (power % 2 == 1) {
            result = (result * base) % mod;
        }
        power >>= 1;
        base = (base * base) % mod;
    }

    return result;
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
            content[i] = modChar(content[i], key);
        }

        fputs(content, outfile);
    }
}