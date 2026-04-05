#pragma once

#include <QMutex>
#include <QString>

class AppLogger {
public:
    enum class Severity {
        Info,
        Warning,
        Error
    };

    explicit AppLogger(QString logFilePath);
    void log(Severity severity, const QString& message);

private:
    QString logFilePath_;
    QMutex mutex_;

    static QString severityToString(Severity severity);
};
