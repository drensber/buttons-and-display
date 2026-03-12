#!/bin/sh

# Create a build folder for tests
rm -rf build_tests
mkdir build_tests
cd build_tests

# Configure CMake with the test flag
cmake -DBUILD_UNIT_TESTS=ON ../ports/host_test

# Compile it
make

# Run your test simulator!
./unit_test_app
