#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#include <openssl/evp.h>

void encrypt_aes(const unsigned char *input, const unsigned char *key, unsigned char *output) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL);
    
    int len;
    EVP_EncryptUpdate(ctx, output, &len, input, 16); // Input length must be 16 bytes
    EVP_EncryptFinal_ex(ctx, output + len, &len);
    
    EVP_CIPHER_CTX_free(ctx);
}

void decrypt_aes(const unsigned char *input, const unsigned char *key, unsigned char *output) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL);
    
    int len;
    EVP_DecryptUpdate(ctx, output, &len, input, 16); // Input length must be 16 bytes
    EVP_DecryptFinal_ex(ctx, output + len, &len);
    
    EVP_CIPHER_CTX_free(ctx);
}

int tryKey(unsigned char *key, const unsigned char *ciphertext) {
    unsigned char decrypted[16];
    decrypt_aes(ciphertext, key, decrypted);
    decrypted[15] = '\0'; // Null terminate for string comparison

    // Check if the decrypted text contains the substring " the "
    return strstr((char *)decrypted, " the ") != NULL;
}

int main(int argc, char *argv[]) {
    int N, id;
    unsigned char key[16]; // AES key size is 16 bytes for 128 bits
    unsigned char ciphertext[16] = { 108, 245, 65, 63, 125, 200, 150, 66, 17, 170, 207, 170,
34, 31, 70, 215}; // Corrected ciphertext (16 bytes)
    long upper = (1L << 56); // upper bound for keys (not realistic for AES, but for this example)
    long mylower, myupper;
    MPI_Comm comm = MPI_COMM_WORLD;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(comm, &N);
    MPI_Comm_rank(comm, &id);

    long range_per_node = upper / N;
    mylower = range_per_node * id;
    myupper = range_per_node * (id + 1) - 1;

    if (id == N - 1) {
        myupper = upper;
    }

    long found = -1;
    
    for (long i = mylower; i < myupper && found == -1; ++i) {
        // Convert i to a key
        for (int j = 0; j < 16; ++j) {
            key[j] = (i >> (8 * j)) & 0xFF;
        }

        if (tryKey(key, ciphertext)) {
            found = i;
            // Send the found key to all nodes
            for (int node = 0; node < N; node++) {
                MPI_Send(&found, 1, MPI_LONG, node, 0, comm);
            }
            break;
        }
    }

    if (id == 0) {
        // Wait for any node to find the key
        MPI_Status status;
        MPI_Recv(&found, 1, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &status);
        decrypt_aes(ciphertext, (unsigned char *)&found, ciphertext); // Decrypt with found key
        printf("Found key: %li, Decrypted text: %s\n", found, ciphertext);
    }

    MPI_Finalize();
    return 0;
}
