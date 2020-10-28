clear && mkdir -p build && rm -rf ./build/*
gcc ./lib/blinky/blinky.cpp ./test/blinky.cpp -I./lib/blinky -I./test -lunity -o ./build/test && ./build/test
