//
// Created by Omer on 15/12/2017.
//

#include "list.h"
#include "GradesSheet.h"
#include "mtm_ex3.h"
#include "UsefulFunctions.h"
#include <stdlib.h>

#define GRADE_INSERT_LAST() \
new_semester = listCreate((CopyListElement) gradeCopyFunction, \
                            (FreeListElement)gradeFreeFunction); \
listInsertLast(new_semester, (ListElement) new_grade); \
listInsertLast(grades_sheet->sheet, (ListElement) new_semester); \

#define COMPARE_GRADE_FIELD($$field$$) \
if(grade1->$$field$$ > grade2->$$field$$){return 1;} \
else if(grade1->$$field$$ == grade2->$$field$$){return 0;} \
else{ return -1;}

#define CHECK_GRADE_NOT_NULL() \
 if (grades_sheet == NULL || grades_sheet->sheet == NULL) \
                {return SHEET_NULL_ARGUMENT;}

#define CHECK_GRADE_OUTPUT_NOT_NULL() \
 if (grades_sheet == NULL || grades_sheet->sheet == NULL || \
        output_channel == NULL) {return SHEET_NULL_ARGUMENT;}

struct GradesSheet_t {
    List sheet;
};

typedef struct Grade_t {
    int semester;
    int course_id;
    int points_x2;
    int grade_value;
} *Grade;

static bool filterCourse(Grade grade, ListFilterKey key);

static List generateCleanList(GradesSheet grades_sheet);

static ListElement semesterListCopy(ListElement semesterGradesList) {
    if ((List) semesterGradesList == NULL) return NULL;
    List new_sheet = listCopy((List) semesterGradesList);
    if (new_sheet == NULL) return NULL;
    listDestroy((List) semesterGradesList);
    return (ListElement) new_sheet;
}

static void semesterListDestroy(ListElement semesterGradesList) {
    listDestroy((List) semesterGradesList);
}

static Grade gradeCopyFunction(Grade grade) {
    if (grade == NULL) return NULL;
    Grade new_grade = MALLOC(new_grade);
    new_grade->points_x2 = grade->points_x2;
    new_grade->grade_value = grade->grade_value;
    new_grade->semester = grade->semester;
    new_grade->course_id = grade->course_id;
    return new_grade;
}

static void gradeFreeFunction(Grade grade) {
    free(grade);
}

static Grade gradeCopy(int semester, int course_id,
                       int points_x2,
                       int grade_value) {
    Grade new_grade = MALLOC(new_grade);
    new_grade->course_id = course_id;
    new_grade->semester = semester;
    new_grade->grade_value = grade_value;
    new_grade->points_x2 = points_x2;
    return new_grade;
}

static int getSemesterLastGradeIndex(List semester_grade_list, int course_id) {
    int index = -1, currindex = 0;
    LIST_FOREACH(Grade, grade, semester_grade_list) {
        if (grade->course_id == course_id) {
            index = currindex;
        }
        currindex++;
    }
    return index;
}

static int getSemesterHighestGrade(List semester_grade_list, int course_id) {
    int max = -1, curr_max = -1;
    LIST_FOREACH(Grade, grade, semester_grade_list) {
        if (grade->course_id == course_id) {
            curr_max = grade->grade_value;
        }
        max = max > curr_max ? max : curr_max;
    }
    return max;
}


static int semesterGradeCompareFunction(Grade grade1, Grade grade2) {
    if (grade1->course_id > grade2->course_id) {
        return 1;
    } else if (grade1->course_id < grade2->course_id) {
        return 0;
    } else {
        return grade1->semester > grade2->semester ? 1 : 0;
    }
}

static int semesterIdCompareFunction(List semester_list1, List semester_list2) {
    Grade first = (Grade) listGetFirst(semester_list1);
    Grade second = (Grade) listGetFirst(semester_list2);
    if (first->semester > second->semester) {
        return 1;
    } else {
        return 0;
    }
}

