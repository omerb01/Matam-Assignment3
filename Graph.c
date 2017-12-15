//
// Created by Omer on 15/12/2017.
//

#include "headers/Graph.h"

struct Graph_t {
    Set vertices;
    Set edges;
};

typedef struct Vertex_t {
    VertexLabel label;
} *Vertex;

typedef struct Edge_t {
    Vertex vertex1;
    Vertex vertex2;
} *Edge;

