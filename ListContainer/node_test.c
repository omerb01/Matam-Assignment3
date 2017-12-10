//
// Created by Omer on 08/12/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include "node_mtm.h"

int test_counter = 1;

#define ASSERT(test_condition)  \
   if (!(test_condition)) {printf("TEST %d FAILED\n", test_counter); } \
   else {printf("TEST %d OK\n", test_counter);} \
    test_counter++;

NodeElement copyInt(NodeElement num) {
    int *result = malloc(sizeof(int));
    if (result == NULL) return NULL;
    *result = *(int *) num;
    return (void *) result;
}

void freeInt(NodeElement num) {
    free(num);
}

int compareInt(NodeElement num1, NodeElement num2, NodeSortKey key) {
    if (*(int *) num1 == *(int *) num2) return 0;
    if (*(int *) num1 > *(int *) num2) return 1;
    else return -1;
}

bool filterInt(NodeElement num, NodeFilterKey key) {
    if(*(int*)num >= *(int*)key) return true;
    else return false;
}

int main() {

    NodeResult error;
    int* element = malloc(sizeof(int));
    printf("start testing node_mtm.c ... \n");

    *element = 1;
    Node node1 = nodeCreate(element, copyInt, freeInt);
    ASSERT(node1 != NULL && *(int*)nodeGetElement(node1) == 1 &&
                   nodeGetNext(node1) == NULL);
    *element=50;
    error = nodeUpdateElement(node1, element);
    ASSERT(error == NODE_OK && *(int*)nodeGetElement(node1) == 50);

    freeInt(element);
    nodeDestroy(node1);


    return 0;
}