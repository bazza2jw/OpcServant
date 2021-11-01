cmake_minimum_required(VERSION 3.10)
set (MODULE LwsApex)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    lwsapexplugin.cpp
    LwsApexUI.cpp
    lwsapexrt.cpp
    LwsApexConfigurationDialog.cpp
    LwsApexPanel.cpp
    LwsApexUI_lwsapex_bitmaps.cpp
    lwsapexobjectmanager.cpp
    lwsapextypeobject.cpp
    lwswebconfiguration.cpp
    lwsapexwebpanel.cpp
)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
