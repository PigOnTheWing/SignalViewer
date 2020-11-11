#ifndef QSETTINGSWRAPPER_H
#define QSETTINGSWRAPPER_H

#include <QSettings>
#include "defaults.h"
#include "logging.h"


class QSettingsWrapper
{
public:
    QSettingsWrapper(QString filename);
    ~QSettingsWrapper();

    int getPort();
    QString getLoggingFile();
    QtMsgType getLoggingLevel();
    Logger::OutputOptions getOutputOptions();

private:
    QSettings mSettings;
};

#endif // QSETTINGSWRAPPER_H
