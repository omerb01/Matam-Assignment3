//
// Created by Omer on 18/12/2017.
//

#ifndef ASSIGNMENT3_USEFULFUNCTIONS_H
#define ASSIGNMENT3_USEFULFUNCTIONS_H

#include "set.h"
#include "Student.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~ USEFUL MACROS ~~~~~~~~~~~~~~~~~~~~~~~~~~

#define MALLOC($$variable$$) \
            malloc(sizeof(*($$variable$$))); \
            if(($$variable$$) == NULL) {return NULL;}

// ~~~~~~~~~~~~~~~~~~~~~~~ VALIDATION FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~

bool isValidId(int id);

bool isValidAction(char *action);

bool isValidSemester(int semester);

bool isValidCourseId(int course_id);

bool isValidPointsX2(int points_x2);

#define MAX_VALID_GRADE 100
#define MIN_VALID_GRADE 0

bool isGradeValid(int grade);

bool isValidAmount(int amount);

bool isValidRequest(char *request);

#endif //ASSIGNMENT3_USEFULFUNCTIONS_H
