//
// Created by Omer on 05/12/2017.
//

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "list_mtm.h"
#include "node_mtm.h"

struct List_t {
    Node head;
    Node iterator;
    CopyListElement copyElement;
    FreeListElement freeElement;
};

List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
    assert(copyElement != NULL && freeElement != NULL);

    List list = malloc(sizeof(*list));
    Node head = nodeCreate(NULL, copyElement, freeElement);
    if (list == NULL || head == NULL) {
        free(list);
        nodeDestroy(head);
        return NULL;
    }

    list->copyElement = copyElement;
    list->freeElement = freeElement;
    list->iterator = NULL;
    return list;
}

int listGetSize(List list) {
    if (list == NULL) return -1;
    if (nodeIsEmpty(list->head)) return 0;

    Node iterator = list->iterator;
    int counter = 0;
    LIST_FOREACH(ListElement, current_element, list) {
        counter++;
    }

    list->iterator = iterator;
    return counter;
}

ListElement listGetNext(List list) {
    if (list == NULL) return NULL;
    if (list->iterator == NULL) return NULL;

    Node next_node = nodeGetNext(list->iterator);
    if (next_node == NULL) {
        list->iterator = NULL;
        return NULL;
    }

    list->iterator = next_node;
    return nodeGetElement(list->iterator);
}

ListResult listInsertFirst(List list, ListElement element) {
    if (list == NULL || element == NULL) return LIST_NULL_ARGUMENT;

    Node iterator = list->iterator;
    listGetFirst(list);
    ListResult list_error = listInsertBeforeCurrent(list, element);
    if (list_error == LIST_OUT_OF_MEMORY) return LIST_OUT_OF_MEMORY;
    assert(list_error == LIST_SUCCESS);

    list->iterator = iterator;
    return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element) {
    if (list == NULL || element == NULL) return LIST_NULL_ARGUMENT;
    if (list->iterator == NULL) return LIST_INVALID_CURRENT;

    NodeResult node_error;
    Node new_node = nodeCreate(element, list->copyElement, list->freeElement);
    if (new_node == NULL) return LIST_OUT_OF_MEMORY;

    if (list->iterator == list->head) {
        node_error = nodeAddNext(new_node, list->iterator);
        assert(node_error == NODE_OK);
        list->head = new_node;
        return LIST_SUCCESS;
    }

    Node iterator = list->iterator;
    LIST_FOREACH(ListElement, current_element, list) {
        Node next_node = nodeGetNext(list->iterator);
        if(next_node == iterator) {
            node_error = nodeRemoveNext(list->iterator);
            assert(node_error == NODE_OK);
            node_error = nodeAddNext(list->iterator, new_node);
            assert(node_error == NODE_OK);
            break;
        }
    }

    node_error = nodeAddNext(new_node, list->iterator);
    assert(node_error == NODE_OK);
    return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list){
    if(list == NULL) return LIST_NULL_ARGUMENT;
    if(list->iterator == NULL) return LIST_INVALID_CURRENT;

    NodeResult node_error;

    if(list->iterator == list->head) {
        Node next_node = nodeGetNext(list->iterator);
        if(next_node == NULL) {
            node_error = nodeClear(list->iterator);
            return LIST_SUCCESS;
        }
        nodeDestroy(list->iterator);
        list->head = next_node;
        return LIST_SUCCESS;
    }

    Node iterator = list->iterator;
    LIST_FOREACH(ListElement, current_element, list) {
        Node next_node = nodeGetNext(list->iterator);
        if(next_node == iterator) {
            Node temp_node = nodeGetNext(next_node);
            node_error = nodeRemoveNext(list->iterator);
            assert(node_error == NODE_OK);
            node_error = nodeAddNext(list->iterator, temp_node);
            assert(node_error == NODE_OK);
            nodeDestroy(iterator);
            break;
        }
    }

    return LIST_SUCCESS;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key){
    if(list == NULL || filterElement == NULL) return NULL;

    ListResult list_error;

    List list_copy = listCopy(list);
    if(list_copy == NULL) return NULL;

    Node current_node = list_copy->head;
    while(current_node != NULL) {
        Node temp_node = nodeGetNext(current_node);
        if(nodeFilter(current_node, filterElement, key) == false) {
            list_copy->iterator = current_node;
            list_error = listRemoveCurrent(list_copy);
            assert(list_error == LIST_SUCCESS);
        }
        current_node = temp_node;
    }

    list_copy->iterator = NULL;
    return list_copy;
}

void listDestroy(List list) {
    if(list == NULL) return;

    ListResult list_error = listClear(list);
    assert(list_error == LIST_SUCCESS);

    nodeDestroy(list->head);
    free(list);
}