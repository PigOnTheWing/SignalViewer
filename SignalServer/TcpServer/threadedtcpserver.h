#ifndef THREADEDTCPSERVER_H
#define THREADEDTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class ThreadedTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ThreadedTcpServer(QObject *parent = nullptr);

signals:
    void descriptorReady(qintptr socketDescriptor);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // THREADEDTCPSERVER_H
