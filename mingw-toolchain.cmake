# mingw-toolchain.cmake
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Specify the compiler
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Static linking is often better for portability
set(CMAKE_FIND_LIBRARY_SUFFIXES .a .lib)

# Prevent CMake from using macOS paths
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
set(CMAKE_CXX_STANDARD_LIBRARIES "")

# Avoid looking for incompatible libraries
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
