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

bool nodeIsEmpty(Node node) {
    assert(node != NULL);
    if (node->data == NULL) {
        return true;
    } else {
        return false;
    }
}

NodeElement nodeGetElement(Node node) {
    assert(node != NULL);
    return node->data;
}

Node nodeGetNext(Node node) {
    assert(node != NULL);
    return node->next;
}

int nodeCompare(Node node1, Node node2, CompareNodeElement compare, NodeSortKey key) {
    assert(node1 != NULL && node2 != NULL && node1->data != NULL &&
           node2->data != NULL);
    return compare(node1->data,node2->data,key);
}

bool nodeFilter(Node node, FilterNodeElement filterElement, NodeFilterKey key) {
    assert(node != NULL && key != NULL && filterElement != NULL);
    assert(node->data != NULL);
    return filterElement(node->data, key);
}

NodeResult nodeAddNext(Node node, Node next){
    assert(node != NULL && next != NULL);
    if(node->next != NULL){
        return NODE_ELEMENT_OVERRIDE;
    }
    else{
        node->next = next;
    }
    return NODE_OK;
}

NodeResult nodeRemoveNext(Node node) {
    assert(node != NULL);
    node->next = NULL;
    return NODE_OK;
}

NodeResult nodeUpdateElement(Node node, NodeElement element){
    assert(node != NULL && element != NULL);
    NodeElement new_data = node->copyElement(element);
    if (new_data == NULL){
        return NODE_MEMORY_ERROR;
    }
    node->freeElement(node->data);
    node->data=new_data;
    return NODE_OK;
}

NodeResult nodeClear(Node node) {
    assert(node != NULL);
    if (node->data == NULL) return NODE_OK;
    node->freeElement(node->data);
    node->data = NULL;
    return NODE_OK;
}

void nodeDestroy(Node node){
    if(node == NULL){
        return;
    }
    else{
        nodeClear(node);
        free(node);
    }
}