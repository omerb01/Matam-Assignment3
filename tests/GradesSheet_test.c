//
// Created by Omer on 15/12/2017.
//

#include <assert.h>
#include <stdlib.h>
#include "../headers/test_utilities.h"
#include "../headers/GradesSheet.h"
#include <stdio.h>


typedef struct Grade_t {
    int semester;
    int course_id;
    int points_x2;
    int grade_value;
} *Grade;

/*static Grade gradeCopy(Grade grade) {
    if (grade == NULL) return NULL;
    Grade new_grade = malloc(sizeof(*new_grade));
    //TODO: check if previous grade needs to be freed
    if (new_grade == NULL) return NULL;;
    new_grade->points_x2 = grade->points_x2;
    new_grade->grade_value = grade->grade_value;
    new_grade->semester = grade->semester;
    new_grade->course_id = grade->course_id;
    return new_grade;
}

static void gradeFree(Grade grade) {
    free(grade);
}*/

static bool testGradesSheetCreate() {
    GradesSheet sheet = sheetCreate();
    ASSERT_TEST(sheet != NULL);

    sheetDestroy(sheet);
    return true;
}

static bool testGradesSheetCopy() {
    GradesSheet gsheet = sheetCreate();
    sheetAddGrade(gsheet, 1, 2, 3, 4);
    sheetAddGrade(gsheet, 5, 6, 7, 8);
    sheetAddGrade(gsheet, 9, 10, 11, 12);
    sheetAddGrade(gsheet, 1, 5, 6, 7);
    sheetAddGrade(gsheet, 1, 8, 9, 10);
    sheetAddGrade(gsheet, 5, 5, 5, 5);
    GradesSheet copied_sheet = sheetCopy(gsheet);
    ASSERT_TEST(copied_sheet != NULL);
    sheetDestroy(gsheet);
    sheetDestroy(copied_sheet);
    return true;
}

static bool testGradesSheetAddGrade() {
    SheetResult errorcode;
    GradesSheet gsheet = sheetCreate();
    sheetAddGrade(gsheet, 1, 2, 3, 4);
    ASSERT_TEST(gsheet != NULL);
    sheetAddGrade(gsheet, 1, 2, 3, 4);
    sheetAddGrade(gsheet, 12, 13, 15, 16);
    sheetAddGrade(gsheet, 12, 69, 32, 56);
    sheetAddGrade(gsheet, 12, 2, 3, 4);
    errorcode = sheetAddGrade(gsheet, -1, 2, 3, 4);
    ASSERT_TEST(errorcode == SHEET_INVALID_ARGUMENT);
    errorcode = sheetAddGrade(gsheet, 1, 1000001, 3, 4);
    ASSERT_TEST(errorcode == SHEET_INVALID_ARGUMENT);
    errorcode = sheetAddGrade(gsheet, 1, 2, 3, 105);
    ASSERT_TEST(errorcode == SHEET_INVALID_ARGUMENT);
    sheetDestroy(gsheet);
    return true;
}

static bool testGradesSheetRemoveGrade() {
    SheetResult errorcode;
    GradesSheet gsheet = sheetCreate();
    ASSERT_TEST(gsheet != NULL);

    sheetAddGrade(gsheet, 1, 2, 3, 4);
    sheetAddGrade(gsheet, 12, 13, 15, 16);
    sheetAddGrade(gsheet, 12, 69, 32, 56);
    sheetAddGrade(gsheet, 12, 69, 3, 4);
    sheetRemoveLastGrade(gsheet, 12, 13);
    sheetRemoveLastGrade(gsheet, 12, 69);
    sheetRemoveLastGrade(gsheet, 12, 69);
    //TODO: decide if it's an error or a valid request:
    errorcode = sheetRemoveLastGrade(gsheet, 12,
                                     69);// removing a course id that was removed
    ASSERT_TEST(errorcode == SHEET_SUCCESS);
    errorcode = sheetRemoveLastGrade(gsheet, -1, 2);
    ASSERT_TEST(errorcode == SHEET_INVALID_ARGUMENT);
    errorcode = sheetRemoveLastGrade(gsheet, 1, 1000001);
    sheetDestroy(gsheet);

    return true;
}

