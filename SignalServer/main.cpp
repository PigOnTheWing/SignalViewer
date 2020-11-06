#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSettings>
#include <QFile>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>
#include "signal_server.h"
#include "defaults.h"


QScopedPointer<QFile> g_loggingFile;


void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QTextStream out(g_loggingFile.data());
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

#ifdef QT_DEBUG
    out << context.file << ":" << context.line << ": ";
#endif

    out << msg << Qt::endl;
    out.flush();
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

    QSettings settings(configFile, QSettings::IniFormat);
    if (settings.allKeys().count() == 0) {
        port = DEFAULT_PORT;
        loggingFile = DEFAULT_LOGGING_FILENAME;

        settings.setValue("General/port", port);
        settings.setValue("Logging/filename", loggingFile);

        qInfo("Creating default config file, port = %d, log file - %s", port, qUtf8Printable(loggingFile));
    } else {
        port = settings.value("General/port").toInt();
        loggingFile = settings.value("Logging/filename").toString();
    }

    g_loggingFile.reset(new QFile(loggingFile));
    if (g_loggingFile.data()->open(QIODevice::WriteOnly | QIODevice::Append)) {
        qInstallMessageHandler(messageHandler);
    } else {
        qCritical("Failed to open log file, logs will be written to stderr");
    }

    SignalServer signalServer(port);

    QObject::connect(&signalServer, &SignalServer::exitPrematurely, &a, &QCoreApplication::quit);

    QTimer::singleShot(0, &signalServer, &SignalServer::startListening);

    return a.exec();
}
