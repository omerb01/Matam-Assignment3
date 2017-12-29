//
// Created by Omer on 15/12/2017.
//

#include "test_utilities.h"
#include "../CourseManager.h"

static bool testManagerCreate() {
    CourseManager manager = managerCreate();
    managerDestroy(manager);
    return true;
}

static bool testManagerAddStudent() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;

    manager_error = managerAddStudent(NULL, 205795511, "Omer", "Belhasin");
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);
    manager_error = managerAddStudent(manager, 205795511, NULL, "Belhasin");
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);
    manager_error = managerAddStudent(manager, 205795511, "Omer", NULL);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);

    manager_error = managerAddStudent(manager, 2057955119, "Omer", "Belhasin");
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerAddStudent(manager, 0, "Omer", "Belhasin");
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerAddStudent(manager, -1, "Omer", "Belhasin");
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);

    manager_error = managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);
    manager_error = managerAddStudent(manager, 205795511, "LOL", "LOL");
    ASSERT_TEST(manager_error == MANAGER_STUDENT_ALREADY_EXISTS);
    manager_error = managerAddStudent(manager, 111111111, "Shlomo", "Artzi");
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);

    managerDestroy(manager);
    return true;
}

static bool testManagerRemoveStudent() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;

    manager_error = managerRemoveStudent(NULL, 205795511);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);
    manager_error = managerRemoveStudent(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);

    manager_error = managerRemoveStudent(manager, 2057955118);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_error = managerRemoveStudent(manager, 0);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_error = managerRemoveStudent(manager, -1);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    manager_error = managerRemoveStudent(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);
    manager_error = managerRemoveStudent(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    managerAddStudent(manager, 111111111, "Shlomo", "Artzi");
    managerAddStudent(manager, 222222222, "Omer", "Adam");
    managerAddStudent(manager, 333333333, "Aviv", "Gefen");

    managerLogin(manager, 111111111);
    managerSendFriendRequest(manager, 205795511);
    managerLogout(manager);

    managerLogin(manager, 222222222);
    managerSendFriendRequest(manager, 205795511);
    managerLogout(manager);

    managerLogin(manager, 333333333);
    managerSendFriendRequest(manager, 205795511);
    managerLogout(manager);

    managerLogin(manager, 205795511);
    managerHandleFriendRequest(manager, 111111111, "accept");
    managerHandleFriendRequest(manager, 222222222, "accept");
    managerSendFriendRequest(manager, 333333333);

    manager_error = managerRemoveStudent(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);
    manager_error = managerRemoveStudent(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_error = managerHandleFriendRequest(manager, 333333333, "accept");
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    managerLogin(manager, 205795511);
    manager_error = managerSendFriendRequest(manager, 333333333);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);
    manager_error = managerSendFriendRequest(manager, 111111111);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);
    manager_error = managerSendFriendRequest(manager, 222222222);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);
    managerLogout(manager);

    managerLogin(manager, 333333333);
    manager_error = managerSendFriendRequest(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);

    managerDestroy(manager);
    return true;
}

static bool testManagerLogin() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;

    manager_error = managerLogin(NULL, 205795511);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);
    manager_error = managerLogin(manager, 2057955118);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_error = managerLogin(manager, 0);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_error = managerLogin(manager, -1);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_error = managerLogin(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    manager_error = managerLogin(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);
    manager_error = managerLogin(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_ALREADY_LOGGED_IN);

    managerAddStudent(manager, 111111111, "TEST", "TEST");
    manager_error = managerLogin(manager, 111111111);
    ASSERT_TEST(manager_error == MANAGER_ALREADY_LOGGED_IN);

    managerDestroy(manager);
    return true;
}

static bool testManagerLogout() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;

    manager_error = managerLogout(NULL);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);
    manager_error = managerLogout(manager);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    manager_error = managerLogout(manager);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerLogin(manager, 205795511);
    manager_error = managerLogout(manager);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);
    manager_error = managerLogout(manager);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerDestroy(manager);
    return true;
}

