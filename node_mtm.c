//
// Created by Omer on 08/12/2017.
//

#include "node_mtm.h"

struct Node_t {
    NodeElement data;
    Node* next;
    CopyNodeElement copy;
    FreeNodeElement free;
};