static void calcSemesterInfo(Grade current_grade, int *effective_point,
                             int *effective_grade_sum) {
    int effective_p = 0, effective_g = 0;

    if (!isGradeFail(current_grade->grade_value)) {
        effective_p += current_grade->points_x2;
    }
    effective_g += effective_p * (current_grade->grade_value);

    *effective_point += effective_p;
    *effective_grade_sum += effective_g;

}

static List getCleanGradesForCurrentSemester(List semester) {
    int *grades_histogram = calloc(1000000, sizeof(int));
    LIST_FOREACH(Grade, current_grade, semester) {
        grades_histogram[current_grade->course_id]++;
    }
    List clean_current_semester_grades = listFilter(semester,
                                                    (FilterListElement) filterCourse,
                                                    (ListFilterKey) grades_histogram);
    if (clean_current_semester_grades == NULL)
        return NULL;
    free(grades_histogram);
    return clean_current_semester_grades;
}

static SheetResult
printSemesterInfo(FILE *output_stream, List semester,
                  int *total_points_sheet, int *failed_total_sheet) {
    int total_points = 0, failed_points = 0, effective_points = 0,
            effective_grade_sum = 0;
    LIST_FOREACH(Grade, current_grade, semester) {
        mtmPrintGradeInfo(output_stream, current_grade->course_id,
                          current_grade->points_x2, current_grade->grade_value);
        if (isGradeFail(current_grade->grade_value)) {
            failed_points += current_grade->points_x2;
        }
        total_points += current_grade->points_x2;
    }
    List clean_grades_current_semester = getCleanGradesForCurrentSemester(
            semester);
    if (clean_grades_current_semester == NULL) return SHEET_OUT_OF_MEMORY;
    LIST_FOREACH(Grade, current_grade, clean_grades_current_semester) {
        calcSemesterInfo(current_grade,
                         &effective_points, &effective_grade_sum);

    }
    mtmPrintSemesterInfo(output_stream,
                         ((Grade) listGetFirst(semester))->semester,
                         total_points, failed_points, effective_points,
                         effective_grade_sum);
    *total_points_sheet += total_points;
    *failed_total_sheet += failed_points;
    listDestroy(clean_grades_current_semester);
    return SHEET_SUCCESS;
}

static SheetResult
printFullSheetSummary(FILE *output_stream, GradesSheet grades_sheet,
                      int total_points_sheet,
                      int failed_total_sheet) {
    List grades_sheet_clean = generateCleanList(grades_sheet);
    if (grades_sheet_clean == NULL) return SHEET_OUT_OF_MEMORY;

    int total_effective_points = 0, total_effective_points_sum = 0;
    LIST_FOREACH(Grade, current_grade, grades_sheet_clean) {
        if (!isGradeFail(current_grade->grade_value)) {
            total_effective_points += current_grade->points_x2;
            total_effective_points_sum +=
                    (current_grade->grade_value) * (current_grade->points_x2);
        }
    }
    mtmPrintSummary(output_stream, total_points_sheet, failed_total_sheet,
                    total_effective_points, total_effective_points_sum);

    listDestroy(grades_sheet_clean);
    return SHEET_SUCCESS;
}

static bool filterOutSportCoursesFunction(Grade grade, ListSortKey key) {
    int *counted = &((int *) key)[grade->course_id];
    if (!isSportCourse(grade->course_id)) {
        return true;
    } else if (*(counted) > 1) {
        (*counted)--;
        return false;
    } else {
        (*counted)--;
        return true;
    }
}

static bool filterCourse(Grade grade, ListFilterKey key) {
    int *counted = &((int *) key)[grade->course_id];
    if (*(counted) > 1) {
        (*counted)--;
        return false;
    } else {
        (*counted)--;
        return true;
    }
}

static List listConcatenator(GradesSheet grade_sheet) {
    List concatanated_list = listCreate((CopyListElement) gradeCopyFunction,
                                        (FreeListElement) gradeFreeFunction);
    if (concatanated_list == NULL) return NULL;
    LIST_FOREACH(List, semester_grade_list, grade_sheet->sheet) {
        LIST_FOREACH(Grade, current_grade, semester_grade_list) {
            ListResult result = listInsertLast(concatanated_list,
                                               current_grade);
            if (result == LIST_OUT_OF_MEMORY) return NULL;
        }
    }
    return concatanated_list;
}

