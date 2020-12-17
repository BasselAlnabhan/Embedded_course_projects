clear && rm -rf build && mkdir -p build
gcc ./test/wl_integration/wl_integration_test.cpp ./src/wl.cpp ./lib/wl_driver/wl_driver.cpp -I./test/wl_integration -I./lib/wl_driver -I./include -lunity -o ./build/test_app_integration && ./build/test_app_integration
