#include "generatorworker.h"

GeneratorWorker::GeneratorWorker(QTcpSocket *s, QObject *parent) : QObject(parent), socket(s)
{
    connect(socket, &QAbstractSocket::disconnected, socket, &QTcpSocket::deleteLater);

    connect(this, &GeneratorWorker::generateNew, this, &GeneratorWorker::generateNext);
    connect(socket, &QIODevice::readyRead, this, &GeneratorWorker::handleCommand);
}

void GeneratorWorker::generateNext()
{
    if (isGenerating) {
        // do generation

        emit generateNew();
    }
}

void GeneratorWorker::handleCommand()
{
    // switch for different commands
    emit userDisconnected();
}

void GeneratorWorker::notifyClient()
{
    return;
}
