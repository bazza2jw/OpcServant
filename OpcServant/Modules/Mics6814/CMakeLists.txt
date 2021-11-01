cmake_minimum_required(VERSION 3.10)
set (MODULE Mics6814)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    Mics6814plugin.cpp
    Mics6814objectmanager.cpp
    Mics6814typeobject.cpp
    Mics6814rt.cpp
    ../Common/ads1015.cpp
)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
