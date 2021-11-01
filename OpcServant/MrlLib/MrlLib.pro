#-------------------------------------------------
#
# Project created by QtCreator 2018-05-09T08:23:55
#
#-------------------------------------------------
CONFIG +=  excludeMrllib excludeCommon
include (../Common.pri)
QT       -= core gui
TARGET = MrlLib
TEMPLATE = lib

DEFINES += MRLLIB_LIBRARY
DESTDIR = $$APPDIR

LIBS += -lboost_system -lboost_thread -lsqlite3 -lmodbus

SOURCES += \
    mrllib.cpp \
    sqlitedb.cpp \
    stats.cpp \
    variant.cpp \
    states.cpp \
    StatesIcons.cpp \
    anypropertytree.cpp \
    variantpropertytree.cpp \
    StatesIcons_bitmaps.cpp \
    json/json_reader.cpp \
    json/json_value.cpp \
    json/json_valueiterator.inl \
    json/json_writer.cpp \
    scrypt.cpp \
    modbusClass.cpp \
    modbusmsg.cpp

HEADERS += \
    mrllib.h \
    mrllib_global.h \
    propertytree.h \
    rollingbuffer.hpp \
    sqlitedb.h \
    stats.hpp \
    variant.h \
    states.h \
    StatesIcons.h \
    mrlsignals.h \
    trace.hpp \
    anypropertytree.h \
    variantpropertytree.h \
    mrlmutex.h \
    CTML.h \
    Document.h \
    Node.h \
    json/allocator.h \
    json/assertions.h \
    json/config.h \
    json/forwards.h \
    json/json_features.h \
    json/json_tool.h \
    json/json.h \
    json/reader.h \
    json/value.h \
    json/version.h \
    json/writer.h \
    scrypt.h \
    modbusClass.h \
    modbusmsg.hpp

DISTFILES += \
    StatesIcons.wxcp \
    StatesIcons_bitmaps.xrc

