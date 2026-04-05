#include <QApplication>
#include <QMessageBox>

#include "controller/graph_controller.h"
#include "logging/app_logger.h"
#include "view/main_window.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    AppLogger logger("task4.log");
    GraphController controller(logger);
    MainWindow window(controller, logger);

    try {
        logger.log(AppLogger::Severity::Info, "Приложение запущено.");
        window.show();
        const int exitCode = QApplication::exec();
        logger.log(AppLogger::Severity::Info, "Приложение завершено. Код: " + QString::number(exitCode));
        return exitCode;
    } catch (const std::exception& ex) {
        QMessageBox::critical(nullptr, "Критическая ошибка", ex.what());
        return 1;
    }
}
