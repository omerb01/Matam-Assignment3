//
// Created by Omer on 08/12/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "node_mtm.h"

struct Node_t {
    NodeElement data;
    Node next;
    CopyNodeElement copyElement;
    FreeNodeElement freeElement;
};

Node
nodeCreate(NodeElement element, CopyNodeElement copyElement, FreeNodeElement
freeElement) {
    assert(copyElement != NULL && freeElement != NULL);
    Node node = malloc(sizeof(*node));
    if (node == NULL) return NULL;
    node->copyElement = copyElement;
    node->freeElement = freeElement;

    if (element != NULL) {
        NodeElement element_copy = copyElement(element);
        if (element_copy == NULL) {
            free(node);
            return NULL;
        }
        node->data = element_copy;
    } else {
        node->data = NULL;
    }

    node->next = NULL;
    return node;
}

Node nodeCopy(Node node) {
    assert(node != NULL);
    assert(node->data != NULL);
    Node node_copy = nodeCreate(node->data, node->copyElement,
                                node->freeElement);
    if (node_copy == NULL) {
        return NULL;
    }
    node_copy->next = node->next;
    return node_copy;
}

Node nodeGetNext(Node node) {
    assert(node != NULL);
    return node->next;
}

bool nodeFilter(Node node, FilterNodeElement filterElement, NodeFilterKey key) {
    assert(node != NULL && key != NULL && filterElement != NULL);
    assert(node->data != NULL);
    return filterElement(node->data, key);
}

NodeResult nodeRemoveNext(Node node) {
    assert(node != NULL);
    node->next = NULL;
    return NODE_OK;
}

NodeResult nodeClear(Node node) {
    assert(node != NULL);
    if(node->data == NULL) return NODE_OK;
    node->freeElement(node->data);
    return NODE_OK;
}