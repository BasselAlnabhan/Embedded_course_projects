clear && rm -rf build && mkdir -p build
gcc ./test/wl_driver/wl_driver_test.cpp ./lib/wl_driver/wl_driver.cpp -I./lib/wl_driver -I./test/wl_driver -I./lib/bsp -I./include -lunity -o ./build/test
./build/test