#include "model/graph_io.h"

#include <QFile>
#include <QTextStream>

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>

Graph GraphIo::loadFromFile(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Не удалось открыть файл для чтения: " + filePath.toStdString());
    }

    Graph graph;
    QTextStream in(&file);
    std::size_t lineNumber = 0;

    while (!in.atEnd()) {
        const QString rawLine = in.readLine();
        ++lineNumber;
        const QString trimmed = rawLine.trimmed();
        if (trimmed.isEmpty()) {
            continue;
        }

        std::string line = trimmed.toStdString();
        std::replace(line.begin(), line.end(), ':', ' ');

        std::istringstream lineStream(line);
        Graph::Vertex vertex{};
        if (!(lineStream >> vertex)) {
            throw std::runtime_error("Ошибка формата в строке " + std::to_string(lineNumber));
        }

        graph.addVertex(vertex);

        Graph::Vertex neighbor{};
        while (lineStream >> neighbor) {
            graph.addEdge(vertex, neighbor);
        }
    }

    return graph;
}

void GraphIo::saveToFile(const Graph& graph, const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filePath.toStdString());
    }

    QTextStream out(&file);
    for (const auto vertex : graph.vertices()) {
        out << vertex << ": ";

        const auto& adjacency = graph.adjacencyList().at(vertex);
        for (std::size_t i = 0; i < adjacency.size(); ++i) {
            out << adjacency[i];
            if (i + 1 < adjacency.size()) {
                out << ' ';
            }
        }
        out << '\n';
    }
}
