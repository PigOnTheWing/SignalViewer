#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QFileInfo>
#include "signal_server.h"
#include "settingswrapper.h"

Logger g_logger;

int g_argc;
char **g_argv;

void appInit();
void close();

void parseArgs(QCommandLineParser &, QString *);
bool parseConfig(QString &, quint16 *);

#ifdef Q_OS_WIN
#include <windows.h>

#define SVCNAME TEXT("SignalServer")

SERVICE_STATUS g_svcStatus;
SERVICE_STATUS_HANDLE g_svcStatusHandle;

VOID WINAPI ctrlHandler(DWORD);
VOID reportStatus(DWORD, DWORD, DWORD);

VOID WINAPI ServiceMain(int, char **);
#endif

int main(int argc, char *argv[])
{
    g_argc = argc;
    g_argv = argv;

#ifdef Q_OS_WIN
    SERVICE_TABLE_ENTRY DispatchTable[] = {
        { SVCNAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain },
        { NULL, NULL }
    };

    if (!StartServiceCtrlDispatcher(DispatchTable)) {
        if (GetLastError() != ERROR_FAILED_SERVICE_CONTROLLER_CONNECT) {
            return 0;
        }
    }
#endif

    appInit();
    close();
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    g_logger.handleMessage(type, context, msg);
}

void appInit() {
    QCoreApplication a(g_argc, g_argv);

    QString configFile;
    QCommandLineParser parser;
    parseArgs(parser, &configFile);

    if (configFile == "") {
        qCritical("No config option given, exiting");
        close();
    }

    quint16 port;
    if (!parseConfig(configFile, &port)) {
        close();
    }

    qInstallMessageHandler(messageHandler);

    SignalServer signalServer;
    if (!signalServer.startListening(port)) {
        qCritical("Could not start the server");
        close();
    }

#ifdef Q_OS_WIN
    reportStatus(SERVICE_RUNNING, NO_ERROR, 0);
#endif

    a.exec();
}

void parseArgs(QCommandLineParser &parser, QString *configFile) {
    parser.setApplicationDescription("A server that generates continuous signal with a given period and amplitude");
    parser.addHelpOption();

    QCommandLineOption configOption(QStringList() << "c" << "config" << "path", "A path to the configuration file", "path to config file");
    parser.addOption(configOption);

    parser.process(QCoreApplication::arguments());

    *configFile = parser.value(configOption);
}

bool parseConfig(QString &configFile, quint16 *port) {
    SettingsWrapper settings(configFile);

    *port = settings.getPort();

    g_logger.setLevel(settings.getLoggingLevel());
    g_logger.setOutputOptions(settings.getOutputOptions());
    if (!g_logger.setLoggingFile(settings.getLoggingFile())) {
        qCritical("Could not open logging file");
        return false;
    }
    g_logger.setLoggingFilters();

    return true;
}

void close() {
#ifdef Q_OS_WIN
    reportStatus(SERVICE_STOPPED, NO_ERROR, 0);
#endif
    exit(EXIT_SUCCESS);
}

#ifdef Q_OS_WIN
VOID WINAPI ServiceMain(int argc, char *argv[])
{
    g_svcStatusHandle = RegisterServiceCtrlHandler(SVCNAME, ctrlHandler);

    if (!g_svcStatusHandle) {
        return;
    }

    g_svcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_svcStatus.dwServiceSpecificExitCode = 0;

    reportStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

    appInit();
    close();
}

VOID WINAPI ctrlHandler(DWORD dwCtrl)
{
    switch (dwCtrl) {
    case SERVICE_CONTROL_STOP:
        reportStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
        QCoreApplication::quit();
        return;
    default:
        break;
    }
}

VOID reportStatus(DWORD currentState, DWORD exitCode, DWORD waitHint) {
    static DWORD checkPoint = 1;

    g_svcStatus.dwCurrentState = currentState;
    g_svcStatus.dwWin32ExitCode = exitCode;
    g_svcStatus.dwWaitHint = waitHint;

    if (currentState == SERVICE_START_PENDING) {
        g_svcStatus.dwControlsAccepted = 0;
    } else {
        g_svcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    }

    if (currentState == SERVICE_RUNNING || currentState == SERVICE_STOPPED) {
        checkPoint = 0;
    } else {
        g_svcStatus.dwCheckPoint = checkPoint++;
    }

    SetServiceStatus(g_svcStatusHandle, &g_svcStatus);
}

#endif
