cmake_minimum_required(VERSION 3.10)
project(SquareRingBot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(SquareRingBot main.cpp)

# Enable debugging symbols in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode enabled")
    add_compile_definitions(DEBUG)
endif()
