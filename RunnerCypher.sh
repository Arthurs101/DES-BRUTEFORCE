mkdir -p build
gcc -o ./build/xor_cipher cipher.c
echo "text to cypher?"
read text
echo "key to use?"
read key
cd build
./xor_cipher "$text" $key
cd ..