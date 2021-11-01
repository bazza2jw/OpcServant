cmake_minimum_required(VERSION 3.10)
set (MODULE EnviroPlus)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    EnviroPlusplugin.cpp
    EnviroPlusobjectmanager.cpp
    EnviroPlustypeobject.cpp
    EnviroPlusrt.cpp
)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
