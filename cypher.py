def long_to_bytes(value):
    return value.to_bytes(8, byteorder='big')

def xor_cipher(plaintext, key):
    key_bytes = long_to_bytes(key)  # Llave convertida a 8 bytes
    ciphertext = bytearray(len(plaintext))
    for i in range(len(plaintext)):
        ciphertext[i] = plaintext[i] ^ key_bytes[i % 8]  # Usar el byte correspondiente de la llave
    return bytes(ciphertext)

def save_to_file(filename, data):
    with open(filename, 'wb') as f:
        f.write(data)

def load_from_file(filename):
    with open(filename, 'rb') as f:
        return f.read()

def main():
    # Texto a cifrar
    plaintext = "Esta es una prueba de proyecto 2".encode('utf-8')
    
    # Llave de ejemplo (123456L como entero largo)
    key = 35543242
    
    # Cifrar el texto
    ciphertext = xor_cipher(plaintext, key)
    save_to_file('cipher.bin', ciphertext)
    print(f"Texto cifrado guardado en 'cipher.bin'")

    # Cargar el texto cifrado para descifrarlo
    loaded_ciphertext = load_from_file('cipher.bin')
    
    # Descifrar con la misma llave
    decrypted_text = xor_cipher(loaded_ciphertext, key)
    print(f"Texto descifrado: {decrypted_text.decode('utf-8')}")

if __name__ == "__main__":
    main()
