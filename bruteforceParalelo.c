#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

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
    MPI_Init(&argc, &argv);  // Inicializar MPI

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obtener el rango del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obtener el número total de procesos

    if (argc != 4) {  // Cambiar a 4 argumentos requeridos
        if (rank == 0) {
            printf("Usage: %s <cipher file> <search term> <max key>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    // Convertir el argumento de max key a unsigned long long
    unsigned long long max_key = strtoull(argv[3], NULL, 10);

    // Cargar el archivo cifrado
    FILE *cipherfile = fopen(argv[1], "rb");
    if (cipherfile == NULL) {
        printf("Error opening cipher file.\n");
        MPI_Finalize();
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

    // Determinar el rango de llaves para cada proceso
    unsigned long long keys_per_process = max_key / size;
    unsigned long long start_key = rank * keys_per_process;
    unsigned long long end_key = (rank + 1) * keys_per_process;

    clock_t start_time = clock();

    unsigned long long found_key = 0;  // Para almacenar la clave encontrada (si la hay)
    for (unsigned long long i = start_key; i < end_key; i++) {
        if (try_key(i, ciphertext, plaintext, len, search_term)) {
            found_key = i;  // Almacenar la clave encontrada
            break;  // Salir al encontrar la clave
        }
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Enviar la clave encontrada a todos los procesos
    MPI_Bcast(&found_key, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    // Si la clave fue encontrada por este proceso, imprimir el resultado
    if (found_key != 0) {
        printf("Proceso %d encontró la clave: %llu\n", rank, found_key);
        printf("Mensaje descifrado: %s\n", plaintext);
    }

    printf("Tiempo transcurrido en proceso %d: %.2f segundos\n", rank, elapsed_time);

    free(ciphertext);
    free(plaintext);
    MPI_Finalize();  // Finalizar MPI
    return 0;
}
