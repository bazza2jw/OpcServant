#-------------------------------------------------
#
# Project created by QtCreator 2019-12-16T10:46:30
#
#-------------------------------------------------

include (../../Common.pri)
CONFIG += plugin
DESTDIR=$$PLUGINSDIR
LIBS += -L$$LIBDIR -lMrlLib -lCommon -lOpen62541Cpp  -lmosquittopp
QT       -= core gui

TARGET = HttpSend
TEMPLATE = lib

DEFINES += HTTPSEND_LIBRARY


SOURCES += \
        httpsend.cpp \
    httpsendobjectmanager.cpp \
    httpsendrt.cpp \
    httpsendwebsetupdialog.cpp \
    HttpSendUI_mqtt_bitmaps.cpp \
    HttpSendSetupDialog.cpp \
    HttpSendUI.cpp

HEADERS += \
        httpsend.h \
        httpsend_global.h \ 
    httpsendobjectmanager.h \
    httpsendrt.h \
    httpsendwebsetupdialog.h \
    HttpSendSetupDialog.h \
    HttpSendUI.h