static bool testManagerFacultyRequest() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;
    FILE *output_stream = stdout;

    manager_error = managerFacultyRequest(NULL, manager, 234122,
                                          "register_course");
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);
    manager_error = managerFacultyRequest(output_stream, NULL, 234122,
                                          "register_course");
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);
    manager_error = managerFacultyRequest(output_stream, manager, 234122,
                                          NULL);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);

    manager_error = managerFacultyRequest(output_stream, manager, 234122444,
                                          "remove_course");
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerFacultyRequest(output_stream, manager, 0,
                                          "remove_course");
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerFacultyRequest(output_stream, manager, -1,
                                          "remove_course");
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerFacultyRequest(output_stream, manager, 234122,
                                          "remove");
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    manager_error = managerFacultyRequest(output_stream, manager, 234122,
                                          "remove_course");
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    manager_error = managerFacultyRequest(output_stream, manager, 234122,
                                          "remove_course");
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerLogin(manager, 205795511);
    manager_error = managerFacultyRequest(output_stream, manager, 234122444,
                                          "remove_course");
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);
    manager_error = managerFacultyRequest(output_stream, manager, 0,
                                          "remove_course");
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);
    manager_error = managerFacultyRequest(output_stream, manager, -1,
                                          "remove_course");
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);
    manager_error = managerFacultyRequest(output_stream, manager, 234122,
                                          "remove");
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);

    manager_error = managerFacultyRequest(output_stream, manager, 234122,
                                          "remove_course");
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);

    printf("\n");
    manager_error = managerFacultyRequest(output_stream, manager, 234122,
                                          "register_course");
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);

    managerAddGrade(manager, 1, 234122, 3, 90);
    manager_error = managerFacultyRequest(output_stream, manager, 234122,
                                          "cancel_course");
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);

    managerDestroy(manager);
    return true;
}

static bool testManagerSendFriendRequest() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;

    manager_error = managerSendFriendRequest(NULL, 205795511);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);

    manager_error = managerSendFriendRequest(manager, 2057955118);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerSendFriendRequest(manager, 0);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerSendFriendRequest(manager, -1);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    manager_error = managerSendFriendRequest(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    manager_error = managerSendFriendRequest(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerLogin(manager, 205795511);
    manager_error = managerSendFriendRequest(manager, 2057955118);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_error = managerSendFriendRequest(manager, 0);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_error = managerSendFriendRequest(manager, -1);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);

    manager_error = managerSendFriendRequest(manager, 205795512);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_error = managerSendFriendRequest(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_ALREADY_FRIEND);

    managerAddStudent(manager, 111111111, "Shlomo", "Artzi");
    manager_error = managerSendFriendRequest(manager, 111111111);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);
    manager_error = managerSendFriendRequest(manager, 111111111);
    ASSERT_TEST(manager_error == MANAGER_ALREADY_REQUESTED);

    managerDestroy(manager);
    return true;
}

static bool testManagerHandleFriendRequest() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;

    manager_error = managerHandleFriendRequest(NULL, 205795511, "accept");
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);
    manager_error = managerHandleFriendRequest(manager, 205795511, NULL);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);

    manager_error = managerHandleFriendRequest(manager, 2057955116, "accept");
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerHandleFriendRequest(manager, 0, "accept");
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerHandleFriendRequest(manager, -1, "accept");
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerHandleFriendRequest(manager, 205795511, "accepted");
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    manager_error = managerHandleFriendRequest(manager, 205795511, "accept");
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    manager_error = managerHandleFriendRequest(manager, 205795511, "accept");
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerLogin(manager, 205795511);
    manager_error = managerHandleFriendRequest(manager, 2057955116, "accept");
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_error = managerHandleFriendRequest(manager, 0, "accept");
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_error = managerHandleFriendRequest(manager, -1, "accept");
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);

    manager_error = managerHandleFriendRequest(manager, 205795511, "accepted");
    ASSERT_TEST(manager_error == MANAGER_NOT_REQUESTED);
    manager_error = managerHandleFriendRequest(manager, 205795511, "accept");
    ASSERT_TEST(manager_error == MANAGER_NOT_REQUESTED);

    managerAddStudent(manager, 111111111, "Shlomo", "Artzi");
    manager_error = managerHandleFriendRequest(manager, 111111111, "accept");
    ASSERT_TEST(manager_error == MANAGER_NOT_REQUESTED);

    managerSendFriendRequest(manager, 111111111);
    managerLogout(manager);
    managerLogin(manager, 111111111);
    managerSendFriendRequest(manager, 205795511);
    manager_error = managerHandleFriendRequest(manager, 205795511, "accepted");
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerHandleFriendRequest(manager, 205795511, "reject");
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);
    manager_error = managerSendFriendRequest(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_ALREADY_REQUESTED);
    managerLogout(manager);
    managerLogin(manager, 205795511);
    manager_error = managerSendFriendRequest(manager, 111111111);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);

    manager_error = managerHandleFriendRequest(manager, 111111111, "accept");
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);
    manager_error = managerSendFriendRequest(manager, 111111111);
    ASSERT_TEST(manager_error == MANAGER_ALREADY_FRIEND);
    managerLogout(manager);
    managerLogin(manager, 111111111);
    manager_error = managerSendFriendRequest(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_ALREADY_FRIEND);

    managerDestroy(manager);
    return true;
}

