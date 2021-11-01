# EMDG qmake settings
#
# set up as a wx application

CONFIG += wxwidgets  thread c++14
QT     -= core gui
CONFIG -= qt
unix: DEFINES += _UNIX
DEFINES += TRACE_ON
PRODUCTNAME = OpcServant
#
# application destination directories
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
        INCLUDEPATH += /usr/local/MRL5/include
        INCLUDEPATH += /usr/local/MRL5/include/Open62541Cpp
        INSTALLBASE  = /usr/local/MRL5/$$PRODUCTNAME
        LIBS += -L/usr/local/MRL5/lib
    }

}

win32 {
# Set location of Boost Libraries
    message("MS Windows PC Build")
    INSTALLBASE = C:/usr/local/$$PRODUCTNAME
    LIBS += -LC:/local/boost_1_66_0/lib32-msvc-12.0 -lboost_system-vc120-mt-x32-1_66 -lWs2_32
    INCLUDEPATH += C:/local/boost_1_66_0
    INCLUDEPATH += C:/usr/local/include
}


#
APPDIR = $$INSTALLBASE/bin
LIBDIR = $$INSTALLBASE/lib
PLUGINSDIR = $$INSTALLBASE/modules
PROJDIR = $$PWD
#
LIBS += -lboost_system -lboost_thread
LIBS += -L$$LIBDIR
#
debug::DEFINES += WXDEBUG
DEFINES +=  wxUSE_DDE_FOR_IPC=0
INCLUDEPATH += $$PROJDIR
INCLUDEPATH += $$INSTALLBASE/include


