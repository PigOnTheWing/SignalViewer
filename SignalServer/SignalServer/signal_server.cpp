#include "signal_server.h"

SignalServer::SignalServer(QObject *parent) : QObject(parent)
{
}

void SignalServer::initConnection()
{
    QThread *sessionThread = new QThread;
    QTcpSocket *s = server->nextPendingConnection();
    GeneratorWorker *worker = new GeneratorWorker(s);

    worker->moveToThread(sessionThread);
    connect(sessionThread, &QThread::finished, worker, &GeneratorWorker::deleteLater);
    connect(sessionThread, &QThread::finished, sessionThread, &QThread::deleteLater);
    connect(worker, &GeneratorWorker::userDisconnected, sessionThread, &QThread::quit);

    QTimer::singleShot(0, worker, &GeneratorWorker::notifyClient);

    sessionThread->start();
}

void SignalServer::startListening()
{
    server = new QTcpServer(this);

    if (!server->listen()) {
        qFatal("Failed to start server");
        emit exitPrematurely();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    for (int i = 0; i < ipAddressesList.size(); ++i) {
       if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
           ipAddressesList.at(i).toIPv4Address()) {
           ipAddress = ipAddressesList.at(i).toString();
           break;
       }
    }

    if (ipAddress.isEmpty()) {
       ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }

    qInfo("Server listening on %s:%d", qUtf8Printable(ipAddress), server->serverPort());

    connect(server, &QTcpServer::newConnection, this, &SignalServer::initConnection);
}
