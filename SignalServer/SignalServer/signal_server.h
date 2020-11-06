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
    explicit SignalServer(quint16 _port = 0, QObject *parent = nullptr);

signals:
    void exitPrematurely();

public slots:
    void startListening();

private slots:
    void initConnection(qintptr decriptor);

private:
     ThreadedTcpServer *server = nullptr;
     quint16 port = 0;
};

#endif // SIGNAL_SERVER_H
