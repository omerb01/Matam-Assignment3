//
// Created by Omer on 15/12/2017.
//

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mtm_ex3.h"
#include "Student.h"
#include "CourseManager.h"
#include "UsefulFunctions.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~ STRUCTURE DECLARATION ~~~~~~~~~~~~~~~~~~~~~~~~~

struct CourseManager_t {
    int current_logged_id;
    Set students;
    Graph friendships;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ CONVERTERS MACROS ~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define CONVERT_SET_RESULT_TO_MANAGER_RESULT($$set_error$$) \
    switch ($$set_error$$) { \
        case SET_SUCCESS: \
            /* DO NOTHING */ \
            break; \
        case SET_OUT_OF_MEMORY: \
            return MANAGER_OUT_OF_MEMORY; \
        case SET_ITEM_DOES_NOT_EXIST: \
            return MANAGER_STUDENT_DOES_NOT_EXIST; \
        case SET_ITEM_ALREADY_EXISTS: \
            return MANAGER_STUDENT_ALREADY_EXISTS; \
        default: \
            assert(false); \
    }

#define CONVERT_GRAPH_RESULT_TO_MANAGER_RESULT($$graph_error$$) \
    switch ($$graph_error$$) { \
        case GRAPH_SUCCCESS: \
            /* DO NOTHING */ \
            break; \
        case GRAPH_OUT_OF_MEMORY: \
            return MANAGER_OUT_OF_MEMORY; \
        case GRAPH_VERTEX_ALREADY_EXISTS: \
            return MANAGER_STUDENT_ALREADY_EXISTS; \
        case GRAPH_VERTEX_DOES_NOT_EXIST: \
            return MANAGER_STUDENT_DOES_NOT_EXIST; \
        case GRAPH_EDGE_DOES_NOT_EXIST: \
            return MANAGER_NOT_FRIEND; \
        case GRAPH_EDGE_ALREADY_EXISTS: \
            return MANAGER_ALREADY_FRIEND; \
        default: \
            assert(false); \
    }

#define CONVERT_STUDENT_RESULT_TO_MANAGER_RESULT($$student_error$$) \
    switch ($$student_error$$) { \
        case STUDENT_SUCCESS: \
            /* DO NOTHING */ \
            break; \
        case STUDENT_OUT_OF_MEMORY: \
            return MANAGER_OUT_OF_MEMORY; \
        case STUDENT_INVALID_ARGUMENT: \
            return MANAGER_INVALID_ARGUMENT; \
        case STUNDET_ALREADY_SENT_REQUEST: \
            return MANAGER_ALREADY_REQUESTED; \
        case STUDENT_DIDNT_SEND_REQUEST: \
            return MANAGER_NOT_REQUESTED; \
        case STUDENT_GRADE_DOES_NOT_EXIST: \
            return MANAGER_COURSE_DOES_NOT_EXIST; \
        default: \
            assert(false); \
    }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ USEFUL MACROS ~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define CREATE_DEMI_STUDENT($$id$$) \
    studentCreate($$id$$, "Demi", "Demi")

#define STORE_LOGGED_STUDENT($$manager$$, $$variable$$) \
    $$variable$$ = NULL; \
    ManagerResult $$manager_error$$ = \
        findStudentById($$manager$$, ($$manager$$)->current_logged_id, \
                        &($$variable$$)); \
    if ($$manager_error$$ == MANAGER_OUT_OF_MEMORY) \
        {return MANAGER_OUT_OF_MEMORY;} \
    assert($$manager_error$$ == MANAGER_SUCCESS)

// ~~~~~~~~~~~~~~~~~~~ USER-INTERFACE STATIC FUNCTIONS ~~~~~~~~~~~~~~~~~~~

static SetElement studentCopyForSet(SetElement element) {
    assert(element != NULL);
    Student student = (Student) element;

    Student student_copy = studentCopy(student);
    if (student_copy == NULL) return NULL;
    return (SetElement) student_copy;
}

static void studentFreeForSet(SetElement element) {
    if (element == NULL) return;
    Student student = (SetElement) element;
    studentDestroy(student);
}

static int studentCompareForSet(SetElement element1, SetElement element2) {
    assert(element1 != NULL && element2 != NULL);
    Student student1 = (Student) element1;
    Student student2 = (Student) element2;

    return studentCompare(student1, student2);
}

static VertexLabel copyInt(VertexLabel label) {
    assert(label != NULL);
    int *num = (int *) label;

    int *num_copy = malloc(sizeof(int));
    if (num_copy == NULL) return NULL;

    *num_copy = *num;
    return (VertexLabel) num_copy;
}

static void freeInt(VertexLabel label) {
    if (label == NULL) return;

    free(label);
}

static int compareInt(VertexLabel label1, VertexLabel label2) {
    assert(label1 != NULL && label2 != NULL);
    int *num1 = (int *) label1;
    int *num2 = (int *) label2;

    if (*num1 == *num2) return 0;
    if (*num1 > *num2) return 1;
    return -1;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~ GENERAL STATIC FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~

static bool isStudentLogged(CourseManager manager) {
    assert(manager != NULL);

    if (manager->current_logged_id != INVALID_ID) return true;
    return false;
}

static ManagerResult findStudentById(CourseManager manager, int id,
                                     Student *result) {
    assert(manager != NULL);

    Student student = CREATE_DEMI_STUDENT(id);
    if (student == NULL) return MANAGER_OUT_OF_MEMORY;

    SET_FOREACH(Student, iterator, manager->students) {
        if (studentCompare(iterator, student) == 0) {
            studentDestroy(student);
            *result = iterator;
            return MANAGER_SUCCESS;
        }
    }

    studentDestroy(student);
    return MANAGER_STUDENT_DOES_NOT_EXIST;
}

// ~~~~~~~~~~~~~~~~~~~~~ COURSE MANAGER IMPLEMENTATION ~~~~~~~~~~~~~~~~~~~~~

CourseManager managerCreate() {
    CourseManager manager = MALLOC(manager);
    manager->current_logged_id = INVALID_ID;
    manager->students = setCreate(studentCopyForSet, studentFreeForSet,
                                  studentCompareForSet);
    manager->friendships = graphCreate(copyInt, freeInt, compareInt);

    if (manager->students == NULL || manager->friendships == NULL) {
        managerDestroy(manager);
        return NULL;
    }

    return manager;
}

ManagerResult
managerAddStudent(CourseManager manager, int id, char *first_name,
                  char *last_name) {
    if (manager == NULL || first_name == NULL || last_name == NULL) {
        return MANAGER_NULL_ARGUMENT;
    }

    Student student = studentCreate(id, first_name, last_name);
    if (student == NULL) return MANAGER_OUT_OF_MEMORY;

    SetResult set_error = setAdd(manager->students, student);
    if (set_error != SET_SUCCESS) studentDestroy(student);
    CONVERT_SET_RESULT_TO_MANAGER_RESULT(set_error);

    if (!isValidId(id)) {
        studentDestroy(student);
        return MANAGER_INVALID_ARGUMENT;
    }

    GraphResult graph_error = graphAddVertex(manager->friendships, &id);
    if (graph_error != GRAPH_SUCCCESS) {
        set_error = setRemove(manager->students, student);
        assert(set_error == SET_SUCCESS);
        studentDestroy(student);
    }
    CONVERT_GRAPH_RESULT_TO_MANAGER_RESULT(graph_error);

    studentDestroy(student);
    return MANAGER_SUCCESS;
}

ManagerResult managerRemoveStudent(CourseManager manager, int id) {
    if (manager == NULL) return MANAGER_NULL_ARGUMENT;

    Student student = CREATE_DEMI_STUDENT(id);
    if (student == NULL) return MANAGER_OUT_OF_MEMORY;

    GraphResult graph_error = graphRemoveVertex(manager->friendships, &id);
    if (graph_error != GRAPH_SUCCCESS) studentDestroy(student);
    CONVERT_GRAPH_RESULT_TO_MANAGER_RESULT(graph_error);

    if (!isValidId(id)) {
        studentDestroy(student);
        return MANAGER_INVALID_ARGUMENT;
    }

    StudentResult student_error;
    SET_FOREACH(Student, iterator, manager->students) {
        student_error = studentRemoveSentFriendRequest(iterator, student);
        assert(student_error == STUDENT_SUCCESS ||
               student_error == STUDENT_DIDNT_SEND_REQUEST);
    }

    SetResult set_error = setRemove(manager->students, student);
    if (set_error != SET_SUCCESS) studentDestroy(student);
    CONVERT_SET_RESULT_TO_MANAGER_RESULT(set_error);

    ManagerResult manager_error;
    if (manager->current_logged_id == id) {
        manager_error = managerLogout(manager);
        assert(manager_error == MANAGER_SUCCESS);
    }

    studentDestroy(student);
    return MANAGER_SUCCESS;
}

ManagerResult managerLogin(CourseManager manager, int id) {
    if (manager == NULL) return MANAGER_NULL_ARGUMENT;

    if (isStudentLogged(manager)) {
        return MANAGER_ALREADY_LOGGED_IN;
    }

    Student student;
    ManagerResult manager_error = findStudentById(manager, id, &student);
    if (manager_error == MANAGER_OUT_OF_MEMORY) return MANAGER_OUT_OF_MEMORY;
    if (manager_error == MANAGER_STUDENT_DOES_NOT_EXIST) {
        return MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    assert(manager_error == MANAGER_SUCCESS);

    if (!isValidId(id)) return MANAGER_INVALID_ARGUMENT;

    manager->current_logged_id = id;
    return MANAGER_SUCCESS;
}

ManagerResult managerLogout(CourseManager manager) {
    if (manager == NULL) return MANAGER_NULL_ARGUMENT;

    if (!isStudentLogged(manager)) return MANAGER_NOT_LOGGED_IN;

    manager->current_logged_id = INVALID_ID;
    return MANAGER_SUCCESS;
}

ManagerResult
managerFacultyRequest(FILE *output_channel, CourseManager manager,
                      int course_id, char *request) {
    if (output_channel == NULL || manager == NULL || request == NULL) {
        return MANAGER_NULL_ARGUMENT;
    }

    if (!isStudentLogged(manager)) return MANAGER_NOT_LOGGED_IN;

    Student logged_student;
    STORE_LOGGED_STUDENT(manager, logged_student);

    StudentResult student_error;
    if (strcmp(request, "remove_course") == 0) {
        bool is_course_done;
        student_error = studentIsCourseDone(logged_student, course_id,
                                            &is_course_done);
        assert(student_error == STUDENT_SUCCESS ||
                       student_error == STUDENT_INVALID_ARGUMENT);
        if (!is_course_done || student_error == STUDENT_INVALID_ARGUMENT) {
            return MANAGER_COURSE_DOES_NOT_EXIST;
        }
    }

    if (!isValidCourseId(course_id) || !isValidRequest(request)) {
        return MANAGER_INVALID_ARGUMENT;
    }

    mtmFacultyResponse(output_channel, "your request was rejected");

    return MANAGER_SUCCESS;
}

ManagerResult managerSendFriendRequest(CourseManager manager,
                                       int id_to_request) {
    if (manager == NULL) return MANAGER_NULL_ARGUMENT;
    if (!isStudentLogged(manager)) return MANAGER_NOT_LOGGED_IN;

    ManagerResult manager_error;
    Student requested = NULL;
    manager_error = findStudentById(manager, id_to_request, &requested);
    if (manager_error == MANAGER_OUT_OF_MEMORY) return MANAGER_OUT_OF_MEMORY;
    if (manager_error == MANAGER_STUDENT_DOES_NOT_EXIST) {
        return MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    assert(manager_error == MANAGER_SUCCESS);

    Student logged_student;
    STORE_LOGGED_STUDENT(manager, logged_student);

    if (id_to_request == manager->current_logged_id) {
        return MANAGER_ALREADY_FRIEND;
    }
    bool is_already_friend;
    GraphResult graph_error = edgeExists(manager->friendships,
                                         &manager->current_logged_id,
                                         &id_to_request, &is_already_friend);
    assert(graph_error == GRAPH_SUCCCESS || graph_error == GRAPH_OUT_OF_MEMORY);
    CONVERT_GRAPH_RESULT_TO_MANAGER_RESULT(graph_error);
    if (is_already_friend == true) return MANAGER_ALREADY_FRIEND;

    StudentResult student_error =
            studentSendFriendReuqest(logged_student, requested);
    CONVERT_STUDENT_RESULT_TO_MANAGER_RESULT(student_error);
    return MANAGER_SUCCESS;
}

ManagerResult
managerHandleFriendRequest(CourseManager manager, int id_waiting_for_response,
                           char *action) {
    if (manager == NULL || action == NULL) return MANAGER_NULL_ARGUMENT;

    if (!isStudentLogged(manager)) return MANAGER_NOT_LOGGED_IN;

    ManagerResult manager_error;
    GraphResult graph_error;
    StudentResult student_error;
    bool result;

    Student sender = NULL;
    manager_error = findStudentById(manager, id_waiting_for_response, &sender);
    if (manager_error == MANAGER_OUT_OF_MEMORY) return MANAGER_OUT_OF_MEMORY;
    if (manager_error == MANAGER_STUDENT_DOES_NOT_EXIST) {
        return MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    assert(manager_error == MANAGER_SUCCESS);

    graph_error = edgeExists(manager->friendships, &id_waiting_for_response,
                             &manager->current_logged_id, &result);
    CONVERT_GRAPH_RESULT_TO_MANAGER_RESULT(graph_error);
    if (result == true) return MANAGER_ALREADY_FRIEND;

    Student logged_student;
    STORE_LOGGED_STUDENT(manager, logged_student);

    if (!isValidId(id_waiting_for_response)) {
        return MANAGER_STUDENT_DOES_NOT_EXIST;
    }

    student_error = studentIsSentFriendRequest(sender, logged_student,
                                               &result);
    assert(student_error == STUDENT_SUCCESS);
    if(result == false) return MANAGER_NOT_REQUESTED;

    if (!isValidAction(action)) return MANAGER_INVALID_ARGUMENT;

    student_error = studentRemoveSentFriendRequest(sender, logged_student);
    assert(student_error != STUDENT_INVALID_ARGUMENT);
    CONVERT_STUDENT_RESULT_TO_MANAGER_RESULT(student_error);
    if (strcmp(action, "reject") == 0) return MANAGER_SUCCESS;

    student_error = studentRemoveSentFriendRequest(logged_student, sender);
    assert(student_error == STUDENT_SUCCESS ||
           student_error == STUDENT_DIDNT_SEND_REQUEST);

    graph_error = graphAddEdge(manager->friendships, &id_waiting_for_response,
                               &manager->current_logged_id);
    CONVERT_GRAPH_RESULT_TO_MANAGER_RESULT(graph_error);
    return MANAGER_SUCCESS;
}

ManagerResult managerUnfriend(CourseManager manager, int id_to_unfriend) {
    if (manager == NULL) return MANAGER_NULL_ARGUMENT;

    if (!isStudentLogged(manager)) return MANAGER_NOT_LOGGED_IN;

    ManagerResult manager_error;
    GraphResult graph_error;

    Student student_to_unfriend = NULL;
    manager_error = findStudentById(manager, id_to_unfriend,
                                    &student_to_unfriend);
    if (manager_error == MANAGER_OUT_OF_MEMORY) return MANAGER_OUT_OF_MEMORY;
    if (manager_error == MANAGER_STUDENT_DOES_NOT_EXIST) {
        return MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    assert(manager_error == MANAGER_SUCCESS);

    graph_error = graphRemoveEdge(manager->friendships,
                                  &manager->current_logged_id, &id_to_unfriend);
    CONVERT_GRAPH_RESULT_TO_MANAGER_RESULT(graph_error);

    if (!isValidId(id_to_unfriend)) return MANAGER_INVALID_ARGUMENT;

    return MANAGER_SUCCESS;
}

ManagerResult
managerAddGrade(CourseManager manager, int semester, int course_id,
                int points_x2, int grade_value) {
    if (manager == NULL) return MANAGER_NULL_ARGUMENT;

    if (!isStudentLogged(manager)) return MANAGER_NOT_LOGGED_IN;

    Student logged_student;
    STORE_LOGGED_STUDENT(manager, logged_student);

    StudentResult student_error = studentAddGrade(logged_student, semester,
                                                  course_id, points_x2,
                                                  grade_value);
    CONVERT_STUDENT_RESULT_TO_MANAGER_RESULT(student_error);
    return MANAGER_SUCCESS;
}

ManagerResult
managerRemoveLastGrade(CourseManager manager, int semester, int course_id) {
    if (manager == NULL) return MANAGER_NULL_ARGUMENT;

    if (!isStudentLogged(manager)) return MANAGER_NOT_LOGGED_IN;

    Student logged_student;
    STORE_LOGGED_STUDENT(manager, logged_student);

    StudentResult student_error = studentRemoveLastGrade(logged_student,
                                                         semester,
                                                         course_id);
    if(student_error == STUDENT_INVALID_ARGUMENT) {
        return MANAGER_COURSE_DOES_NOT_EXIST;
    }
    CONVERT_STUDENT_RESULT_TO_MANAGER_RESULT(student_error);
    return MANAGER_SUCCESS;
}

ManagerResult
managerUpdateLastGrade(CourseManager manager, int course_id, int new_grade) {
    if (manager == NULL) return MANAGER_NULL_ARGUMENT;

    if (!isStudentLogged(manager)) return MANAGER_NOT_LOGGED_IN;

    Student logged_student;
    STORE_LOGGED_STUDENT(manager, logged_student);

    StudentResult student_error = studentUpdateLastGrade(logged_student,
                                                         course_id, new_grade);
    if(student_error == STUDENT_INVALID_ARGUMENT) {
        return MANAGER_COURSE_DOES_NOT_EXIST;
    }
    CONVERT_STUDENT_RESULT_TO_MANAGER_RESULT(student_error);
    return MANAGER_SUCCESS;
}

static ManagerResult managerPrintSheet(char *flag, FILE *output_channel,
                                       CourseManager manager) {
    if (output_channel == NULL || manager == NULL) return MANAGER_NULL_ARGUMENT;

    if (!isStudentLogged(manager)) return MANAGER_NOT_LOGGED_IN;

    Student logged_student;
    STORE_LOGGED_STUDENT(manager, logged_student);

    StudentResult student_error = STUDENT_SUCCESS;
    if (strcmp(flag, "full") == 0) {
        student_error = studentPrintFullSheet(output_channel, logged_student);
    } else if (strcmp(flag, "clean") == 0) {
        student_error = studentPrintCleanSheet(output_channel, logged_student);
    } else {
        assert(false);
    }

    CONVERT_STUDENT_RESULT_TO_MANAGER_RESULT(student_error);
    return MANAGER_SUCCESS;
}

ManagerResult managerPrintFullSheet(FILE *output_channel,
                                    CourseManager manager) {
    return managerPrintSheet("full", output_channel, manager);
}

ManagerResult managerPrintCleanSheet(FILE *output_channel,
                                     CourseManager manager) {
    return managerPrintSheet("clean", output_channel, manager);
}

static ManagerResult
managerPrintGrades(char *flag, FILE *output_channel, CourseManager manager,
                   int amount) {
    if (output_channel == NULL || manager == NULL) return MANAGER_NULL_ARGUMENT;

    if (!isStudentLogged(manager)) return MANAGER_NOT_LOGGED_IN;

    Student logged_student;
    STORE_LOGGED_STUDENT(manager, logged_student);

    StudentResult student_error = STUDENT_SUCCESS;
    if (strcmp(flag, "highest") == 0) {
        student_error = studentPrintHighestGrades(output_channel,
                                                  logged_student, amount);
    } else if (strcmp(flag, "lowest") == 0) {
        student_error = studentPrintLowestGrades(output_channel,
                                                 logged_student, amount);
    } else {
        assert(false);
    }

    CONVERT_STUDENT_RESULT_TO_MANAGER_RESULT(student_error);
    return MANAGER_SUCCESS;
}

ManagerResult
managerPrintHighestGrades(FILE *output_channel, CourseManager manager,
                          int amount) {
    return managerPrintGrades("highest", output_channel, manager, amount);
}

ManagerResult
managerPrintLowestGrades(FILE *output_channel, CourseManager manager,
                         int amount) {
    return managerPrintGrades("lowest", output_channel, manager, amount);
}

static Set buildFriendsSet(CourseManager manager, Set friends_ids) {
    assert(manager != NULL && friends_ids != NULL);

    Set friends = setCreate(dontCopyElement, dontFreeElement,
                            studentCompareForSet);
    if (friends == NULL) return NULL;

    SET_FOREACH(int*, iterator, friends_ids) {
        Student friend = NULL;
        ManagerResult manager_error =
                findStudentById(manager, *iterator, &friend);
        if (manager_error == MANAGER_OUT_OF_MEMORY) {
            setDestroy(friends);
            return NULL;
        }
        assert(manager_error == MANAGER_SUCCESS);

        SetResult set_error = setAdd(friends, friend);
        if (set_error == SET_OUT_OF_MEMORY) {
            setDestroy(friends);
            return NULL;
        }
        assert(set_error == SET_SUCCESS);
    }

    return friends;
}

ManagerResult
managerPrintReferences(FILE *output_channel, CourseManager manager,
                       int course_id, int amount) {
    if (output_channel == NULL || manager == NULL)return MANAGER_NULL_ARGUMENT;

    if (!isStudentLogged(manager)) return MANAGER_NOT_LOGGED_IN;

    Student logged_student;
    STORE_LOGGED_STUDENT(manager, logged_student);

    Set friends_ids = graphNeighbors(manager->friendships,
                                     &manager->current_logged_id);
    if (friends_ids == NULL) return MANAGER_OUT_OF_MEMORY;

    Set friends = buildFriendsSet(manager, friends_ids);
    setDestroy(friends_ids);
    if (friends == NULL) return MANAGER_OUT_OF_MEMORY;

    StudentResult student_error = studentPrintReferences(output_channel,
                                                         friends, course_id,
                                                         amount);
    setDestroy(friends);
    CONVERT_STUDENT_RESULT_TO_MANAGER_RESULT(student_error);
    return MANAGER_SUCCESS;
}

void managerDestroy(CourseManager manager) {
    if (manager == NULL) return;

    setDestroy(manager->students);
    graphDestroy(manager->friendships);
    free(manager);
}