#pragma once

#include <QLineEdit>
#include <QMainWindow>
#include <QTextEdit>

#include "controller/graph_controller.h"
#include "logging/app_logger.h"

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(GraphController& controller, AppLogger& logger, QWidget* parent = nullptr);

private slots:
    void onBrowseInput();
    void onBrowseOutput();
    void onLoadGraph();
    void onBuildSpanningTree();
    void onSaveSpanningTree();

private:
    GraphController& controller_;
    AppLogger& logger_;
    QLineEdit* inputPathEdit_ = nullptr;
    QLineEdit* outputPathEdit_ = nullptr;
    QTextEdit* statusArea_ = nullptr;

    void appendStatus(const QString& message);
    void showHandledError(const QString& action, const std::exception& ex);
};
