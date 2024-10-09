text="Esto es una prueba de proyecto 2"
keyword="una prueba"
allkeys=(1528012 23451232 36028797018963969 45035996273704960 15836833854489655)
parallel_times="parallel_times.txt"  # Output file
cd "build"
touch $parallel_times
for key in ${allkeys[@]}; do
    echo "Running with key: $key" >> "$parallel_times" 
    ./xor_cipher "$text" $key
    mpirun -np 4 ./bruteforce_mpi cipherC.bin  "$keyword" >> "$parallel_times"
    echo "" >> "$parallel_times"
done
cd ..