cmake_minimum_required(VERSION 3.10)
project(Servant)

set(CMAKE_CXX_STANDARD 17)

set(LIB_SOURCES
    main.cpp 
    buiapp.cpp 
    Opc/opcthread.cpp 
    Web/buiwebapplication.cpp 
    Web/buiwebmessagepane.cpp 
    Web/bwebnavigationtree.cpp 
    Web/bwebuimain.cpp 
    Web/websystempropertiesdialog.cpp
    Web/bwebadminpane.cpp
    Web/buiwebuserpage.cpp
    Daq/daqthread.cpp 
    serverobject.cpp
    Gui/BUI.cpp 
    Gui/BUI_bui_bitmaps.cpp 
    Gui/Mainframe.cpp 
     )

add_executable(Servant ${LIB_SOURCES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_LIB_FLAGS  MrlLib Common VisualScriptEngine)
target_link_libraries(Servant ${wxWidgets_LIBRARIES} ${WTLIBS} ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS}  )

IF( DEFINED CROSSBUILD )
target_link_libraries(Servant ${PIROOT}/opt/vc/lib/libvchiq_arm.so ${PIROOT}/opt/vc/lib/libvcos.so)
ENDIF( DEFINED CROSSBUILD )


install( TARGETS Servant DESTINATION bin)
