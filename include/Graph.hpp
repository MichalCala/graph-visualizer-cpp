
#pragma once

#include <vector>

class Graph
{
private:
    int numberOfVertices;
    int numberOfEdges;
    bool directed;

    std::vector<std::vector<bool>> adjacencyMatrix;

    void DFSRecursive(
        int vertex,
        std::vector<bool>& visited,
        std::vector<int>& order
    ) const;

public:
    Graph(int vertices, bool isDirected);

    int AddVertex();
    void AddEdge(int from, int to);

    bool IsEdge(int from, int to) const;

    int NumberOfVertices() const;
    int NumberOfEdges() const;

    std::vector<int> DFS(int startVertex) const;
};