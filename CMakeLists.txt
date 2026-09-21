cmake_minimum_required(VERSION 3.28)
project(Tether LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

execute_process(
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json
)

file(GLOB_RECURSE TETHER_SOURCES src/*.cpp)
add_executable(tether ${TETHER_SOURCES})

target_compile_features(tether PRIVATE cxx_std_23)