static bool testManagerUnfriend() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;

    manager_error = managerUnfriend(NULL, 205795511);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);

    manager_error = managerUnfriend(manager, 2057955118);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerUnfriend(manager, 0);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerUnfriend(manager, -1);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    manager_error = managerUnfriend(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");

    manager_error = managerUnfriend(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerLogin(manager, 205795511);
    manager_error = managerUnfriend(manager, 2057955118);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_error = managerUnfriend(manager, 0);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_error = managerUnfriend(manager, -1);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);

    manager_error = managerUnfriend(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_NOT_FRIEND);
    manager_error = managerUnfriend(manager, 111111111);
    ASSERT_TEST(manager_error == MANAGER_STUDENT_DOES_NOT_EXIST);

    managerAddStudent(manager, 111111111, "Shlomo", "Artzi");
    manager_error = managerUnfriend(manager, 111111111);
    ASSERT_TEST(manager_error == MANAGER_NOT_FRIEND);

    managerSendFriendRequest(manager, 111111111);
    managerLogout(manager);
    managerLogin(manager, 111111111);
    managerHandleFriendRequest(manager, 205795511, "accept");
    manager_error = managerUnfriend(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);
    manager_error = managerUnfriend(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_NOT_FRIEND);

    managerSendFriendRequest(manager, 205795511);
    managerLogout(manager);
    managerLogin(manager, 205795511);
    managerHandleFriendRequest(manager, 111111111, "accept");
    managerLogout(manager);
    managerLogin(manager, 111111111);
    manager_error = managerUnfriend(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);
    manager_error = managerUnfriend(manager, 205795511);
    ASSERT_TEST(manager_error == MANAGER_NOT_FRIEND);

    managerDestroy(manager);
    return true;
}

static bool testManagerAddGrade() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;

    manager_error = managerAddGrade(NULL, 1, 234122, 6, 80);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);

    manager_error = managerAddGrade(manager, 0, 234122, 6, 80);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerAddGrade(manager, -1, 234122, 6, 80);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerAddGrade(manager, 1, 234122888, 6, 80);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerAddGrade(manager, 1, 0, 6, 80);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerAddGrade(manager, 1, -1, 6, 80);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerAddGrade(manager, 1, 234122, -1, 80);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerAddGrade(manager, 1, 234122, 6, 101);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerAddGrade(manager, 1, 234122, 6, -1);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    manager_error = managerAddGrade(manager, 1, 234122, 6, 80);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    managerLogin(manager, 205795511);

    manager_error = managerAddGrade(manager, 0, 234122, 6, 80);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerAddGrade(manager, -1, 234122, 6, 80);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerAddGrade(manager, 1, 234122888, 6, 80);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerAddGrade(manager, 1, 0, 6, 80);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerAddGrade(manager, 1, -1, 6, 80);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerAddGrade(manager, 1, 234122, -1, 80);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerAddGrade(manager, 1, 234122, 6, 101);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerAddGrade(manager, 1, 234122, 6, -1);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);

    manager_error = managerAddGrade(manager, 1, 234122, 6, 80);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);

    managerDestroy(manager);
    return true;
}

