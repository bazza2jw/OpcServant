TEMPLATE = app
include (../Common.pri)
LIBS += -lCommon -lMrlLib -lmodbus
DESTDIR = $$APPDIR
SOURCES += main.cpp \
    buiapp.cpp \
    Opc/opcthread.cpp \
    Web/buiwebapplication.cpp \
    Web/buiwebmessagepane.cpp \
    Web/bwebnavigationtree.cpp \
    Web/bwebuimain.cpp \
    Web/websystempropertiesdialog.cpp \
    Daq/daqthread.cpp \
    serverobject.cpp
# enable GUI sources if building with GUI
usegui: SOURCES +=  Gui/BUI.cpp \
    Gui/BUI_bui_bitmaps.cpp \
    Gui/Mainframe.cpp \
    Gui/SystemPropertiesDialog.cpp

HEADERS += \
    buiapp.h \
    Opc/opcthread.h \
    Web/buiwebapplication.h \
    Web/buiwebmessagepane.h \
    Web/bwebnavigationtree.h \
    Web/bwebuimain.h \
    Web/websystempropertiesdialog.h \
    Daq/daqthread.h \
    serverobject.h
#
usegui: HEADERS +=  Gui/BUI.h \
    Gui/Mainframe.h \
    Gui/SystemPropertiesDialog.h

DISTFILES += \
    Gui/BUI_bui_bitmaps.xrc \
    Gui/BUI.wxcp
