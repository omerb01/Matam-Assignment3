#ifndef MTM_EX3_H_
#define MTM_EX3_H_

// Updates:
// 02.12.2017: an example in the comments of mtmPrintSemesterInfo was fixed.

#include <stdio.h> // For FILE*
// You should avoid including unnecessary header files
#include <stdbool.h>

/**
* Maximal input line length
*/
#define MAX_LEN 120

/**
* This type defines all errors for the system.
* These codes should be used for calling mtmPrintErrorMessage to report
* error messages.
*
* Notice that after calling mtmPrintErrorMessage with one of the first
* three messages you should abort the program according to the exercise
* rules.
*/
typedef enum {
	MTM_OUT_OF_MEMORY, // You should exit program after this error
	MTM_INVALID_COMMAND_LINE_PARAMETERS, // You should exit the program after this error
	MTM_CANNOT_OPEN_FILE, // You should exit program after this error
	MTM_INVALID_PARAMETERS,
	MTM_NOT_LOGGED_IN,
	MTM_ALREADY_LOGGED_IN,
	MTM_STUDENT_DOES_NOT_EXIST,
	MTM_STUDENT_ALREADY_EXISTS,
	MTM_NOT_FRIEND,
	MTM_ALREADY_FRIEND,
	MTM_NOT_REQUESTED,
	MTM_ALREADY_REQUESTED,
	MTM_FILE_ERROR,
	MTM_COURSE_DOES_NOT_EXIST
} MtmErrorCode;

/**
* mtmPrintErrorMessage - Prints an error message into the given output channel.
*     Use the standard error channel to print error messages.
*
* @param error_channel - File descriptor for the error channel.
* @param code - Error code to print message for.
*/
void mtmPrintErrorMessage(FILE* error_channel, MtmErrorCode code);

/**
* mtmPrintStudentName - Prints student's name.
*
* @param output_channel - File descriptor for the output channel.
* @param first_name - The student's first name.
* @param last_name - The student's last name.
*/
void mtmPrintStudentName(FILE* output_channel, const char* first_name,
	const char* last_name);

/**
* mtmPrintStudentInfo - Prints student details.
*
* @param output_channel - File descriptor for the output channel.
* @param id - The student's id.
* @param first_name - The student's first name.
* @param last_name - The student's last name.
*/
void mtmPrintStudentInfo(FILE* output_channel, int id, const char* first_name,
	const char* last_name);

/**
* mtmPrintGradeInfo - Prints grade details.
*
* @param output_channel - File descriptor for the output channel.
* @param course_id - The course id.
* @param points_x2 - The number of course points, multiplied by 2. For example,
*     if "Physics 1m" is 3.5 points, pass 7.
* @param grade - The course grade.
*/
void mtmPrintGradeInfo(FILE* output_channel, int course_id, int points_x2,
	int grade);

/**
* mtmPrintSemesterInfo - Prints a line containing semester details.
*
* @param output_channel - File descriptor for the output channel.
* @param semester - The semester number.
* @param total_points_x2 - The number of total course points, multiplied by 2.
*     Courses taken multiple times are counted multiple times.
* @param failed_points_x2 - The number of failed course points, multiplied by 2.
*     A failed course is a course with a grade lower than 55. Courses failed
*     multiple times are counted multiple times.
* @param effective_points_x2 - The number of total effective course points,
*     multiplied by 2. Effective points are points of courses whose grade counts.
*     A grade doesn't count if another grade of the same course id appears later
*     in the semester.
* @param effective_grade_sum_x2 - The sum of the effective course grades, when
*     each grade is multiplied by the number of course points, multiplied by 2.
*
* For example, for the following semester:
*     Course: 234122, points: 3.0, grade: 51
*     Course: 114071, points: 3.5, grade: 75
*     Course: 234122, points: 3.0, grade: 90
* The parameters are:
*     total_points_x2 = (3.0+3.5+3.0)*2 = 19
*     failed_points_x2 = 3.0*2 = 6
*     effective_points_x2 = (3.5+3.0)*2 = 13
*     effective_grade_sum_x2 = (3.5*75+3.0*90)*2 = 1065
*/
void mtmPrintSemesterInfo(FILE* output_channel, int semester,
	int total_points_x2, int failed_points_x2, int effective_points_x2,
	int effective_grade_sum_x2);

/**
* mtmPrintSummary - Prints the summary of all the grade sheet.
*     The parameters are similar to the ones of mtmPrintSemesterInfo,
*     but the sums are not limited to a single semester.
*
* @param output_channel - File descriptor for the output channel.
* @param total_points_x2 - The number of total course points, multiplied by 2.
*     Courses taken multiple times are counted multiple times.
* @param failed_points_x2 - The number of failed course points, multiplied by 2.
*     A failed course is a course with a grade lower than 55. Courses failed
*     multiple times are counted multiple times.
* @param effective_points_x2 - The number of total effective course points,
*     multiplied by 2. Effective points are points of courses whose grade counts.
*     A grade doesn't count if another grade of the same course id appears later
*     in the grade sheet. Sport courses are an exception (course id 39xxxx): a
*     sport grade doesn't count only if another grade of the same course id
*     appears later in the same semester (not the whole grade sheet).
* @param effective_grade_sum_x2 - The sum of the effective course grades, when
*     each grade is multiplied by the number of course points, multiplied by 2.
*/
void mtmPrintSummary(FILE* output_channel, int total_points_x2,
	int failed_points_x2, int effective_points_x2, int effective_grade_sum_x2);

/**
* mtmPrintCleanSummary - Prints the summary of all the grade sheet.
*     Just like mtmPrintSummary, but without the parameters to calculate
*     the amount of failed grades - they don't appear in the clean sheet.
*
* @param output_channel - File descriptor for the output channel.
* @param effective_points_x2 - The number of total effective course points,
*     multiplied by 2. Effective points are points of courses whose grade counts.
*     A grade doesn't count if another grade of the same course id appears later
*     in the grade sheet. Sport courses are an exception (course id 39xxxx): a
*     sport grade doesn't count only if another grade of the same course id
*     appears later in the same semester (not the whole grade sheet).
* @param effective_grade_sum_x2 - The sum of the effective course grades, when
*     each grade is multiplied by the number of course points, multiplied by 2.
*/
void mtmPrintCleanSummary(FILE* output_channel, int effective_points_x2,
	int effective_grade_sum_x2);

/**
* mtmFacultyResponse - Prints a response to a faculty request.
*
* @param output_channel - File descriptor for the output channel.
* @param response - The response.
*/
void mtmFacultyResponse(FILE* output_channel, const char* response);

#endif /* MTM_EX3_H_ */
