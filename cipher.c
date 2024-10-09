#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Converts a long key into an 8-byte sequence
void long_to_bytes(unsigned long long value, char *bytes) {
    for (int i = 0; i < 8; i++) {
        bytes[i] = (value >> (56 - 8 * i)) & 0xFF;  // Extract each byte
    }
}

// XOR function to encrypt/decrypt text with the key
void xor_cipher(const char *input, unsigned long long key, char *output, int len) {
    char key_bytes[8];
    long_to_bytes(key, key_bytes);  // Convert key to 8 bytes

    for (int i = 0; i < len; i++) {
        output[i] = input[i] ^ key_bytes[i % 8];  // Use corresponding byte of key
    }
}

// Save data to a file
void save_to_file(const char *filename, const char *data, int len) {
    FILE *file = fopen(filename, "wb");
    if (file != NULL) {
        fwrite(data, sizeof(char), len, file);
        fclose(file);
        printf("Ciphertext saved in '%s'\n", filename);
    } else {
        perror("Error opening the file");
    }
}

// Load data from a file
char* load_from_file(const char *filename, int *len) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening the file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *len = ftell(file);
    rewind(file);

    char *data = (char *)malloc(*len);
    if (data == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }
    
    fread(data, sizeof(char), *len, file);
    fclose(file);
    return data;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <plaintext> <key>\n", argv[0]);
        return 1;
    }
    // Texto a cifrar
    const char *plaintext = argv[1];
    printf("text %s \n",plaintext);
    // Convertir la llave de cadena a unsigned long long
    unsigned long long key = strtoull(argv[2], NULL, 10);
     printf("key  %lld \n",key);
    // Longitud del texto plano
    int len = strlen(plaintext);

    // Allocate memory for the ciphertext
    char *ciphertext = (char *)malloc(len);
    if (ciphertext == NULL) {
        perror("Error allocating memory for ciphertext");
        return 1;
    }
    
    // Encrypt the text
    xor_cipher(plaintext, key, ciphertext, len);  

    // Save the ciphertext to a file
    save_to_file("cipherC.bin", ciphertext, len);

    // Clean up memory
    free(ciphertext);

    return 0;  // Successful exit
}

