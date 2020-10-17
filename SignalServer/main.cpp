#include <QCoreApplication>
#include "signal_server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SignalServer signalServer;

    QObject::connect(&signalServer, &SignalServer::exitPrematurely, &a, &QCoreApplication::quit);

    QTimer::singleShot(0, &signalServer, &SignalServer::startListening);

    return a.exec();
}
