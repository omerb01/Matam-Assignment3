//
// Created by Omer on 08/12/2017.
//

#include "headers/CourseManager.h"
#include <string.h>
#include <stdlib.h>

#define ROUTER_CMP($$name$$) \
!strcmp(prime_command, $$name$$)

#define PRINT_ERR($$error$$) \
mtmPrintErrorMessage(stderr, $$error$$);

#define STUDENT_COMMAND($$name$$) \
!strcmp(sub_command, $$name$$)

#define STUDENT_INTRO() \
ManagerResult errorcode; \
int id = stringToInt((char *) listGetNext(command)); \
if (id == -1) {return MANAGER_FAIL;}

static bool isAlphaOrDigit(char c);

static bool isStringEmpty(char *input_string);

static bool isStringComment(char *input_string);

static int getNextWord(char *str);

static char *getStringWithoutOffset(char *string_with_offset);

static bool isDelimiter(char c);

static void destroyCommand(char *command);

static bool isNumberArgumentsValid(int argc) {
    switch (argc) {
        case 5:
            return true;
        case 3:
            return true;
        case 1:
            return true;
        default:
            return false;
    }
}

static void getInputFile(int argc, char **argv, FILE **input) {
    if (argc == 1) *input = stdin;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-i")) {
            *input = fopen(argv[i + 1], "r");
            if (!*input) {
                mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
            }
            return;
        }
    }
    *input = stdin;
}

static void getOutputFile(int argc, char **argv, FILE **output) {
    if (argc == 1) *output = stdin;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-o")) {
            *output = fopen(argv[i + 1], "w");
            if (!*output) {
                mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
            }
            return;
        }
    }
    *output = stdout;
}

static char *copyCommand(char *command);

static bool isDelimiter(char c) {
    if (c == ' ' || c == '\t') return true;
    return false;
}

static char *getStringWithoutOffset(char *string_with_offset) {
    char *iterator = string_with_offset;
    char *iterator_end = string_with_offset;
    iterator_end += strlen(string_with_offset);
    while (!isAlphaOrDigit(*iterator)) iterator++;
    while (!isAlphaOrDigit(*iterator_end)) iterator_end--;
    *(++iterator_end) = '\0';

    char *stripped_string = malloc(
            sizeof(char) * (iterator_end - iterator + 1));
    stripped_string = strcpy(stripped_string, iterator);
    return stripped_string;

}

static int getNextWord(char *str) {
    int len = 0;
    char *iterator = str;
    while (!isDelimiter(*iterator) && *iterator != '\0') {
        len++;
        iterator++;
    }
    return len;
}

static void loadCommandsIntoList(List command, char *command_input) {
    char *stripped_string = getStringWithoutOffset(command_input);
    char *iterator = stripped_string;
    int len;
    while (*iterator != '\0') {
        len = getNextWord(iterator);
        if (*(iterator + len) != '\0') {
            *(iterator + len) = '\0';
        } else {
            listInsertLast(command, (ListElement) iterator);
            break;
        }
        listInsertLast(command, (ListElement) iterator);
        iterator += (len + 1);
        while (isDelimiter(*iterator)) iterator++;
    }
    free(stripped_string);
}

static bool isAlphaOrDigit(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9'))
        return true;
    return false;
}

static bool isStringComment(char *input_string) {
    char *iterator = input_string;
    while (*iterator != '\0' && !isAlphaOrDigit(*iterator) &&
           *iterator != '\n') {
        if (*iterator == '#') return true;
        iterator++;
    }
    return false;
}


static bool isStringEmpty(char *input_string) {
    char *iterator;
    iterator = input_string;
    while (*iterator != '\0') {
        if (isAlphaOrDigit(*iterator)) return false;
        iterator++;
    }
    return true;
}

static char *copyCommand(char *command) {
    char *new_string = malloc(strlen(command) + 1);
    strcpy(new_string, command);
    return new_string;
}

static void destroyCommand(char *command) {
    free(command);
}

