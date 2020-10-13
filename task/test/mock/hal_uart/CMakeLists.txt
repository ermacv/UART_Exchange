##############################################################################
cmake_minimum_required(VERSION 3.16)
project(hal_uart_mock C)
##############################################################################
set(lib_src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/hal_uart_mock.c
)

set(lib_inc
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${lib_src})
target_include_directories(${PROJECT_NAME} PUBLIC ${lib_inc})
target_link_libraries(${PROJECT_NAME} PRIVATE hal_uart)
##############################################################################
