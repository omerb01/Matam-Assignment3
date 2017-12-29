//
// Created by Omer on 15/12/2017.
//

#include <assert.h>
#include <stdlib.h>
#include "test_utilities.h"
#include "../Graph.h"

static SetElement copyInt(SetElement element) {
    assert(element != NULL);
    int* num = (int*)element;

    int* num_copy = malloc(sizeof(int));
    *num_copy = *num;

    return (SetElement)num_copy;
}

static void freeInt(SetElement element) {
    free(element);
}

static int compareInt(SetElement element1, SetElement element2) {
    assert(element1 != NULL && element2 != NULL);
    int* num1 = (int*)element1;
    int* num2 = (int*)element2;

    if(*num1 == *num2) return 0;
    else if(*num1 > *num2) return 1;
    else return -1;
}

static bool testGraphCreate() {

    Graph graph = graphCreate(copyInt, freeInt, compareInt);
    ASSERT_TEST(graph != NULL);

    graphDestroy(graph);
    return true;
}

static bool testGraphAddVertex() {

    Graph graph = graphCreate(copyInt, freeInt, compareInt);
    int* element = malloc(sizeof(int));
    GraphResult graph_error;

    *element = 10;
    graph_error = graphAddVertex(NULL, element);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = graphAddVertex(graph, NULL);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = graphAddVertex(graph, element);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    graph_error = graphAddVertex(graph, element);
    ASSERT_TEST(graph_error == GRAPH_VERTEX_ALREADY_EXISTS);

    *element = 20;
    graph_error = graphAddVertex(graph, element);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);

    freeInt(element);
    graphDestroy(graph);
    return true;
}

static bool testGraphAddEdge() {

    Graph graph = graphCreate(copyInt, freeInt, compareInt);
    int* element1 = malloc(sizeof(int));
    int* element2 = malloc(sizeof(int));
    GraphResult graph_error;

    *element1 = 1;
    graphAddVertex(graph, element1);
    *element1 = 2;
    graphAddVertex(graph, element1);
    *element1 = 3;
    graphAddVertex(graph, element1);

    *element1 = 1;
    *element2 = 2;
    graph_error = graphAddEdge(NULL, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = graphAddEdge(graph, NULL, element2);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = graphAddEdge(graph, element1, NULL);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = graphAddEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    graph_error = graphAddEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_EDGE_ALREADY_EXISTS);
    graph_error = graphAddEdge(graph, element2, element1);
    ASSERT_TEST(graph_error == GRAPH_EDGE_ALREADY_EXISTS);

    *element1 = 1;
    *element2 = 3;
    graph_error = graphAddEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);

    *element1 = 1;
    *element2 = 1;
    graph_error = graphAddEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_INVALID_ARGUMENT);

    *element1 = 1;
    *element2 = 4;
    graph_error = graphAddEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_VERTEX_DOES_NOT_EXIST);

    *element1 = 4;
    *element2 = 4;
    graph_error = graphAddEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_VERTEX_DOES_NOT_EXIST);

    freeInt(element1);
    freeInt(element2);
    graphDestroy(graph);
    return true;
}

static bool testGraphRemoveVertex() {

    Graph graph = graphCreate(copyInt, freeInt, compareInt);
    int* element1 = malloc(sizeof(int));
    int* element2 = malloc(sizeof(int));
    GraphResult graph_error;

    *element1 = 1;
    graphAddVertex(graph, element1);
    *element1 = 2;
    graphAddVertex(graph, element1);
    *element1 = 3;
    graphAddVertex(graph, element1);

    *element1 = 1;
    *element2 = 2;
    graphAddEdge(graph, element1, element2);

    *element1 = 3;
    *element2 = 1;
    graphAddEdge(graph, element1, element2);

    *element1 = 2;
    *element2 = 3;
    graphAddEdge(graph, element1, element2);

    *element1 = 1;
    graph_error = graphRemoveVertex(NULL, element1);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = graphRemoveVertex(graph, NULL);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = graphRemoveVertex(graph, element1);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    graph_error = graphAddVertex(graph, element1);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);

    *element1 = 1;
    *element2 = 2;
    graph_error = graphAddEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);

    *element1 = 3;
    *element2 = 1;
    graph_error = graphAddEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);

    *element1 = 2;
    *element2 = 3;
    graph_error = graphAddEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_EDGE_ALREADY_EXISTS);

    *element1 = 4;
    graph_error = graphRemoveVertex(graph, element1);
    ASSERT_TEST(graph_error == GRAPH_VERTEX_DOES_NOT_EXIST);

    freeInt(element1);
    freeInt(element2);
    graphDestroy(graph);
    return true;
}

