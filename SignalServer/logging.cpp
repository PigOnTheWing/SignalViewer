#include "logging.h"

void Logger::setLoggingFilters()
{
    QString filterStr = "";

    switch (m_level) {
    case QtMsgType::QtFatalMsg:
    case QtMsgType::QtCriticalMsg: filterStr += "*.critical=false\n";
    case QtMsgType::QtWarningMsg: filterStr += "*.warning=false\n";
    case QtMsgType::QtInfoMsg: filterStr += "*.info=false\n";
    case QtMsgType::QtDebugMsg: filterStr += "*.debug=false\n";
    }

    QLoggingCategory::setFilterRules(filterStr);
}


bool Logger::setLoggingFile(QString filename)
{
    if (m_outputOptions.testFlag(Logger::File)) {
        m_loggingFile.reset(new QFile(filename));
        return m_loggingFile.data()->open(QIODevice::WriteOnly | QIODevice::Append);
    }
    return true;
}

void Logger::setOutputOptions(OutputOptions options)
{
    m_outputOptions = options;
}

void Logger::setLevel(QtMsgType level)
{
    m_level = level;
}

void Logger::handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (m_outputOptions.testFlag(Logger::File)) {
        QTextStream out(m_loggingFile.data());
        printMessage(type, context, msg, out);
    }

    if (m_outputOptions.testFlag(Logger::StandardOutput)) {
        QTextStream out(stdout);
        printMessage(type, context, msg, out);
    }
}

void Logger::printMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg, QTextStream &out)
{
    out << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ");

    switch (type) {
    case QtDebugMsg: {
        out << "DEBUG ";
        break;
    }
    case QtInfoMsg: {
        out << "INFO ";
        break;
    }
    case QtWarningMsg: {
        out << "WARN ";
        break;
    }
    case QtCriticalMsg: {
        out << "CRITICAL ";
        break;
    }
    case QtFatalMsg: {
        out << "FATAL ";
        break;
    }
    }

    out << msg;

#ifdef QT_DEBUG
    out << "(" << context.file << " :" << context.line << ")";
#endif

    out << Qt::endl;
    out.flush();
}