static bool isDigit(char c) {
    if (c < 48 || c > 57) return false;
    return true;
}

static int stringToInt(char *str) {
    int number = 0;
    if (str == NULL) return -1;
    char *string_number = str;
    while (*string_number != '\0') {
        if (!isDigit(*string_number)) {
            mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
            return -1;
        } else {
            number = number * 10 + ((*string_number) - '0');
        }
        string_number++;
    }
    return number;
}

static ManagerResult
mainAddStudent(List command, CourseManager course_manager) {
    STUDENT_INTRO();
    char *first_name = (char *) listGetNext(command);
    char *last_name = (char *) listGetNext(command);

    errorcode = managerAddStudent(course_manager, id, first_name, last_name);
    return errorcode;
}

static ManagerResult
mainRemoveStudent(List command, CourseManager course_manager) {
    STUDENT_INTRO();
    errorcode = managerRemoveStudent(course_manager, id);
    return errorcode;
}

static ManagerResult
mainLoginStudent(List command, CourseManager course_manager) {
    STUDENT_INTRO();
    errorcode = managerLogin(course_manager, id);
    return errorcode;
}

static ManagerResult
mainFriendRequest(List command, CourseManager course_manager) {
    STUDENT_INTRO();
    errorcode = managerSendFriendRequest(course_manager, id);
    return errorcode;
}

static ManagerResult
mainHandleRequest(List command, CourseManager course_manager) {
    STUDENT_INTRO();
    char *action = (char *) listGetNext(command);
    errorcode = managerHandleFriendRequest(course_manager, id, action);
    return errorcode;
}

static ManagerResult
mainUnfriendStudent(List command, CourseManager course_manager) {
    STUDENT_INTRO();
    errorcode = managerUnfriend(course_manager, id);
    return errorcode;
}


static ManagerResult mainLogoutStudent(CourseManager course_manager) {
    ManagerResult errorcode;
    errorcode = managerLogout(course_manager);
    return errorcode;
}

static ManagerResult studentRouter(List command, CourseManager course_manager) {
    char *sub_command = (char *) listGetNext(command);
    if (STUDENT_COMMAND("add")) {
        return mainAddStudent(command, course_manager);
    } else if (STUDENT_COMMAND("remove")) {
        return mainRemoveStudent(command, course_manager);
    } else if (STUDENT_COMMAND("login")) {
        return mainLoginStudent(command, course_manager);
    } else if (STUDENT_COMMAND("logout")) {
        return mainLogoutStudent(course_manager);
    } else if (STUDENT_COMMAND("friend_request")) {
        return mainFriendRequest(command, course_manager);
    } else if (STUDENT_COMMAND("handle_request")) {
        return mainHandleRequest(command, course_manager);
    } else if (STUDENT_COMMAND("unfriend")) {
        return mainUnfriendStudent(command, course_manager);
    } else {
        return MANAGER_FAIL;
    }
}

static int isFloat(char *number) {
    char *iterator = number;
    int index = 0;
    while (*iterator != '\0') {
        if (*iterator == '.') {
            return index;
        }
        index++;
        iterator++;
    }
    return -1;
}

static int pointsToNumber(char *points) {
    int point_index = isFloat(points);
    int points2x = 0;
    char *iterator = points;
    if (point_index == -1) {
        return stringToInt(points);
    } else {
        *(iterator + point_index) = '\0';
        points2x = stringToInt(iterator);
        points2x *= 2;
        if (points2x == -1) {
            return -1;
        } else {
            return *(points + point_index + 1) == '0' ? points2x : (points2x +
                                                                    1);
        }
    }
}

static ManagerResult mainAddGrade(List command, CourseManager course_manager) {
    ManagerResult errorcode;
    int semester = stringToInt((char *) listGetNext(command));
    int course_id = stringToInt((char *) listGetNext(command));
    int points = pointsToNumber((char *) listGetNext(command));
    int grade = stringToInt((char *) listGetNext(command));
    if (semester == -1 || course_id == -1 || points == -1 || grade == -1) {
        return MANAGER_INVALID_ARGUMENT;
    } else {
        errorcode = managerAddGrade(course_manager, semester, course_id, points,
                                    grade);
        return errorcode;
    }
}

