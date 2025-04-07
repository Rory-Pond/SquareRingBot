cmake_minimum_required(VERSION 3.14)
project(SquareRingBot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable testing
include(CTest)
include(GoogleTest)

enable_testing()

# Add Google Test
add_subdirectory(external/googletest)

# Add executable
add_executable(SquareRingBot main.cpp Board.cpp Player.cpp  BestCurrent.cpp mcts.cpp)

# Create a test executable
add_executable(test_SquareRing test_main.cpp Board.cpp Player.cpp  BestCurrent.cpp mcts.cpp)

# Link Google Test and pthread
target_link_libraries(test_SquareRing gtest gtest_main pthread)

# Add the tests to CTest
add_test(NAME test_SquareRingBot COMMAND test_SquareRing)

# Enable debugging symbols in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode enabled")
    add_compile_definitions(DEBUG)
endif()

gtest_discover_tests(test_SquareRing)