#-------------------------------------------------
#
# Project created by QtCreator 2019-11-15T15:58:27
#
#-------------------------------------------------
include (../../Common.pri)
QT       -= core gui

TARGET = ICPCON
TEMPLATE = lib
CONFIG += plugin
DESTDIR=$$PLUGINSDIR
LIBS += -L$$LIBDIR -lMrlLib -lCommon -lOpen62541Cpp -lmysqlpp
DEFINES += ICPCON_LIBRARY



SOURCES += \
        icpcon.cpp \
        ICPCON7017/icpcon7017objectmanager.cpp \
        ICPCON7017/icpcon7017objecttype.cpp \
        ICPCON7017/icpcon7017rt.cpp \
    ICPCON7017/ICPCON7017SetupDialog.cpp \
    ICPCON7017/ICPCON7017UI_icpcon7017_bitmaps.cpp \
    ICPCON7017/ICPCON7017UI.cpp \
    ICPCON7017/icpcon7017setupwebdialog.cpp \
    ICPCON7065/icpcon7065objectmanager.cpp \
    ICPCON7065/icpcon7065objecttype.cpp \
    ICPCON7065/icpcon7065rt.cpp \
    ICPCON7065/icpcon7065setupwebdialog.cpp \
    ICPCON7065/icpcon7065nodecontext.cpp \
    ICPCON7065/ICOPCON7065SetupDialog.cpp \
    ICPCON7065/ICPCON7065UI_icpcon7065_bitmaps.cpp \
    ICPCON7065/ICPCON7065UI.cpp \
    ICPCON7017/ICPCON7017Panel.cpp

HEADERS += \
        icpcon.h \
        icpcon_global.h \ 
        ICPCON7017/icpcon7017objectmanager.h \
        ICPCON7017/icpcon7017objecttype.h \
        ICPCON7017/icpcon7017rt.h \
    ICPCON7017/ICPCON7017SetupDialog.h \
    ICPCON7017/ICPCON7017UI.h \
    ICPCON7017/icpcon7017setupwebdialog.h \
    ICPCON7065/icpcon7065objectmanager.h \
    ICPCON7065/icpcon7065objecttype.h \
    ICPCON7065/icpcon7065rt.h \
    ICPCON7065/icpcon7065setupwebdialog.h \
    ICPCON7065/icpcon7065nodecontext.h \
    ICPCON7065/ICOPCON7065SetupDialog.h \
    ICPCON7065/ICPCON7065UI.h \
    ICPCON7017/ICPCON7017Panel.h

