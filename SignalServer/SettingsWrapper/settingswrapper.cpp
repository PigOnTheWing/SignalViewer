#include "settingswrapper.h"

SettingsWrapper::SettingsWrapper(QString filename) : m_settings(filename, QSettings::IniFormat)
{
}

SettingsWrapper::~SettingsWrapper()
{

}

int SettingsWrapper::getPort()
{
    return m_settings.value("General/port", defaultPort).toInt();
}

QString SettingsWrapper::getLoggingFile()
{
    return m_settings.value("Logging/filename", defaultLogFilename).toString();
}

QtMsgType SettingsWrapper::getLoggingLevel()
{
    QVariant levelVariant = m_settings.value("Logging/level");
    if (levelVariant.isNull()) {
        return defaultMsgType;
    }
    return static_cast<QtMsgType>(levelVariant.toInt());
}

Logger::OutputOptions SettingsWrapper::getOutputOptions()
{
    QVariant optionsVariant = m_settings.value("Logging/output");
    if (optionsVariant.isNull()) {
        return defaultOutOptions;
    }
    return static_cast<Logger::OutputOptions>(optionsVariant.toInt());
}
