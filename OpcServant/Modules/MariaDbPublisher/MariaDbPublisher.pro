#-------------------------------------------------
#
# Project created by QtCreator 2019-11-10T10:05:37
#
#-------------------------------------------------
include (../../Common.pri)
QT       -= core gui

TARGET = MariaDbPublisher
TEMPLATE = lib
CONFIG += plugin
DESTDIR=$$PLUGINSDIR
LIBS += -L$$LIBDIR -lMrlLib -lCommon -lOpen62541Cpp -lmysqlpp
DEFINES += MARIADBPUBLISHER_LIBRARY


SOURCES += \
        mariadbpublisher.cpp \
    mariadbpublisherobjectmanager.cpp \
    mariadbpublisherconfigurationwebdialog.cpp \
    mariadbpublisherrt.cpp \
    MariaDbPubSetup.cpp \
    MariaDbPubSetup_mariadbpublisher_bitmaps.cpp \
    MariaDbSetupDialog.cpp \
    mariadbwebsetupdialog.cpp \
    mariadbobjecttype.cpp

HEADERS += \
        mariadbpublisher.h \
        mariadbpublisher_global.h \ 
    mariadbpublisherobjectmanager.h \
    mariadbpublisherconfigurationwebdialog.h \
    mariadbpublisherrt.h \
    MariaDbPubSetup.h \
    MariaDbSetupDialog.h \
    mariadbwebsetupdialog.h \
    mariadbobjecttype.h