static bool testManagerRemoveLastGrade() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;

    manager_error = managerRemoveLastGrade(NULL, 1, 234122);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);

    manager_error = managerRemoveLastGrade(manager, 0, 234122);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerRemoveLastGrade(manager, -1, 234122);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerRemoveLastGrade(manager, 1, 234122999);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerRemoveLastGrade(manager, 1, 0);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerRemoveLastGrade(manager, 1, -1);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    manager_error = managerRemoveLastGrade(manager, 1, 234122);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    managerLogin(manager, 205795511);

    manager_error = managerRemoveLastGrade(manager, 0, 234122);
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);
    manager_error = managerRemoveLastGrade(manager, -1, 234122);
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);
    manager_error = managerRemoveLastGrade(manager, 1, 234122999);
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);
    manager_error = managerRemoveLastGrade(manager, 1, 0);
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);
    manager_error = managerRemoveLastGrade(manager, 1, -1);
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);

    manager_error = managerRemoveLastGrade(manager, 1, 234122);
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);

    managerAddGrade(manager, 1, 234122, 6, 80);
    manager_error = managerRemoveLastGrade(manager, 1, 234122);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);

    manager_error = managerRemoveLastGrade(manager, 1, 234122);
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);

    managerDestroy(manager);
    return true;
}

static bool testManagerUpdateLastGrade() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;

    manager_error = managerUpdateLastGrade(NULL, 234122, 90);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);

    manager_error = managerUpdateLastGrade(manager, 234122888, 90);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerUpdateLastGrade(manager, 0, 90);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerUpdateLastGrade(manager, -1, 90);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerUpdateLastGrade(manager, 234122, 101);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerUpdateLastGrade(manager, 234122, -1);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    manager_error = managerUpdateLastGrade(manager, 234122, 90);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    managerLogin(manager, 205795511);

    manager_error = managerUpdateLastGrade(manager, 234122888, 90);
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);
    manager_error = managerUpdateLastGrade(manager, 0, 90);
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);
    manager_error = managerUpdateLastGrade(manager, -1, 90);
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);
    manager_error = managerUpdateLastGrade(manager, 234122, 101);
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);
    manager_error = managerUpdateLastGrade(manager, 234122, -1);
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);

    manager_error = managerUpdateLastGrade(manager, 234122, 90);
    ASSERT_TEST(manager_error == MANAGER_COURSE_DOES_NOT_EXIST);

    managerAddGrade(manager, 1, 234122, 6, 80);
    manager_error = managerUpdateLastGrade(manager, 234122, 90);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);

    managerDestroy(manager);
    return true;
}

static bool testManagerPrintFullSheet() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;
    FILE *output_stream = stdout;

    manager_error = managerPrintFullSheet(NULL, manager);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);
    manager_error = managerPrintFullSheet(output_stream, NULL);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);

    manager_error = managerPrintFullSheet(output_stream, manager);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    managerLogin(manager, 205795511);

    managerAddGrade(manager, 4, 234247, 6, 95);
    managerAddGrade(manager, 4, 234267, 6, 60);
    managerAddGrade(manager, 4, 234267, 6, 68);
    managerAddGrade(manager, 4, 236353, 6, 78);
    managerAddGrade(manager, 4, 394902, 3, 92);
    managerAddGrade(manager, 5, 234107, 8, 51);
    managerAddGrade(manager, 5, 234107, 8, 63);
    managerAddGrade(manager, 5, 234900, 4, 74);
    managerAddGrade(manager, 5, 236327, 6, 77);

    printf("\n");
    manager_error = managerPrintFullSheet(output_stream, manager);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);

    managerDestroy(manager);
    return true;
}

static bool testManagerPrintCleanSheet() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;
    FILE *output_stream = stdout;

    manager_error = managerPrintCleanSheet(NULL, manager);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);
    manager_error = managerPrintCleanSheet(output_stream, NULL);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);

    manager_error = managerPrintCleanSheet(output_stream, manager);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    managerLogin(manager, 205795511);

    managerAddGrade(manager, 4, 234247, 6, 95);
    managerAddGrade(manager, 4, 234267, 6, 60);
    managerAddGrade(manager, 4, 234267, 6, 68);
    managerAddGrade(manager, 4, 236353, 6, 78);
    managerAddGrade(manager, 4, 394902, 3, 92);
    managerAddGrade(manager, 5, 234107, 8, 51);
    managerAddGrade(manager, 5, 234107, 8, 63);
    managerAddGrade(manager, 5, 234900, 4, 74);
    managerAddGrade(manager, 5, 236327, 6, 77);

    printf("\n");
    manager_error = managerPrintCleanSheet(output_stream, manager);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);

    managerDestroy(manager);
    return true;
}

