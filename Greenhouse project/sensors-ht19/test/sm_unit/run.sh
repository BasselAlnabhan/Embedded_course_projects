clear && rm -rf build && mkdir -p build
gcc ./test/sm_unit/sm_unit_test.cpp ./src/sm.cpp  -I./test/sm_unit -I./include -lunity -o ./build/test && ./build/test