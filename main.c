//
// Created by Omer on 08/12/2017.
//

#include <stdlib.h>
#include <stdio.h>
#include "ListContainer/list_mtm.h"
#include "ListContainer/node_mtm.h"

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

}