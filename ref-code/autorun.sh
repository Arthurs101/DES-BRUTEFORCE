#!bin/bash
mpicc bruteforce.c -o bruteforce -lssl -lcrypto
echo "running with 4 process"
mpirun -np 4 ./bruteforce
