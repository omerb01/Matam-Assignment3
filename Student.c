//
// Created by Omer on 15/12/2017.
//

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "set.h"
#include "list.h"
#include "mtm_ex3.h"
#include "GradesSheet.h"
#include "Student.h"
#include "UsefulFunctions.h"

struct Student_t {
    int id;
    char *first_name;
    char *last_name;
    GradesSheet grades_sheet;
    Set friends_requests_sent;
};

typedef struct RefrenceSource_t {
    Student student;
    int highest_last_grade;
} *RefrenceSource;

static StudentResult convertSetResultToStudentResult(SetResult set_error) {
    switch (set_error) {
        case SET_SUCCESS:
            return STUDENT_SUCCESS;
        case SET_OUT_OF_MEMORY:
            return STUDENT_OUT_OF_MEMORY;
        case SET_ITEM_ALREADY_EXISTS:
            return STUNDET_ALREADY_SENT_REQUEST;
        case SET_ITEM_DOES_NOT_EXIST:
            return STUDENT_DIDNT_SEND_REQUEST;
        default:
            assert(false);
    }
    return STUDENT_SUCCESS;
}

static StudentResult convertSheetResultToStudentResult(SheetResult
                                                       sheet_error) {
    switch (sheet_error) {
        case SHEET_SUCCESS:
            return STUDENT_SUCCESS;
        case SHEET_OUT_OF_MEMORY:
            return STUDENT_OUT_OF_MEMORY;
        case SHEET_INVALID_ARGUMENT:
            return STUDENT_INVALID_ARGUMENT;
        case SHEET_GRADE_DOES_NOT_EXIST:
            return STUDENT_GRADE_DOES_NOT_EXIST;
        default:
            assert(false);
    }
    return STUDENT_SUCCESS;
}

static char *strCopy(char *str) {
    int size = strlen(str);
    char *str_copy = malloc(sizeof(char) * size + 1);

    if (str_copy == NULL) return NULL;
    return strcpy(str_copy, str);
}

static int studentCompareForSet(SetElement element1, SetElement element2) {
    assert(element1 != NULL && element2 != NULL);
    Student student1 = (Student) element1;
    Student student2 = (Student) element2;

    return studentCompare(student1, student2);
}

Student studentCreate(int id, char *first_name, char *last_name) {
    assert(first_name != NULL && last_name != NULL);

    Student student = MALLOC(student);

    char *first_name_copy = strCopy(first_name);
    char *last_name_copy = strCopy(last_name);
    GradesSheet grades_sheet = sheetCreate();
    Set friends_requests_sent = setCreate(dontCopyElement, dontFreeElement,
                                          studentCompareForSet);
    if (first_name_copy == NULL || last_name_copy == NULL ||
        grades_sheet == NULL || friends_requests_sent == NULL) {
        free(first_name_copy);
        free(last_name_copy);
        sheetDestroy(grades_sheet);
        setDestroy(friends_requests_sent);
        free(student);
        return NULL;
    }

    student->id = id;
    student->first_name = first_name_copy;
    student->last_name = last_name_copy;
    student->grades_sheet = grades_sheet;
    student->friends_requests_sent = friends_requests_sent;

    return student;
}

Student studentCopy(Student student) {
    if (student == NULL) return NULL;

    Student student_copy = studentCreate(student->id, student->first_name,
                                         student->last_name);
    GradesSheet grades_sheet_copy = sheetCopy(student->grades_sheet);
    Set friends_requests_sent_copy = setCopy(student->friends_requests_sent);
    if (student_copy == NULL || grades_sheet_copy == NULL ||
        friends_requests_sent_copy == NULL) {
        studentDestroy(student_copy);
        sheetDestroy(grades_sheet_copy);
        setDestroy(friends_requests_sent_copy);
        return NULL;
    }

    sheetDestroy(student_copy->grades_sheet);
    setDestroy(student_copy->friends_requests_sent);

    student_copy->grades_sheet = grades_sheet_copy;
    student_copy->friends_requests_sent = friends_requests_sent_copy;

    return student_copy;
}

int studentCompare(Student student1, Student student2) {
    if (student1->id == student2->id) return 0;
    if (student1->id > student2->id) return 1;
    return -1;
}

