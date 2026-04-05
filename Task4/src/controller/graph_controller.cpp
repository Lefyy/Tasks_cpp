#include "controller/graph_controller.h"

#include <stdexcept>

GraphController::GraphController(AppLogger& logger)
    : logger_(logger) {}

void GraphController::loadGraph(const QString& filePath) {
    logger_.log(AppLogger::Severity::Info, "Запрос на загрузку графа из файла: " + filePath);

    sourceGraph_ = graphIo_.loadFromFile(filePath);
    hasSourceGraph_ = true;
    hasSpanningTree_ = false;

    logger_.log(AppLogger::Severity::Info, "Граф успешно загружен.");
}

void GraphController::buildSpanningTree() {
    if (!hasSourceGraph_) {
        logger_.log(AppLogger::Severity::Warning, "Попытка построить остов без исходного графа.");
        throw std::runtime_error("Сначала загрузите граф из файла.");
    }

    logger_.log(AppLogger::Severity::Info, "Запущено построение остовного дерева (Прим).");
    spanningTree_ = primSpanningTree_.build(sourceGraph_);
    hasSpanningTree_ = true;
    logger_.log(AppLogger::Severity::Info, "Остовное дерево построено.");
}

void GraphController::saveSpanningTree(const QString& filePath) {
    if (!hasSpanningTree_) {
        logger_.log(AppLogger::Severity::Warning, "Попытка сохранить несуществующее остовное дерево.");
        throw std::runtime_error("Остовное дерево не построено.");
    }

    logger_.log(AppLogger::Severity::Info, "Сохранение остовного дерева в файл: " + filePath);
    graphIo_.saveToFile(spanningTree_, filePath);
    logger_.log(AppLogger::Severity::Info, "Остовное дерево успешно сохранено.");
}
