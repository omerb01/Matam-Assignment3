//
// Created by Omer on 15/12/2017.
//

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "headers/Graph.h"

#define MALLOC($$type$$, $$variable$$) \
$$type$$ $$variable$$ = malloc(sizeof(*($$variable$$))); \
if(($$variable$$) == NULL) {return NULL;}

#define VERTEX_CREATE($$label$$, $$graph$$) \
vertexCreate($$label$$, ($$graph$$)->copyLabel, ($$graph$$)->freeLabel, \
                ($$graph$$)->compareLabel)

#define EDGE_CREATE($$label1$$, $$label2$$, $$graph$$) \
edgeCreate($$label1$$, $$label2$$, ($$graph$$)->copyLabel, \
                ($$graph$$)->freeLabel, ($$graph$$)->compareLabel)

/* ~~~~~~~~~~~~~~~~~~~~ STRUCTURES DECLARATIONS ~~~~~~~~~~~~~~~~~~~~ */

struct Graph_t {
    Set vertices;
    Set edges;
    CopyVertexLabel copyLabel;
    FreeVertexLabel freeLabel;
    CompareVertexLabel compareLabel;
};

typedef struct Vertex_t {
    VertexLabel label;
    CopyVertexLabel copyLabel;
    FreeVertexLabel freeLabel;
    CompareVertexLabel compareLabel;
} *Vertex;

typedef struct Edge_t {
    Vertex vertex1;
    Vertex vertex2;
} *Edge;

/* ~~~~~~~~~~~~~~~~~~ STATIC FUNCTIONS DECLARATIONS ~~~~~~~~~~~~~~~~~~ */

static Vertex vertexCreate(VertexLabel, CopyVertexLabel,
                           FreeVertexLabel, CompareVertexLabel);

static SetElement vertexCopy(SetElement);

static void vertexDestroy(SetElement);

static int vertexCompare(SetElement, SetElement);

static Edge edgeCreate(VertexLabel, VertexLabel, CopyVertexLabel,
                       FreeVertexLabel, CompareVertexLabel);

static SetElement edgeCopy(SetElement);

static void edgeDestroy(SetElement);

static int edgeCompare(SetElement, SetElement);

static GraphResult convertToGraphResult(SetResult, char *relevant_type);

/* ~~~~~~~~~~~~~~~ VERTEX FUNCTIONS IMPLEMENTATIONS ~~~~~~~~~~~~~~~ */

static Vertex vertexCreate(VertexLabel label, CopyVertexLabel copyLabel,
                           FreeVertexLabel freeLabel,
                           CompareVertexLabel compareLabel) {
    assert(label != NULL && copyLabel != NULL && freeLabel != NULL &&
           compareLabel != NULL);

    MALLOC(Vertex, vertex);

    vertex->copyLabel = copyLabel;
    vertex->freeLabel = freeLabel;
    vertex->compareLabel = compareLabel;

    VertexLabel label_copy = vertex->copyLabel(label);
    if (label_copy == NULL) {
        vertexDestroy(vertex);
        return NULL;
    }
    vertex->label = label_copy;

    return vertex;
}

static SetElement vertexCopy(SetElement element) {
    assert(element != NULL);
    Vertex vertex = (Vertex) element;

    Vertex vertex_copy = vertexCreate(vertex->label, vertex->copyLabel,
                                      vertex->freeLabel, vertex->compareLabel);
    if (vertex_copy == NULL) return NULL;

    return (SetElement) vertex_copy;
}

static void vertexDestroy(SetElement element) {
    if (element == NULL) return;
    Vertex vertex = (Vertex) element;

    vertex->freeLabel(vertex->label);
    free(vertex);
}

static int vertexCompare(SetElement element1, SetElement element2) {
    assert(element1 != NULL && element2 != NULL);
    Vertex vertex1 = (Vertex) element1;
    Vertex vertex2 = (Vertex) element2;

    assert(vertex1->compareLabel == vertex2->compareLabel);
    return vertex1->compareLabel(vertex1->label, vertex2->label);
}

