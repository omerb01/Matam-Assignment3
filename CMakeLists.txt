cmake_minimum_required(VERSION 3.9)
project(Assignment3 C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Werror -pedantic")

link_directories(headers)

add_executable(Assignment3 main.c headers/GradesSheet.h
        headers/Student.h headers/Graph.h headers/CourseManager.h GradesSheet.c
        Student.c Graph.c CourseManager.c UsefulFunctions.c
        headers/UsefulFunctions.h headers/list.h headers/set.h
        headers/mtm_ex3.h headers/test_utilities.h)

target_link_libraries(Assignment3 libmtm.a)