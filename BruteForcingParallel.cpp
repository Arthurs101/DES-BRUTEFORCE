#include <openssl/des.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <omp.h>

// Function to convert hex string to byte array
std::vector<unsigned char> hex_to_bytes(const std::string &hex) {
    std::vector<unsigned char> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(strtol(byteString.c_str(), nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

// Try decrypting with the current key and check if the decrypted message contains the known plaintext
bool try_key(DES_cblock &key, unsigned char *cipher, int len, const std::string &known_plaintext) {
    // Set up DES variables
    DES_key_schedule schedule;
    DES_set_key_unchecked(&key, &schedule);

    // Decrypt the ciphertext
    unsigned char decrypted[16];
    memcpy(decrypted, cipher, len);

    DES_ecb_encrypt((DES_cblock *)decrypted, (DES_cblock *)decrypted, &schedule, DES_DECRYPT);

    // Check if the known plaintext exists in the decrypted text
    std::string decrypted_str(reinterpret_cast<char*>(decrypted), len);
    return decrypted_str.find(known_plaintext) != std::string::npos;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <hex_cipher_text> <known_plain_text>\n";
        return 1;
    }

    // Read input parameters
    std::string cipher_hex = argv[1];
    std::string known_plaintext = argv[2];

    // Convert the cipher text from hex string to byte array
    std::vector<unsigned char> cipher = hex_to_bytes(cipher_hex);
    int cipher_len = cipher.size();

    //DES uses 56-bit keys, meaning 2^56 possible keys)
    long max_key = (1L << 56);  
    DES_cblock key;

    long found_key = -1;

    // Parallelize using OpenMP
    #pragma omp  num_treads(12) parallel for shared(found_key) private(key)
    for (long i = 0; i < max_key; ++i) {
        if (found_key != -1) {
            continue; // Skip further computation if key is already found
        }

        // Generate key
        for (int j = 0; j < 8; ++j) {
            key[j] = (i >> (j * 8)) & 0xFF;
        }

        // Try the current key
        if (try_key(key, cipher.data(), cipher_len, known_plaintext)) {
            #pragma omp critical
            {
                found_key = i;
                std::cout << "Key found by thread " << omp_get_thread_num() << ": ";
                for (int j = 0; j < 8; ++j) {
                    std::cout << std::hex << (int)key[j] << " ";
                }
                std::cout << std::endl;

                // Print decrypted message
                unsigned char decrypted[16];
                DES_key_schedule schedule;
                DES_set_key_unchecked(&key, &schedule);
                DES_ecb_encrypt((DES_cblock *)cipher.data(), (DES_cblock *)decrypted, &schedule, DES_DECRYPT);
                std::cout << "Decrypted message: " << decrypted << std::endl;
            }
        }
    }

    if (found_key == -1) {
        std::cout << "Key not found in the given range." << std::endl;
    }

    return 0;
}
