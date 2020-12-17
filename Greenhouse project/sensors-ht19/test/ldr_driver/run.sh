clear && rm -rf build && mkdir -p build
gcc ./test/ldr_driver/ldr_driver_test.cpp ./lib/ldr_driver/ldr_driver.cpp -I./test/ldr_driver -I./lib/ldr_driver -I./include -lunity -o ./build/test && ./build/test
# To run (in repo-root): sh ./test/ldr/run.sh