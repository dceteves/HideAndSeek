#include "graph.h"

void Graph::InitGraph(int n)
{
    size = n;
    adjacent.SetDimensions(size, size);
    adjacent.InitMatrix();

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            adjacent.ChangeMatrixValue(i, j, 0);
}

int Graph::GetEdge(int v1, int v2) {
    if (v1 > size) {
        std::cout << "Vertex " << v1 << " does not exist\n";
        return -1;
    }
    if (v2 > size) {
        std::cout << "Vertex " << v2 << " does not exist\n";
        return -1;
    }

    return adjacent[v1][v2];
}

void Graph::AddEdge(int v1, int v2, int w) {
    int edge = GetEdge(v1, v2);

    if (!edge) // if edge does not exist
        adjacent.ChangeMatrixValue(v1, v2, w);
}

void Graph::RemoveEdge(int v1, int v2) {
    bool edge = GetEdge(v1, v2);

    if (!edge) // if edge does not exist
        std::cout << "Edge between " << v1 << " & " << v2 << " does not exist\n";
    else {
        adjacent.ChangeMatrixValue(v1, v2, 0);
    }
}


int* Graph::GetAdjacentVertices(int v) {
    if (v > size) {
        std::cout << "Vertex " << v << " does not exist\n";
        return nullptr;
    }
    return adjacent[v];
}

void Graph::DisplayAdjacent() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            std::cout << adjacent[i][j] << " ";
        std::cout << std::endl;
    }
}

void Graph::BFS(int v)
{
    bool* visited = new bool[size];
    for (int i = 0; i < size; i++)
        visited[i] = false;

    std::list<int> queue;

    // visit the first vertex
    visited[v] = true;
    queue.push_back(v);

    while (!queue.empty())
    {
        int* visiting = GetAdjacentVertices(queue.front());

        std::cout << queue.front() << " ";
        queue.pop_front();

        for (int i = 0; i < size; i++)
        {
            if (visiting[i] && !visited[i]) {
                queue.push_back(i);
                visited[i] = true;
            }
        }
    }
    std::cout << std::endl;
}

int* Graph::ShortestPath(int source)
{
    int* distance = new int[size];
    int* previous = new int[size];
    bool* visited = new bool[size];

    // create a min priority queue of pairs that hold 
    // a distance and its vertex (sorted by distance)
    std::priority_queue<int_Pair, std::vector<int_Pair>, std::greater<int_Pair> > pq; 
    
    for (int i = 0; i < size; i++)
    {
        distance[i] = INT_MAX;
        previous[i] = -1;
        visited[i] = false;
    }
    distance[source] = 0;
    visited[source] = true;

    pq.push(std::make_pair(0, source));

    while (!pq.empty())
    {
        int min = pq.top().second;
        pq.pop();

        int* adj_vertices = GetAdjacentVertices(min);
        visited[min] = true;

        int vertex = 0;
        while (vertex < size)
        {
            if (adj_vertices[vertex] > 0)
            {
                int weight = GetEdge(min, vertex);
                if (!visited[vertex] && distance[vertex] > distance[min] + weight)
                {
                    previous[vertex] = min;
                    visited[vertex] = true;
                    distance[vertex] = distance[min] + weight;
                    pq.push(std::make_pair(distance[vertex], vertex));
                }
            }
            vertex++;
        }
    }

    //for (int i = 0; i < size; i++)
    //{
    //    std::cout << previous[i] << " ";
    //}
    //std::cout << std::endl;

    return previous;
}