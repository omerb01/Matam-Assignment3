//
// Created by Omer on 08/12/2017.
//

#include "headers/CourseManager.h"
#include <string.h>
#include <stdlib.h>

static bool isAlpha(char c);

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
static bool isDelimiter(char c){
    if(c == ' ' || c == '\t') return true;
    return false;
}
static char* getStringWithoutOffset(char *string_with_offset) {
    char *iterator = string_with_offset;
    char *iterator_end=string_with_offset;
    iterator_end+=strlen(string_with_offset);
    while(!isAlpha(*iterator)) iterator++;
    while(!isAlpha(*iterator_end)) iterator_end--;
    *(++iterator_end)='\0';

    char* stripped_string = malloc(
            sizeof(char) * (iterator_end-iterator+1));
    stripped_string = strcpy(stripped_string, iterator);
    return stripped_string;

}

static int countDelimitersNum(char *str){
    int num=0;
    char *iterator = str;
    while(*iterator != '\0'){
        if(isDelimiter(*(iterator))){
            num++;
            while(isDelimiter(*iterator)) iterator++;
        }
        else{
            iterator++;
        }
    }
    return num;
}

static int getNextWord(char *str){
    int len=0;
    char* iterator = str;
    while(!isDelimiter(*iterator) && *iterator!='\0'){
        len++;
        iterator++;
    }
    return len;
}

static char** str_remove_trailing(char *input_string, int* len_d){
    char* splitted_string = getStringWithoutOffset(input_string);
    char* iterator= splitted_string;
    int new_word_len=0;
    int delimiters_num = countDelimitersNum(splitted_string),len=0;
    char **command = malloc(sizeof(char) * (delimiters_num+1));
    command[delimiters_num+1]='\0';
    while(*iterator != '\0'){
        new_word_len = getNextWord(iterator);
        command[len] = malloc(sizeof(char)*(new_word_len+1));
        command[len] = strncpy(command[len], iterator, new_word_len+1);
        command[len][new_word_len] = '\0';
        iterator+=new_word_len;
        while(isDelimiter(*iterator)){
            iterator++;
        }
        len++;
    }
    free(splitted_string);
    *len_d = delimiters_num+1;
    return command;

}
static void printCommand(char **command, int num) {
    printf("%s ", command[0]);
    for (int i = 1; i < num; i++) {
        printf("\n%s ", command[i]);
    }
    printf("\n");
}

static bool isAlpha(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return true;
    return false;
}
static bool isStringComment(char *input_string, int *offset) {
    char *iterator = input_string;
    int num = 0;
    while (*iterator != '\0' && !isAlpha(*iterator) && *iterator != '\n') {
        if (*iterator == '#') return true;
        iterator++;
        num++;
    }
    *offset = num;
    return false;
}


static bool isStringEmpty(char *input_string) {
    char *iterator;
    iterator = input_string;
    while (*iterator != '\0') {
        if (isAlpha(*iterator)) return false;
        iterator++;
    }
    return true;
}

int main(int argc, char **argv) {
    FILE *output = NULL;
    FILE *input = NULL;

    if (!isNumberArgumentsValid(argc)) {
        mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
    }
    getInputFile(argc, argv, &input);
    getOutputFile(argc, argv, &output);
    if(input == NULL || output == NULL) return 1;
    char buffer[MAX_LEN];
    char **command=NULL;
    int offset,len;
    while (fgets(buffer, MAX_LEN, input) != NULL) {
        if (isStringEmpty(buffer)) continue;
        else if (isStringComment(buffer, &offset)) continue;
        command = str_remove_trailing(buffer, &len);
        printCommand(command, len);
    }
    for(int i=0; i<len; i++){
        free(command[i]);
    }
    free(command);
    if(output != stdout) fclose(output);
    if(input != stdin) fclose(input);
}