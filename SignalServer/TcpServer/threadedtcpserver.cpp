#include "threadedtcpserver.h"

ThreadedTcpServer::ThreadedTcpServer(QObject *parent) : QTcpServer(parent)
{
}

void ThreadedTcpServer::incomingConnection(qintptr socketDescriptor)
{
    emit descriptorReady(socketDescriptor);
}
