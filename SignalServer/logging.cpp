#include "logging.h"

void Logging::setLoggingFilters(QtMsgType level)
{
    QString filterStr = "";

    switch (level) {
    case QtMsgType::QtFatalMsg:
    case QtMsgType::QtCriticalMsg: filterStr += "*.critical=false\n";
    case QtMsgType::QtWarningMsg: filterStr += "*.warning=false\n";
    case QtMsgType::QtInfoMsg: filterStr += "*.info=false\n";
    case QtMsgType::QtDebugMsg: filterStr += "*.debug=false\n";
    }

    QLoggingCategory::setFilterRules(filterStr);
}
