//
// Created by Omer on 18/12/2017.
//

#ifndef ASSIGNMENT3_USEFULFUNCTIONS_H
#define ASSIGNMENT3_USEFULFUNCTIONS_H

#include "set.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~ USEFUL MACROS ~~~~~~~~~~~~~~~~~~~~~~~~~~

#define MALLOC($$variable$$) \
            malloc(sizeof(*($$variable$$))); \
            if(($$variable$$) == NULL) {return NULL;}

// ~~~~~~~~~~~~~~~~~~~~~ USER INTERFACE FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~

SetElement dontCopyElement(SetElement);

void dontFreeElement(SetElement);

// ~~~~~~~~~~~~~~~~~~~~~~~ VALIDATION FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~

#define INVALID_ID -1

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

bool isGradeFail(int grade_value);

bool isSportCourse(int course_id);
#endif //ASSIGNMENT3_USEFULFUNCTIONS_H
