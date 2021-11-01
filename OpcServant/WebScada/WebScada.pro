TEMPLATE = app
CONFIG += wxwidgets thread c++17
CONFIG -= qt

unix: DEFINES += _UNIX
#DEFINES += TRACE_ON
PRODUCTNAME = OpcServant

unix {
    options = $$find(QMAKESPEC, "rpi")
    count(options,1) {
# watchout for include path searching -
        SYSROOT_BASE = /opt/RPiSysroot2
        INSTALLBASE = $$SYSROOT_BASE/usr/local/$$PRODUCTNAME
        message("Raspberry Pi Build")
        DEFINES += DL_RASPBERRY_PI_BUILD
        CONFIG += raspi

    } else {
        message("PC Build")
        INCLUDEPATH += /usr/local/include
        INCLUDEPATH += /usr/local/include/open62541cpp
        INSTALLBASE  = /usr/local/MRL5/$$PRODUCTNAME
        LIBS += -L/usr/local/MRL5/lib
    }
INCLUDEPATH += /usr/include/mysql
INCLUDEPATH += ../
}


#
APPDIR = $$INSTALLBASE/bin
LIBDIR = $$INSTALLBASE/lib
PLUGINSDIR = $$INSTALLBASE/modules
PROJDIR = $$PWD
#
LIBS += -lboost_system -lboost_thread
LIBS += -L$$APPDIR
LIBS += -lMrlLib -lOpen62541Cpp -L/usr/local/lib -lwt -lwthttp
#
DESTDIR = $$APPDIR
debug::DEFINES += WXDEBUG
DEFINES +=  wxUSE_DDE_FOR_IPC=0
INCLUDEPATH += $$PROJDIR
INCLUDEPATH += $$INSTALLBASE/include

SOURCES += main.cpp \
    webscadaapplication.cpp \
    webscadaapp.cpp \
    webscadamain.cpp \
    wscadamapwindow.cpp \
    webscadastatus.cpp \
    webscadastatustable.cpp
    #datasource.cpp \
    #opcdatasource.cpp \
    #mqttdatasource.cpp \
    #modbusdatasource.cpp

HEADERS += \
    webscadaapplication.h \
    webscadaapp.h \
    webscadamain.h \
    wscadamapwindow.h \
    webscadastatus.h \
    webscadastatustable.h
    #datasource.h \
    #opcdatasource.h \
   #mqttdatasource.h \
    #modbusdatasource.h
