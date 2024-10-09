#!/bin/bash
echo "A ejecutar? 1. secuencial, 2.paralelo MPI, 3. paralelo Openmp"
read input
echo "search term? "
read sterm
mkdir -p build
if [[ "$input" == "2" ]];
then
    echo "ejecutando paralelo a 4 procesos"
    mpicc -o ./build/bruteforce_mpi bruteforce_mpi.c
    cd build
    mpirun -np 4 ./bruteforce_mpi cipherC.bin  "$sterm"
    cd ..
fi

if [[ "$input" == "1" ]]; then
    echo "ejecutando secuencial"
    gcc -o ./build/bruteforce_sequential bruteforce_secuencial.c
    cd build
    ./bruteforce_sequential cipherC.bin "$sterm"
    cd ..
fi
if [[ "$input" == "3" ]]; then
    echo "ejecutando paralelo MPI y OpenMP"
    mpicc -fopenmp -o ./build/bruteforce_mpi_openmp brute_force_mpi_openmp.c
    cd build
    mpirun -np 4 ./bruteforce_mpi_openmp cipherC.bin  "$sterm"
    cd ..
fi

if [[ "$input" != "1" && "$input" != "2" && "$input" != "3" ]]; then
    echo "Opción inválida"
fi