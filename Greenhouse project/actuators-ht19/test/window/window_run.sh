clear && mkdir -p ./test/window/build && rm -rf ./test/window/build/*
gcc ./lib/window/window_driver.cpp ./test/window/window_test.cpp -I./test/window -I./lib/bsp -I./lib/window -I./include -lunity -o ./test/window/build/test
./test/window/build/test