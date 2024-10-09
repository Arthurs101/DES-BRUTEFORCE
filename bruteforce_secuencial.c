#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Función de XOR para descifrar el texto con la llave
void xor_cipher(char *key, char *input, char *output, int len) {
    for (int i = 0; i < len; i++) {
        output[i] = input[i] ^ key[i % 8];  // Cada llave es de 8 bytes
    }
}

// Convierte una llave larga (long) en una secuencia de 8 bytes
void long_to_key(unsigned long long key_long, char *key) {
    for (int i = 0; i < 8; i++) {
        key[i] = (key_long >> (56 - 8 * i)) & 0xFF;
    }
}

// Intenta descifrar el texto con la clave y comprueba si contiene la palabra clave
int try_key(unsigned long long key_long, char *ciphertext, char *plaintext, int len, char *search_term) {
    char key[8];  // Llave de 8 bytes
    long_to_key(key_long, key);  // Convertir la llave larga a bytes
    xor_cipher(key, ciphertext, plaintext, len);
    plaintext[len] = '\0';  // Asegurarse de que sea una cadena válida
    return strstr(plaintext, search_term) != NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <cipher file> <search term>\n", argv[0]);
        return 1;
    }

    // Cargar el archivo cifrado
    FILE *cipherfile = fopen(argv[1], "rb");
    if (cipherfile == NULL) {
        printf("Error opening cipher file.\n");
        return 1;
    }
    fseek(cipherfile, 0, SEEK_END);
    int len = ftell(cipherfile);
    rewind(cipherfile);

    char *ciphertext = (char *)malloc(len);
    fread(ciphertext, 1, len, cipherfile);
    fclose(cipherfile);

    char *search_term = argv[2];
    char *plaintext = (char *)malloc(len + 1);

    // Fuerza bruta para encontrar la clave correcta
    clock_t start_time = clock();
    unsigned long long key_candidate = 0;  // Inicializa la clave candidata

    while (1) {  // Bucle infinito para probar todas las claves
        if (try_key(key_candidate, ciphertext, plaintext, len, search_term)) {
            printf("Clave encontrada: %llu\n", key_candidate);
            printf("Mensaje descifrado: %s\n", plaintext);
            break;  // Salir al encontrar la clave
        }
        key_candidate++;  // Incrementar la clave candidata
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Tiempo transcurrido: %.2f segundos\n", elapsed_time);

    free(ciphertext);
    free(plaintext);
    return 0;  // Salida exitosa
}
