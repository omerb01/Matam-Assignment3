//
// Created by Omer on 15/12/2017.
//

#include "../headers/test_utilities.h"
#include "../headers/Student.h"
#include "../headers/UsefulFunctions.h"

static bool testStudentCreate() {
    Student student = NULL;
    student = studentCreate(205795511, "Omer", "Belhasin");
    ASSERT_TEST(student != NULL);

    studentDestroy(student);
    return true;
}

static bool testStudentCopy() {
    Student student = studentCreate(205795511, "Omer", "Belhasin");
    Student student_copy = studentCopy(student);

    ASSERT_TEST(studentCompare(student, student_copy) == 0);

    studentDestroy(student);
    studentDestroy(student_copy);
    return true;
}

static bool testStudentCompare() {
    Student student1 = studentCreate(205795511, "Omer", "Belhasin");
    Student student2 = studentCreate(111111111, "Omer", "Belhasin");
    Student student3 = studentCreate(333333333, "Omer", "Belhasin");
    Student student4 = studentCreate(205795511, "ABA", "ABA");

    ASSERT_TEST(studentCompare(student1, student4) == 0);
    ASSERT_TEST(studentCompare(student1, student2) > 0);
    ASSERT_TEST(studentCompare(student1, student3) < 0);

    studentDestroy(student1);
    studentDestroy(student2);
    studentDestroy(student3);
    studentDestroy(student4);
    return true;
}

static bool testStudentSendFriendRequest() {
    Student student1 = studentCreate(205795511, "Omer", "Belhasin");
    Student student2 = studentCreate(111111111, "Shlomo", "Artzi");
    Student student3 = studentCreate(222222222, "Omer", "Adam");
    Student student4 = studentCreate(333333333, "Yosi", "Banai");
    StudentResult student_error;

    student_error = studentSendFriendReuqest(NULL, student2);
    ASSERT_TEST(student_error == STUDENT_NULL_ARGUMENT);
    student_error = studentSendFriendReuqest(student1, NULL);
    ASSERT_TEST(student_error == STUDENT_NULL_ARGUMENT);

    student_error = studentSendFriendReuqest(student1, student2);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    student_error = studentSendFriendReuqest(student1, student3);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    student_error = studentSendFriendReuqest(student2, student1);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);

    student_error = studentSendFriendReuqest(student1, student2);
    ASSERT_TEST(student_error == STUNDET_ALREADY_SENT_REQUEST);
    student_error = studentSendFriendReuqest(student2, student1);
    ASSERT_TEST(student_error == STUNDET_ALREADY_SENT_REQUEST);
    student_error = studentSendFriendReuqest(student1, student3);
    ASSERT_TEST(student_error == STUNDET_ALREADY_SENT_REQUEST);

    studentDestroy(student1);
    studentDestroy(student2);
    studentDestroy(student3);
    studentDestroy(student4);
    return true;
}

static bool testStudentIsSentFriendRequest() {
    Student student1 = studentCreate(205795511, "Omer", "Belhasin");
    Student student2 = studentCreate(111111111, "Shlomo", "Artzi");
    Student student3 = studentCreate(222222222, "Omer", "Adam");
    Student student4 = studentCreate(333333333, "Yosi", "Banai");
    StudentResult student_error;
    bool result;

    student_error = studentIsSentFriendRequest(NULL, student2, &result);
    ASSERT_TEST(student_error == STUDENT_NULL_ARGUMENT);
    student_error = studentIsSentFriendRequest(student1, NULL, &result);
    ASSERT_TEST(student_error == STUDENT_NULL_ARGUMENT);
    student_error = studentIsSentFriendRequest(student1, student2, NULL);
    ASSERT_TEST(student_error == STUDENT_NULL_ARGUMENT);

    studentSendFriendReuqest(student1, student2);
    studentSendFriendReuqest(student1, student3);
    studentSendFriendReuqest(student2, student1);

    student_error = studentIsSentFriendRequest(student1, student2, &result);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    ASSERT_TEST(result == true);

    student_error = studentIsSentFriendRequest(student3, student2, &result);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    ASSERT_TEST(result == false);

    student_error = studentIsSentFriendRequest(student1, student3, &result);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    ASSERT_TEST(result == true);

    student_error = studentIsSentFriendRequest(student2, student3, &result);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    ASSERT_TEST(result == false);

    student_error = studentIsSentFriendRequest(student2, student1, &result);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    ASSERT_TEST(result == true);

    student_error = studentIsSentFriendRequest(student3, student1, &result);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    ASSERT_TEST(result == false);

    studentDestroy(student1);
    studentDestroy(student2);
    studentDestroy(student3);
    studentDestroy(student4);
    return true;
}

