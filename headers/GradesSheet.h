//
// Created by Ilya on 12/14/2017.
//

#ifndef ASSIGNMENT3_GRADESSHEET_H
#define ASSIGNMENT3_GRADESSHEET_H
#include <stdio.h>
#include "list.h"
#include "../headers/mtm_ex3.h"

typedef struct GradesSheet_t *GradesSheet;


typedef enum SheetResult_t {
    SHEET_SUCCESS,
    SHEET_OUT_OF_MEMORY,
    SHEET_NULL_ARGUMENT,
    SHEET_INVALID_ARGUMENT,
    SHEET_GRADE_DOES_NOT_EXIST,
} SheetResult;

/* Creates a new Grades Sheet
 * allocates a new memory for it
 * returns:
 * NULL if memory allocation failed
 * otherwise returns a pointer to the new grades sheet
 */
GradesSheet sheetCreate();

/* Copies GradesSheet into a new copy
 * allocates new memory for it
 * returns:
 * NULL is memory allocation failed or if given GradesSheet is NULL
 * otherwise returns a pointer to the new GradesSheet
 */
GradesSheet sheetCopy(GradesSheet);

/* Adds a new grade to the Grades Sheet
 * allocates new memory for data storing
 * returns:
 * SHEET_OUT_OF_MEMORY if memory allocation failed
 * SHEET_NULL_ARGUMENT if the given grades sheet is null
 * SHEET_INVALID_ARGUMENT if one of the given arguments is invalid
 * otherwise SHEET_SUCCESS
 */
SheetResult
sheetAddGrade(GradesSheet, int semester, int course_id, int points_x2,
              int grade_value);

/* Removes the last(latest) grade of the given semester's course (also given)
 * returns:
 * SHEET_INVALID_ARGUMENT if one of the arguments is invalid
 * SHEET_NULL_ARGUMENT if the given grades sheet is null
 * otherwise SHEET_SUCCESS
 */
SheetResult sheetRemoveLastGrade(GradesSheet, int semester, int course_id);

/* Updates the given course's(latest taken) last(latest) grade with the given new grade
 * returns:
 * SHEET_NULL_ARGUMENT if the given grades sheet is null
 * SHEET_INVALID_ARGUMENT if one of the arguments is invalid
 * SHEET_GRADE_DOES_NOT_EXIST if course's current grade doesn't exist
 * otherwise SHEET_SUCCESS
 */
SheetResult sheetUpdateLastGrade(GradesSheet, int course_id, int new_grade);

/* Stores in a given pointer the latest highest grade by a given course_id
 * returns:
 * SHEET_NULL_ARGUMENT if the given grades sheet is null or result
 * SHEET_INVALID_ARGUMENT if one of the arguments is invalid
 * SHEET_GRADE_DOES_NOT_EXIST if course's current grade doesn't exist
 * otherwise SHEET_SUCCESS
 */
SheetResult sheetHighestLastGrade(GradesSheet, int course_id, int *result);

/* Prints a full version (no grades restriction) of the given grades sheet
 * returns:
 * SHEET_OUT_OF_MEMORY if memory problem occurred
 * SHEET_NULL_ARGUMENT if the given grades sheet is null
 * otherwise SHEET_SUCCESS
 */
SheetResult sheetPrintFull(FILE* output_channel, GradesSheet);

/* Prints a clean version (the effective grades) of the given grades sheet
 * returns:
 * SHEET_NULL_ARGUMENT if the given grades sheet is null
 * SHEET_OUT_OF_MEMORY if memory problem occurred
 * otherwise SHEET_SUCCESS
 */
SheetResult sheetPrintClean(FILE* output_channel, GradesSheet);

/* Prints a given amount of highest effective grades in the *clean version* grades sheet
 * returns:
 * SHEET_NULL_ARGUMENT if the given grades sheet is null
 * SHEET_INVALID_ARGUMENT if the argument is invalid
 * SHEET_OUT_OF_MEMORY if memory problem occurred
 * otherwise SHEET_SUCCESS
 */
SheetResult sheetPrintHighestGrades(FILE* output_channel, GradesSheet, int amount);
/* Prints a given amount of lowest effective grades in the *clean version* grades sheet
 * returns:
 * SHEET_NULL_ARGUMENT if the given grades sheet is null
 * SHEET_INVALID_ARGUMENT if the argument is invalid
 * SHEET_OUT_OF_MEMORY if memory problem occurred
 * otherwise SHEET_SUCCESS
 */
SheetResult sheetPrintLowestGrades(FILE* output_channel, GradesSheet, int amount);
/* Destroys the grade sheet
 * de-allocates all relevant memory
 */
void sheetDestroy(GradesSheet sheet);

#endif //ASSIGNMENT3_GRADESSHEET_H
