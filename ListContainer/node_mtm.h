//
// Created by Omer on 05/12/2017.
//

#ifndef ASSIGNMENT3_NODE_MTM_H
#define ASSIGNMENT3_NODE_MTM_H

#include <stdbool.h>

typedef void *NodeElement;

typedef void (*FreeNodeElement)(NodeElement);
typedef NodeElement (*CopyNodeElement)(NodeElement);

typedef struct Node_t *Node;

/*******************************************************************/
/* Interface of data type Node  */

typedef enum {
    NODE_OK, NODE_MEMORY_ERROR, NODE_ELEMENT_OVERRIDE
} NodeResult;

//------------------------------------------------------------------------------------------
// Create a new node and connects a given element
// makes duplication for the given element
// returns the new node or NULL if can not create one
Node nodeCreate(NodeElement element, CopyNodeElement, FreeNodeElement);

//------------------------------------------------------------------------------------------
// returns TRUE if the given node doesnt include any element
// otherwise returns FALSE
// checks by assert if the given node isnt NULL
bool nodeIsEmpty(Node node);

//------------------------------------------------------------------------------------------
// creates a new exact copy of a given node
// checks by assert if the given node isnt NULL
// creates new memory by duplication function that stored in the given node
// returns a pointer for the node copy or NULL if failed to allocate memory
Node nodeCopy(Node node);

//------------------------------------------------------------------------------------------
// returns the element that stored by a given node
// checks by assert if the given node isnt NULL
NodeElement nodeGetElement(Node node);

//------------------------------------------------------------------------------------------
// returns the next node which connects to the given node
// checks by assert if the given node isnt NULL
Node nodeGetNext(Node node);

//------------------------------------------------------------------------------------------
// makes comparison between 2 nodes by specific compare function and a sort key
// returns 0 if the nodes are equal, 1 if node1 is bigger than
// node2 and -1 if node1 is smaller than node2
// checks by assert if the given nodes and their stored elements are not NULL
typedef void* NodeSortKey;
typedef int (*CompareNodeElement)(NodeElement, NodeElement, NodeSortKey);
int nodeCompare(Node node1, Node node2, CompareNodeElement, NodeSortKey);

//------------------------------------------------------------------------------------------
// returns TRUE is a given node is setting up a given condition which
// garenteed by filterFunction and a filter key
// overwise returns FALSE
// checks by assert if the given nodes and their stored elements are not NULL
typedef void* NodeFilterKey;
typedef bool (*FilterNodeElement)(NodeElement, NodeFilterKey);
bool nodeFilter(Node node, FilterNodeElement, NodeFilterKey);

//------------------------------------------------------------------------------------------
// connects a given "next node" to the next node
// if node's next != NULL returns NODE_ELEMENT_OVERIDE error
// checks by assert node != NULL && next != NULL
// no need for duplication or destroy
// returns NODE_OK, NODE_ELEMENT_OVERRIDE
NodeResult nodeAddNext(Node node, Node next);

//------------------------------------------------------------------------------------------
// initilize the next node by NULL
// no need to deallocate anything
// checks by assert if the given node isnt NULL
// returns NODE_OK
NodeResult nodeRemoveNext(Node node);

//------------------------------------------------------------------------------------------
// update an existing element in a given position (index) to be identical
// to a given element. the given element must not be changed.
// checks by assert if the given node isnt NULL
// return values : NODE_OK, NODE_MEMORY_ERROR
NodeResult nodeUpdateElement(Node node, NodeElement element);

//------------------------------------------------------------------------------------------
// deallocate an element which stored by a given node
// initilizes element by NULL
// checks by assert if the given node isnt NULL
// return values : NODE_OK
NodeResult nodeClear(Node node);

//------------------------------------------------------------------------------------------
// deallocate all relevant memory and stop using the node
void nodeDestroy(Node);

//------------------------------------------------------------------------------------------

#endif //ASSIGNMENT3_NODE_MTM_H
