##############################################################################
cmake_minimum_required(VERSION 3.16)
project(uart_task C)
##############################################################################
add_compile_options("-Wall")
add_compile_options("-Wextra")
add_compile_options("-Wpedantic")
add_compile_options("-Werror")
add_compile_options("-O3")
add_compile_options("-std=gnu99")
##############################################################################
add_subdirectory(hal/uart)
add_subdirectory(task)
##############################################################################
