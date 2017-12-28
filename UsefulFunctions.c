//
// Created by Omer on 18/12/2017.
//

#include <stdbool.h>
#include <string.h>
#include "headers/UsefulFunctions.h"

SetElement dontCopyElement(SetElement element) {
    /* DO NOTHING */
    return element;
}

void dontFreeElement(SetElement element) {
    /* DO NOTHING */
}

bool isValidId(int id) {
    if (id > 0 && id < 1000000000) return true;
    return false;
}

bool isValidAction(char *action) {
    if (strcmp(action, "reject") == 0 || strcmp(action, "accept") == 0) {
        return true;
    }
    return false;
}

bool isValidSemester(int semester) {
    if (semester > 0) return true;
    return false;
}

bool isValidCourseId(int course_id) {
    if (course_id > 0 && course_id < 1000000) return true;
    return false;
}

bool isValidPointsX2(int points_x2) {
    if (points_x2 >= 0) return true;
    return false;
}

bool isGradeValid(int grade) {
    if (grade >= MIN_VALID_GRADE && grade <= MAX_VALID_GRADE) return true;
    return false;
}

bool isValidAmount(int amount) {
    if (amount > 0) return true;
    return false;
}

bool isSportCourse(int course_id){
    if(course_id <= 399999 && course_id >= 390000){
        return true;
    }
    return false;
}

bool isGradeFail(int grade_value){
    if(grade_value >= 55){
        return false;
    }
    else{
        return true;
    }
}

bool isValidRequest(char *request) {
    if (strcmp(request, "cancel_course") == 0 ||
        strcmp(request, "register_course") == 0 ||
        strcmp(request, "remove_course") == 0) {
        return true;
    }
    return false;
}