#ifndef SIGNAL_SERVER_H
#define SIGNAL_SERVER_H

#include <QObject>
#include <QThread>
#include <QNetworkInterface>
#include <QTimer>
#include "generatorworker.h"
#include "threadedtcpserver.h"

class SignalServer : public QObject
{
    Q_OBJECT
public:
    explicit SignalServer(QObject *parent = nullptr);
    bool startListening(quint16 port);

private slots:
    void initConnection(qintptr decriptor);

private:
     ThreadedTcpServer *mServer = nullptr;
};

#endif // SIGNAL_SERVER_H