StudentResult studentSendFriendReuqest(Student student, Student requested) {
    if (student == NULL || requested == NULL) return STUDENT_NULL_ARGUMENT;

    SetResult set_error = setAdd(student->friends_requests_sent, requested);
    return convertSetResultToStudentResult(set_error);
}

StudentResult
studentIsSentFriendRequest(Student student, Student requested, bool *result) {
    if (student == NULL || requested == NULL || result == NULL) {
        return STUDENT_NULL_ARGUMENT;
    }

    *result = setIsIn(student->friends_requests_sent, requested);
    return STUDENT_SUCCESS;
}

StudentResult studentRemoveSentFriendRequest(Student student, Student
requested) {
    if (student == NULL || requested == NULL) return STUDENT_NULL_ARGUMENT;

    SetResult set_error = setRemove(student->friends_requests_sent, requested);
    return convertSetResultToStudentResult(set_error);
}

StudentResult studentAddGrade(Student student, int semester, int course_id,
                              int points_x2, int grade_value) {
    if (student == NULL) return STUDENT_NULL_ARGUMENT;

    SheetResult sheet_error = sheetAddGrade(student->grades_sheet, semester,
                                            course_id, points_x2, grade_value);
    return convertSheetResultToStudentResult(sheet_error);
}

StudentResult studentUpdateLastGrade(Student student, int course_id,
                                     int new_grade) {
    if (student == NULL) return STUDENT_NULL_ARGUMENT;

    SheetResult sheet_error = sheetUpdateLastGrade(student->grades_sheet,
                                                   course_id, new_grade);
    return convertSheetResultToStudentResult(sheet_error);
}

StudentResult studentRemoveLastGrade(Student student, int semester,
                                     int course_id) {
    if (student == NULL) return STUDENT_NULL_ARGUMENT;

    SheetResult sheet_error = sheetRemoveLastGrade(student->grades_sheet,
                                                   semester, course_id);
    return convertSheetResultToStudentResult(sheet_error);
}

StudentResult studentIsCourseDone(Student student, int course_id,
                                  bool *result) {
    if (student == NULL || result == NULL) return STUDENT_NULL_ARGUMENT;

    int highest_grade;
    SheetResult sheet_error = sheetHighestGrade(student->grades_sheet,
                                                course_id,
                                                &highest_grade);
    if (sheet_error == SHEET_INVALID_ARGUMENT) {
        return STUDENT_INVALID_ARGUMENT;
    }
    if (sheet_error == SHEET_GRADE_DOES_NOT_EXIST) {
        *result = false;
        return STUDENT_SUCCESS;
    }
    assert(sheet_error == SHEET_SUCCESS);
    *result = true;
    return STUDENT_SUCCESS;
}

StudentResult studentPrintFullSheet(FILE *output_channel, Student student) {
    if (output_channel == NULL || student == NULL) return STUDENT_NULL_ARGUMENT;

    mtmPrintStudentInfo(output_channel, student->id, student->first_name,
                        student->last_name);

    SheetResult sheet_error = sheetPrintFull(output_channel,
                                             student->grades_sheet);
    return convertSheetResultToStudentResult(sheet_error);
}

StudentResult studentPrintCleanSheet(FILE *output_channel, Student student) {
    if (output_channel == NULL || student == NULL) return STUDENT_NULL_ARGUMENT;

    mtmPrintStudentInfo(output_channel, student->id, student->first_name,
                        student->last_name);

    SheetResult sheet_error = sheetPrintClean(output_channel,
                                              student->grades_sheet);
    return convertSheetResultToStudentResult(sheet_error);
}

StudentResult studentPrintHighestGrades(FILE *output_channel, Student student,
                                        int amount) {
    if (output_channel == NULL || student == NULL) return STUDENT_NULL_ARGUMENT;

    SheetResult sheet_error = sheetPrintHighestGrades(output_channel,
                                                      student->grades_sheet,
                                                      amount);
    return convertSheetResultToStudentResult(sheet_error);
}

StudentResult studentPrintLowestGrades(FILE *output_channel, Student student,
                                       int amount) {
    if (output_channel == NULL || student == NULL) return STUDENT_NULL_ARGUMENT;

    SheetResult sheet_error = sheetPrintLowestGrades(output_channel,
                                                     student->grades_sheet,
                                                     amount);
    return convertSheetResultToStudentResult(sheet_error);
}

