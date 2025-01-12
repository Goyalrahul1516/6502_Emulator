cmake_minimum_required(VERSION 3.5)
project(6502Emulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(6502Test)
add_subdirectory(6502Lib)

enable_testing()