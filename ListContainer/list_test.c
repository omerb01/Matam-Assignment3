//
// Created by Ilya on 12/28/2017.
//

#include <stdio.h>
#include "..\headers\test_utilities.h"
#include "list_mtm.h"
#include <stdlib.h>


int test_counter = 1;
#define ASSERT(test_condition)  \
   {if (!(test_condition)) {printf("TEST %d FAILED at line %d\n", \
    test_counter, __LINE__); } \
   else {printf("TEST %d OK at line %d\n", test_counter, __LINE__);} \
    test_counter++;}

ListElement copyInt(ListElement num) {
    int *result = malloc(sizeof(int));
    if (result == NULL) return NULL;
    *result = *(int *) num;
    return (void *) result;
}

void freeInt(ListElement num) {
    free(num);
}

int compareInt(ListElement num1, ListElement num2, ListSortKey key) {
    if (*(int *) num1 == *(int *) num2) return 0;
    if (*(int *) num1 > *(int *) num2) return 1;
    else return -1;
}

bool filterInt(ListElement num, ListFilterKey key) {
    if (*(int *) num % *(int *) key == 0) return true;
    else return false;
}


/*static bool testCreateList(){
    List new_list = listCreate(copyInt, freeInt);
    listDestroy(new_list);
    return true;
}

static bool testCopyList(){
    List new_list = listCreate(copyInt, freeInt);
    List list = listCreate(copyInt, freeInt);

    int *element = malloc(sizeof(*element));
    *element = 1;
    listInsertFirst(list, (void*)element);
    *element = 2;
    listInsertFirst(list, (void*)element);
    *element = 3;
    listInsertLast(list, (void*)element);
    printList(list);
    free(element);
    listDestroy(list);
    listDestroy(new_list);
    return true;
}*/

