clear && rm -rf build && mkdir -p build
gcc ./test/wl_unit/wl_unit_test.cpp ./src/wl.cpp  -I./test/wl_unit -I./include -lunity -o ./build/test && ./build/test 