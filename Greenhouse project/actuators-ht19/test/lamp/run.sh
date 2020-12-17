clear && mkdir -p build && rm -rf ./build/*
gcc ./lib/lamp/lamp.cpp ./test/lamp/lamp.cpp -I./lib/lamp -I./include -I test -I./lib/bsp -lunity -o ./build/test && ./build/test