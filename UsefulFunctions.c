//
// Created by Omer on 18/12/2017.
//

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "headers/UsefulFunctions.h"

SetElement studentCopyForSet(SetElement element) {
    /* DO NOTHING */
    return element;
}

void studentFreeForSet(SetElement element) {
    /* DO NOTHING */
}

int studentCompareForSet(SetElement element1, SetElement element2) {
    assert(element1 != NULL && element2 != NULL);
    Student student1 = (Student) element1;
    Student student2 = (Student) element2;

    return studentCompare(student1, student2);
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

bool isValidRequest(char *request) {
    if (strcmp(request, "cancel_course") == 0 ||
        strcmp(request, "register_course") == 0 ||
        strcmp(request, "remove_course") == 0) {
        return true;
    }
    return false;
}