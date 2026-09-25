build target="Debug":
    cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE={{target}} -DCMAKE_CXX_COMPILER=g++
    cmake --build build

run port="6969":
    ./build/bin/tether --port {{port}}

dev port:
    just build
    just run {{port}}