static void printSemesterInfoClean(FILE *output_stream, List grade_sheet,
                                   int *effective_point_sheet,
                                   int *effective_grade_sum_sheet) {
    int effective_p = 0, effective_g = 0;
    LIST_FOREACH(Grade, current_grade, grade_sheet) {
        mtmPrintGradeInfo(output_stream, current_grade->course_id,
                          current_grade->points_x2,
                          current_grade->grade_value);
        effective_p += current_grade->points_x2;
        effective_g +=
                (current_grade->points_x2) * (current_grade->grade_value);
    }

    *effective_point_sheet += effective_p;
    *effective_grade_sum_sheet += effective_g;

}

static ListResult filterOutSportCourses(GradesSheet grades_sheet,
                                        GradesSheet sport_filtered_sheet,
                                        int *histogram) {
    List temp_filtered_list;

    LIST_FOREACH(List, semester_grades_list, grades_sheet->sheet) {
        LIST_FOREACH(Grade, current_grade, semester_grades_list) {
            histogram[current_grade->course_id]++;
        }
        temp_filtered_list = listFilter(semester_grades_list,
                                        (FilterListElement) filterOutSportCoursesFunction,
                                        (ListSortKey) histogram);
        ListResult result = listInsertLast(sport_filtered_sheet->sheet,
                                           temp_filtered_list);
        if (result == LIST_OUT_OF_MEMORY) return LIST_OUT_OF_MEMORY;
    }
    return LIST_SUCCESS;
}

static List generateCleanList(GradesSheet grades_sheet) {
    List clean_list;
    int *histogram = calloc(1000000, sizeof(int));
    if (histogram == NULL) return NULL;
    GradesSheet sport_filtered_sheet = sheetCreate();
    if (sport_filtered_sheet == NULL) return NULL;
    ListResult result = filterOutSportCourses(grades_sheet,
                                              sport_filtered_sheet,
                                              (ListFilterKey) histogram);
    if (result == LIST_OUT_OF_MEMORY) return NULL;
    List joined_grade_sheet = listConcatenator(sport_filtered_sheet);
    if (joined_grade_sheet == NULL) return NULL;
    clean_list = listFilter(joined_grade_sheet,
                            (FilterListElement) filterCourse,
                            (ListFilterKey) histogram);
    if (clean_list == NULL) return NULL;
    free(histogram);
    listDestroy(joined_grade_sheet);
    sheetDestroy(sport_filtered_sheet);
    return clean_list;
}

static int compareCourseId(Grade grade1, Grade grade2) {
    COMPARE_GRADE_FIELD(course_id);
}

static int compareGradeValue(Grade grade1, Grade grade2) {
    COMPARE_GRADE_FIELD(grade_value);
}

static int compareSemester(Grade grade1, Grade grade2) {
    COMPARE_GRADE_FIELD(semester);
}

static int
semeserGradesSortFunction(Grade grade1, Grade grade2, ListSortKey key) {
    int condition = *(int *) key; //1 for lowest, 0 for highest
    if (compareGradeValue(grade1, grade2) == 1) {
        return condition;
    } else if (compareGradeValue(grade1, grade2) == 0) {
        if (compareSemester(grade1, grade2) == 1) {
            return 1; //was !condition
        } else if (compareSemester(grade1, grade2) == -1) {
            return 0;
        } else {
            if (compareCourseId(grade1, grade2) == 1) {
                return 1;
            } else {
                return 0;
            }
        }
    } else {
        return !condition;
    }
}

static List
createSortedCleanSheetByKey(GradesSheet grades_sheet,
                            ListSortKey key) {
    List clean_grades_list = generateCleanList(grades_sheet);
    if (clean_grades_list == NULL) return NULL;

    ListResult result = listSort(clean_grades_list,
                                 (CompareListElements) semeserGradesSortFunction,
                                 (ListSortKey) key);
    if (result == LIST_OUT_OF_MEMORY) return NULL;
    return clean_grades_list;
}

