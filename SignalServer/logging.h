#ifndef LOGGING_H
#define LOGGING_H

#include <QFile>
#include <QFlags>
#include <QScopedPointer>
#include <QLoggingCategory>

namespace Logging {
    enum OutputOption {
        StandardOutput = 0b00000001,
        File = 0b00000010
    };
    Q_DECLARE_FLAGS(OutputOptions, OutputOption)

    struct LoggingParams {
        QScopedPointer<QFile> loggingFile;
        OutputOptions outputOptions;
        QtMsgType level;
    };

    void setLoggingFilters(QtMsgType level);
}

Q_DECLARE_OPERATORS_FOR_FLAGS(Logging::OutputOptions);

#endif // LOGGING_H
