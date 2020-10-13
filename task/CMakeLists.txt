##############################################################################
cmake_minimum_required(VERSION 3.16)
project(uart_exchange C)
##############################################################################
set(lib_src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/uart_exchange.c
)

set(lib_inc
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

set(lib_conf
  ${CMAKE_CURRENT_SOURCE_DIR}/conf
)

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${lib_src})
target_include_directories(${PROJECT_NAME} PUBLIC ${lib_inc})
target_include_directories(${PROJECT_NAME} PRIVATE ${lib_conf})
target_link_libraries(${PROJECT_NAME} PRIVATE hal_uart)
##############################################################################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/mock/hal_uart)

set(lib_test_src
  ${CMAKE_CURRENT_SOURCE_DIR}/test/uart_exchange_test.c
)

add_executable(${PROJECT_NAME}_test EXCLUDE_FROM_ALL)
target_sources(${PROJECT_NAME}_test PRIVATE ${lib_test_src})
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_test PRIVATE hal_uart)
target_link_libraries(${PROJECT_NAME}_test PRIVATE hal_uart_mock)
target_link_libraries(${PROJECT_NAME}_test PRIVATE pthread)
##############################################################################
