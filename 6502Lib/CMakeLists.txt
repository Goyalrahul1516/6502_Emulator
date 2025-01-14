cmake_minimum_required(VERSION 3.15)
project(6502Lib)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the source files
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/m6502.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/testclass.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/execute.h
)

# Create the library
add_library(${PROJECT_NAME} INTERFACE ${SOURCE_FILES})

# Create the directory
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)