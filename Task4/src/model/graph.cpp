#include "model/graph.h"

#include <algorithm>

void Graph::clear() {
    adjacencyList_.clear();
}

void Graph::addVertex(const Vertex vertex) {
    adjacencyList_.try_emplace(vertex, std::vector<Vertex>{});
}

void Graph::addEdge(const Vertex from, const Vertex to) {
    addUniqueNeighbor(adjacencyList_[from], to);
    addUniqueNeighbor(adjacencyList_[to], from);
}

bool Graph::empty() const {
    return adjacencyList_.empty();
}

const Graph::AdjacencyList& Graph::adjacencyList() const {
    return adjacencyList_;
}

std::vector<Graph::Vertex> Graph::vertices() const {
    std::vector<Vertex> result;
    result.reserve(adjacencyList_.size());
    std::transform(adjacencyList_.begin(), adjacencyList_.end(), std::back_inserter(result),
                   [](const auto& pair) { return pair.first; });
    std::sort(result.begin(), result.end());
    return result;
}

void Graph::addUniqueNeighbor(std::vector<Vertex>& neighbors, const Vertex value) {
    if (std::find(neighbors.begin(), neighbors.end(), value) == neighbors.end()) {
        neighbors.push_back(value);
    }
    sortAndUnique(neighbors);
}

void Graph::sortAndUnique(std::vector<Vertex>& neighbors) {
    std::sort(neighbors.begin(), neighbors.end());
    neighbors.erase(std::unique(neighbors.begin(), neighbors.end()), neighbors.end());
}
