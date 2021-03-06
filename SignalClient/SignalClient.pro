QT       += core gui network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    Connect/ \
    ChartView/ \
    Width/

SOURCES += \
    ChartView/chartview.cpp \
    Connect/connectiondialog.cpp \
    Width/widthdialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ChartView/chartview.h \
    Connect/connectiondialog.h \
    Width/widthdialog.h \
    mainwindow.h

FORMS += \
    Connect/connectiondialog.ui \
    Width/widthdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../JsonParser/release/ -lJsonParser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../JsonParser/debug/ -lJsonParser
else:unix:!macx: LIBS += -L$$OUT_PWD/../JsonParser/ -lJsonParser

INCLUDEPATH += $$PWD/../JsonParser
DEPENDPATH += $$PWD/../JsonParser
