cmake_minimum_required(VERSION 3.20)
project(SquareRingBot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Default to Release build if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Optional: Set minimum macOS version
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum macOS deployment version")
endif()

# Add submodules
add_subdirectory(external/json)

# Main Executable
add_executable(SquareRingBot
    Main.cpp
    Board.cpp
    Player.cpp
    CardLibrary.cpp
    CardFactory.cpp
    Tournament.cpp
)

target_link_libraries(SquareRingBot PRIVATE nlohmann_json::nlohmann_json)

# Target-specific compile options
target_compile_options(SquareRingBot PRIVATE 
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
    $<$<CONFIG:Debug>:-g -DDEBUG>
)

# Strip symbols in Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT WIN32)
    set_target_properties(SquareRingBot PROPERTIES LINK_FLAGS "-Wl,-s")
endif()

# Only build and enable tests if not cross-compiling for Windows
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include(CTest)
    include(GoogleTest)
    enable_testing()

    add_subdirectory(external/googletest)

    add_executable(test_SquareRing
        test_main.cpp
        Board.cpp
        Player.cpp
        CardLibrary.cpp
        CardFactory.cpp
        Tournament.cpp
    )

    target_link_libraries(test_SquareRing PRIVATE gtest gtest_main pthread nlohmann_json::nlohmann_json)

    add_test(NAME test_SquareRingBot COMMAND test_SquareRing)
    gtest_discover_tests(test_SquareRing)
endif()
