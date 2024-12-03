cmake_minimum_required(VERSION 3.14)

project( 6502 )

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory("6502/6502Lib")
add_subdirectory("6502/6502Test")