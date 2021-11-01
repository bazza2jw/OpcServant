#-------------------------------------------------
#
# Project created by QtCreator 2019-11-14T16:26:05
#
#-------------------------------------------------
include (../../Common.pri)
QT       -= core gui

TARGET = Interpreter
TEMPLATE = lib
CONFIG += plugin
DESTDIR=$$PLUGINSDIR
LIBS += -L$$LIBDIR -lMrlLib -lCommon -lOpen62541Cpp -lmysqlpp
DEFINES += INTERPRETER_LIBRARY



SOURCES += \
        interpreter.cpp \
    interpreterobjectmanager.cpp \
    interpreterrt.cpp \
    interpretertypeobject.cpp

HEADERS += \
        interpreter.h \
        interpreter_global.h \ 
    interpreterobjectmanager.h \
    interpreterrt.h \
    interpretertypeobject.h


