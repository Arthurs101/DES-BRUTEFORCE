# DES-BRUTEFORCE

## Para compilar bruteforceFix.c
'bash'
mpicc bruteforceFix.c -o bruteforceFix -lssl -lcrypto -Wno-deprecated-declarations

## Correr
mpirun -np 4 ./bruteforce         


## Para AES bruteforceAes.c
mpicc bruteforceAes.c -o bruteforceAes -lssl -lcrypto

## Correr
mpirun -np 4 ./bruteforceAes
