clear && rm -rf ./build/* && mkdir -p ./build
gcc ./test/watering/watering_unit_test.cpp ./src/watering/watering.cpp ./lib/canbus/canbus.cpp ./lib/canbus/can_service.cpp -I./test/watering -I./src/watering -I./lib/canbus -I./include -I./lib/bsp -lunity -o ./build/test_watering && ./build/test_watering
# To run (in repo-root): sh ./test/watering/run.sh