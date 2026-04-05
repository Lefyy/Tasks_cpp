#pragma once

#include <QString>

#include "model/graph.h"

class GraphIo {
public:
    Graph loadFromFile(const QString& filePath) const;
    void saveToFile(const Graph& graph, const QString& filePath) const;
};
