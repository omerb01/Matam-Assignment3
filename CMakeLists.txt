cmake_minimum_required(VERSION 3.9)
project(Assignment3 C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Werror -pedantic")

link_directories(headers)

add_executable(mtm_cm headers/GradesSheet.h headers/Graph.h headers/mtm_ex3.h
        headers/Student.h headers/CourseManager.h headers/UsefulFunctions.h headers/CourseManager.h
        headers/set.h UsefulFunctions.c GradesSheet.c Student.c CourseManager.c Graph.c main.c)

target_link_libraries(mtm_cm libmtm.a)