/* ~~~~~~~~~~~~~~~~~ EDGE FUNCTIONS IMPLEMENTATIONS ~~~~~~~~~~~~~~~~~ */

static Edge edgeCreate(VertexLabel label1, VertexLabel label2,
                       CopyVertexLabel copyLabel,
                       FreeVertexLabel freeLabel, CompareVertexLabel
                       compareLabel) {
    assert(label1 != NULL && label2 != NULL && copyLabel != NULL &&
           freeLabel != NULL && compareLabel != NULL);

    MALLOC(Edge, edge);
    edge->vertex1 = vertexCreate(label1, copyLabel, freeLabel, compareLabel);
    edge->vertex2 = vertexCreate(label2, copyLabel, freeLabel, compareLabel);
    if (edge->vertex1 == NULL || edge->vertex2 == NULL) {
        edgeDestroy(edge);
        return NULL;
    }

    return edge;
}

static SetElement edgeCopy(SetElement element) {
    assert(element != NULL);
    Edge edge = (Edge) element;

    Edge edge_copy = edgeCreate(edge->vertex1->label, edge->vertex2->label,
                                edge->vertex1->copyLabel,
                                edge->vertex1->freeLabel,
                                edge->vertex1->compareLabel);
    if (edge_copy == NULL) return NULL;

    return (SetElement) edge_copy;
}

static void edgeDestroy(SetElement element) {
    if (element == NULL) return;
    Edge edge = (Edge) element;

    vertexDestroy(edge->vertex1);
    vertexDestroy(edge->vertex2);
    free(edge);
}

static int edgeCompare(SetElement element1, SetElement element2) {
    assert(element1 != NULL && element2 != NULL);
    Edge edge1 = (Edge) element1;
    Edge edge2 = (Edge) element2;

    if (vertexCompare(edge1->vertex1, edge2->vertex1) == 0 &&
        vertexCompare(edge1->vertex2, edge2->vertex2) == 0) {
        return 0;
    }
    if (vertexCompare(edge1->vertex1, edge2->vertex2) == 0 &&
        vertexCompare(edge1->vertex2, edge2->vertex1) == 0) {
        return 0;
    }

    return 1;
}

/* ~~~~~~~~~~~~~~~~ SET RESULT CONVERTER IMPLEMENTATION ~~~~~~~~~~~~~~~~ */

static GraphResult convertToGraphResult(SetResult set_error, char *
relevant_type) {
    assert(strcmp(relevant_type, "vertex") == 0 ||
           strcmp(relevant_type, "edge") == 0);

    switch (set_error) {
        case SET_SUCCESS:
            return GRAPH_SUCCCESS;
        case SET_OUT_OF_MEMORY:
            return GRAPH_OUT_OF_MEMORY;
        case SET_NULL_ARGUMENT:
            return GRAPH_NULL_ARGUMENT;
        case SET_ITEM_ALREADY_EXISTS:
            if (strcmp(relevant_type, "vertex") == 0) {
                return GRAPH_VERTEX_ALREADY_EXISTS;
            } else {
                return GRAPH_EDGE_ALREADY_EXISTS;
            }
        case SET_ITEM_DOES_NOT_EXIST:
            if (strcmp(relevant_type, "vertex") == 0) {
                return GRAPH_VERTEX_DOES_NOT_EXIST;
            } else {
                return GRAPH_EDGE_DOES_NOT_EXIST;
            }
        default:
            assert(false);
    }

    return GRAPH_SUCCCESS;
}

/* ~~~~~~~~~~~~~~~~~~ GRAPH FUNCTIONS IMPLEMENTATIONS ~~~~~~~~~~~~~~~~~~ */