int main() {
    ListResult error;
    int i;
    int *element = malloc(sizeof(int));
    printf("start testing list_mtm.c ... \n");

    List list1;
    list1 = listCreate(copyInt, NULL);
    ASSERT(list1 == NULL);
    list1 = listCreate(NULL, freeInt);
    ASSERT(list1 == NULL);
    list1 = listCreate(copyInt, freeInt);
    ASSERT(listGetCurrent(list1) == NULL);
    ASSERT(listGetFirst(list1) == NULL);
    ASSERT(listGetNext(list1) == NULL);

    List temp_list;
    temp_list = listCopy(NULL);
    ASSERT(temp_list == NULL);
    temp_list = listCopy(list1);
    ASSERT(listGetFirst(temp_list) == NULL);
    ASSERT(listGetSize(temp_list) == 0);


    *element = 1;
    error = listInsertFirst(list1, NULL);
    ASSERT(error == LIST_NULL_ARGUMENT);
    error = listInsertFirst(NULL, element);
    ASSERT(error == LIST_NULL_ARGUMENT);
    error = listInsertFirst(list1, element);
    ASSERT(error == LIST_SUCCESS);
    ASSERT(*(int *) listGetFirst(list1) == 1);
    ASSERT(*(int *) listGetCurrent(list1) == 1);
    ASSERT(listGetNext(list1) == NULL);
    ASSERT(listGetSize(list1) == 1);
    *element = 999;
    ASSERT(*(int *) listGetFirst(list1) == 1);

    ASSERT(listGetFirst(temp_list) == NULL);
    ASSERT(listGetSize(temp_list) == 0);
    error = listRemoveCurrent(list1);
    ASSERT(error == LIST_SUCCESS);
    ASSERT(listGetCurrent(list1) == NULL);
    ASSERT(listGetFirst(list1) == NULL);

    *element = 2;
    error = listInsertBeforeCurrent(list1, element);
    ASSERT(error == LIST_INVALID_CURRENT);
    error = listInsertAfterCurrent(list1, element);
    ASSERT(error == LIST_INVALID_CURRENT);
    error = listInsertLast(list1, NULL);
    ASSERT(error == LIST_NULL_ARGUMENT);
    error = listInsertLast(NULL, element);
    ASSERT(error == LIST_NULL_ARGUMENT);
    error = listInsertLast(list1, element);
    ASSERT(error == LIST_SUCCESS);
    ASSERT(*(int *) listGetFirst(list1) == 2);
    ASSERT(*(int *) listGetCurrent(list1) == 2);
    ASSERT(listGetNext(list1) == NULL);
    ASSERT(listGetSize(list1) == 1);
    *element = 999;
    ASSERT(*(int *) listGetFirst(list1) == 2);

    *element = 3;
    error = listInsertLast(list1, element);
    ASSERT(error == LIST_SUCCESS);
    *element = 1;
    error = listInsertFirst(list1, element);
    ASSERT(error == LIST_SUCCESS);

    listGetFirst(list1);
    listGetNext(list1);

    i = 1;
    LIST_FOREACH(int*, current_num, list1) {
        ASSERT(*current_num == i);
        i++;
    }
    ASSERT(listGetCurrent(list1) == NULL);
    ASSERT(listGetSize(list1) == 3);

    listGetFirst(list1);
    *element = 0;
    error = listInsertBeforeCurrent(list1, NULL);
    ASSERT(error == LIST_NULL_ARGUMENT);
    error = listInsertBeforeCurrent(NULL, element);
    ASSERT(error == LIST_NULL_ARGUMENT);
    error = listInsertBeforeCurrent(list1, element);
    ASSERT(error == LIST_SUCCESS);
    ASSERT(*(int *) listGetCurrent(list1) == 1);
    ASSERT(*(int *) listGetFirst(list1) == 0);
    *element = 999;
    ASSERT(*(int *) listGetFirst(list1) == 0);

    i = listGetSize(list1);
    LIST_FOREACH(int*, current_num, list1) {
        if(i == 1) break;
        i--;
    }
    ASSERT(*(int*)listGetCurrent(list1) == 3);

    *element = 4;
    error = listInsertAfterCurrent(list1, NULL);
    ASSERT(error == LIST_NULL_ARGUMENT);
    error = listInsertAfterCurrent(NULL, element);
    ASSERT(error == LIST_NULL_ARGUMENT);
    //printList(list1);
    error = listInsertAfterCurrent(list1, element);
    ASSERT(error == LIST_SUCCESS);
    ASSERT(*(int *) listGetCurrent(list1) == 3);
    ASSERT(*(int *) listGetNext(list1) == 4);
    *element = 999;
    ASSERT(*(int *) listGetCurrent(list1) == 4);

    i = listGetSize(list1);
    LIST_FOREACH(int*, current_num, list1) {
        if(i == 3) break;
        i--;
    }
    ASSERT(*(int*)listGetCurrent(list1) == 2);

    *element = 999;
    error = listInsertBeforeCurrent(list1, element);
    ASSERT(error == LIST_SUCCESS);
    error = listInsertAfterCurrent(list1, element);
    ASSERT(error == LIST_SUCCESS);

    i = listGetSize(list1);
    LIST_FOREACH(int*, current_num, list1) {
        if(i == 7) ASSERT(*current_num == 0);
        if(i == 6) ASSERT(*current_num == 1);
        if(i == 5) ASSERT(*current_num == 999);
        if(i == 4) ASSERT(*current_num == 2);
        if(i == 3) ASSERT(*current_num == 999);
        if(i == 2) ASSERT(*current_num == 3);
        if(i == 1) ASSERT(*current_num == 4);
        i--;
    }

    List list2 = listCopy(list1);
    ASSERT(list2 != NULL);

    i = listGetSize(list2);
    LIST_FOREACH(int*, current_num, list2) {
        if(i == 7) ASSERT(*current_num == 0);
        if(i == 6) ASSERT(*current_num == 1);
        if(i == 5) ASSERT(*current_num == 999);
        if(i == 4) ASSERT(*current_num == 2);
        if(i == 3) ASSERT(*current_num == 999);
        if(i == 2) ASSERT(*current_num == 3);
        if(i == 1) ASSERT(*current_num == 4);
        i--;
    }
    error = listSort(list1, NULL, NULL);
    ASSERT(error == LIST_NULL_ARGUMENT);
    error = listSort(NULL, compareInt, NULL);
    ASSERT(error == LIST_NULL_ARGUMENT);
    error = listSort(list1, compareInt, NULL);
    ASSERT(error == LIST_SUCCESS);

    i = listGetSize(list1);
    LIST_FOREACH(int*, current_num, list1) {
        if (i == 7) ASSERT(*current_num == 0);
        if (i == 6) ASSERT(*current_num == 1);
        if (i == 5) ASSERT(*current_num == 2);
        if (i == 4) ASSERT(*current_num == 3);
        if (i == 3) ASSERT(*current_num == 4);
        if (i == 2) ASSERT(*current_num == 999);
        if (i == 1) ASSERT(*current_num == 999);
        i--;
    }
    i = listGetSize(list2);
    LIST_FOREACH(int*, current_num, list2) {
        if(i == 7) ASSERT(*current_num == 0);
        if(i == 6) ASSERT(*current_num == 1);
        if(i == 5) ASSERT(*current_num == 999);
        if(i == 4) ASSERT(*current_num == 2);
        if(i == 3) ASSERT(*current_num == 999);
        if(i == 2) ASSERT(*current_num == 3);
        if(i == 1) ASSERT(*current_num == 4);
        i--;
    }

    LIST_FOREACH(int*, current_num, list1) {
        if(*current_num == 999) {
            error = listRemoveCurrent(list1); // removes one "999"
            ASSERT(error == LIST_SUCCESS);
        }
    }
    error = listRemoveCurrent(list1);
    ASSERT(error == LIST_INVALID_CURRENT);

    i = listGetSize(list1);
    LIST_FOREACH(int*, current_num, list1) {
        if(i == 6) ASSERT(*current_num == 0);
        if(i == 5) ASSERT(*current_num == 1);
        if(i == 4) ASSERT(*current_num == 2);
        if(i == 3) ASSERT(*current_num == 3);
        if(i == 2) ASSERT(*current_num == 4);
        if(i == 1) ASSERT(*current_num == 999);
        i--;
    }

    LIST_FOREACH(int*, current_num, list1) {
        if(*current_num == 999) {
            error = listRemoveCurrent(list1); // removes one "999"
            ASSERT(error == LIST_SUCCESS);
        }
    }

    i = listGetSize(list1);
    LIST_FOREACH(int*, current_num, list1) {
        if(i == 5) ASSERT(*current_num == 0);
        if(i == 4) ASSERT(*current_num == 1);
        if(i == 3) ASSERT(*current_num == 2);
        if(i == 2) ASSERT(*current_num == 3);
        if(i == 1) ASSERT(*current_num == 4);
        i--;
    }

    int n = 2;
    ListFilterKey key = &n;
    List list3;
    list3 = listFilter(list1, NULL, key);
    ASSERT(list3 == NULL);
    list3 = listFilter(NULL, filterInt, key);
    ASSERT(list3 == NULL);
    list3 = listFilter(list1, filterInt, key);

    i = listGetSize(list3);
    LIST_FOREACH(int*, current_num, list3) {
        if(i == 3) ASSERT(*current_num == 0);
        if(i == 2) ASSERT(*current_num == 2);
        if(i == 1) ASSERT(*current_num == 4);
        i--;
    }

    i = listGetSize(list1);
    LIST_FOREACH(int*, current_num, list1) {
        if(i == 5) ASSERT(*current_num == 0);
        if(i == 4) ASSERT(*current_num == 1);
        if(i == 3) ASSERT(*current_num == 2);
        if(i == 2) ASSERT(*current_num == 3);
        if(i == 1) ASSERT(*current_num == 4);
        i--;
    }

    listDestroy(temp_list);
    listDestroy(list1);
    listDestroy(list2);
    listDestroy(list3);
    free(element);


    printf("FINISHED\n");
}