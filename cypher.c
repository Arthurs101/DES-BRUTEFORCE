#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Convierte una llave larga (long) en una secuencia de 8 bytes
void long_to_bytes(unsigned long long value, char *bytes) {
    for (int i = 0; i < 8; i++) {
        bytes[i] = (value >> (56 - 8 * i)) & 0xFF;  // Extraer cada byte
    }
}

// Función de XOR para cifrar/descifrar el texto con la llave
void xor_cipher(const char *input, unsigned long long key, char *output, int len) {
    char key_bytes[8];
    long_to_bytes(key, key_bytes);  // Convertir la llave a 8 bytes

    for (int i = 0; i < len; i++) {
        output[i] = input[i] ^ key_bytes[i % 8];  // Usar el byte correspondiente de la llave
    }
}

// Guardar datos en un archivo
void save_to_file(const char *filename, const char *data, int len) {
    FILE *file = fopen(filename, "wb");
    if (file != NULL) {
        fwrite(data, sizeof(char), len, file);
        fclose(file);
        printf("Texto cifrado guardado en '%s'\n", filename);
    } else {
        perror("Error al abrir el archivo");
    }
}

// Cargar datos desde un archivo
char* load_from_file(const char *filename, int *len) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *len = ftell(file);
    rewind(file);

    char *data = (char *)malloc(*len);
    if (data == NULL) {
        perror("Error al alocar memoria");
        fclose(file);
        return NULL;
    }
    
    fread(data, sizeof(char), *len, file);
    fclose(file);
    return data;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {  // Espera dos argumentos: texto y llave
        printf("Uso: %s <texto a cifrar> <llave>\n", argv[0]);
        return 1;
    }

    // Texto a cifrar desde argumentos de línea de comandos
    const char *plaintext = argv[1];

    // Convertir el argumento de llave a unsigned long long
    char *endptr;
    unsigned long long key = strtoull(argv[2], &endptr, 10);

    // Verificar si se realizó una conversión válida y que la clave está en el rango esperado
    if (endptr == argv[2] || *endptr != '\0' || key > 18446744073709551615ULL) {
        printf("Error: La llave debe ser un número válido entre 0 y 18446744073709551615.\n");
        return 1;
    }

    // Longitud del texto plano
    int len = strlen(plaintext);

    // Alocar memoria para el texto cifrado
    char *ciphertext = (char *)malloc(len);
    if (ciphertext == NULL) {
        perror("Error al alocar memoria para ciphertext");
        return 1;
    }

    xor_cipher(plaintext, key, ciphertext, len);  // Cifrar el texto

    // Guardar el texto cifrado en un archivo
    save_to_file("cipherC.bin", ciphertext, len);

    // Cargar el texto cifrado para descifrarlo
    int loaded_len;
    char *loaded_ciphertext = load_from_file("cipherC.bin", &loaded_len);
    if (loaded_ciphertext == NULL) {
        free(ciphertext);
        return 1;  // Salir si no se pudo cargar el archivo
    }

    // Alocar memoria para el texto descifrado
    char *decrypted_text = (char *)malloc(loaded_len + 1); // +1 para el terminador nulo
    if (decrypted_text == NULL) {
        perror("Error al alocar memoria para decrypted_text");
        free(ciphertext);
        free(loaded_ciphertext);
        return 1;
    }

    xor_cipher(loaded_ciphertext, key, decrypted_text, loaded_len);  // Descifrar con la misma llave
    decrypted_text[loaded_len] = '\0';  // Asegurarse de que el texto descifrado sea una cadena válida

    // Imprimir el texto descifrado
    printf("Texto descifrado: %s\n", decrypted_text);

    // Liberar memoria
    free(ciphertext);
    free(loaded_ciphertext);
    free(decrypted_text);

    return 0;
}
