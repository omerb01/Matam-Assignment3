cmake_minimum_required(VERSION 3.9)
project(Assignment3 C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Werror -pedantic")

link_directories(.)

add_executable(Assignment3 main.c
        GradesSheet.h
        Student.h Graph.h CourseManager.h GradesSheet.c
        Student.c Graph.c CourseManager.c UsefulFunctions.c
        UsefulFunctions.h list.h set.h
        mtm_ex3.h tests/test_utilities.h)

target_link_libraries(Assignment3 libmtm.a)