static bool testManagerPrintHighestGrades() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;
    FILE *output_stream = stdout;

    manager_error = managerPrintHighestGrades(NULL, manager, 2);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);
    manager_error = managerPrintHighestGrades(output_stream, NULL, 2);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);

    manager_error = managerPrintHighestGrades(output_stream, manager, 0);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerPrintHighestGrades(output_stream, manager, -1);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    manager_error = managerPrintHighestGrades(output_stream, manager, 2);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    managerLogin(manager, 205795511);

    manager_error = managerPrintHighestGrades(output_stream, manager, 0);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerPrintHighestGrades(output_stream, manager, -1);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);

    managerAddGrade(manager, 4, 234247, 6, 95);
    managerAddGrade(manager, 4, 234267, 6, 60);
    managerAddGrade(manager, 4, 234267, 6, 68);
    managerAddGrade(manager, 4, 236353, 6, 78);
    managerAddGrade(manager, 4, 394902, 3, 92);
    managerAddGrade(manager, 5, 234107, 8, 51);
    managerAddGrade(manager, 5, 234107, 8, 63);
    managerAddGrade(manager, 5, 234900, 4, 74);
    managerAddGrade(manager, 5, 236327, 6, 77);

    printf("\n");
    manager_error = managerPrintHighestGrades(output_stream, manager, 2);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);

    managerDestroy(manager);
    return true;
}

static bool testManagerPrintLowestGrades() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;
    FILE *output_stream = stdout;

    manager_error = managerPrintLowestGrades(NULL, manager, 2);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);
    manager_error = managerPrintLowestGrades(output_stream, NULL, 2);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);

    manager_error = managerPrintLowestGrades(output_stream, manager, 0);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerPrintLowestGrades(output_stream, manager, -1);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    manager_error = managerPrintLowestGrades(output_stream, manager, 2);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    managerLogin(manager, 205795511);

    manager_error = managerPrintLowestGrades(output_stream, manager, 0);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerPrintLowestGrades(output_stream, manager, -1);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);

    managerAddGrade(manager, 4, 234247, 6, 95);
    managerAddGrade(manager, 4, 234267, 6, 60);
    managerAddGrade(manager, 4, 234267, 6, 68);
    managerAddGrade(manager, 4, 236353, 6, 78);
    managerAddGrade(manager, 4, 394902, 3, 92);
    managerAddGrade(manager, 5, 234107, 8, 51);
    managerAddGrade(manager, 5, 234107, 8, 63);
    managerAddGrade(manager, 5, 234900, 4, 74);
    managerAddGrade(manager, 5, 236327, 6, 77);

    printf("\n");
    manager_error = managerPrintLowestGrades(output_stream, manager, 2);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);

    managerDestroy(manager);
    return true;
}

