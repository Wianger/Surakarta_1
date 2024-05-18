QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    choosedlg.cpp \
    main.cpp \
    mainwindow.cpp \
    networkdata.cpp \
    surakartaboard.cpp \
    surakartagame.cpp \
    surakartapiece.cpp \
    surakartareason.cpp \
    surakartarulemanager.cpp

HEADERS += \
    choosedlg.h \
    mainwindow.h \
    networkdata.h \
    surakartaboard.h \
    surakartacommon.h \
    surakartagame.h \
    surakartapiece.h \
    surakartareason.h \
    surakartarulemanager.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
