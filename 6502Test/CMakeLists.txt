cmake_minimum_required(VERSION 3.5)
project(6502Test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set source files
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main_6502.cpp
)

# add an executable
add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

#link the library
target_link_libraries(${PROJECT_NAME} PRIVATE 6502Lib)

# Link the Google test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/7d76a231b0e29caf86e68d1df858308cd53b2a66.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)