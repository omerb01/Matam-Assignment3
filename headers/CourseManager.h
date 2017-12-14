//
// Created by Ilya on 12/14/2017.
//

#ifndef ASSIGNMENT3_COURSEMANAGER_H
#define ASSIGNMENT3_COURSEMANAGER_H
#include "Graph.h"
#include "Student.h"

typedef struct CourseManager_t *CourseManager;
typedef enum ManagerResult_t{
    MANAGER_OUT_OF_MEMORY,
    MANAGER_INVALID_PARAMETERS,
    MANAGER_NOT_LOGGED_IN,
    MANAGER_ALREADY_LOGGED_IN,
    MANAGER_STUDENT_DOES_NOT_EXIST,
    MANAGER_STUDENT_ALREADY_EXISTS,
    MANAGER_NOT_FRIEND,
    MANAGER_ALREADY_FRIEND,
    MANAGER_NOT_REQUESTED,
    MANAGER_ALREADY_REQUESTED,
    MANAGER_COURSE_DOES_NOT_EXIST
} ManagerResult;

// enum error codes are defined in mtm_ex3.h
VertexLabel intCopy(VertexLabel);
VertexLabel intDestroy(VertexLabel);
VertexLabel intCompare(VertexLabel,VertexLabel);

CourseManager managerCreate(FILE* output_channel);

ManagerResult managerAddStudent(CourseManager ,int id, char* first_name, char* last_name);
ManagerResult managerRemoveStudent(CourseManager, int id);
ManagerResult managerLogin(CourseManager, int id);
ManagerResult managerLogout(CourseManager, int id);
ManagerResult managerFacultyRequest(CourseManager, int course_id, char* request);
ManagerResult managerSendFriendRequest(CourseManager, int id_to_request);
ManagerResult managerHandleFriendRequest(CourseManager, int id_waiting_for_reponse, char* action);


#endif //ASSIGNMENT3_COURSEMANAGER_H
