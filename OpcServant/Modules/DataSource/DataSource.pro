#-------------------------------------------------
#
# Project created by QtCreator 2019-12-20T16:23:29
#
#-------------------------------------------------
include (../../Common.pri)
CONFIG += plugin
DESTDIR=$$PLUGINSDIR
LIBS += -L$$LIBDIR -lMrlLib -lCommon -lOpen62541Cpp  -lmosquittopp -lmodbus
QT       -= core gui

TARGET = DataSource
TEMPLATE = lib

DEFINES += DATASOURCE_LIBRARY


SOURCES += \
        datasource.cpp \
    MODBUS/datasourcemodbusobjectmanager.cpp \
    OPC/datasourceopcobjectmanager.cpp \
    MQTT/datasourcemqttobjectmanager.cpp \
    MODBUS/datasourcemodbusrt.cpp \
    OPC/datasourceopcrt.cpp \
    MQTT/datasourcemqttrt.cpp

HEADERS += \
        datasource.h \
        datasource_global.h \ 
    MODBUS/datasourcemodbusobjectmanager.h \
    OPC/datasourceopcobjectmanager.h \
    MQTT/datasourcemqttobjectmanager.h \
    MODBUS/datasourcemodbusrt.h \
    OPC/datasourceopcrt.h \
    MQTT/datasourcemqttrt.h

