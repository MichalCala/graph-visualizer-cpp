
#include "Graph.hpp"

#include <stdexcept>

Graph::Graph(int vertices, bool isDirected)
    : numberOfVertices(vertices),
      numberOfEdges(0),
      directed(isDirected)
{
    if (vertices <= 0)
    {
        throw std::invalid_argument(
            "Liczba wierzcholkow musi byc dodatnia"
        );
    }

    adjacencyMatrix.resize(
        vertices,
        std::vector<bool>(vertices, false)
    );
}

void Graph::AddEdge(int from, int to)
{
    if (from < 0 || from >= numberOfVertices ||
        to < 0 || to >= numberOfVertices)
    {
        throw std::out_of_range("Niepoprawny wierzcholek");
    }

    if (adjacencyMatrix[from][to])
    {
        return;
    }

    adjacencyMatrix[from][to] = true;

    if (!directed)
    {
        adjacencyMatrix[to][from] = true;
    }

    numberOfEdges++;
}

bool Graph::IsEdge(int from, int to) const
{
    if (from < 0 || from >= numberOfVertices ||
        to < 0 || to >= numberOfVertices)
    {
        throw std::out_of_range("Niepoprawny wierzcholek");
    }

    return adjacencyMatrix[from][to];
}

int Graph::NumberOfVertices() const
{
    return numberOfVertices;
}

int Graph::NumberOfEdges() const
{
    return numberOfEdges;
}

std::vector<int> Graph::DFS(int startVertex) const
{
    if (startVertex < 0 || startVertex >= numberOfVertices)
    {
        throw std::out_of_range("Niepoprawny wierzcholek");
    }

    std::vector<bool> visited(numberOfVertices, false);
    std::vector<int> order;

    DFSRecursive(startVertex, visited, order);

    return order;
}

void Graph::DFSRecursive(
    int vertex,
    std::vector<bool>& visited,
    std::vector<int>& order
) const
{
    visited[vertex] = true;
    order.push_back(vertex);

    for (int neighbour = 0;
         neighbour < numberOfVertices;
         neighbour++)
    {
        if (adjacencyMatrix[vertex][neighbour] &&
            !visited[neighbour])
        {
            DFSRecursive(neighbour, visited, order);
        }
    }
}