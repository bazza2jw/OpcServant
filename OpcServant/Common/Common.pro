#-------------------------------------------------
#
# Project created by QtCreator 2018-06-22T08:04:06
#
#-------------------------------------------------
CONFIG += excludeCommon
include (../Common.pri)
QT       -= core gui
TARGET = Common
TEMPLATE = lib
DEFINES += COMMON_LIBRARY
DESTDIR = $$APPDIR
LIBS +=  -lmodbus -lmysqlpp -lmosquittopp
SOURCES += \
    common.cpp \
    database.cpp \
    plugin.cpp \
    Opc/opccommon.cpp \
    datastore.cpp \
    datastoretree.cpp \
    displaypropertytree.cpp \
    objectmanager.cpp \
    bobject.cpp \
    Web/webdatamodel.cpp \
    Web/webaddobjectdialog.cpp \
    Gui/AddObjectDialog.cpp \
    Gui/BourbonCommonUI.cpp \
    Gui/BourbonCommonUI_bourboncommon_bitmaps.cpp \
    objectdataitem.cpp \
    objectdatavalidator.cpp \
    Opc/opcserver.cpp \
    Opc/readonlycontext.cpp \
    Opc/readwritecontext.cpp \
    Daq/daqcommon.cpp \
    Daq/modbusClass.cpp \
    Daq/modbusmsg.cpp \
    Daq/rtobject.cpp \
    Daq/serial.cpp \
    Daq/daq.cpp \
    messages.cpp \
    storagedatabase.cpp \
    Daq/serialinterface.cpp \
    Daq/serialpacket.cpp \
    MQTT/mqttconnection.cpp \
    togglebutton.cpp \
    TsSpin_bitmaps.cpp \
    TsSpin.cpp \
    TsSpinBase.cpp

HEADERS += \
    common.h \
    common_global.h \
    database.h \
    plugin.h \
    Opc/opccommon.h \
    datapropertyitem.h \
    datastore.h \
    datastoretree.h \
    displaypropertytree.h \
    objectmanager.h \
    bobject.h \
    Web/webdatamodel.h \
    Web/webdialog.h \
    Web/webaddobjectdialog.h \
    Gui/AddObjectDialog.h \
    Gui/BourbonCommonUI.h \
    objectdataitem.h \
    objectdatavalidator.h \
    Opc/opcserver.h \
    Opc/readonlycontext.h \
    Opc/readwritecontext.h \
    Daq/daqcommon.h \
    Daq/modbusClass.h \
    Daq/modbusmsg.hpp \
    Daq/rtobject.h \
    Daq/selene.h \
    Daq/serial.hpp \
    Daq/daq.h \
    Opc/opcnodeids.h \
    messages.h \
    messageids.h \
    parameterids.h \
    stockobjectids.h \
    storagedatabase.h \
    Daq/serialinterface.h \
    Daq/serialpacket.h \
    MQTT/mqttconnection.h \
    togglebutton.h \
    TsSpin.h \
    TsSpinBase.h

DISTFILES += \
    Gui/BourbonCommonUI_bourboncommon_bitmaps.xrc \
    Gui/BourbonCommonUI.wxcp

