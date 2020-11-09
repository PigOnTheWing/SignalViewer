#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "signal_server.h"
#include "defaults.h"
#include "logging.h"


Logging::LoggingParams g_loggingParams;

void printMsg(QtMsgType type, const QMessageLogContext &context, const QString &msg, QTextStream &out) {
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
    out << context.file << ":" << context.line << ": ";
#endif

    out << Qt::endl;
    out.flush();
}


void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (g_loggingParams.outputOptions.testFlag(Logging::File)) {
        QTextStream out(g_loggingParams.loggingFile.data());
        printMsg(type, context, msg, out);
    }

    if (g_loggingParams.outputOptions.testFlag(Logging::StandardOutput)) {
        QTextStream out(stderr);
        printMsg(type, context, msg, out);
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("SignalServer");

    QCommandLineParser parser;
    parser.setApplicationDescription("A server that generates continuous signal with a given period and amplitude");
    parser.addHelpOption();
    parser.addPositionalArgument("configSource", "A path to the configuration file");

    parser.process(a);

    QString configFile = parser.positionalArguments().first();

    qint16 port;
    QString loggingFile;
    QtMsgType msgType;
    Logging::OutputOptions outOptions;

    QSettings settings(configFile, QSettings::IniFormat);
    if (settings.allKeys().count() == 0) {
        port = defaultPort;
        loggingFile = defaultLogFilename;
        msgType = defaultMsgType;
        outOptions = defaultOutOptions;

        settings.setValue("General/port", port);
        settings.setValue("Logging/filename", loggingFile);
        settings.setValue("Logging/level", QString::number(msgType));
        settings.setValue("Logging/output", QString::number(outOptions));

        qInfo("Creating default config file, port = %d, log file - %s", port, qUtf8Printable(loggingFile));
    } else {
        port = settings.value("General/port").toInt();
        loggingFile = settings.value("Logging/filename").toString();
        msgType = static_cast<QtMsgType>(settings.value("Logging/level").toInt());
        outOptions = static_cast<Logging::OutputOptions>(settings.value("Logging/output").toInt());
    }

    Logging::setLoggingFilters(msgType);

    g_loggingParams.loggingFile.reset(new QFile(loggingFile));
    if (g_loggingParams.outputOptions.testFlag(Logging::File)) {
        if (!g_loggingParams.loggingFile.data()->open(QIODevice::WriteOnly | QIODevice::Append)) {
            qFatal("Could not open file %s, exiting", qUtf8Printable(loggingFile));
        }
    }

    qInstallMessageHandler(messageHandler);

    SignalServer signalServer(port);

    QObject::connect(&signalServer, &SignalServer::exitPrematurely, &a, &QCoreApplication::quit);

    QTimer::singleShot(0, &signalServer, &SignalServer::startListening);

    return a.exec();
}