static int getLastSemesterNumber(GradesSheet grades_sheet) {
    int semester_number = -1, highest_semester_number = -1;
    LIST_FOREACH(List, semester_grades_list, grades_sheet->sheet) {
        semester_number = ((Grade) listGetFirst(
                semester_grades_list))->semester;
        highest_semester_number = highest_semester_number > semester_number
                                  ? highest_semester_number : semester_number;
    }
    return highest_semester_number;
}

static List
getLatestCourse(GradesSheet grades_sheet, int *latest_index, int course_id) {
    int last_grade_index;
    List latest_semester_list = NULL;
    LIST_FOREACH(List, current_semester_list, grades_sheet->sheet) {
        last_grade_index = getSemesterLastGradeIndex(current_semester_list,
                                                     course_id);
        if (last_grade_index != -1) {
            *latest_index = last_grade_index;
            latest_semester_list = current_semester_list;
        }
    }
    return latest_semester_list;

}

static List
getLatestCourseToRemove(GradesSheet grades_sheet, int *latest_index,
                        int course_id, int semester) {
    int last_grade_index;
    List latest_semester_list = NULL;
    LIST_FOREACH(List, current_semester_list, grades_sheet->sheet) {
        if (((Grade) listGetFirst(current_semester_list))->semester ==
            semester) {
            last_grade_index = getSemesterLastGradeIndex(current_semester_list,
                                                         course_id);
            *latest_index = last_grade_index;
            latest_semester_list = current_semester_list;
        }
    }
    return latest_semester_list;

}

static void listRemoveFromSheet(List list_to_remove, GradesSheet grades_sheet) {
    LIST_FOREACH(List, current_semester_list, grades_sheet->sheet) {
        if (list_to_remove == current_semester_list) {
            listRemoveCurrent(grades_sheet->sheet);
            break;
        }
    }
}

static SheetResult
insertGradeIntoSheet(GradesSheet grades_sheet, Grade new_grade,
                     int *grade_inserted) {
    LIST_FOREACH(List, semester_grades_list, grades_sheet->sheet) {
        Grade first_grade_in_semester = (Grade) listGetFirst(
                semester_grades_list);
        if (first_grade_in_semester->semester == new_grade->semester) {
            ListResult result = listInsertLast(semester_grades_list,
                                               (ListElement) new_grade);
            if (result == LIST_OUT_OF_MEMORY) {
                free(new_grade);
                return SHEET_OUT_OF_MEMORY;
            }
            *grade_inserted = 1;
            break;
        }
    }
    return SHEET_SUCCESS;
}

GradesSheet sheetCreate() {
    GradesSheet new_sheet = malloc(sizeof(*new_sheet));
    new_sheet->sheet = listCreate((CopyListElement) semesterListCopy,
                                  (FreeListElement) semesterListDestroy);
    if (new_sheet->sheet == NULL) return NULL;
    return new_sheet;
}

GradesSheet sheetCopy(GradesSheet sheet_to_be_copied) {
    if (sheet_to_be_copied == NULL || sheet_to_be_copied->sheet == NULL)
        return NULL;
    GradesSheet new_sheet = sheetCreate();
    if (new_sheet == NULL) return NULL;
    LIST_FOREACH(List, semesterGradesList, sheet_to_be_copied->sheet) {
        List copied_semester_grades_list = listCopy(semesterGradesList);
        if (copied_semester_grades_list == NULL) {
            listDestroy(new_sheet->sheet);
            return NULL;
        }
        listInsertLast(new_sheet->sheet, copied_semester_grades_list);
    }
    return new_sheet;
}


