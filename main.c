#include <stdio.h>
#include "./RSA/generate_keys.c"

int main() {
    char fileName[50];

    Keys keys = generateKeys();
    printf("Priv.de: %d\n", keys.priv.de);

    printf("Enter a filename: ");
    scanf("%s", fileName);

    FILE* infile;
    FILE* outfile;
    
    infile = fopen(fileName, "r");
    outfile = fopen("ciphertext.txt", "w");

    char content[1000];
    int buffSize = sizeof(content);

    if (infile == NULL) {
        printf("Error opening file");
        return 0;
    } else {
        encryptFile(infile, keys.priv);
    }
    
    fclose(infile);
    fclose(outfile);

    return 0;
}