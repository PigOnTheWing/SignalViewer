#include "signal_server.h"

SignalServer::SignalServer(QObject *parent) : QObject(parent)
{
}

void SignalServer::initConnection(qintptr descriptor)
{
    qInfo("Handling incoming connection");

    QThread *sessionThread = new QThread;
    GeneratorWorker *worker = new GeneratorWorker(descriptor);

    worker->moveToThread(sessionThread);
    connect(sessionThread, &QThread::finished, worker, &GeneratorWorker::deleteLater);
    connect(sessionThread, &QThread::finished, sessionThread, &QThread::deleteLater);
    connect(worker, &GeneratorWorker::userDisconnected, sessionThread, &QThread::quit);

    QTimer::singleShot(0, worker, &GeneratorWorker::notifyClient);

    sessionThread->start();
    qInfo("Client thread started");
}

bool SignalServer::startListening(quint16 port)
{
    mServer = new ThreadedTcpServer(this);

    if (!mServer->listen(QHostAddress::Any, port)) {
        qCritical("Failed to start server");
        return false;
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

    qInfo("Server listening on %s:%d", qUtf8Printable(ipAddress), mServer->serverPort());

    connect(mServer, &ThreadedTcpServer::descriptorReady, this, &SignalServer::initConnection);
    return true;
}
