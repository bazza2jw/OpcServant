#-------------------------------------------------
#
# Project created by QtCreator 2019-11-24T11:37:52
#
#-------------------------------------------------
include (../../Common.pri)
QT       -= core gui
TARGET = Values
TEMPLATE = lib
CONFIG += plugin
DESTDIR=$$PLUGINSDIR
LIBS += -L$$LIBDIR -lMrlLib -lCommon -lOpen62541Cpp


TEMPLATE = lib

DEFINES += VALUES_LIBRARY



SOURCES += \
        values.cpp \
    ValuesUI_values_bitmaps.cpp \
    ValuesUI.cpp \
    ValueConfigurationDialog.cpp \
    valueobjectmanager.cpp \
    valuert.cpp \
    valuetypeobject.cpp \
    valuesetupwebdialog.cpp

HEADERS += \
        values.h \
        values_global.h \ 
    ValuesUI.h \
    ValueConfigurationDialog.h \
    valueobjectmanager.h \
    valuert.h \
    valuetypeobject.h \
    valuesetupwebdialog.h

