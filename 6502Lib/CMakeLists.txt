# Method to add path to variable
set(dir "${CMAKE_CURRENT_SOURCE_DIR}/src")

list(
  APPEND srcs
  ${dir}
)

set(6502Lib_SRC_DIR ${srcs} PARENT_SCOPE)