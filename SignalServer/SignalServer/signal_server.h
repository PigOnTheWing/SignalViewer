#ifndef SIGNAL_SERVER_H
#define SIGNAL_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QThread>
#include <QNetworkInterface>
#include <QTimer>
#include "generatorworker.h"

class SignalServer : public QObject
{
    Q_OBJECT
public:
    explicit SignalServer(QObject *parent = nullptr);

signals:
    void exitPrematurely();

public slots:
    void startListening();

private slots:
    void initConnection();

private:
    QTcpServer *server = nullptr;
};

#endif // SIGNAL_SERVER_H
