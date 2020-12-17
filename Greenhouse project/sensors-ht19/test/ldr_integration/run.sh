clear && rm -rf build && mkdir -p build

# gcc for integration test
gcc ./test/ldr_integration/ldr_integration_test.cpp ./src/ldr.cpp ./lib/ldr_driver/ldr_driver.cpp -I./test/ldr_integration -I./lib/ldr_driver -I./include -I./lib/canbus -lunity -o ./build/test_app_integration && ./build/test_app_integration

# To run (in repo-root): sh ./test/ldr_app/run.sh