#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QFileInfo>
#include "signal_server.h"
#include "qsettingswrapper.h"


Logger gLogger;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    gLogger.handleMessage(type, context, msg);
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("SignalServer");

    QCommandLineParser parser;
    parser.setApplicationDescription("A server that generates continuous signal with a given period and amplitude");
    parser.addHelpOption();

    QCommandLineOption configOption(QStringList() << "c" << "config" << "path", "A path to the configuration file", "path to config file");
    parser.addOption(configOption);

    parser.process(a);

    QString configFile = parser.value(configOption);

    if (configFile == "") {
        qCritical("No config option given, exiting");
        exit(0);
    }

    QSettingsWrapper settings(configFile);

    qint16 port = settings.getPort();

    gLogger.setLevel(settings.getLoggingLevel());
    gLogger.setOutputOptions(settings.getOutputOptions());
    if (!gLogger.setLoggingFile(settings.getLoggingFile())) {
        qCritical("Could not open logging file");
        exit(0);
    }

    gLogger.setLoggingFilters();

    qInstallMessageHandler(messageHandler);

    SignalServer signalServer;
    if (!signalServer.startListening(port)) {
        qCritical("Could not start the server");
        exit(0);
    }

    return a.exec();
}