Graph graphCreate(CopyVertexLabel copyLabel, FreeVertexLabel freeLabel,
                  CompareVertexLabel compareLabel) {
    assert(copyLabel != NULL && freeLabel != NULL && compareLabel != NULL);

    MALLOC(Graph, graph);

    graph->copyLabel = copyLabel;
    graph->freeLabel = freeLabel;
    graph->compareLabel = compareLabel;

    graph->vertices = setCreate(vertexCopy, vertexDestroy, vertexCompare);
    graph->edges = setCreate(edgeCopy, edgeDestroy, edgeCompare);

    if (graph->vertices == NULL || graph->edges == NULL) {
        graphDestroy(graph);
        return NULL;
    }

    return graph;
}

GraphResult graphAddVertex(Graph graph, VertexLabel label) {
    if (graph == NULL || label == NULL) return GRAPH_NULL_ARGUMENT;

    Vertex vertex = VERTEX_CREATE(label, graph);
    if (vertex == NULL) return GRAPH_OUT_OF_MEMORY;

    SetResult set_error = setAdd(graph->vertices, vertex);

    vertexDestroy(vertex);
    return convertToGraphResult(set_error, "vertex");
}

GraphResult graphAddEdge(Graph graph, VertexLabel label1, VertexLabel label2) {
    if (graph == NULL || label1 == NULL || label2 == NULL) {
        return GRAPH_NULL_ARGUMENT;
    }
    GraphResult graph_error;

    bool is_vertex1_exists;
    graph_error = vertexExists(graph, label1, &is_vertex1_exists);
    if (graph_error == GRAPH_OUT_OF_MEMORY) return GRAPH_OUT_OF_MEMORY;
    assert(graph_error == GRAPH_SUCCCESS);
    bool is_vertex2_exists;
    graph_error = vertexExists(graph, label2, &is_vertex2_exists);
    if (graph_error == GRAPH_OUT_OF_MEMORY) return GRAPH_OUT_OF_MEMORY;
    assert(graph_error == GRAPH_SUCCCESS);

    if (!is_vertex1_exists || !is_vertex2_exists) {
        return GRAPH_VERTEX_DOES_NOT_EXIST;
    }

    if (graph->compareLabel(label1, label2) == 0) {
        return GRAPH_INVALID_ARGUMENT;
    }

    Edge edge = EDGE_CREATE(label1, label2, graph);
    if (edge == NULL) return GRAPH_OUT_OF_MEMORY;

    SetResult set_error = setAdd(graph->edges, edge);

    edgeDestroy(edge);
    return convertToGraphResult(set_error, "edge");
}

GraphResult graphRemoveVertex(Graph graph, VertexLabel label) {
    if (graph == NULL || label == NULL) return GRAPH_NULL_ARGUMENT;
    SetResult set_error;

    Vertex vertex = VERTEX_CREATE(label, graph);
    if (vertex == NULL) return GRAPH_OUT_OF_MEMORY;

    SET_FOREACH(Vertex, iterator, graph->vertices) {

        Edge edge = EDGE_CREATE(vertex->label, iterator->label, graph);
        if (edge == NULL) {
            vertexDestroy(vertex);
            return GRAPH_OUT_OF_MEMORY;
        }

        set_error = setRemove(graph->edges, edge);
        assert(set_error == SET_ITEM_DOES_NOT_EXIST ||
               set_error == SET_SUCCESS);

        edgeDestroy(edge);
    }

    set_error = setRemove(graph->vertices, vertex);

    vertexDestroy(vertex);
    return convertToGraphResult(set_error, "vertex");
}

GraphResult graphRemoveEdge(Graph graph, VertexLabel label1,
                            VertexLabel label2) {
    if (graph == NULL || label1 == NULL || label2 == NULL) {
        return GRAPH_NULL_ARGUMENT;
    }
    GraphResult graph_error;

    bool is_vertex1_exists;
    graph_error = vertexExists(graph, label1, &is_vertex1_exists);
    if (graph_error == GRAPH_OUT_OF_MEMORY) return GRAPH_OUT_OF_MEMORY;
    assert(graph_error == GRAPH_SUCCCESS);
    bool is_vertex2_exists;
    graph_error = vertexExists(graph, label2, &is_vertex2_exists);
    if (graph_error == GRAPH_OUT_OF_MEMORY) return GRAPH_OUT_OF_MEMORY;
    assert(graph_error == GRAPH_SUCCCESS);

    if (!is_vertex1_exists || !is_vertex2_exists) {
        return GRAPH_VERTEX_DOES_NOT_EXIST;
    }

    Edge edge = EDGE_CREATE(label1, label2, graph);
    if (edge == NULL) return GRAPH_OUT_OF_MEMORY;

    SetResult set_error = setRemove(graph->edges, edge);

    edgeDestroy(edge);
    return convertToGraphResult(set_error, "edge");
}

