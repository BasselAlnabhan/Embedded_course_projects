clear && rm -rf build && mkdir -p build

# gcc for unit test
gcc ./test/ldr_unit/ldr_unit_test.cpp ./src/ldr.cpp -I./test/ldr_unit -I./include -lunity -o ./build/test_app_unit && ./build/test_app_unit

# To run (in repo-root): sh ./test/ldr_app/run.sh