static bool testStundetRemoveSentFriendRequest() {
    Student student1 = studentCreate(205795511, "Omer", "Belhasin");
    Student student2 = studentCreate(111111111, "Shlomo", "Artzi");
    StudentResult student_error;
    bool result;

    studentSendFriendReuqest(student1, student2);
    studentSendFriendReuqest(student2, student1);

    student_error = stundetRemoveSentFriendRequest(NULL, student2);
    ASSERT_TEST(student_error == STUDENT_NULL_ARGUMENT);
    student_error = stundetRemoveSentFriendRequest(student1, NULL);
    ASSERT_TEST(student_error == STUDENT_NULL_ARGUMENT);
    student_error = stundetRemoveSentFriendRequest(student1, student2);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    student_error = stundetRemoveSentFriendRequest(student1, student2);
    ASSERT_TEST(student_error == STUDENT_DIDNT_SEND_REQUEST);
    studentIsSentFriendRequest(student2, student1, &result);
    ASSERT_TEST(result == true);

    studentDestroy(student1);
    studentDestroy(student2);
    return true;
}

static bool testStudentAddGrade() {
    Student student = studentCreate(205795511, "Omer", "Belhasin");
    StudentResult student_error;

    student_error = studentAddGrade(NULL, 1, 104012, 11, 84);
    ASSERT_TEST(student_error == STUDENT_NULL_ARGUMENT);

    student_error = studentAddGrade(student, 1, 104012, 11, 84);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    student_error = studentAddGrade(student, 1, 104012, 11, 84);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);

    student_error = studentAddGrade(student, -1, 104167, 10, 43);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentAddGrade(student, 0, 104167, 10, 71);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentAddGrade(student, 1, 234114666, 8, 88);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentAddGrade(student, 1, -1, 6, 98);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentAddGrade(student, 1, 0, 2, 92);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentAddGrade(student, 2, 104013, 11, 101);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentAddGrade(student, 2, 104013, 11, -1);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentAddGrade(student, 2, 104013, 11, 0);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    //student_error = studentAddGrade(student, 1, 104167, -1, 43);
    //ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    // TODO: tell Ilya to fix points validation
    student_error = studentAddGrade(student, 1, 104167, 0, 43);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    
    studentDestroy(student);
    return true;
}

static bool testStudentUpdateLastGrade() {
    // TODO: assure grades_sheet's function works
    Student student = studentCreate(205795511, "Omer", "Belhasin");
    StudentResult student_error;

    studentAddGrade(student, 2, 104013, 11, 66);
    studentAddGrade(student, 2, 104013, 11, 63);
    studentAddGrade(student, 2, 114071, 7, 98);
    studentAddGrade(student, 2, 234118, 6, 98);
    studentAddGrade(student, 3, 12345, 10, 100);
    studentAddGrade(student, 3, 12346, 10, 100);
    studentAddGrade(student, 3, 12347, 10, 100);
    studentAddGrade(student, 2, 234122, 6, 96);
    studentAddGrade(student, 2, 234141, 6, 73);
    studentAddGrade(student, 2, 394806, 2, 96);

    student_error = studentUpdateLastGrade(NULL, 234122, 30);
    ASSERT_TEST(student_error == STUDENT_NULL_ARGUMENT);
    student_error = studentUpdateLastGrade(student, 2, 30);
    ASSERT_TEST(student_error == STUDENT_GRADE_DOES_NOT_EXIST);
    student_error = studentUpdateLastGrade(student, -1, 30);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentUpdateLastGrade(student, 0, 30);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentUpdateLastGrade(student, 234122, 101);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentUpdateLastGrade(student, 234122, -1);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);

    student_error = studentUpdateLastGrade(student, 234122, 0);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    student_error = studentUpdateLastGrade(student, 234122, 30);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);

    FILE *output_stream = stdout;
    printf("\n");
    studentPrintFullSheet(output_stream, student);

    studentDestroy(student);
    return true;
}

static bool testStudentRemoveLastGrade() {
    // TODO: assure grades_sheet's function works
    Student student = studentCreate(205795511, "Omer", "Belhasin");
    StudentResult student_error;
    //FILE *output_stream = stdout;

    studentAddGrade(student, 3, 234122, 10, 30);
    studentAddGrade(student, 3, 234122, 10, 50);
    studentAddGrade(student, 3, 234122, 10, 40);
    studentAddGrade(student, 2, 234122, 6, 60);
    studentAddGrade(student, 2, 234122, 6, 80);
    studentAddGrade(student, 2, 234122, 6, 70);

    student_error = studentRemoveLastGrade(NULL, 2, 234122);
    ASSERT_TEST(student_error == STUDENT_NULL_ARGUMENT);
    student_error = studentRemoveLastGrade(student, 0, 234122);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentRemoveLastGrade(student, -1, 234122);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentRemoveLastGrade(student, 2, 234122888);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentRemoveLastGrade(student, 2, 0);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentRemoveLastGrade(student, 2, -1);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);

    student_error = studentRemoveLastGrade(student, 2, 234122);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);

    //printf("\n");
    //studentPrintFullSheet(output_stream, student);

    //student_error = studentRemoveLastGrade(student, 3, 234122);
    //ASSERT_TEST(student_error == STUDENT_SUCCESS);

    //printf("\n");
    //studentPrintFullSheet(output_stream, student);

    studentDestroy(student);
    return true;
}

