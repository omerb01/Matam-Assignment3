//
// Created by Omer on 05/12/2017.
//

#include "list_mtm.h"
#include "node_mtm.h"

struct Node_t {
    ListElement data;
    Node next;
    Node previous;
    compareNodeElement compare;
    copyNodeElement copy;
    displayNodeElement display;
};

struct List_t {
    Node head;
    ListElement iterator;
};

int main() {
    return 0;
}