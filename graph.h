#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <utility>
#include <functional>
#include "matrix.h"

typedef std::pair<int, int> int_Pair;

class Graph 
{
    Matrix<int> adjacent; // adjacency matrix
    int size;

public:
    Graph() : size(0) {}
    Graph(int n) : adjacent(n, n), size(n) { adjacent.InitMatrix(); }

    void InitGraph(int n);
    int GetEdge(int v1, int v2);
    void AddEdge(int v1, int v2, int w); 
    void RemoveEdge(int v1, int v2); 
    int* GetAdjacentVertices(int v);
    void DisplayAdjacent();
    void BFS(int v);
    int* ShortestPath(int source);

    int get_size() const { return size; }
};