static bool testGradesSheetUpdateGrade() {
    SheetResult errorcode;
    GradesSheet gsheet = sheetCreate();
    ASSERT_TEST(gsheet != NULL);

    sheetAddGrade(gsheet, 1, 2, 3, 24);
    sheetAddGrade(gsheet, 5, 6, 7, 8);
    sheetAddGrade(gsheet, 1, 2, 3, 24);
    sheetAddGrade(gsheet, 1, 2, 3, 24);
    sheetUpdateLastGrade(gsheet, 2, 100); // << WOW!
    errorcode = sheetUpdateLastGrade(gsheet, 5, 99);
    ASSERT_TEST(errorcode == SHEET_GRADE_DOES_NOT_EXIST);
    errorcode = sheetUpdateLastGrade(gsheet, 1, 1000001);
    ASSERT_TEST(errorcode == SHEET_INVALID_ARGUMENT);
    errorcode = sheetUpdateLastGrade(gsheet, -1, 2);
    ASSERT_TEST(errorcode == SHEET_INVALID_ARGUMENT);

    sheetDestroy(gsheet);
    return true;
}

static bool testGradesSheetHighestLastGrade() {
    SheetResult errorcode;
    GradesSheet gsheet = sheetCreate();
    ASSERT_TEST(gsheet != NULL);

    sheetAddGrade(gsheet, 1, 2, 3, 55);
    sheetAddGrade(gsheet, 5, 6, 7, 8);
    sheetAddGrade(gsheet, 1, 2, 3, 0);
    sheetAddGrade(gsheet, 1, 2, 3, 54);
    int result;
    errorcode = sheetHighestLastGrade(gsheet, 5, &result);
    ASSERT_TEST(errorcode == SHEET_GRADE_DOES_NOT_EXIST);
    errorcode = sheetHighestLastGrade(gsheet, 1, NULL);
    ASSERT_TEST(errorcode == SHEET_NULL_ARGUMENT);
    errorcode = sheetHighestLastGrade(gsheet, -1, &result);
    ASSERT_TEST(errorcode == SHEET_INVALID_ARGUMENT);
    sheetDestroy(gsheet);
    return true;
}


static bool testSheetPrintFull() {
    SheetResult errorcode;
    GradesSheet gsheet = sheetCreate();
    ASSERT_TEST(gsheet != NULL);
    sheetAddGrade(gsheet, 1, 104012, 11, 84);
    sheetAddGrade(gsheet, 1, 104167, 10, 43);
    sheetAddGrade(gsheet, 1, 104167, 10, 71);
    sheetAddGrade(gsheet, 1, 234114, 8, 88);
    sheetAddGrade(gsheet, 1, 234145, 6, 98);
    sheetAddGrade(gsheet, 1, 394808, 2, 92);
    sheetAddGrade(gsheet, 2, 104013, 11, 66);
    sheetAddGrade(gsheet, 2, 104013, 11, 63);
    sheetAddGrade(gsheet, 2, 114071, 7, 98);
    sheetAddGrade(gsheet, 2, 234118, 6, 98);
    sheetAddGrade(gsheet, 2, 234122, 6, 28);
    sheetAddGrade(gsheet, 2, 114071, 6, 96);
    sheetAddGrade(gsheet, 2, 234141, 6, 73);
    sheetAddGrade(gsheet, 2, 394806, 2, 96);

    FILE *output_stream = stdout;
    errorcode = sheetPrintFull(output_stream, gsheet);
    ASSERT_TEST(errorcode == SHEET_SUCCESS);
    errorcode = sheetPrintFull(output_stream, NULL);
    ASSERT_TEST(errorcode == SHEET_NULL_ARGUMENT);

    sheetDestroy(gsheet);
    return true;
}

