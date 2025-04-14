#include <stdio.h>
#include "./RSA/generate_keys.c"

int main() {
    char fileName[50];
    Key publicKey, privateKey;

    // ask user to if they would like to generate keys, if not then ask for keys from user input
    printf("Generate new keys? (y/n): ");
    char generateKeysChoice;
    scanf(" %c", &generateKeysChoice);

    if (generateKeysChoice == 'y') {
        printf("Generating new keys...\n");
        FILE* keysFile = fopen("keys.txt", "w");
        if (keysFile == NULL) {
            printf("Error opening keys file\n");
            return 1;
        }

        // Save keys to file
        Keys keys = generateKeys();
        publicKey = keys.pub;
        privateKey = keys.priv;

        fprintf(keysFile, "Public Key: {%llu, %lu}\n", publicKey.n, publicKey.de);
        fprintf(keysFile, "Private Key: {%llu, %lu}\n", privateKey.n, privateKey.de);
        printf("Keys generated and saved to keys.txt\n");
    } else {
        // ask user to enter a public or private key or both
        printf("Would you like to enter a public key? (y/n): ");
        char publicKeyChoice; 
        scanf(" %c", &publicKeyChoice);
        if (publicKeyChoice == 'y') {
            printf("Enter public key (n e): ");
            scanf("%llu %lu", &publicKey.n, &publicKey.de);
        }

        printf("Would you like to enter a private key? (y/n): ");
        char privateKeyChoice;
        scanf(" %c", &privateKeyChoice);
        if (privateKeyChoice == 'y') {
            printf("Enter private key (n d): ");
            scanf("%llu %lu", &privateKey.n, &privateKey.de);
        }
    }
    
    FILE* infile;
    char anotherChoice;
    do {
        printf("Enter a filename: ");
        scanf("%s", fileName);
    
        
        infile = fopen(fileName, "r");
    
        if (infile == NULL) {
            printf("Error opening file");
            return 1;
        } else {
            printf("Which key would you like to use (u/v): ");
            char keyChoice;
            scanf(" %c", &keyChoice);

            printf("Encrypt or decrypt? (e/d): ");
            char encryptionChoice;
            scanf(" %c", &encryptionChoice);
            int encrypt = 0;
            if (encryptionChoice == 'e') {
                encrypt = 1;
            } else if (encryptionChoice == 'd') {
                encrypt = 0;
            } else {
                printf("Invalid encryption choice\n");
            }

            printf("Enter new filename: ");
            char newFileName[50];
            scanf("%s", newFileName);

            if (keyChoice == 'u') {
                printf("Encrypting...\n");
                rsa(infile, publicKey, newFileName, encrypt);
            } else if (keyChoice == 'v') {
                printf("Decrypting...\n");
                rsa(infile, privateKey, newFileName, encrypt);
            } else {
                printf("Invalid key choice\n");
            }
        }

        fclose(infile);
        printf("Do you want to encrypt/decrypt another file? (y/n): ");
        scanf(" %c", &anotherChoice);
    } while (anotherChoice == 'y');
    
    return 0;
}