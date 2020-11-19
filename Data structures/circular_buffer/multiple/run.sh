clear
mkdir -p build
gcc ./test/test.c ./lib/circular_buffer.c -I./lib -lunity -o ./build/test
./build/test 