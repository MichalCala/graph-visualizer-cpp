

#include <iostream>
#include <vector>

#include "Graph.hpp"

int main()
{
    Graph graph(10, false);

    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 3);
    graph.AddEdge(3, 4);
    graph.AddEdge(3, 7);
    graph.AddEdge(4, 5);
    graph.AddEdge(5, 9);
    graph.AddEdge(9, 9);
    graph.AddEdge(6, 8);
    graph.AddEdge(8, 6);
    graph.AddEdge(0, 8);

    std::cout << "Liczba wierzcholkow: "
              << graph.NumberOfVertices() << "\n";

    std::cout << "Liczba krawedzi: "
              << graph.NumberOfEdges() << "\n";

    std::vector<int> order = graph.DFS(0);

    std::cout << "Kolejnosc DFS: ";

    for (int vertex : order)
    {
        std::cout << vertex << " ";
    }

    std::cout << "\n";

    return 0;
}