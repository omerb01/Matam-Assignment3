//
// Created by Omer on 15/12/2017.
//

#include "headers/Student.h"

struct Student_t {
    int id;
    char* first_name;
    char* last_name;
    GradesSheet grades_sheet;
    Set friends_requests_sent;
};

typedef struct RefrenceSource_t {
    int id;
    int course_id;
    int highest_grade;
} *RefrenceSource;