SheetResult
sheetAddGrade(GradesSheet grades_sheet, int semester, int course_id,
              int points_x2,
              int grade_value) {
    CHECK_GRADE_NOT_NULL();
    if (!(isValidSemester(semester) && isValidCourseId(course_id) &&
          isValidPointsX2(points_x2) && isGradeValid(grade_value)))
        return SHEET_INVALID_ARGUMENT;
    Grade new_grade = gradeCopy(semester, course_id, points_x2,
                                grade_value);
    if (new_grade == NULL) return SHEET_OUT_OF_MEMORY;
    List new_semester = NULL;
    int grade_inserted = 0;

    SheetResult result = insertGradeIntoSheet(grades_sheet, new_grade,
                                              &grade_inserted);
    if (result == SHEET_OUT_OF_MEMORY) {
        return SHEET_OUT_OF_MEMORY;
    } else if (!grade_inserted) {
        GRADE_INSERT_LAST();
    }
    free(new_grade);
    return SHEET_SUCCESS;
}

SheetResult
sheetRemoveLastGrade(GradesSheet grades_sheet, int semester,
                     int course_id) {
    CHECK_GRADE_NOT_NULL();
    if (!(isValidSemester(semester) && isValidCourseId(course_id)))
        return SHEET_INVALID_ARGUMENT;
    int last_grade_index;
    List lastest_semester_list = getLatestCourseToRemove(grades_sheet,
                                                         &last_grade_index,
                                                         course_id, semester);

    if (semester > getLastSemesterNumber(grades_sheet)) {
        return SHEET_GRADE_DOES_NOT_EXIST;
    } else if (lastest_semester_list == NULL) {
        return SHEET_OUT_OF_MEMORY;
    } else if (last_grade_index == -1) {
        return SHEET_GRADE_DOES_NOT_EXIST;
    } else if (listGetSize(lastest_semester_list) == 1) {
        listRemoveFromSheet(lastest_semester_list, grades_sheet);
    } else {
        LIST_FOREACH(Grade, grade_iterator, lastest_semester_list) {
            if (!last_grade_index) {
                listRemoveCurrent(lastest_semester_list);
                break;
            }
            last_grade_index--;
        }
    }
    return SHEET_SUCCESS;
}

SheetResult
sheetUpdateLastGrade(GradesSheet grades_sheet, int course_id,
                     int new_grade) {
    CHECK_GRADE_NOT_NULL();
    if (!(isValidCourseId(course_id) && isGradeValid(new_grade)))
        return SHEET_INVALID_ARGUMENT;

    int last_grade_index = -1;
    List lastest_semester_list = getLatestCourse(grades_sheet,
                                                 &last_grade_index, course_id);


    if (last_grade_index != -1 && lastest_semester_list != NULL) {
        LIST_FOREACH(Grade, grade_iterator, lastest_semester_list) {
            if (!last_grade_index) {
                grade_iterator->grade_value = new_grade;
                break;
            }
            last_grade_index--;
        }
        return SHEET_SUCCESS;
    } else if (last_grade_index == -1 && lastest_semester_list == NULL) {
        return SHEET_GRADE_DOES_NOT_EXIST;
    }
    return SHEET_OUT_OF_MEMORY;
}

SheetResult
sheetHighestGrade(GradesSheet grades_sheet, int course_id,
                  int *result) {
    CHECK_GRADE_NOT_NULL();
    if (!(isValidCourseId(course_id)))
        return SHEET_INVALID_ARGUMENT;
    if (result == NULL) return SHEET_NULL_ARGUMENT;
    int curr_max = -1, max = -1;

    LIST_FOREACH(List, semester_grades_list, grades_sheet->sheet) {
        curr_max = getSemesterHighestGrade(semester_grades_list,
                                           course_id);
        max = max > curr_max ? max : curr_max;
    }
    if (max == -1) return SHEET_GRADE_DOES_NOT_EXIST;
    *result = max;
    return SHEET_SUCCESS;

}

