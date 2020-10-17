QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
        SignalServer/ \
        GeneratorWorker/

SOURCES += \
        GeneratorWorker/generatorworker.cpp \
        SignalServer/signal_server.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    GeneratorWorker/generatorworker.h \
    SignalServer/signal_server.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../JsonParser/release/ -lJsonParser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../JsonParser/debug/ -lJsonParser
else:unix: LIBS += -L$$OUT_PWD/../JsonParser/ -lJsonParser

INCLUDEPATH += $$PWD/../JsonParser
DEPENDPATH += $$PWD/../JsonParser