static bool testGraphRemoveEdge() {

    Graph graph = graphCreate(copyInt, freeInt, compareInt);
    int* element1 = malloc(sizeof(int));
    int* element2 = malloc(sizeof(int));
    GraphResult graph_error;

    *element1 = 1;
    graphAddVertex(graph, element1);
    *element1 = 2;
    graphAddVertex(graph, element1);
    *element1 = 3;
    graphAddVertex(graph, element1);

    *element1 = 1;
    *element2 = 2;
    graphAddEdge(graph, element1, element2);
    graph_error = graphRemoveEdge(NULL, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = graphRemoveEdge(graph, NULL, element2);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = graphRemoveEdge(graph, element1, NULL);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = graphRemoveEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    graph_error = graphAddEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);

    *element1 = 1;
    *element2 = 3;
    graph_error = graphRemoveEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_EDGE_DOES_NOT_EXIST);

    *element1 = 1;
    *element2 = 1;
    graph_error = graphRemoveEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_EDGE_DOES_NOT_EXIST);

    *element1 = 1;
    *element2 = 4;
    graph_error = graphRemoveEdge(graph, element1, element2);
    ASSERT_TEST(graph_error == GRAPH_VERTEX_DOES_NOT_EXIST);

    freeInt(element1);
    freeInt(element2);
    graphDestroy(graph);
    return true;
}

static bool testGraphNeighbors() {

    Graph graph = graphCreate(copyInt, freeInt, compareInt);
    int* element1 = malloc(sizeof(int));
    int* element2 = malloc(sizeof(int));
    Set neighbors;

    *element1 = 1;
    graphAddVertex(graph, element1);
    *element1 = 2;
    graphAddVertex(graph, element1);
    *element1 = 3;
    graphAddVertex(graph, element1);
    *element1 = 4;
    graphAddVertex(graph, element1);
    *element1 = 5;
    graphAddVertex(graph, element1);

    *element1 = 1;
    *element2 = 2;
    graphAddEdge(graph, element1, element2);

    *element1 = 1;
    *element2 = 3;
    graphAddEdge(graph, element1, element2);

    *element1 = 1;
    *element2 = 4;
    graphAddEdge(graph, element1, element2);

    *element1 = 6;
    neighbors = graphNeighbors(graph, element1);
    ASSERT_TEST(neighbors == NULL);

    *element1 = 1;
    neighbors = graphNeighbors(NULL, element1);
    ASSERT_TEST(neighbors == NULL);
    neighbors = graphNeighbors(graph, NULL);
    ASSERT_TEST(neighbors == NULL);

    *element1 = 1;
    neighbors = graphNeighbors(graph, element1);
    ASSERT_TEST(neighbors != NULL);
    ASSERT_TEST(setGetSize(neighbors) == 3);

    int i = 0;
    SET_FOREACH(int*, iterator, neighbors) {
        if(i == 0) ASSERT_TEST(*iterator == 2);
        if(i == 1) ASSERT_TEST(*iterator == 3);
        if(i == 2) ASSERT_TEST(*iterator == 4);
        i++;
    }
    setDestroy(neighbors);

    *element1 = 5;
    neighbors = graphNeighbors(graph, element1);
    ASSERT_TEST(neighbors != NULL);
    ASSERT_TEST(setGetSize(neighbors) == 0);
    setDestroy(neighbors);

    freeInt(element1);
    freeInt(element2);
    graphDestroy(graph);
    return true;
}

