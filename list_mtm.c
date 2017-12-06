//
// Created by Omer on 05/12/2017.
//

#include "list_mtm.h"
#include "node_mtm.h"

struct Node_t {
    NodeElement data;
    Node next;
    Node previous;
    CopyNodeElement copy;
    FreeNodeElement free;
};

struct List_t {
    Node head;
    Node iterator;
};

int main() {
    return 0;
}