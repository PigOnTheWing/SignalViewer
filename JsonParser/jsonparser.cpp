#include <QJsonDocument>
#include <QJsonObject>
#include "jsonparser.h"

QByteArray JsonParser::toJson(const JsonParser::Message m)
{
    QJsonObject msgObj;
    msgObj["Command"] = m.command;

    switch (m.command) {
    case JsonParser::NEXT_VAL:
    case JsonParser::AMPLITUDE:
    case JsonParser::PERIOD: {
        msgObj["Attr"] = m.param;
        break;
    }
    default: {}
    }

    return QJsonDocument(msgObj).toJson();
}

JsonParser::Message JsonParser::fromJson(const QByteArray json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject msgObj = doc.object();

    JsonParser::Message m;
    m.command = static_cast<JsonParser::Command>(msgObj.value("Command").toInt());

    switch (m.command) {
    case JsonParser::NEXT_VAL:
    case JsonParser::AMPLITUDE:
    case JsonParser::PERIOD: {
        m.param = msgObj.value("Attr").toDouble();
        break;
    }
    default: {}
    }

    return m;
}
