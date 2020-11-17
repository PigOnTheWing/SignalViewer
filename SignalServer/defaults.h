#ifndef DEFAULTS_H
#define DEFAULTS_H

#include "logging.h"
#include <QString>

const Logger::OutputOptions defaultOutOptions = Logger::File | Logger::StandardOutput;
const QtMsgType defaultMsgType = QtMsgType::QtDebugMsg;
const QString defaultLogFilename = "log.txt";
const quint16 defaultPort = 0;

#endif // DEFAULTS_H
