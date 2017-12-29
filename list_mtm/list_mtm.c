#include <stdio.h>
#include <stdlib.h>
#include "list_mtm.h"
#include <assert.h>

typedef struct node_t {
    ListElement element;
    struct node_t *next;
} *Node;

struct List_t {
    Node head;
    Node iterator;
    CopyListElement copy;
    FreeListElement free;
};

#define ASSERT($$condition$$) \
if($$condition$$){} \
assert($$condition$$)

static ListResult
allocateMemory(Node *new_node, ListElement element, List list) {
    ListElement temp = list->copy(element);
    if (temp == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    *new_node = (Node) malloc(sizeof(**(new_node)));
    if (*new_node == NULL) {
        free(temp);
        return LIST_OUT_OF_MEMORY;
    }
    (*new_node)->element = temp;
    (*new_node)->next = NULL;
    return LIST_SUCCESS;
}

static void setOldIterator(Node old_iterator, List list) {
    LIST_FOREACH(ListElement, iterator, list) {
        if ((Node) iterator == old_iterator) break;
    }
}

static void swapElements(Node *element1, Node *element2) {
    ListElement temp = (*element1)->element;
    (*element1)->element = (*element2)->element;
    (*element2)->element = temp;
}

static ListResult maxSort(List list, CompareListElements compareElement,
                          ListSortKey key) {
    ASSERT(list != NULL && compareElement != NULL);
    Node temp = list->iterator;
    for (listGetFirst(list);
         list->iterator != NULL; list->iterator = list->iterator->next) {
        for (temp = list->iterator->next; temp != NULL; temp = temp->next) {
            if (compareElement(list->iterator->element, temp->element, key) >
                0) {
                swapElements(&(list->iterator), &(temp));
            }
            if (temp->next == NULL) {
                break;
            }
        }
        if (list->iterator->next == NULL) {
            break;
        }
    }
    return LIST_SUCCESS;
}

List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
    if (copyElement == NULL || freeElement == NULL) return NULL;
    List new_list = malloc(sizeof(*new_list));
    if (new_list == NULL) return NULL;
    Node head = malloc(sizeof(*head));
    if (head == NULL) return NULL;
    head->element = NULL;
    head->next = NULL;
    new_list->head = head;
    new_list->iterator = new_list->head;
    new_list->free = freeElement;
    new_list->copy = copyElement;
    return new_list;
}

List listCopy(List list) {
    if (list == NULL) return NULL;
    List copied_list = listCreate(list->copy, list->free);
    Node temp_iterator = list->iterator;
    ListResult error;
    LIST_FOREACH(ListElement, iterator, list) {
        error = listInsertLast(copied_list, iterator);
        if (error == LIST_OUT_OF_MEMORY) {
            listDestroy(copied_list);
        } else if (list->iterator == temp_iterator) {
            LIST_FOREACH(Node, copied_list_iterator, copied_list) {}
        }
    }
    list->iterator = temp_iterator;
    return copied_list;

}

int listGetSize(List list) {
    if (list == NULL) return -1;
    else if (list->head->element == NULL) return 0;
    Node temp_iterator = list->iterator;
    int counter = 0;
    LIST_FOREACH(ListElement, current_element, list) {
        counter++;
    }

    list->iterator = temp_iterator;
    return counter;
}

ListElement listGetFirst(List list) {
    ASSERT(list != NULL);
    if (list->head->element == NULL) {
        list->iterator = NULL;
        return NULL;
    }
    list->iterator = list->head;
    return list->head->element;
}

ListElement listGetNext(List list) {
    if (list == NULL) return NULL;
    if (list->iterator == NULL) return NULL;

    Node next_node = list->iterator->next;
    if (next_node == NULL) {
        list->iterator = NULL;
        return NULL;
    }
    list->iterator = next_node;
    return next_node->element;
}

ListElement listGetCurrent(List list) {
    if (list == NULL || list->iterator == NULL) {
        return NULL;
    }
    return list->iterator->element;
}

