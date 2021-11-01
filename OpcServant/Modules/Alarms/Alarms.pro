#-------------------------------------------------
#
# Project created by QtCreator 2019-11-25T16:14:08
#
#-------------------------------------------------
include (../../Common.pri)
QT       -= core gui
TARGET = Alarms
TEMPLATE = lib
CONFIG += plugin
DESTDIR=$$PLUGINSDIR
LIBS += -L$$LIBDIR -lMrlLib -lCommon -lOpen62541Cpp
QT       -= core gui



DEFINES += ALARMS_LIBRARY



SOURCES += \
        alarms.cpp \
    alarmobjectmanager.cpp \
    alarmrt.cpp \
    alarmtypeobject.cpp \
    alarmconfigurationwebdialog.cpp \
    AlarmConfigurationDialog.cpp \
    AlarmsUI_alarms_bitmaps.cpp \
    AlarmsUI.cpp

HEADERS += \
        alarms.h \
        alarms_global.h \ 
    alarmobjectmanager.h \
    alarmrt.h \
    alarmtypeobject.h \
    alarmconfigurationwebdialog.h \
    AlarmConfigurationDialog.h \
    AlarmsUI.h


