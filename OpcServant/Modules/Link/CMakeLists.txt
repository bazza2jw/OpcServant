cmake_minimum_required(VERSION 3.10)
set (MODULE Link)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    Linkplugin.cpp
    Linkobjectmanager.cpp
    Linktypeobject.cpp
    Linkrt.cpp
    LinkSetupDialog.cpp
    LinkUI_Link_bitmaps.cpp
    Linkwebsetupdialog.cpp
    LinkUI.cpp
)

set(PROJECT_LIB_FLAGS Common MrlLib socket++ socket)
add_library(${MODULE} SHARED ${LIB_SOURCES})
target_link_libraries(${MODULE} ${wxWidgets_LIBRARIES} ${WTLIBS} ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
