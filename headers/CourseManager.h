//
// Created by Ilya on 12/14/2017.
//

#ifndef ASSIGNMENT3_COURSEMANAGER_H
#define ASSIGNMENT3_COURSEMANAGER_H

#include "Graph.h"
#include "Student.h"

typedef struct CourseManager_t *CourseManager;
typedef enum ManagerResult_t {
    MANAGER_SUCCESS,
    MANAGER_OUT_OF_MEMORY,
    MANAGER_INVALID_ARGUMENT,
    MANAGER_NOT_LOGGED_IN,
    MANAGER_ALREADY_LOGGED_IN,
    MANAGER_STUDENT_DOES_NOT_EXIST,
    MANAGER_STUDENT_ALREADY_EXISTS,
    MANAGER_NOT_FRIEND,
    MANAGER_ALREADY_FRIEND,
    MANAGER_NOT_REQUESTED,
    MANAGER_ALREADY_REQUESTED,
    MANAGER_COURSE_DOES_NOT_EXIST,
    MANAGER_NULL_ARGUMENT,
} ManagerResult;

/* Creates a new Course Manager
 * allocates new memory for inner structs
 * returns:
 * NULL if there was memory problem
 * otherwise returns pointer to the course manager
 */
CourseManager managerCreate();

/* Adds a new student to the Course Manager
 * allocates new memory
 * returns:
 * MANAGER_OUT_OF_MEMORY if memory allocation failed
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_INVALID_ARGUMENT if one of the relevant arguments is invalid
 * MANAGER_STUDENT_ALREADY_EXISTS if student already exists
 * otherwise MANAGER_SUCCESS
 */
ManagerResult
managerAddStudent(CourseManager, int id, char *first_name, char *last_name);

/* Removes student from the Course Manager and his associated memory
 * returns:
 * MANAGER_OUT_OF_MEMORY if memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_INVALID_ARGUMENT if one of the relevant arguments is invalid
 * MANAGER_STUDENT_DOES_NOT_EXIST if the student does not exist
 * otherwise MANAGER_SUCCESS
 */
ManagerResult managerRemoveStudent(CourseManager, int id);

/* Sign-in a student into the Course Manager system
 * returns:
 * MANAGER_OUT_OF_MEMORY if memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_INVALID_ARGUMENT if one of the relevant arguments is invalid
 * MANAGER_ALREADY_LOGGED_IN if student is already logged in the system
 * MANAGER_STUDENT_DOES_NOT_EXIST if the student does not exist
 * otherwise MANAGER_SUCCESS
 */
ManagerResult managerLogin(CourseManager, int id);

/* Logs out the student from the Course Manager system
 * returns:
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_NOT_LOGGED_IN if the student doesn't logged in
 * otherwise MANAGER_SUCCESS
 */
ManagerResult managerLogout(CourseManager);

/* Prints a message if the request is legitimate
 * returns:
 * MANAGER_OUT_OF_MEMORY if a memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_NOT_LOGGED_IN if the student doesn't logged in
 * MANAGER_INVALID_ARGUMENT if one of the relevant arguments is invalid
 * MANAGER_COURSE_DOES_NOT_EXIST if the course doesn't exist
 * otherwise MANAGER_SUCCESS
 */
ManagerResult
managerFacultyRequest(FILE *output_channel, CourseManager, int course_id,
                      char *request);

/* Sends a friend request to the given id
 * returns:
 * MANAGER_OUT_OF_MEMORY if a memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_NOT_LOGGED_IN if the student doesn't logged in
 * MANAGER_INVALID_ARGUMENT if one of the relevant arguments is invalid
 * MANAGER_STUDENT_DOES_NOT_EXIST if the student does not exist
 * MANAGER_ALREADY_FRIEND if the students are already friends
 * MANAGER_ALREADY_REQUESTED if the request was already sent beforehand
 * otherwise MANAGER_SUCCESS
 */
ManagerResult managerSendFriendRequest(CourseManager, int id_to_request);

/* Handles a friends request according to the given action
 * returns:
 * MANAGER_OUT_OF_MEMORY if a memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_NOT_LOGGED_IN if the student doesn't logged in
 * MANAGER_INVALID_ARGUMENT if one of the relevant arguments is invalid
 * MANAGER_STUDENT_DOES_NOT_EXIST if the student does not exist
 * MANAGER_ALREADY_FRIEND if the students are already friends
 * MANAGER_NOT_REQUESTED if no request was sent by the given id to the logged student
 * otherwise MANAGER_SUCCESS
 */
