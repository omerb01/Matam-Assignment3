//
// Created by Omer on 08/12/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include "node_mtm.h"

int test_counter = 0;

#define ASSERT(test_condition)  \
   if (!(test_condition)) {printf("TEST %d FAILED\n", test_counter); } \
   else {printf("TEST %d OK\n", test_counter);} \
    test_counter++;

NodeElement copyInt(NodeElement num) {
    int *result = malloc(sizeof(*result));
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
    int* element = malloc(sizeof(*element));
    printf("start testing node_mtm.c ... \n");

    *element = 1;
    Node node1 = nodeCreate(element, copyInt, freeInt);
    ASSERT(node1 != NULL && *(int*)nodeGetElement(node1) == 1 &&
                   nodeGetNext(node1) == NULL);

    *element = 10;
    ASSERT(*(int*)nodeGetElement(node1) == 1);

    ASSERT(nodeIsEmpty(node1) == false);

    *element = 2;
    Node node2 = nodeCreate(element, copyInt, freeInt);
    *element = 1;
    Node node3 = nodeCreate(element, copyInt, freeInt);

    int sort_key = 0;
    ASSERT(nodeCompare(node1, node3, compareInt, (void*)&sort_key) == 0);
    ASSERT(nodeCompare(node1, node2, compareInt, (void*)&sort_key) == -1);
    ASSERT(nodeCompare(node2, node1, compareInt, (void*)&sort_key) == 1);

    int filter_key = 2;
    ASSERT(nodeFilter(node1, filterInt, (void*)&filter_key) == false);
    ASSERT(nodeFilter(node2, filterInt, (void*)&filter_key) == true);

    error = nodeAddNext(node1, node2);
    ASSERT(error == NODE_OK && nodeGetNext(node1) == node2);

    error = nodeAddNext(node1, node2);
    ASSERT(error == NODE_ELEMENT_OVERRIDE);

    Node node4 = nodeCopy(node1);
    ASSERT(node4 != NULL && *(int*)nodeGetElement(node4) == 1 &&
           nodeGetNext(node4) == node2);

    error = nodeRemoveNext(node1);
    ASSERT(error == NODE_OK && nodeGetNext(node1) == NULL);

    *element = 50;
    error = nodeUpdateElement(node1, element);
    ASSERT(error == NODE_OK && *(int*)nodeGetElement(node1) == 50);

    *element = 25;
    ASSERT(*(int*)nodeGetElement(node1) == 50);

    error = nodeClear(node1);
    ASSERT(error == NODE_OK && nodeIsEmpty(node1) == true);

    freeInt(element);
    nodeDestroy(node1);
    nodeDestroy(node2);
    nodeDestroy(node3);
    nodeDestroy(node4);

    return 0;
}