Set graphNeighbors(Graph graph, VertexLabel label) {
    if (graph == NULL || label == NULL) {
        return NULL;
    }
    GraphResult graph_error;

    bool is_vertex_exists;
    graph_error = vertexExists(graph, label, &is_vertex_exists);
    if (graph_error == GRAPH_OUT_OF_MEMORY) return NULL;
    assert(graph_error == GRAPH_SUCCCESS);

    if (!is_vertex_exists) return NULL;

    Vertex vertex = VERTEX_CREATE(label, graph);
    if (vertex == NULL) return NULL;

    Set neighbors = setCreate(graph->copyLabel, graph->freeLabel,
                              graph->compareLabel);
    if (neighbors == NULL) {
        vertexDestroy(vertex);
        return NULL;
    }

    SetResult set_error;
    SET_FOREACH(Edge, iterator, graph->edges) {
        set_error = SET_SUCCESS;
        if (vertexCompare(iterator->vertex1, vertex) == 0) {
            set_error = setAdd(neighbors, iterator->vertex2->label);
        } else if (vertexCompare(iterator->vertex2, vertex) == 0) {
            set_error = setAdd(neighbors, iterator->vertex1->label);
        }
        if (set_error == SET_OUT_OF_MEMORY) {
            vertexDestroy(vertex);
            setDestroy(neighbors);
            return NULL;
        }
        assert(set_error == SET_SUCCESS);
    }

    vertexDestroy(vertex);
    return neighbors;
}

GraphResult vertexExists(Graph graph, VertexLabel label, bool *result) {
    if (graph == NULL || label == NULL || result == NULL) {
        return GRAPH_NULL_ARGUMENT;
    }

    Vertex vertex = VERTEX_CREATE(label, graph);
    if (vertex == NULL) return GRAPH_OUT_OF_MEMORY;

    *result = setIsIn(graph->vertices, vertex);
    vertexDestroy(vertex);
    return GRAPH_SUCCCESS;
}

GraphResult edgeExists(Graph graph, VertexLabel label1, VertexLabel label2,
                       bool *result) {
    if (graph == NULL || label1 == NULL || label2 == NULL || result == NULL) {
        return GRAPH_NULL_ARGUMENT;
    }
    GraphResult graph_error;

    bool is_vertex1_exists;
    graph_error = vertexExists(graph, label1, &is_vertex1_exists);
    if (graph_error == GRAPH_OUT_OF_MEMORY) return GRAPH_OUT_OF_MEMORY;
    assert(graph_error == GRAPH_SUCCCESS);
    bool is_vertex2_exists;
    graph_error = vertexExists(graph, label2, &is_vertex2_exists);
    if (graph_error == GRAPH_OUT_OF_MEMORY) return GRAPH_OUT_OF_MEMORY;
    assert(graph_error == GRAPH_SUCCCESS);

    if (!is_vertex1_exists || !is_vertex2_exists) {
        return GRAPH_VERTEX_DOES_NOT_EXIST;
    }

    Edge edge = EDGE_CREATE(label1, label2, graph);
    if (edge == NULL) return GRAPH_OUT_OF_MEMORY;

    *result = setIsIn(graph->edges, edge);
    edgeDestroy(edge);
    return GRAPH_SUCCCESS;
}

void graphDestroy(Graph graph) {
    if (graph == NULL) return;
    setDestroy(graph->vertices);
    setDestroy(graph->edges);
    free(graph);
}
