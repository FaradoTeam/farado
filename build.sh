#!/bin/sh

mkdir -p build

# Configure
CC=gcc-14 CXX=g++-14 cmake -B build \
    -DENABLE_TESTING=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=cmake/conan_provider.cmake

# Build
cmake --build build -j$(($(nproc) - 1))

# Test
ctest --test-dir build --output-on-failure
