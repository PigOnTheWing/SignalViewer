#ifndef LOGGING_H
#define LOGGING_H

#include <QFile>
#include <QFlags>
#include <QScopedPointer>
#include <QLoggingCategory>
#include <QDateTime>

class Logger {

public:
    Logger() {}
    ~Logger() {}

    enum OutputOption {
        StandardOutput = 0x1,
        File = 0x2
    };
    Q_DECLARE_FLAGS(OutputOptions, OutputOption)

    void setLoggingFilters();

    bool setLoggingFile(QString filename);
    void setOutputOptions(OutputOptions options);
    void setLevel(QtMsgType level);

    void handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    QScopedPointer<QFile> m_loggingFile;
    OutputOptions m_outputOptions;
    QtMsgType m_level;

    void printMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg, QTextStream &out);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Logger::OutputOptions);

#endif // LOGGING_H