ListResult listInsertFirst(List list, ListElement element) {
    if (list == NULL || element == NULL) return LIST_NULL_ARGUMENT;
    Node new_node = NULL;
    ListResult error;
    error = allocateMemory(&new_node, element, list);
    if (error == LIST_OUT_OF_MEMORY)
        return LIST_OUT_OF_MEMORY;

    if (list->head->element == NULL) {
        list->head->element = new_node->element;
        free(new_node);
        return LIST_SUCCESS;
    }
    new_node->next = list->head;
    list->head = new_node;
    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element) {
    Node temp_iterator, new_node = NULL;
    if (list == NULL || element == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    if (allocateMemory(&new_node, element, list) == LIST_OUT_OF_MEMORY)
        return LIST_OUT_OF_MEMORY;
    if (list->head->element == NULL) {
        list->head->element = new_node->element;
        free(new_node);
        return LIST_SUCCESS;
    }
    temp_iterator = list->iterator;
    for (list->iterator = list->head;
         list->iterator->next != NULL; list->iterator = list->iterator->next) {}
    list->iterator->next = new_node;
    setOldIterator(temp_iterator, list);
    return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element) {
    if (list == NULL || element == NULL) return LIST_NULL_ARGUMENT;
    if (list->iterator == NULL) return LIST_INVALID_CURRENT;
    if (list->iterator == list->head) {
        listInsertFirst(list, element);
        return LIST_SUCCESS;
    }
    Node new_node, temp_iterator;
    if (allocateMemory(&new_node, element, list) == LIST_OUT_OF_MEMORY)
        return LIST_OUT_OF_MEMORY;
    temp_iterator = list->iterator;
    for (listGetFirst(list); (list->iterator->next != NULL &&
                              list->iterator->next !=
                              temp_iterator); list->iterator = list->iterator->next) {}
    list->iterator->next = new_node;
    new_node->next = temp_iterator;
    list->iterator = temp_iterator;
    return LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(List list, ListElement element) {

    if (list == NULL || element == NULL) {
        return LIST_NULL_ARGUMENT;
    } else if (list->iterator == NULL) {
        return LIST_INVALID_CURRENT;
    }
    Node new_node, temp_iterator;
    if (allocateMemory(&new_node, element, list) == LIST_OUT_OF_MEMORY)
        return LIST_OUT_OF_MEMORY;
    temp_iterator = list->iterator->next;
    list->iterator->next = new_node;
    new_node->next = temp_iterator;
    return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list) {
    if (list == NULL) return LIST_NULL_ARGUMENT;
    if (list->iterator == NULL) return LIST_INVALID_CURRENT;
    Node temp_node, temp_iterator;
    if (list->iterator == list->head) {
        if (list->head->next == NULL) {
            list->free(list->head->element);
            list->head->element = NULL;
        } else {
            temp_node = list->head->next;
            list->free(list->head->element);
            free(list->head);
            list->head = temp_node;
        }
    } else {
        temp_iterator = list->iterator;
        for (listGetFirst(list); (list->iterator->next != NULL &&
                                  list->iterator->next !=
                                  temp_iterator); list->iterator = list->iterator->next) {}
        temp_node = (list->iterator->next)->next;
        list->free((list->iterator->next)->element);
        free((list->iterator->next));
        list->iterator->next = temp_node;
    }
    list->iterator = NULL;
    return LIST_SUCCESS;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {
    if (list == NULL || filterElement == NULL) return NULL;
    ListResult list_error;
    List list_copy = listCopy(list);
    if (list_copy == NULL) return NULL;
    listGetFirst(list_copy);
    while (list_copy->iterator != NULL) {
        if (filterElement(list_copy->iterator->element, key) == false) {
            list_error = listRemoveCurrent(list_copy);
            listGetFirst(list_copy);
            ASSERT(list_error == LIST_SUCCESS);
        }
        listGetNext(list_copy);
    }
    list_copy->iterator = NULL;
    return list_copy;

}

ListResult listClear(List list) {
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    if (list->head->element == NULL) {
        return LIST_SUCCESS;
    } else {
        listGetFirst(list);
        do {
            listRemoveCurrent(list);
            listGetFirst(list);
        } while (list->iterator != NULL);
    }
    return LIST_SUCCESS;
}

ListResult
listSort(List list, CompareListElements compareElement, ListSortKey key) {
    if (list == NULL || compareElement == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    return maxSort(list, compareElement, key);
}

void listDestroy(List list) {
    ASSERT(list != NULL);
    ListResult error;
    error = listClear(list);
    ASSERT(error == LIST_SUCCESS);
    list->free(list->head);
    free(list);
}