static bool testSheetPrintClear(){
    SheetResult errorcode;
    GradesSheet gsheet = sheetCreate();
    ASSERT_TEST(gsheet != NULL);
    sheetAddGrade(gsheet, 1, 104012, 11, 84);
    sheetAddGrade(gsheet, 1, 399999, 10, 43);
    sheetAddGrade(gsheet, 1, 104167, 10, 71);
    sheetAddGrade(gsheet, 1, 399599, 8, 88);
    sheetAddGrade(gsheet, 1, 399599, 6, 98);
    sheetAddGrade(gsheet, 1, 394808, 2, 92);
    sheetAddGrade(gsheet, 1, 399999, 2, 96);
    sheetAddGrade(gsheet, 2, 104013, 11, 66);
    sheetAddGrade(gsheet, 2, 104013, 11, 63);
    sheetAddGrade(gsheet, 2, 114071, 7, 98);
    sheetAddGrade(gsheet, 2, 234118, 6, 100);
    sheetAddGrade(gsheet, 2, 399399, 6, 28);
    sheetAddGrade(gsheet, 2, 114071, 6, 96);
    sheetAddGrade(gsheet, 2, 234141, 6, 73);
    sheetAddGrade(gsheet, 2, 399999, 2, 96);

    FILE *output_stream = stdout;
    errorcode = sheetPrintClean(output_stream,gsheet);
    ASSERT_TEST(errorcode == SHEET_SUCCESS);
    errorcode = sheetPrintClean(output_stream, NULL);
    ASSERT_TEST(errorcode == SHEET_NULL_ARGUMENT);

    sheetDestroy(gsheet);
    return true;

}
static bool testSheetPrintHighestGrades(){
    SheetResult errorcode;
    GradesSheet gsheet = sheetCreate();
    ASSERT_TEST(gsheet != NULL);
    sheetAddGrade(gsheet, 1, 104012, 11, 84);
    sheetAddGrade(gsheet, 1, 399999, 10, 43);
    sheetAddGrade(gsheet, 1, 104167, 10, 71);
    sheetAddGrade(gsheet, 1, 399599, 8, 88);
    sheetAddGrade(gsheet, 1, 399599, 6, 98);
    sheetAddGrade(gsheet, 1, 394808, 2, 92);
    sheetAddGrade(gsheet, 2, 104013, 11, 66);
    sheetAddGrade(gsheet, 2, 104013, 11, 63);
    sheetAddGrade(gsheet, 2, 114071, 7, 98);
    sheetAddGrade(gsheet, 2, 234118, 6, 100); // << WOW!
    sheetAddGrade(gsheet, 2, 399399, 6, 28);
    sheetAddGrade(gsheet, 2, 114071, 6, 96);
    sheetAddGrade(gsheet, 2, 234141, 6, 73);
    sheetAddGrade(gsheet, 1, 399999, 2, 96);
    sheetAddGrade(gsheet, 2, 399998, 2, 96);

    FILE *output_stream = stdout;
    errorcode = sheetPrintHighestGrades(output_stream,gsheet, 15);
    ASSERT_TEST(errorcode == SHEET_SUCCESS);
    errorcode = sheetPrintHighestGrades(output_stream, NULL,5);
    ASSERT_TEST(errorcode == SHEET_NULL_ARGUMENT);
    errorcode = sheetPrintHighestGrades(output_stream, gsheet,-2);
    ASSERT_TEST(errorcode == SHEET_INVALID_ARGUMENT);
    sheetDestroy(gsheet);
    return true;
}

static bool testSheetPrintLowestGrades(){
    SheetResult errorcode;
    GradesSheet gsheet = sheetCreate();
    ASSERT_TEST(gsheet != NULL);
    sheetAddGrade(gsheet, 1, 104012, 11, 84);
    sheetAddGrade(gsheet, 1, 399999, 10, 43);
    sheetAddGrade(gsheet, 1, 104167, 10, 71);
    sheetAddGrade(gsheet, 1, 399599, 8, 88);
    sheetAddGrade(gsheet, 1, 399599, 6, 98);
    sheetAddGrade(gsheet, 1, 394808, 2, 92);
    sheetAddGrade(gsheet, 2, 104013, 11, 66);
    sheetAddGrade(gsheet, 2, 104013, 11, 63);
    sheetAddGrade(gsheet, 2, 114071, 7, 98);
    sheetAddGrade(gsheet, 2, 234118, 6, 100); // << WOW!
    sheetAddGrade(gsheet, 2, 399399, 6, 28);
    sheetAddGrade(gsheet, 2, 114071, 6, 96);
    sheetAddGrade(gsheet, 2, 234141, 6, 73);
    sheetAddGrade(gsheet, 1, 399999, 2, 96);
    sheetAddGrade(gsheet, 2, 399998, 2, 96);

    FILE *output_stream = stdout;
    errorcode = sheetPrintLowestGrades(output_stream,gsheet, 5);
    ASSERT_TEST(errorcode == SHEET_SUCCESS);
    errorcode = sheetPrintLowestGrades(output_stream, NULL,5);
    ASSERT_TEST(errorcode == SHEET_NULL_ARGUMENT);
    errorcode = sheetPrintLowestGrades(output_stream, gsheet,-2);
    ASSERT_TEST(errorcode == SHEET_INVALID_ARGUMENT);
    sheetDestroy(gsheet);
    return true;
}

int main() {
    RUN_TEST(testGradesSheetCreate);
    RUN_TEST(testGradesSheetCopy);
    RUN_TEST(testGradesSheetAddGrade);
    RUN_TEST(testGradesSheetRemoveGrade);
    RUN_TEST(testGradesSheetUpdateGrade);
    RUN_TEST(testGradesSheetHighestLastGrade);
    RUN_TEST(testSheetPrintFull);
    RUN_TEST(testSheetPrintClear);
    RUN_TEST(testSheetPrintHighestGrades);
    RUN_TEST(testSheetPrintLowestGrades);
}