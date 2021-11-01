TEMPLATE = app
include (../Common.pri)
LIBS += -L$$LIBDIR  -lmosquittopp -lsqlite3
DESTDIR = $$APPDIR

SOURCES += main.cpp \
    osclientapp.cpp \
    OpcClientMainFrame.cpp \
    OsClientUI_osclient_bitmaps.cpp \
    OsClientUI.cpp \
    m_datasourcemodel.cpp

HEADERS += \
    osclientapp.h \
    OpcClientMainFrame.h \
    OsClientUI.h \
    m_datasourcemodel.h

DISTFILES += \
    OsClientUI_osclient_bitmaps.xrc
