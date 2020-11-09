#ifndef DEFAULTS_H
#define DEFAULTS_H

#include "logging.h"
#include <QString>

const Logging::OutputOptions defaultOutOptions = Logging::File | Logging::StandardOutput;
const QtMsgType defaultMsgType = QtMsgType::QtDebugMsg;
const QString defaultLogFilename = "./log";
const quint16 defaultPort = 1234;

#endif // DEFAULTS_H
