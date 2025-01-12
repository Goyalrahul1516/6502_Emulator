# Minimum version of CMake required
cmake_minimum_required(VERSION 3.15)

# Project name
project(6502Emulator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the subdirectories
add_subdirectory(6502Test)
add_subdirectory(6502Lib)

# Define the executable
add_executable(${PROJECT_NAME}
    ${6502_Sources}   # Source file for the executable
)

# Include the header files
target_include_directories(${PROJECT_NAME} PUBLIC ${6502Lib_SRC_DIR})