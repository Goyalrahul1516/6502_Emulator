cmake_minimum_required(VERSION 3.15)
project(6502Test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set source files
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests_6502.cpp
)

# add an executable
add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

#link the library
target_link_libraries(${PROJECT_NAME} PRIVATE 6502Lib)
target_link_libraries(${PROJECT_NAME} PRIVATE gtest_main)

add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME})