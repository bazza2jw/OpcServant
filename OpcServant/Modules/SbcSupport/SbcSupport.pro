#-------------------------------------------------
#
# Project created by QtCreator 2020-01-12T16:21:34
#
#-------------------------------------------------
include (../../Common.pri)
#
# SBC support - gives touch screen access to set the clock etc
#
QT       -= core gui
TARGET = SbcSupport
TEMPLATE = lib
CONFIG += plugin
DESTDIR=$$PLUGINSDIR
LIBS += -L$$LIBDIR -lMrlLib -lCommon -lOpen62541Cpp

DEFINES += SBCSUPPORT_LIBRARY



SOURCES += \
        sbcsupport.cpp \
    SbcSupportUI_sbcsupport_bitmaps.cpp \
    SbcSupportSetupDialog.cpp \
    SbcSupportUI.cpp \
    sbcsupportobjectmanager.cpp \
    sbcsupportrt.cpp \
    SbcSupportTabPanel.cpp \
    SetClock.cpp

HEADERS += \
        sbcsupport.h \
        sbcsupport_global.h \ 
    SbcSupportSetupDialog.h \
    SbcSupportUI.h \
    sbcsupportobjectmanager.h \
    sbcsupportrt.h \
    SbcSupportTabPanel.h \
    SetClock.h