static bool testStudentPrintFullSheet() {
    return true;
}

static bool testStudentPrintCleanSheet() {
    return true;
}

static bool testStudentPrintHighestGrades() {
    return true;
}

static bool testStudentPrintLowestGrades() {
    return true;
}

static bool testStudentPrintReferences() {
    Student student1 = studentCreate(205795511, "Omer", "Belhasin");
    Student student2 = studentCreate(111111111, "Shlomo", "Artzi");
    Student student3 = studentCreate(222222222, "Omer", "Adam");
    Student student4 = studentCreate(333333333, "Yosi", "Banai");
    Student student5 = studentCreate(444444444, "Moshe", "Peretz");
    Student student6 = studentCreate(555555555, "Aviv", "Gefen");
    Student student7 = studentCreate(666666666, "LOL", "LOL");
    StudentResult student_error;
    FILE *output_stream = stdout;

    studentAddGrade(student1, 2, 234122, 6, 80);
    studentAddGrade(student1, 2, 234122, 6, 23);
    //studentAddGrade(student1, 3, 234122, 6, 80);
    studentAddGrade(student2, 2, 234122, 6, 55);
    studentAddGrade(student3, 2, 234122, 6, 80);
    studentAddGrade(student5, 2, 234122, 6, 50);
    studentAddGrade(student6, 2, 234122, 3, 55);
    studentAddGrade(student7, 2, 234111, 3, 55);

    Set friends = setCreate(studentCopyForSet, studentFreeForSet,
                            studentCompareForSet);
    setAdd(friends, student1);
    setAdd(friends, student2);
    setAdd(friends, student3);
    setAdd(friends, student4);
    setAdd(friends, student5);
    setAdd(friends, student6);
    setAdd(friends, student7);

    student_error = studentPrintReferences(NULL, friends, 234122, 10);
    ASSERT_TEST(student_error == STUDENT_NULL_ARGUMENT);
    student_error = studentPrintReferences(output_stream, NULL, 234122, 10);
    ASSERT_TEST(student_error == STUDENT_NULL_ARGUMENT);

    student_error = studentPrintReferences(output_stream, friends, 234122555,
                                           10);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentPrintReferences(output_stream, friends, 0, 10);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentPrintReferences(output_stream, friends, -1, 10);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentPrintReferences(output_stream, friends, 234122, 0);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);
    student_error = studentPrintReferences(output_stream, friends, 234122, -1);
    ASSERT_TEST(student_error == STUDENT_INVALID_ARGUMENT);

    // TODO: highest grade sheet doesnt give the last grade
/*
    printf("\n");
    student_error = studentPrintReferences(output_stream, friends, 234122, 2);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    printf("\n");
    student_error = studentPrintReferences(output_stream, friends, 234122, 10);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
    printf("\n");
    student_error = studentPrintReferences(output_stream, friends, 234111, 10);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);

    student_error = studentPrintReferences(output_stream, friends, 234112, 10);
    ASSERT_TEST(student_error == STUDENT_SUCCESS);
*/

    setDestroy(friends);
    studentDestroy(student1);
    studentDestroy(student2);
    studentDestroy(student3);
    studentDestroy(student4);
    studentDestroy(student5);
    studentDestroy(student6);
    studentDestroy(student7);
    return true;
}

static bool testStudentDestroy() {
    studentDestroy(NULL);
    return true;
}

int main() {
    RUN_TEST(testStudentCreate);
    RUN_TEST(testStudentCopy);
    RUN_TEST(testStudentCompare);
    RUN_TEST(testStudentSendFriendRequest);
    RUN_TEST(testStudentIsSentFriendRequest);
    RUN_TEST(testStundetRemoveSentFriendRequest);
    RUN_TEST(testStudentAddGrade);
    RUN_TEST(testStudentUpdateLastGrade);
    RUN_TEST(testStudentRemoveLastGrade);
    RUN_TEST(testStudentPrintFullSheet);
    RUN_TEST(testStudentPrintCleanSheet);
    RUN_TEST(testStudentPrintHighestGrades);
    RUN_TEST(testStudentPrintLowestGrades);
    RUN_TEST(testStudentPrintReferences);
    RUN_TEST(testStudentDestroy);
    return 0;
}