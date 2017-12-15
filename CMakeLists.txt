cmake_minimum_required(VERSION 3.9)
project(Assignment3 C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Werror -pedantic")

link_directories(headers)

add_executable(Assignment3 main.c GradesSheet.c Student.c Graph.c CourseManager.c tests/GradesSheet_test.c tests/Student_test.c tests/Graph_test.c tests/CourseManager_test.c)

target_link_libraries (Assignment3 libmtm.a)