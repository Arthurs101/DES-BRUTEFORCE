#!/bin/bash

text="Esto es una prueba de proyecto 2"
keyword="una prueba"
allkeys=(
123456
)
parallel_times="parallel_times.txt"  # Output file

cd "build"
touch $parallel_times

for key in ${allkeys[@]}; do
    echo "Parallel Running with key: $key" >> "$parallel_times" 
    ./xor_cipher "$text" $key
    mpirun -np 4 ./bruteforce_mpi cipherC.bin "$keyword" >> "$parallel_times"
    echo "" >> "$parallel_times"
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