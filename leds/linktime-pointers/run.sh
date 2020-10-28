rm -rf build && clear && mkdir -p build && cd build
gcc ../test/blinky.cpp ../lib/blinky/blinky.cpp -I../test -I../lib/blinky -lunity -o blinky && ./blinky