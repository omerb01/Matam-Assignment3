//
// Created by Omer on 15/12/2017.
//

#include "headers/GradesSheet.h"

struct GradesSheet_t {
    List sheet;
};

typedef struct Grade_t {
    int semester;
    int course_id;
    int points_x2;
    int grade_value;
} *Grade;

