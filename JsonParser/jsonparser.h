#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "JsonParser_global.h"

namespace JsonParser {
    enum JSONPARSER_EXPORT Command {
        READY,
        START,
        STOP,
        NEXT_VAL,
        AMPLITUDE,
        PERIOD,
        DISCONNECT
    };

    struct JSONPARSER_EXPORT Message {
        Command command;
        qreal param;
    };

    JSONPARSER_EXPORT QByteArray toJson(const Message m);
    JSONPARSER_EXPORT Message fromJson(const QByteArray json);
}

#endif // JSONPARSER_H
