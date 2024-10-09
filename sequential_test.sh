#!/bin/bash

text="Esto es una prueba de proyecto 2"
keyword="una prueba"
allkeys=(

123456

)
sec_times="sequential_times.txt"  # Output file

cd "build"
touch $sec_times

for key in ${allkeys[@]}; do
    echo "Secuential Running with key: $key" >> "$sec_times"
    
    # Generar el archivo cifrado con la llave actual
    ./xor_cipher "$text" $key

    # Ejecutar el brute force secuencial con el archivo cifrado
    ./bruteforce_sequential cipherC.bin "$keyword" >> "$sec_times"
    
    echo "" >> "$sec_times"
done

cd ..


# 12345678
# 87654321
# 11223344
# 98765432
# 31415926
# 27182818
# 11111111
# 22222222


