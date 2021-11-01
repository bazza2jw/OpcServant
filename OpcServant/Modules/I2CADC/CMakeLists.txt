cmake_minimum_required(VERSION 3.10)
set (MODULE I2CADC)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    I2CADCplugin.cpp
    I2CADCobjectmanager.cpp
    I2CADCtypeobject.cpp
    I2CADCrt.cpp
)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
