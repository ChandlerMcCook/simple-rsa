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

        fprintf(keysFile, "Public Key: {%lu %u\n", publicKey.n, publicKey.de);
        fprintf(keysFile, "Private Key: {%lu %u\n", privateKey.n, privateKey.de);
        printf("Keys generated and saved to keys.txt\n");
    } else {
        
    }
    
    FILE* infile;
    char anotherChoice;
    do {
        printf("Enter a filename: ");
        scanf("%s", fileName);
    
        // encrypt or decrypt the file
        printf("Encrypt or decrypt? (e/d): ");
        char choice;
        scanf(" %c", &choice);
        
        infile = fopen(fileName, "r");
    
        char content[1000];
        int buffSize = sizeof(content);
    
        if (infile == NULL) {
            printf("Error opening file");
            return 1;
        } else {
            if (choice == 'e') {
                printf("Encrypting...\n");
                rsa(infile, keys.pub, "ciphertext.txt", 1);
            } else if (choice == 'd') {
                printf("Decrypting...\n");
                rsa(infile, keys.priv, "decrypted.txt", 0);
            } else {
                printf("Invalid choice\n");
            }
        }

        fclose(infile);
        printf("Do you want to encrypt/decrypt another file? (y/n): ");
        scanf(" %c", &anotherChoice);
    } while (anotherChoice == 'y');
    
    return 0;
}