cmake_minimum_required(VERSION 3.9)
project(Assignment3 C)

set(CMAKE_C_STANDARD 99)

add_executable(Assignment3 list_mtm.c node_mtm.c main.c)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Werror -pedantic")