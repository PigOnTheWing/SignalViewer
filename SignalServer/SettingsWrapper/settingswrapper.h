#ifndef QSETTINGSWRAPPER_H
#define QSETTINGSWRAPPER_H

#include <QSettings>
#include "defaults.h"
#include "logging.h"


class SettingsWrapper
{
public:
    SettingsWrapper(QString filename);
    ~SettingsWrapper();

    int getPort();
    QString getLoggingFile();
    QtMsgType getLoggingLevel();
    Logger::OutputOptions getOutputOptions();

private:
    QSettings m_settings;
};

#endif // QSETTINGSWRAPPER_H
