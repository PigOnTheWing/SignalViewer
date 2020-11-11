#include "qsettingswrapper.h"

QSettingsWrapper::QSettingsWrapper(QString filename) : mSettings(filename, QSettings::IniFormat)
{

}

QSettingsWrapper::~QSettingsWrapper()
{

}

int QSettingsWrapper::getPort()
{
    return mSettings.value("General/port", defaultPort).toInt();
}

QString QSettingsWrapper::getLoggingFile()
{
    return mSettings.value("Logging/filename", defaultLogFilename).toString();
}

QtMsgType QSettingsWrapper::getLoggingLevel()
{
    QVariant levelVariant = mSettings.value("Logging/level");
    if (levelVariant.isNull()) {
        return defaultMsgType;
    }
    return static_cast<QtMsgType>(levelVariant.toInt());
}

Logger::OutputOptions QSettingsWrapper::getOutputOptions()
{
    QVariant optionsVariant = mSettings.value("Logging/output");
    if (optionsVariant.isNull()) {
        return defaultOutOptions;
    }
    return static_cast<Logger::OutputOptions>(optionsVariant.toInt());
}
