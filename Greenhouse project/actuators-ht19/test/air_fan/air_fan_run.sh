clear && mkdir -p ./test/air_fan/build && rm -rf ./test/air_fan/build/*
gcc ./lib/air_fan/air_fan_driver.cpp ./test/air_fan/air_fan_test.cpp -I./lib/air_fan -I./include -I./lib/bsp -lunity -o ./test/air_fan/build/test
./test/air_fan/build/test
