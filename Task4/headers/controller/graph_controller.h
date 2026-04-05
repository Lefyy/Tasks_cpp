#pragma once

#include <QString>

#include "core/prim_spanning_tree.h"
#include "logging/app_logger.h"
#include "model/graph.h"
#include "model/graph_io.h"

class GraphController {
public:
    explicit GraphController(AppLogger& logger);

    void loadGraph(const QString& filePath);
    void buildSpanningTree();
    void saveSpanningTree(const QString& filePath);

private:
    AppLogger& logger_;
    GraphIo graphIo_;
    PrimSpanningTree primSpanningTree_;
    Graph sourceGraph_;
    Graph spanningTree_;
    bool hasSourceGraph_ = false;
    bool hasSpanningTree_ = false;
};