ManagerResult
managerHandleFriendRequest(CourseManager, int id_waiting_for_response,
                           char *action);

/* Un-friends a friend and removes any related friend requests
 * returns:
 * MANAGER_OUT_OF_MEMORY if a memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_NOT_LOGGED_IN if the student doesn't logged in
 * MANAGER_INVALID_ARGUMENT if one of the relevant arguments is invalid
 * MANAGER_STUDENT_DOES_NOT_EXIST if the student does not exist
 * MANAGER_NOT_FRIEND if both of the students are not friends
 * otherwise MANAGER_SUCCESS
 */
ManagerResult managerUnfriend(CourseManager, int id_to_unfriend);

/* Adds a new grade for the student
 * returns:
 * MANAGER_OUT_OF_MEMORY if a memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_NOT_LOGGED_IN if the student doesn't logged in
 * MANAGER_INVALID_ARGUMENT if one of the relevant arguments is invalid
 * otherwise MANAGER_SUCCESS
 */
ManagerResult
managerAddGrade(CourseManager, int semester, int course_id, int points_x2,
                int grade_value);

/* Removes the last(latest) grade of the given semester's course (also given)
 * returns:
 * MANAGER_OUT_OF_MEMORY if a memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_NOT_LOGGED_IN if the student doesn't logged in
 * MANAGER_INVALID_ARGUMENT if one of the relevant arguments is invalid
 * MANAGER_COURSE_DOES_NOT_EXIST if the course doesn't exist
 * otherwise MANAGER_SUCCESS
 */
ManagerResult
managerRemoveLastGrade(CourseManager, int semester, int course_id);

/* Updates the given course's(latest taken) last(latest) grade with the given new grade
 * returns:
 * MANAGER_OUT_OF_MEMORY if a memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_NOT_LOGGED_IN if the student doesn't logged in
 * MANAGER_INVALID_ARGUMENT if one of the relevant arguments is invalid
 * MANAGER_COURSE_DOES_NOT_EXIST if the course doesn't exist
 * otherwise MANAGER_SUCCESS
 */
ManagerResult
managerUpdateLastGrade(CourseManager, int course_id, int new_grade);

/* Prints a full version (no grades restriction) of the logged student grades sheet
 * returns:
 * MANAGER_OUT_OF_MEMORY if a memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_NOT_LOGGED_IN if the student doesn't logged in
 * otherwise MANAGER_SUCCESS
 */
ManagerResult managerPrintFullSheet(FILE *output_channel, CourseManager);

/* Prints a clean version (the effective grades) of the logged student grades sheet
 * returns:
 * MANAGER_OUT_OF_MEMORY if a memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_NOT_LOGGED_IN if the student doesn't logged in
 * otherwise MANAGER_SUCCESS
 */
ManagerResult managerPrintCleanSheet(FILE *output_channel, CourseManager);

/* Prints a given amount of highest effective grades in the *clean version* grades sheet
 * returns:
 * MANAGER_OUT_OF_MEMORY if a memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_NOT_LOGGED_IN if the student doesn't logged in
 * MANAGER_INVALID_ARGUMENT if one of the relevant arguments is invalid
 * otherwise MANAGER_SUCCESS
 */
ManagerResult
managerPrintHighestGrades(FILE *output_channel, CourseManager, int amount);

/* Prints a given amount of lowest effective grades in the *clean version* logged student's grades sheet
 * returns:
 * MANAGER_OUT_OF_MEMORY if a memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_NOT_LOGGED_IN if the student doesn't logged in
 * MANAGER_INVALID_ARGUMENT if one of the relevant arguments is invalid
 * otherwise MANAGER_SUCCESS
 */
ManagerResult
managerPrintLowestGrades(FILE *output_channel, CourseManager, int amount);

/* Prints student's reference sources according to his friends grades in the given course id.
 * returns:
 * MANAGER_OUT_OF_MEMORY if a memory problem occurred
 * MANAGER_NULL_ARGUMENT if one of the relevant arguments is NULL
 * MANAGER_NOT_LOGGED_IN if the student doesn't logged in
 * MANAGER_INVALID_ARGUMENT if one of the relevant arguments is invalid
 * otherwise MANAGER_SUCCESS
 */
ManagerResult
managerPrintReferences(FILE *output_channel, CourseManager, int course_id,
                       int amount);

/* Destroys the given Course Manager
 * dealloactes all associated memory
 */
void managerDestroy(CourseManager);


#endif //ASSIGNMENT3_COURSEMANAGER_H
