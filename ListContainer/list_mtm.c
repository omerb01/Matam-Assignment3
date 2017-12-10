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

List listCopy(List list){
    assert(list != NULL);
    List new_list = listCreate(list->copyElement,list->freeElement);
    if (new_list == NULL){
        return NULL;
    }
    ListResult error;
    int nop_counter=0;
    Node temp_iterator = list->iterator;
    LIST_FOREACH(ListElement, iterator, list){
        error = listInsertLast(new_list,iterator);
        if (error == LIST_OUT_OF_MEMORY){
            listDestroy(new_list);
        }
        if (iterator == temp_iterator){
            LIST_FOREACH(ListElement, iterator2, new_list){
                nop_counter++;
            }
            break;
        }
    }
    list->iterator = temp_iterator;
    return new_list;
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

ListElement listGetFirst(List list){
    assert(list != NULL);
    list->iterator = list->head;
    return nodeGetElement(list->iterator);
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

ListElement listGetCurrent(List list){
    if (list == NULL || list->iterator == NULL){
        return NULL;
    }
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

ListResult listInsertLast(List list, ListElement element){
    ListResult error;
    ListElement temp_iterator;
    Node next_node;
    if (list == NULL || element == NULL){
        return LIST_NULL_ARGUMENT;
    }
    if (nodeIsEmpty(list->head) == true){
        error = listInsertFirst(list,element);
        return error;
    }
    temp_iterator = list->iterator;
    LIST_FOREACH(ListElement, iterator, list){
        next_node = nodeGetNext(list->iterator);
        if (next_node == NULL){
            error = listInsertAfterCurrent(list, iterator);
            if (error == LIST_OUT_OF_MEMORY){ //TODO: make it look better
                return LIST_OUT_OF_MEMORY;
            }
            assert(error == LIST_SUCCESS);
            break;
        }

    }
    list->iterator = temp_iterator;
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

ListResult listInsertAfterCurrent(List list, ListElement element){
    Node new_element, temp_iterator, temp_node;
    if (list == NULL || element == NULL){
        return LIST_NULL_ARGUMENT;
    }
    else if(list->iterator == NULL){
        return LIST_INVALID_CURRENT;
    }
    new_element = nodeCreate(element, list->copyElement,list->freeElement);
    if (new_element == NULL){
        return LIST_OUT_OF_MEMORY;
    }
    temp_iterator = list->iterator;
    LIST_FOREACH(ListElement, iterator, list){
        if (iterator == element){
            temp_node = iterator;
            iterator = nodeGetNext(list->iterator);
            assert (nodeRemoveNext(iterator) == NODE_OK);
            assert (nodeAddNext(iterator, element) == NODE_OK); //TODO: element?
            break;
        }
    }
    assert (nodeAddNext(element,temp_node));
    list->iterator = temp_iterator;
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

static void quickSort(List list, CompareListElements compareElement,ListFilterKey key){

}

ListResult listSort(List list, CompareListElements compareElement, ListSortKey key){
    return LIST_SUCCESS; // DUMMY, for now
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