static bool testVertexExists() {

    Graph graph = graphCreate(copyInt, freeInt, compareInt);
    int* element1 = malloc(sizeof(int));
    int* element2 = malloc(sizeof(int));
    GraphResult graph_error;
    bool result;

    *element1 = 0;
    graph_error = vertexExists(NULL, element1, &result);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = vertexExists(graph, NULL, &result);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = vertexExists(graph, element1, NULL);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);

    *element1 = 1;
    graph_error = vertexExists(graph, element1, &result);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    ASSERT_TEST(result == false);
    graphAddVertex(graph, element1);
    graph_error = vertexExists(graph, element1, &result);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    ASSERT_TEST(result == true);

    *element1 = 2;
    graph_error = vertexExists(graph, element1, &result);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    ASSERT_TEST(result == false);
    graphAddVertex(graph, element1);
    graph_error = vertexExists(graph, element1, &result);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    ASSERT_TEST(result == true);

    freeInt(element1);
    freeInt(element2);
    graphDestroy(graph);
    return true;
}

static bool testEdgeExists() {

    Graph graph = graphCreate(copyInt, freeInt, compareInt);
    int* element1 = malloc(sizeof(int));
    int* element2 = malloc(sizeof(int));
    GraphResult graph_error;
    bool result;

    *element1 = 0;
    *element2 = 0;
    graph_error = edgeExists(NULL, element1, element2, &result);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = edgeExists(graph, NULL, element2, &result);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = edgeExists(graph, element1, NULL, &result);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);
    graph_error = edgeExists(graph, element1, element2, NULL);
    ASSERT_TEST(graph_error == GRAPH_NULL_ARGUMENT);

    *element1 = 1;
    graphAddVertex(graph, element1);
    *element1 = 2;
    graphAddVertex(graph, element1);
    *element1 = 3;
    graphAddVertex(graph, element1);

    *element1 = 1;
    *element1 = 6;
    graph_error = edgeExists(graph, element1, element2, &result);
    ASSERT_TEST(graph_error == GRAPH_VERTEX_DOES_NOT_EXIST);

    *element1 = 1;
    *element2 = 2;
    graph_error = edgeExists(graph, element1, element2, &result);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    ASSERT_TEST(result == false);

    *element1 = 1;
    *element2 = 2;
    graphAddEdge(graph, element1, element2);

    *element1 = 2;
    *element2 = 3;
    graphAddEdge(graph, element1, element2);

    *element1 = 1;
    *element2 = 2;
    graph_error = edgeExists(graph, element1, element2, &result);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    ASSERT_TEST(result == true);

    *element1 = 2;
    *element2 = 2;
    graph_error = edgeExists(graph, element1, element2, &result);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    ASSERT_TEST(result == false);

    *element1 = 2;
    *element2 = 3;
    graph_error = edgeExists(graph, element1, element2, &result);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    ASSERT_TEST(result == true);

    *element1 = 3;
    *element2 = 2;
    graph_error = edgeExists(graph, element1, element2, &result);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    ASSERT_TEST(result == true);

    *element1 = 1;
    *element2 = 3;
    graph_error = edgeExists(graph, element1, element2, &result);
    ASSERT_TEST(graph_error == GRAPH_SUCCCESS);
    ASSERT_TEST(result == false);

    freeInt(element1);
    freeInt(element2);
    graphDestroy(graph);
    return true;
}

static bool testGraphDestroy() {

    Graph graph;
    graph = graphCreate(copyInt, freeInt, compareInt);
    graphDestroy(graph);

    graphDestroy(NULL);

    graph = graphCreate(copyInt, freeInt, compareInt);
    int* element1 = malloc(sizeof(int));
    int* element2 = malloc(sizeof(int));

    *element1 = 1;
    graphAddVertex(graph, element1);
    *element1 = 2;
    graphAddVertex(graph, element1);

    *element1 = 1;
    *element2 = 2;
    graphAddEdge(graph, element1, element2);

    freeInt(element1);
    freeInt(element2);
    graphDestroy(graph);
    return true;
}

int main(){
    RUN_TEST(testGraphCreate);
    RUN_TEST(testGraphAddVertex);
    RUN_TEST(testGraphAddEdge);
    RUN_TEST(testGraphRemoveVertex);
    RUN_TEST(testGraphRemoveEdge);
    RUN_TEST(testGraphNeighbors);
    RUN_TEST(testVertexExists);
    RUN_TEST(testEdgeExists);
    RUN_TEST(testGraphDestroy);
    return 0;
}