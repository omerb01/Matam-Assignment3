//
// Created by Omer on 05/12/2017.
//

#ifndef ASSIGNMENT3_NODE_MTM_H
#define ASSIGNMENT3_NODE_MTM_H

typedef void *NodeElement;

typedef int (*compareNodeElement)(NodeElement, NodeElement);
typedef int (*copyNodeElement)(NodeElement, NodeElement);
typedef int (*displayNodeElement)(NodeElement, NodeElement);

typedef struct Node_t *Node;


/*******************************************************************/
/* Interface of data type Node  */


typedef enum {
    NODE_OK, NODE_MEMORY_ERROR, NODE_ILLEGAL_INDEX
} NodeResult;

//------------------------------------------------------------------------------------------
// Create a new node and connects a given element
// uses duplication for the given element
// returns the new node or NULL if can not create one
Node nodeCreate(NodeElement element, compareNodeElement, displayNodeElement, copyNodeElement);

//------------------------------------------------------------------------------------------
// add a given node in a position that appears
// just before a given existing index. First index is 0.
// the added element is inserted by a simple assignment (without duplication).
// return values : NODE_OK, NODE_MEMORY_ERROR, NODE_ILLEGAL_INDEX
NodeResult nodeAddNodeElementBefore(Node, NodeElement, int);

//------------------------------------------------------------------------------------------
// add a given element in a position at the start
// the added element is inserted by a simple assignment (without duplication).
// return values : NODE_OK, NODE_MEMORY_ERROR.
NodeResult nodeAddNodeElementStart(Node, NodeElement);

//------------------------------------------------------------------------------------------
// add a given element in a position at the end
// the added element is inserted by a simple assignment (without duplication).
// return values : NODE_OK, NODE_MEMORY_ERROR.
NodeResult nodeAddNodeElementEnd(Node, NodeElement);

//------------------------------------------------------------------------------------------
// result_index gets an index at which there exists an element that is equal 
// to a given element by compare function
// the result index is the first (lowest) index that is equal or greater to base_index.
// if No element is found, then result_index gets -1.
// node ,element and result_index must not be NULL. (checked by assert).
// return values : NODE_OK, NODE_ILLEGAL_INDEX.
NodeResult
nodeIndexOfNodeElement(Node node, NodeElement element, int base_index,
                       compareNodeElement,
                       int *result_index);

//------------------------------------------------------------------------------------------
// Delete an element at a given position (index)
// return values : NODE_OK, NODE_MEMORY_ERROR , NODE_ILLEGAL_INDEX
NodeResult nodeRemoveNodeElement(Node, int);

//------------------------------------------------------------------------------------------
// update an existing element in a given position (index) to be identical
// to a given element. the given element must not be changed.
// no need to duplicate element. just do a simple assignment.
// return values : NODE_OK , NODE_ILLEGAL_INDEX
NodeResult nodeUpdateNodeElement(Node da, int id, NodeElement element);

//------------------------------------------------------------------------------------------
// display the element in the given index i.
// this function uses displayNodeElement that is guaranteed by the user
// no need to do here '\n'.
// return values : NODE_OK, NODE_ILLEGAL_INDEX
NodeResult nodeDisplayNodeElement(Node da, int i, displayNodeElement);

//------------------------------------------------------------------------------------------
// display all the elements according to their position, from left to right.
// each element is displayed in a single line and '\n at the end of the line.
// the elements are displayed by using the nodeDisplayNodeElement function.
void nodeDisplayAllNodeElements(Node);

//------------------------------------------------------------------------------------------
// returns the number of elements in the array.
int size(Node);

//------------------------------------------------------------------------------------------
// deallocate all relevant memory and stop using the array.
void nodeDestroy(Node);

//------------------------------------------------------------------------------------------

#endif //ASSIGNMENT3_NODE_MTM_H
