#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

class Graph {
public:
    using Vertex = int;
    using AdjacencyList = std::unordered_map<Vertex, std::vector<Vertex>>;

    void clear();
    void addVertex(Vertex vertex);
    void addEdge(Vertex from, Vertex to);
    bool empty() const;

    const AdjacencyList& adjacencyList() const;
    std::vector<Vertex> vertices() const;

private:
    AdjacencyList adjacencyList_;

    static void addUniqueNeighbor(std::vector<Vertex>& neighbors, Vertex value);
    static void sortAndUnique(std::vector<Vertex>& neighbors);
};
