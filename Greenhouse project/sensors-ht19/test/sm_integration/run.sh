clear && rm -rf build && mkdir -p build
gcc ./test/sm_integration/sm_integration_test.cpp ./src/sm.cpp ./lib/sm_driver/sm_driver.cpp -I./test/sm_integration -I./lib/sm_driver -I./include -I./lib/canbus -lunity -o ./build/test_app && ./build/test_app
