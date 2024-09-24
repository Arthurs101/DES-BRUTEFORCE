#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

// Convert hex to binary
string hex2bin(string s) {
    string bin = "";
    for (char c : s) {
        switch (toupper(c)) {
            case '0': bin += "0000"; break;
            case '1': bin += "0001"; break;
            case '2': bin += "0010"; break;
            case '3': bin += "0011"; break;
            case '4': bin += "0100"; break;
            case '5': bin += "0101"; break;
            case '6': bin += "0110"; break;
            case '7': bin += "0111"; break;
            case '8': bin += "1000"; break;
            case '9': bin += "1001"; break;
            case 'A': bin += "1010"; break;
            case 'B': bin += "1011"; break;
            case 'C': bin += "1100"; break;
            case 'D': bin += "1101"; break;
            case 'E': bin += "1110"; break;
            case 'F': bin += "1111"; break;
        }
    }
    return bin;
}

// Convert binary to hex
string bin2hex(string s) {
    string hex = "";
    for (size_t i = 0; i < s.size(); i += 4) {
        string ch = s.substr(i, 4);
        if (ch == "0000") hex += '0';
        else if (ch == "0001") hex += '1';
        else if (ch == "0010") hex += '2';
        else if (ch == "0011") hex += '3';
        else if (ch == "0100") hex += '4';
        else if (ch == "0101") hex += '5';
        else if (ch == "0110") hex += '6';
        else if (ch == "0111") hex += '7';
        else if (ch == "1000") hex += '8';
        else if (ch == "1001") hex += '9';
        else if (ch == "1010") hex += 'A';
        else if (ch == "1011") hex += 'B';
        else if (ch == "1100") hex += 'C';
        else if (ch == "1101") hex += 'D';
        else if (ch == "1110") hex += 'E';
        else if (ch == "1111") hex += 'F';
    }
    return hex;
}

// Convert binary to decimal
int bin2dec(string binary) {
    return stoi(binary, nullptr, 2);
}

// Convert decimal to binary
string dec2bin(int num) {
    string res = bitset<4>(num).to_string();
    return res;
}

// Permute function to rearrange the bits
string permute(string k, int* arr, int n) {
    string per = "";
    for (int i = 0; i < n; i++) {
        per += k[arr[i] - 1];
    }
    return per;
}

// Shift bits to the left
string shift_left(string k, int shifts) {
    string shifted = k;
    for (int i = 0; i < shifts; i++) {
        shifted = shifted.substr(1, shifted.size() - 1) + shifted[0];
    }
    return shifted;
}

// XOR operation between two binary strings
string xor_op(string a, string b) {
    string result = "";
    for (size_t i = 0; i < a.size(); i++) {
        result += (a[i] == b[i]) ? "0" : "1";
    }
    return result;
}

// Initial Permutation Table
int initial_perm[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

// Expansion D-box Table
int exp_d[48] = {
    32, 1, 2, 3, 4, 5, 4, 5,
    6, 7, 8, 9, 8, 9, 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27,
    28, 29, 28, 29, 30, 31, 32, 1
};

// Straight Permutation Table
int per[32] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25
};

// S-box Table
int sbox[8][4][16] = {
    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
     {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
     {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
     {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    // Add the remaining S-boxes similarly
};

// Final Permutation Table
int final_perm[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

// Key Permutation Tables
int keyp[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

int shift_table[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// Compression Table
int key_comp[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

// Function to perform encryption (same can be used for decryption)
string encrypt(string pt, vector<string> rkb, vector<string> rk) {
    pt = hex2bin(pt);

    // Initial Permutation
    pt = permute(pt, initial_perm, 64);
    cout << "After initial permutation: " << bin2hex(pt) << endl;

    // Split into left and right
    string left = pt.substr(0, 32);
    string right = pt.substr(32, 32);

    for (int i = 0; i < 16; i++) {
        // Expansion D-box
        string right_expanded = permute(right, exp_d, 48);

        // XOR with RoundKey[i]
        string xor_x = xor_op(right_expanded, rkb[i]);

        // S-box substitution and straight D-box
        string sbox_str = "";
        for (int j = 0; j < 8; j++) {
            int row = bin2dec(xor_x.substr(j * 6, 1) + xor_x[j * 6 + 5]);
            int col = bin2dec(xor_x.substr(j * 6 + 1, 4));
            int val = sbox[j][row][col];
            sbox_str += dec2bin(val);
        }

        // Straight D-box
        sbox_str = permute(sbox_str, per, 32);

        // XOR with left and swap
        string result = xor_op(left, sbox_str);
        left = result;

        if (i != 15) {
            swap(left, right);
        }
    }

    // Final combination
    string combine = left + right;

    // Final Permutation
    string cipher_text = permute(combine, final_perm, 64);
    return bin2hex(cipher_text);
}

// Key generation function
void key_generation(string key, vector<string>& rkb, vector<string>& rk) {
    // Convert key to binary
    key = hex2bin(key);

    // Permute key with parity bit drop
    key = permute(key, keyp, 56);

    // Split key into two halves
    string left = key.substr(0, 28);
    string right = key.substr(28, 28);

    for (int i = 0; i < 16; i++) {
        // Perform left shift
        left = shift_left(left, shift_table[i]);
        right = shift_left(right, shift_table[i]);

        // Combine left and right
        string combined = left + right;

        // Perform key compression
        string round_key = permute(combined, key_comp, 48);

        rkb.push_back(round_key); // Store binary key
        rk.push_back(bin2hex(round_key)); // Store hex key
    }
}

int main() {
    string pt = "123456ABCD132536";
    string key = "AABB09182736CCDD";

    vector<string> rkb; // Binary RoundKeys
    vector<string> rk;  // Hex RoundKeys

    // Key generation
    key_generation(key, rkb, rk);

    // Encryption
    string cipher_text = encrypt(pt, rkb, rk);
    cout << "Cipher Text: " << cipher_text << endl;

    // Decryption
    reverse(rkb.begin(), rkb.end());
    reverse(rk.begin(), rk.end());
    string plain_text = encrypt(cipher_text, rkb, rk);
    cout << "Plain Text: " << plain_text << endl;

    return 0;
}
