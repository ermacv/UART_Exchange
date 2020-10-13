##############################################################################
cmake_minimum_required(VERSION 3.16)
project(hal_uart C)
##############################################################################

set(lib_inc
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${lib_inc})
##############################################################################
