TEMPLATE = app
TARGET = Correlator
unix {
	INCLUDEPATH += /usr/local/qwt/include
	LIBS += -L/usr/local/qwt/lib -lqwt
	DEFINES = UNIX
}
win32 {
	INCLUDEPATH += /qwt/include
	LIBS += -L/qwt/lib -lqwt5
	DEFINES = WIN32 _WINDOWS
}
SOURCES = Correlator.cpp Utility.cpp fifo.cpp serial.cpp  
HEADERS = Correlator.hpp 
RESOURCES = Correlator.qrc
FORMS = CorrelatorUI.ui
QT += xml network

