#include "view/main_window.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(GraphController& controller, AppLogger& logger, QWidget* parent)
    : QMainWindow(parent),
      controller_(controller),
      logger_(logger) {
    auto* centralWidget = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(centralWidget);

    auto* inputLayout = new QHBoxLayout();
    auto* inputLabel = new QLabel("Файл исходного графа:", this);
    inputPathEdit_ = new QLineEdit(this);
    auto* inputBrowseButton = new QPushButton("Обзор", this);
    inputLayout->addWidget(inputLabel);
    inputLayout->addWidget(inputPathEdit_);
    inputLayout->addWidget(inputBrowseButton);

    auto* outputLayout = new QHBoxLayout();
    auto* outputLabel = new QLabel("Файл для остовного дерева:", this);
    outputPathEdit_ = new QLineEdit(this);
    auto* outputBrowseButton = new QPushButton("Обзор", this);
    outputLayout->addWidget(outputLabel);
    outputLayout->addWidget(outputPathEdit_);
    outputLayout->addWidget(outputBrowseButton);

    auto* actionsLayout = new QHBoxLayout();
    auto* loadButton = new QPushButton("1) Загрузить граф", this);
    auto* buildButton = new QPushButton("2) Построить остов", this);
    auto* saveButton = new QPushButton("3) Сохранить остов", this);
    actionsLayout->addWidget(loadButton);
    actionsLayout->addWidget(buildButton);
    actionsLayout->addWidget(saveButton);

    statusArea_ = new QTextEdit(this);
    statusArea_->setReadOnly(true);
    statusArea_->setPlaceholderText("Здесь отображаются действия пользователя и статусы приложения...");

    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(outputLayout);
    mainLayout->addLayout(actionsLayout);
    mainLayout->addWidget(statusArea_);

    setCentralWidget(centralWidget);
    setWindowTitle("Task4 — Поиск остовного дерева (Прим)");
    resize(900, 450);

    connect(inputBrowseButton, &QPushButton::clicked, this, &MainWindow::onBrowseInput);
    connect(outputBrowseButton, &QPushButton::clicked, this, &MainWindow::onBrowseOutput);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::onLoadGraph);
    connect(buildButton, &QPushButton::clicked, this, &MainWindow::onBuildSpanningTree);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveSpanningTree);
}

void MainWindow::onBrowseInput() {
    logger_.log(AppLogger::Severity::Info, "Пользователь открыл диалог выбора входного файла.");
    const QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл графа", {}, "Text Files (*.txt);;All Files (*)");
    if (!filePath.isEmpty()) {
        inputPathEdit_->setText(filePath);
        appendStatus("Выбран входной файл: " + filePath);
    }
}

void MainWindow::onBrowseOutput() {
    logger_.log(AppLogger::Severity::Info, "Пользователь открыл диалог выбора выходного файла.");
    const QString filePath = QFileDialog::getSaveFileName(this, "Сохранить остовное дерево", {}, "Text Files (*.txt);;All Files (*)");
    if (!filePath.isEmpty()) {
        outputPathEdit_->setText(filePath);
        appendStatus("Выбран выходной файл: " + filePath);
    }
}

void MainWindow::onLoadGraph() {
    try {
        logger_.log(AppLogger::Severity::Info, "Пользователь запустил загрузку графа.");
        const QString inputPath = inputPathEdit_->text().trimmed();
        if (inputPath.isEmpty()) {
            throw std::runtime_error("Укажите путь к файлу исходного графа.");
        }

        controller_.loadGraph(inputPath);
        appendStatus("Граф загружен успешно.");
    } catch (const std::exception& ex) {
        logger_.log(AppLogger::Severity::Error, "Ошибка загрузки графа: " + QString::fromUtf8(ex.what()));
        showHandledError("загрузке графа", ex);
    }
}

void MainWindow::onBuildSpanningTree() {
    try {
        logger_.log(AppLogger::Severity::Info, "Пользователь запустил построение остовного дерева.");
        controller_.buildSpanningTree();
        appendStatus("Остовное дерево построено.");
    } catch (const std::exception& ex) {
        logger_.log(AppLogger::Severity::Error, "Ошибка построения остовного дерева: " + QString::fromUtf8(ex.what()));
        showHandledError("построении остовного дерева", ex);
    }
}

void MainWindow::onSaveSpanningTree() {
    try {
        logger_.log(AppLogger::Severity::Info, "Пользователь запустил сохранение остовного дерева.");
        const QString outputPath = outputPathEdit_->text().trimmed();
        if (outputPath.isEmpty()) {
            throw std::runtime_error("Укажите путь к файлу сохранения остова.");
        }

        controller_.saveSpanningTree(outputPath);
        appendStatus("Остовное дерево сохранено: " + outputPath);
    } catch (const std::exception& ex) {
        logger_.log(AppLogger::Severity::Error, "Ошибка сохранения остовного дерева: " + QString::fromUtf8(ex.what()));
        showHandledError("сохранении остовного дерева", ex);
    }
}

void MainWindow::appendStatus(const QString& message) {
    statusArea_->append(message);
}

void MainWindow::showHandledError(const QString& action, const std::exception& ex) {
    const QString text = QString("Ошибка при %1: %2").arg(action, ex.what());
    appendStatus(text);
    QMessageBox::warning(this, "Ошибка", text);
}
