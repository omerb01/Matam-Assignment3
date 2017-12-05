//
// Created by Omer on 05/12/2017.
//

#include "list_mtm.h"

typedef struct Node_t *Node;
struct Node_t {
    ListElement data;
    Node next;
};

typedef struct List_t {
    Node head;
    ListElement iterator;
} *List;

int main() {
    return 0;
}