// ~~~~~~~~~~~~~~~~~ REFRENCE SOURCES STATIC FUNCTIONS ~~~~~~~~~~~~~~~~~

static RefrenceSource refrenceCreate(Student student, int highest_last_grade) {
    assert(student != NULL);

    RefrenceSource refrence_source = MALLOC(refrence_source);
    refrence_source->student = student;
    refrence_source->highest_last_grade = highest_last_grade;

    return refrence_source;
}

static ListElement refrenceCopy(ListElement element) {
    /* DO NOTHING */
    return element;
}

static void refrenceFree(ListElement element) {
    if (element == NULL) return;
    free(element);
}

static int refrenceCompare(ListElement element1, ListElement element2,
                           ListSortKey key) {
    assert(element1 != NULL && element2 != NULL);
    RefrenceSource refrence_source1 = (RefrenceSource) element1;
    RefrenceSource refrence_source2 = (RefrenceSource) element2;

    if (refrence_source1->highest_last_grade >
        refrence_source2->highest_last_grade) {
        return -1;
    }
    if (refrence_source1->highest_last_grade <
        refrence_source2->highest_last_grade) {
        return 1;
    }

    assert(refrence_source1->highest_last_grade ==
           refrence_source2->highest_last_grade);

    if (studentCompare(refrence_source1->student, refrence_source2->student) >
        0) {
        return 1;
    }
    if (studentCompare(refrence_source1->student, refrence_source2->student) <
        0) {
        return -1;
    }
    return 0;
}

#define NONE_GRADE (MIN_VALID_GRADE - 1)

static List buildRefrencesList(Set friends, int course_id) {
    SheetResult sheet_error;
    ListResult list_error;
    RefrenceSource refrence_source;
    int highest_grade;
    List refrences = listCreate(refrenceCopy, refrenceFree);
    if (refrences == NULL) return NULL;
    SET_FOREACH(Student, iterator, friends) {
        sheet_error = sheetHighestGrade(iterator->grades_sheet, course_id,
                                        &highest_grade);
        if (sheet_error == SHEET_GRADE_DOES_NOT_EXIST) {
            highest_grade = NONE_GRADE;
        } else if (sheet_error == SHEET_OUT_OF_MEMORY) {
            listDestroy(refrences);
            return NULL;
        } else {
            assert(sheet_error == SHEET_SUCCESS);
        }
        refrence_source = refrenceCreate(iterator, highest_grade);
        if (refrence_source == NULL) {
            listDestroy(refrences);
            return NULL;
        }
        list_error = listInsertLast(refrences, refrence_source);
        if (list_error == LIST_OUT_OF_MEMORY) {
            listDestroy(refrences);
            refrenceFree(refrence_source);
            return NULL;
        }
        assert(list_error == LIST_SUCCESS);
    }
    return refrences;
}

// ~~~~~~~~~~~~~~ END OF - REFRENCE SOURCES STATIC FUNCTIONS ~~~~~~~~~~~~~~

StudentResult
studentPrintReferences(FILE *output_channel, Set friends,
                       int course_id, int amount) {
    if (output_channel == NULL || friends == NULL) {
        return STUDENT_NULL_ARGUMENT;
    }

    if (!isValidAmount(amount)) return STUDENT_INVALID_ARGUMENT;
    if (!isValidCourseId(course_id)) return STUDENT_SUCCESS;

    List refrences = buildRefrencesList(friends, course_id);
    if (refrences == NULL) return STUDENT_OUT_OF_MEMORY;
    ListResult list_error = listSort(refrences, refrenceCompare, NULL);
    if (list_error == LIST_OUT_OF_MEMORY) {
        listDestroy(refrences);
        return STUDENT_OUT_OF_MEMORY;
    }
    assert(list_error == LIST_SUCCESS);

    LIST_FOREACH(RefrenceSource, iterator, refrences) {
        if (amount <= 0) break;
        if (iterator->highest_last_grade != NONE_GRADE) {
            mtmPrintStudentName(output_channel, iterator->student->first_name,
                                iterator->student->last_name);
        }
        amount--;
    }

    listDestroy(refrences);
    return STUDENT_SUCCESS;
}

void studentDestroy(Student student) {
    if (student == NULL) return;

    free(student->first_name);
    free(student->last_name);
    sheetDestroy(student->grades_sheet);
    setDestroy(student->friends_requests_sent);
    free(student);
}