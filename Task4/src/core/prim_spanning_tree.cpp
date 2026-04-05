#include "core/prim_spanning_tree.h"

#include <functional>
#include <queue>
#include <stdexcept>
#include <unordered_set>

namespace {
struct Edge {
    Graph::Vertex from{};
    Graph::Vertex to{};
};
}

Graph PrimSpanningTree::build(const Graph& sourceGraph) const {
    if (sourceGraph.empty()) {
        throw std::runtime_error("Граф пуст. Невозможно построить остовное дерево.");
    }

    Graph result;
    const auto vertices = sourceGraph.vertices();
    for (const auto vertex : vertices) {
        result.addVertex(vertex);
    }

    std::unordered_set<Graph::Vertex> visited;

    auto processComponent = [&](const Graph::Vertex startVertex) {
        auto comparator = [](const Edge& left, const Edge& right) {
            if (left.to == right.to) {
                return left.from > right.from;
            }
            return left.to > right.to;
        };

        std::priority_queue<Edge, std::vector<Edge>, decltype(comparator)> edgeQueue(comparator);

        auto addOutgoing = [&](const Graph::Vertex vertex) {
            visited.insert(vertex);
            const auto& neighbors = sourceGraph.adjacencyList().at(vertex);
            for (const auto neighbor : neighbors) {
                if (visited.find(neighbor) == visited.end()) {
                    edgeQueue.push(Edge{vertex, neighbor});
                }
            }
        };

        addOutgoing(startVertex);

        while (!edgeQueue.empty()) {
            const Edge edge = edgeQueue.top();
            edgeQueue.pop();

            if (visited.find(edge.to) != visited.end()) {
                continue;
            }

            result.addEdge(edge.from, edge.to);
            addOutgoing(edge.to);
        }
    };

    for (const auto vertex : vertices) {
        if (visited.find(vertex) == visited.end()) {
            processComponent(vertex);
        }
    }

    return result;
}