static ManagerResult
mainRemoveGrade(List command, CourseManager course_manager) {
    ManagerResult errorcode;
    int semester = stringToInt((char *) listGetNext(command));
    int course_id = stringToInt((char *) listGetNext(command));
    if (semester == -1 || course_id == -1) {
        return MANAGER_INVALID_ARGUMENT;
    } else {
        errorcode = managerRemoveLastGrade(course_manager, semester,
                                           course_id); //TODO:make sure it's the right command
        return errorcode;
    }
}

static ManagerResult
mainUpdateGrade(List command, CourseManager course_manager) {
    ManagerResult errorcode;
    int course_id = stringToInt((char *) listGetNext(command));
    int new_grade = stringToInt((char *) listGetNext(command));
    if (course_id == -1 || new_grade == -1) {
        return MANAGER_INVALID_ARGUMENT;
    } else {
        errorcode = managerUpdateLastGrade(course_manager, course_id,
                                           new_grade);
        return errorcode;
    }
}

static ManagerResult
gradesSheetRouter(List command, CourseManager course_manager) {
    char *sub_command = (char *) listGetNext(command);
    if (STUDENT_COMMAND("add")) {
        return mainAddGrade(command, course_manager);
    } else if (STUDENT_COMMAND("remove")) {
        return mainRemoveGrade(command, course_manager);
    } else if (STUDENT_COMMAND("update")) {
        return mainUpdateGrade(command, course_manager);
    } else {
        return MANAGER_FAIL;
    }
}

static ManagerResult
mainFacultyRequest(List command, CourseManager course_manager, FILE *output) {
    int course_id = stringToInt((char *) listGetNext(command));
    char *request = (char *) listGetNext(command);
    if (course_id == -1) return MANAGER_INVALID_ARGUMENT;
    return managerFacultyRequest(output, course_manager, course_id, request);
}

static ManagerResult
mainReference(List command, CourseManager course_manager, FILE *output) {
    int course_id = stringToInt((char *) listGetNext(command));
    int amount = stringToInt((char *) listGetNext(command));
    if (course_id == -1 || amount == -1) return MANAGER_INVALID_ARGUMENT;
    return managerPrintReferences(output, course_manager, course_id, amount);
}

static ManagerResult
reportRouter(List command, CourseManager course_manager, FILE *output) {
    char *sub_command = (char *) listGetNext(command);
    if (STUDENT_COMMAND("full")) {
        return managerPrintFullSheet(output, course_manager);
    } else if (STUDENT_COMMAND("clean")) {
        return managerPrintCleanSheet(output, course_manager);
    } else if (STUDENT_COMMAND("best")) {
        int amount = stringToInt((char *) listGetNext(command));
        if (amount == -1) return MANAGER_INVALID_ARGUMENT;
        return managerPrintHighestGrades(output, course_manager, amount);
    } else if (STUDENT_COMMAND("worst")) {
        int amount = stringToInt((char *) listGetNext(command));
        if (amount == -1) return MANAGER_INVALID_ARGUMENT;
        return managerPrintLowestGrades(output, course_manager, amount);
    } else if (STUDENT_COMMAND("reference")) {
        return mainReference(command, course_manager, output);
    } else if (STUDENT_COMMAND("faculty_request")) {
        return mainFacultyRequest(command, course_manager, output);
    } else {
        return MANAGER_FAIL;
    }
}

