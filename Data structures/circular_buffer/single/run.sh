clear
mkdir -p build
rm -rf ./build/*
gcc ./lib/circular_buffer.c ./test/test.c -I./lib -lunity -lgcov -o ./build/test
./build/test