static bool testManagerPrintReferences() {
    CourseManager manager = managerCreate();
    ManagerResult manager_error;
    FILE *output_stream = stdout;

    manager_error = managerPrintReferences(NULL, manager, 234122, 2);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);
    manager_error = managerPrintReferences(output_stream, NULL, 234122, 2);
    ASSERT_TEST(manager_error == MANAGER_NULL_ARGUMENT);

    manager_error = managerPrintReferences(output_stream, manager, 234122, 0);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerPrintReferences(output_stream, manager, 234122, -1);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerPrintReferences(output_stream, manager, 234122888,
                                           2);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerPrintReferences(output_stream, manager, 0, 2);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);
    manager_error = managerPrintReferences(output_stream, manager, -1, 2);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    manager_error = managerPrintReferences(output_stream, manager, 234122, 2);
    ASSERT_TEST(manager_error == MANAGER_NOT_LOGGED_IN);

    managerAddStudent(manager, 205795511, "Omer", "Belhasin");
    managerLogin(manager, 205795511);
    manager_error = managerPrintReferences(output_stream, manager, 234122, 0);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerPrintReferences(output_stream, manager, 234122, -1);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerPrintReferences(output_stream, manager, 234122888,
                                           2);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerPrintReferences(output_stream, manager, 0, 2);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);
    manager_error = managerPrintReferences(output_stream, manager, -1, 2);
    ASSERT_TEST(manager_error == MANAGER_INVALID_ARGUMENT);

    managerAddGrade(manager, 7, 104174, 7, 25);
    managerAddGrade(manager, 7, 104174, 7, 90);
    managerAddGrade(manager, 7, 236350, 6, 84);
    managerAddGrade(manager, 7, 236503, 6, 71);
    managerAddGrade(manager, 7, 236603, 4, 71);
    managerAddGrade(manager, 7, 394902, 3, 98);
    managerAddGrade(manager, 7, 236703, 6, 100);

    managerAddStudent(manager, 111111111, "Shlomo", "Artzi");
    managerLogout(manager);
    managerLogin(manager, 111111111);
    managerSendFriendRequest(manager, 205795511);
    managerAddGrade(manager, 6, 94591, 7, 86);
    managerAddGrade(manager, 6, 234319, 6, 93);
    managerAddGrade(manager, 6, 236370, 6, 90);
    managerAddGrade(manager, 6, 236504, 6, 89);
    managerAddGrade(manager, 6, 236604, 4, 65);
    managerAddGrade(manager, 6, 236653, 6, 87);
    managerAddGrade(manager, 6, 324290, 4, 73);
    managerAddGrade(manager, 6, 394902, 3, 93);

    managerAddStudent(manager, 222222222, "Omer", "Adam");
    managerLogout(manager);
    managerLogin(manager, 222222222);
    managerSendFriendRequest(manager, 205795511);
    managerAddGrade(manager, 5, 234107, 8, 51);
    managerAddGrade(manager, 5, 234107, 8, 63);
    managerAddGrade(manager, 5, 234900, 4, 74);
    managerAddGrade(manager, 5, 236327, 6, 77);
    managerAddGrade(manager, 5, 236334, 6, 64);
    managerAddGrade(manager, 5, 236334, 6, 68);
    managerAddGrade(manager, 5, 236343, 6, 80);
    managerAddGrade(manager, 5, 236360, 6, 71);
    managerAddGrade(manager, 5, 324440, 4, 79);
    managerAddGrade(manager, 5, 394902, 3, 97);

    managerAddStudent(manager, 333333333, "Aviv", "Gefen");
    managerLogout(manager);
    managerLogin(manager, 333333333);
    managerSendFriendRequest(manager, 205795511);
    managerAddGrade(manager, 4, 134020, 7, 56);
    managerAddGrade(manager, 4, 134020, 7, 76);
    managerAddGrade(manager, 4, 234123, 9, 94);
    managerAddGrade(manager, 4, 234247, 6, 95);
    managerAddGrade(manager, 4, 234267, 6, 60);
    managerAddGrade(manager, 4, 234267, 6, 68);
    managerAddGrade(manager, 4, 236353, 6, 78);
    managerAddGrade(manager, 4, 394902, 3, 92);

    managerLogout(manager);
    managerLogin(manager, 205795511);

    managerHandleFriendRequest(manager, 111111111, "accept");
    managerHandleFriendRequest(manager, 222222222, "accept");
    managerHandleFriendRequest(manager, 333333333, "accept");

    printf("\n");
    manager_error = managerPrintReferences(output_stream, manager, 394902, 2);
    ASSERT_TEST(manager_error == MANAGER_SUCCESS);

    managerDestroy(manager);
    return true;
}

static bool testManagerDestroy() {
    managerDestroy(NULL);
    return true;
}

int main() {
    RUN_TEST(testManagerCreate);
    RUN_TEST(testManagerAddStudent);
    RUN_TEST(testManagerRemoveStudent);
    RUN_TEST(testManagerLogin);
    RUN_TEST(testManagerLogout);
    RUN_TEST(testManagerFacultyRequest);
    RUN_TEST(testManagerSendFriendRequest);
    RUN_TEST(testManagerHandleFriendRequest);
    RUN_TEST(testManagerUnfriend);
    RUN_TEST(testManagerAddGrade);
    RUN_TEST(testManagerRemoveLastGrade);
    RUN_TEST(testManagerUpdateLastGrade);
    RUN_TEST(testManagerPrintFullSheet);
    RUN_TEST(testManagerPrintCleanSheet);
    RUN_TEST(testManagerPrintHighestGrades);
    RUN_TEST(testManagerPrintLowestGrades);
    RUN_TEST(testManagerPrintReferences);
    RUN_TEST(testManagerDestroy);
    return 0;
}