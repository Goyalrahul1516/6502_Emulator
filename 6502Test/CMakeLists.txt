# Method to source files to variable
set(dir "${CMAKE_CURRENT_SOURCE_DIR}/src")

list(
  APPEND srcs
  ${dir}/main_6502.cpp
)

set(6502_Sources ${srcs} PARENT_SCOPE)