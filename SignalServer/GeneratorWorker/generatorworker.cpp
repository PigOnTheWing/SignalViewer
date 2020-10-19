#include <jsonparser.h>
#include <QTimer>
#include "generatorworker.h"

GeneratorWorker::GeneratorWorker(qintptr descriptor, QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    if (!socket->setSocketDescriptor(descriptor)) {
        qDebug("Failed to create a socket");
    }

    connect(this, &GeneratorWorker::generateNew, this, &GeneratorWorker::generateNext, Qt::QueuedConnection);
    connect(socket, &QIODevice::readyRead, this, &GeneratorWorker::handleCommand);
}

void GeneratorWorker::generateNext()
{
    if (isGenerating) {
        qreal nextVal = generator.next();

        JsonParser::Message m = {.command = JsonParser::NEXT_VAL, .param = nextVal};
        QByteArray json = JsonParser::toJson(m);

        socket->write(json);

        QTimer::singleShot(100, this, &GeneratorWorker::generateNext);
    }
}

void GeneratorWorker::handleCommand()
{
    QByteArray json = socket->readAll();

    JsonParser::Message m = JsonParser::fromJson(json);
    switch (m.command) {
    case JsonParser::START: {
        isGenerating = true;
        QTimer::singleShot(1000, this, &GeneratorWorker::generateNext);
        break;
    }

    case JsonParser::STOP: {
        isGenerating = false;
        break;
    }

    case JsonParser::AMPLITUDE: {
        generator.setAmplitude(m.param);
        break;
    }

    case JsonParser::PERIOD: {
        generator.setPeriod(m.param);
        break;
    }

    case JsonParser::DISCONNECT: {
        isGenerating = false;
        emit userDisconnected();
        break;
    }
    default: { break; }
    }
}

void GeneratorWorker::notifyClient()
{
    JsonParser::Message m = {.command = JsonParser::READY, .param = 0};
    QByteArray json = JsonParser::toJson(m);

    socket->write(json);
}
