#include "logging/app_logger.h"

#include <QDateTime>
#include <QFile>
#include <QMutexLocker>
#include <QTextStream>

#include <stdexcept>

AppLogger::AppLogger(QString logFilePath)
    : logFilePath_(std::move(logFilePath)) {}

void AppLogger::log(const Severity severity, const QString& message) {
    QMutexLocker locker(&mutex_);

    QFile file(logFilePath_);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        throw std::runtime_error("Не удалось открыть лог-файл: " + logFilePath_.toStdString());
    }

    QTextStream out(&file);
    const QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
    out << timestamp << " | " << severityToString(severity) << " | " << message << '\n';
}

QString AppLogger::severityToString(const Severity severity) {
    switch (severity) {
    case Severity::Info:
        return "INFO";
    case Severity::Warning:
        return "WARNING";
    case Severity::Error:
        return "ERROR";
    }

    return "UNKNOWN";
}
