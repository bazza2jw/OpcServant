#-------------------------------------------------
#
# Project created by QtCreator 2019-11-29T10:06:26
#
#-------------------------------------------------
include (../../Common.pri)
CONFIG += plugin
DESTDIR=$$PLUGINSDIR
LIBS += -L$$LIBDIR -lMrlLib -lCommon -lOpen62541Cpp  -lmosquittopp
QT       -= core gui

TARGET = MQTT
TEMPLATE = lib

DEFINES += MQTT_LIBRARY



SOURCES += \
        mqtt.cpp \
    mqttobjectmanager.cpp \
    mqttrt.cpp \
    MqttSetupDialog.cpp \
    MQTTUI_mqtt_bitmaps.cpp \
    MQTTUI.cpp \
    mqttwebsetupdialog.cpp \
    mqttcommand.cpp

HEADERS += \
        mqtt.h \
        mqtt_global.h \ 
    mqttobjectmanager.h \
    mqttrt.h \
    MqttSetupDialog.h \
    MQTTUI.h \
    mqttwebsetupdialog.h \
    mqttcommand.h

