//
// Created by Ilya on 12/14/2017.
//

#ifndef ASSIGNMENT3_GRAPH_H
#define ASSIGNMENT3_GRAPH_H

#include "set.h"

typedef struct Graph_t *Graph;

typedef void *VertexLabel;

typedef VertexLabel (*CopyVertexLabel)(VertexLabel);

typedef void (*FreeVertexLabel)(VertexLabel);

typedef int (*CompareVertexLabel)(VertexLabel, VertexLabel);

typedef enum GrapResult_t {
    GRAPH_SUCCCESS,
    GRAPH_OUT_OF_MEMORY,
    GRAPH_NULL_ARGUMENT,
    GRAPH_VERTEX_ALREADY_EXISTS,
    GRAPH_VERTEX_DOES_NOT_EXIST,
    GRAPH_EDGE_DOES_NOT_EXIST,
    GRAPH_EDGE_ALREADY_EXISTS
} GraphResult;

/*  Creates a new Graph
 *  allocates new memory for it
 *  check by assert assuring functions are not null
 *  returns:
 *  NULL if memory cannot be allocated
 *  otherwise returns a pointer to the new Graph
 */
Graph graphCreate(CopyVertexLabel copyLabel, FreeVertexLabel freeLabel,
                  CompareVertexLabel compareLabel);

/* Adds a new vertex to the Graph
 * allocates new memory for the vertex
 * returns:
 * GRAPH_OUT_OF_MEMORY if memory allocation failed
 * GRAPH_NULL_ARGUMENT if graph or vertexlabel are NULL
 * GRAPH_VERTEX_ALREADY_EXISTS if the given vertex already exists
 * otherwise GRAPH_SUCCESS
 */
GraphResult graphAddVertex(Graph, VertexLabel);

/* Adds a new Edge between two vertices
 * allocates new memory for the edge
 * returns:
 * GRAPH_OUT_OF_MEMORY if memory allocation failed
 * GRAPH_NULL_ARGUMENT if graph or vertexlabel are NULL
 * GRAPH_VERTEX_DOES_NOT_EXIST if one of the given vertices doesn't exist
 * GRAPH_EDGE_ALREADY_EXISTS if edge between the two given vertices already exists
 * otherwise GRAPH_SUCCESS
 */
GraphResult graphAddEdge(Graph, VertexLabel, VertexLabel);

/* Removes a vertex and it's edges from the graph
 * returns:
 * GRAPH_NULL_ARGUMENT if graph or vertexlabel are NULL
 * GRAPH_VERTEX_DOES_NOT_EXIST if the given vertex doesn't exist
 * otherwise GRAPH_SUCCESS
 *
 */
GraphResult graphRemoveVertex(Graph, VertexLabel);

/* Removes a single edge between two given vertices
 * returns:
 * GRAPH_NULL_ARGUMENT if graph or vertexlabel are NULL
 * GRAPH_VERTEX_DOES_NOT_EXIST if one of the given vertices doesn't exist
 * GRAPH_EDGE_DOES_NOT_EXIST if two of the given vertices doesn't share an edge
 * otherwise GRAPH_SUCCESS
 */
GraphResult graphRemoveEdge(Graph, VertexLabel, VertexLabel);

/* Returns a SET of neighbors connected with a given vertex by edges
 * allocates a net SET of vertexlabels
 * returns:
 * GRAPH_NULL_ARGUMENT if graph or vertexlabel are NULL
 * GRAPH_VERTEX_DOES_NOT_EXIST if the given vertex doesn't exist
 * otherwise GRAPH_SUCCESS
 */
Set graphNeighbors(Graph, VertexLabel);

/* Check's if an edge exists between two given vertices and stores the result in a given result pointer
 * returns:
 * GRAPH_NULL_ARGUMENT if graph or vertexlabel are NULL
 * GRAPH_VERTEX_DOES_NOT_EXIST if one of the given vertices doesn't exist
 * otherwise GRAPH_SUCCESS
 */
GraphResult edgeExists(Graph, VertexLabel, VertexLabel, bool *result);

/* Destroys a GRAPH
 * de-allocates all existing memory in a given Graph
 */
void graphDestroy(Graph);


#endif //ASSIGNMENT3_GRAPH_H
