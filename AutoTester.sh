text="Esto es una prueba de proyecto 2"
keyword="una prueba"
allkeys=(1528012 23451232 36028797018963969)
parallel_times="parallel_times.txt"  # Output file
sec_times="secuential_times.txt"
cd "build"
touch $parallel_times
touch $sec_times
for key in ${allkeys[@]}; do
    echo "Parallel Running with key: $key" >> "$parallel_times" 
    ./xor_cipher "$text" $key
    mpirun -np 4 ./bruteforce_mpi cipherC.bin  "$keyword" >> "$parallel_times"
    echo "" >> "$parallel_times"
done

for key in ${allkeys[@]}; do
    echo "Secuential Running with key: $key" >> "$sec_times" 
    ./xor_cipher "$text" $key
    ./bruteforce_sequential cipherC.bin "$keyword" >> "$sec_times"
    echo "" >> "$sec_times"
done
cd ..