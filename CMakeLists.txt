cmake_minimum_required(VERSION 3.20)
project(SquareRingBot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable testing
include(CTest)
include(GoogleTest)

enable_testing()

# Add submodules
add_subdirectory(external/json)
add_subdirectory(external/googletest)

# Main Executable
add_executable(SquareRingBot
    main.cpp
    Board.cpp
    Player.cpp
    BestCurrent.cpp
    mcts.cpp
    CardLibrary.cpp
    CardFactory.cpp
)

# Test Executable
add_executable(test_SquareRing
    test_main.cpp
    Board.cpp
    Player.cpp
    BestCurrent.cpp
    mcts.cpp
)
target_link_libraries(SquareRingBot PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(test_SquareRing PRIVATE gtest gtest_main pthread nlohmann_json::nlohmann_json)

# Add the tests to CTest
add_test(NAME test_SquareRingBot COMMAND test_SquareRing)

# Enable debugging symbols in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode enabled")
    add_compile_definitions(DEBUG)
endif()

gtest_discover_tests(test_SquareRing)
