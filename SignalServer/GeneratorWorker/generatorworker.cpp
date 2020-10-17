#include <jsonparser.h>
#include "generatorworker.h"

GeneratorWorker::GeneratorWorker(QTcpSocket *s, QObject *parent) : QObject(parent), socket(s)
{
    in.setDevice(socket);

    connect(socket, &QAbstractSocket::disconnected, socket, &QTcpSocket::deleteLater);

    connect(this, &GeneratorWorker::generateNew, this, &GeneratorWorker::generateNext);
    connect(socket, &QIODevice::readyRead, this, &GeneratorWorker::handleCommand);
}

void GeneratorWorker::generateNext()
{
    if (isGenerating) {
        qreal nextVal = generator.next();

        JsonParser::Message m = {.command = JsonParser::NEXT_VAL, .param = nextVal};
        QByteArray json = JsonParser::toJson(m);

        socket->write(json);

        emit generateNew();
    }
}

void GeneratorWorker::handleCommand()
{
    in.startTransaction();

    QByteArray json;
    in >> json;

    if (!in.commitTransaction()) {
        return;
    }

    JsonParser::Message m = JsonParser::fromJson(json);
    switch (m.command) {
    case JsonParser::START: {
        isGenerating = true;
        emit generateNew();
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
