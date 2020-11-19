clear
mkdir -p build
gcc ./test/test.c ./lib/greeting.c -I./lib -lunity -o ./build/test
./build/test
