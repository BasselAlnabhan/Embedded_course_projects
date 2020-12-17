clear && mkdir -p build && rm -rf ./build/*
gcc ./lib/heater/heater.cpp ./test/heater/test.cpp -I./lib/heater -I./include -I./test/heater -lunity -o ./build/test && ./build/test