SheetResult sheetPrintFull(FILE *output_channel, GradesSheet grades_sheet) {
    CHECK_GRADE_OUTPUT_NOT_NULL();

    List clean_grades_sheet = generateCleanList(grades_sheet);
    if (clean_grades_sheet == NULL) return SHEET_OUT_OF_MEMORY;

    int total_points_sheet = 0, failed_points_sheet = 0;
    ListResult list_result = listSort(grades_sheet->sheet,
                                      (CompareListElements) semesterIdCompareFunction,
                                      0);
    if (list_result == LIST_OUT_OF_MEMORY) return SHEET_OUT_OF_MEMORY;
    LIST_FOREACH(List, semester_grades_list, grades_sheet->sheet) {
        SheetResult sheet_result = printSemesterInfo(output_channel,
                                                     semester_grades_list,
                                                     &total_points_sheet,
                                                     &failed_points_sheet);
        if (sheet_result == SHEET_OUT_OF_MEMORY) {
            listDestroy(clean_grades_sheet);
            return SHEET_OUT_OF_MEMORY;
        }
    }
    SheetResult sheet_result = printFullSheetSummary(output_channel,
                                                     grades_sheet,
                                                     total_points_sheet,
                                                     failed_points_sheet);
    if (sheet_result == SHEET_OUT_OF_MEMORY) return SHEET_OUT_OF_MEMORY;

    listDestroy(clean_grades_sheet);

    return SHEET_SUCCESS;
}

SheetResult
sheetPrintClean(FILE *output_channel, GradesSheet grades_sheet) {
    CHECK_GRADE_OUTPUT_NOT_NULL();

    int key = 0;

    List clean_grades_list = generateCleanList(grades_sheet);
    if (clean_grades_list == NULL) return SHEET_OUT_OF_MEMORY;
    ListResult result = listSort(clean_grades_list,
                                 (CompareListElements) semesterGradeCompareFunction,
                                 (ListSortKey) &key);
    if (result == LIST_OUT_OF_MEMORY) return SHEET_OUT_OF_MEMORY;

    int effective_point_sheet = 0, effective_grade_sum_sheet = 0;
    printSemesterInfoClean(output_channel, clean_grades_list,
                           &effective_point_sheet,
                           &effective_grade_sum_sheet);

    mtmPrintCleanSummary(output_channel, effective_point_sheet,
                         effective_grade_sum_sheet);

    listDestroy(clean_grades_list);
    return SHEET_SUCCESS;

}

SheetResult
sheetPrintHighestGrades(FILE *output_channel, GradesSheet grades_sheet,
                        int amount) {
    CHECK_GRADE_OUTPUT_NOT_NULL();
    if (amount <= 0) return SHEET_INVALID_ARGUMENT;
    int key = 0, counter = amount;
    List clean_grades_list = createSortedCleanSheetByKey(grades_sheet,
                                                         (ListSortKey) &key);
    if (clean_grades_list == NULL) return SHEET_OUT_OF_MEMORY;

    LIST_FOREACH(Grade, current_grade, clean_grades_list) {
        if (counter == 0) break;
        mtmPrintGradeInfo(output_channel, current_grade->course_id,
                          current_grade->points_x2,
                          current_grade->grade_value);
        counter--;

    }
    listDestroy(clean_grades_list);
    return SHEET_SUCCESS;
}

SheetResult
sheetPrintLowestGrades(FILE *output_channel, GradesSheet grades_sheet,
                       int amount) {
    CHECK_GRADE_OUTPUT_NOT_NULL();
    if (!isValidAmount(amount)) return SHEET_INVALID_ARGUMENT;
    int key = 1, counter = amount;
    List clean_grades_list = createSortedCleanSheetByKey(grades_sheet,
                                                         (ListSortKey) &key);
    if (clean_grades_list == NULL) return SHEET_OUT_OF_MEMORY;

    LIST_FOREACH(Grade, current_grade, clean_grades_list) {
        if (counter == 0) break;
        mtmPrintGradeInfo(output_channel, current_grade->course_id,
                          current_grade->points_x2,
                          current_grade->grade_value);
        counter--;
    }
    listDestroy(clean_grades_list);
    return SHEET_SUCCESS;
}

void sheetDestroy(GradesSheet grades_sheet) {
    listDestroy(grades_sheet->sheet);
    free(grades_sheet);
}

