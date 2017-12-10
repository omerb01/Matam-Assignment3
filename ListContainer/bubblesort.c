//
// Created by Omer on 03/12/2017.
//
#include "bubblesort.h"
#include <stdlib.h>
#include <assert.h>

void swap(ListElement p1, ListElement p2) {
    NodeElement temp = nodeGetElement(p1);
    assert(nodeUpdateElement(p1, nodeGetElement(p2)) == NODE_OK);
    assert(nodeUpdateElement(p2, temp) == NODE_OK);
}

void bubbleSort(List list, CompareListElements compare, ListSortKey key){
    Node left=listGetFirst(list),right=nodeGetNext(left);
    // TODO: list should be bigger than 1
    while(left!=NULL){
        while(right!=NULL){
            if(compare(left,right, key) == 0){
                swap(left,right);
                left=nodeGetNext(left);
            }
            right=nodeGetNext(right);
        }
        left=nodeGetNext(left);
    }
}

