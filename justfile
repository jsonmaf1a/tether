build target="Debug":
    cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE={{target}} -DCMAKE_CXX_COMPILER=g++
    cmake --build build

run:
    ./build/bin/tether

dev:
    just build
    just run

tidy:
    clang-tidy src/**/*.cpp -p build/