static int studentErrorConverter(ManagerResult error) {
    if (error == MANAGER_NULL_ARGUMENT) {
        PRINT_ERR(MTM_INVALID_PARAMETERS);
        return 0;
    } else if (error == MANAGER_ALREADY_FRIEND) {
        PRINT_ERR(MTM_ALREADY_FRIEND);
        return 0;
    } else if (error == MANAGER_ALREADY_LOGGED_IN) {
        PRINT_ERR(MTM_ALREADY_LOGGED_IN);
        return 0;
    } else if (error == MANAGER_ALREADY_REQUESTED) {
        PRINT_ERR(MTM_ALREADY_REQUESTED);
        return 0;
    } else if (error == MANAGER_COURSE_DOES_NOT_EXIST) {
        PRINT_ERR(MTM_COURSE_DOES_NOT_EXIST);
        return 0;
    } else if (error == MANAGER_INVALID_ARGUMENT) {
        PRINT_ERR(MTM_INVALID_PARAMETERS);
        return 0;
    } else if (error == MANAGER_NOT_FRIEND) {
        PRINT_ERR(MTM_NOT_FRIEND);
        return 0;
    } else if (error == MANAGER_NOT_LOGGED_IN) {
        PRINT_ERR(MTM_NOT_LOGGED_IN);
        return 0;
    } else if (error == MANAGER_NOT_REQUESTED) {
        PRINT_ERR(MTM_NOT_REQUESTED);
        return 0;
    } else if (error == MANAGER_OUT_OF_MEMORY) {
        PRINT_ERR(MTM_OUT_OF_MEMORY);
        return -1;
    } else if (error == MANAGER_STUDENT_ALREADY_EXISTS) {
        PRINT_ERR(MTM_STUDENT_ALREADY_EXISTS);
        return 0;
    } else if (error == MANAGER_STUDENT_DOES_NOT_EXIST) {
        PRINT_ERR(MTM_STUDENT_DOES_NOT_EXIST);
        return 0;
    } else if (error == MANAGER_FAIL) {
        PRINT_ERR(MTM_INVALID_COMMAND_LINE_PARAMETERS);
        return -1;
    } else {
        return 0;
    }
}
//TODO:FUCK THIS SHIT
static int
commandRouter(List command, CourseManager course_manager, FILE *output) {
    char *prime_command = (char *) listGetFirst(command);
    ManagerResult error_code;
    if (ROUTER_CMP("student")) {
        error_code = studentRouter(command, course_manager);
        return studentErrorConverter(error_code);
    } else if (ROUTER_CMP("grade_sheet")) {
        error_code = gradesSheetRouter(command, course_manager);
        return studentErrorConverter(error_code);
    } else if (ROUTER_CMP("report")) {
        error_code = reportRouter(command, course_manager, output);
        return MTM_INVALID_COMMAND_LINE_PARAMETERS;
    } else {
        mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
        return MTM_INVALID_COMMAND_LINE_PARAMETERS;
    }
}


int main(int argc, char **argv) {
    FILE *output = NULL;
    FILE *input = NULL;
    List command = listCreate((CopyListElement) copyCommand,
                              (FreeListElement) destroyCommand);
    CourseManager course_manager = managerCreate();
    int critical_status = 0;
    if (command == NULL) {
        mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
        return 1;
    } else if (!isNumberArgumentsValid(argc)) {
        mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
        return 1;
    }
    getInputFile(argc, argv, &input);
    getOutputFile(argc, argv, &output);

    if (input == NULL || output == NULL) return 1;
    char buffer[MAX_LEN];

    while (fgets(buffer, MAX_LEN, input) != NULL) {
        if (isStringEmpty(buffer)) continue;
        else if (isStringComment(buffer)) continue;
        loadCommandsIntoList(command, buffer);
        critical_status = commandRouter(command, course_manager, output);
        if (critical_status == -1) {
            managerDestroy(course_manager);
            listDestroy(command);
            if (output != stdout) fclose(output);
            if (input != stdin) fclose(input);
            return 1;
        }
        listClear(command);
    }




    /*LIST_FOREACH(char*, arg, command){
        printf("%s\n", arg);
    }*/
    managerDestroy(course_manager);
    listDestroy(command);
    if (output != stdout) fclose(output);
    if (input != stdin) fclose(input);
}