#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

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

// Load data from a file
char* load_from_file(const char *filename, int *len) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
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
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Synchronize all processes
    // MPI_Barrier(MPI_COMM_WORLD);

    if (argc != 3) {
        if (rank == 0) {
            printf("Usage: %s <cipher file> <search term>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;  // Exit if the arguments are not correct
    }

    if (rank == 0) {
        printf("size to use: %d \n", size);
    }
    
    // Load the ciphertext
    int loaded_len;
    char *loaded_ciphertext = load_from_file(argv[1], &loaded_len);
    if (loaded_ciphertext == NULL) {
        MPI_Finalize();
        return 1;  // Exit if loading the file failed
    }

    // Variables for brute-force search
    long max_key = (1L << 56); // Maximum key value for brute-forcing
    long keys_per_process = max_key / size;
    long start_key = rank * keys_per_process;
    long end_key = (rank + 1) * keys_per_process;
    char *decrypted_text = (char *)malloc(loaded_len + 1); // +1 for null terminator
    

    unsigned long long found_key = 0;
    int key_found_flag,queue_message = 0;
    double start_time = MPI_Wtime();  // Start timing
    // Brute-force key search
    for (unsigned long long key = start_key; key < end_key; key++) {
        // Check if a key was found previously
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &queue_message, MPI_STATUS_IGNORE);
        if(queue_message){
            MPI_Recv(&key_found_flag, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (key_found_flag) {
                break; // Exit if key was found by another process
            }
        }
        xor_cipher(loaded_ciphertext, key, decrypted_text, loaded_len);
        decrypted_text[loaded_len] = '\0'; // Ensure null termination

        if (strstr(decrypted_text, argv[2]) != NULL) { // Use the provided search term
            found_key = key; // Store the found key
            key_found_flag = 1;  // Set the flag indicating a key was found
            printf("Process %d found a matching key: %llu\n", rank, found_key);
            // Broadcast key_found_flag to all processes
            for (int j = 0; j < size; ++j) {
                MPI_Send(&key_found_flag, 1, MPI_INT, j, 0, MPI_COMM_WORLD); // Inform all processes
            }
            break; // Exit the loop after finding the key
        }
    }
    // Broadcast the found key to all processes
    double end_time = MPI_Wtime();  // End timing
    double elapsed_time = end_time - start_time;

    // Only the root process prints the final results
    if (found_key != 0) { // Check if any process found a key
        xor_cipher(loaded_ciphertext, found_key, decrypted_text, loaded_len);
        decrypted_text[loaded_len] = '\0'; // Ensure null termination
        printf("Found the key: %llu\n", found_key);
        printf("Decrypted text: %s\n", decrypted_text);
        printf("Total elapsed time: %.6f seconds\n", elapsed_time);
    } 
    

    free(loaded_ciphertext);
    free(decrypted_text);

    MPI_Finalize();
    return 0;  // Successful exit
}
