cmake_minimum_required(VERSION 3.10)
set (MODULE LocalDB)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    localdb.cpp
    localdbrt.cpp
    localdbobjectmanager.cpp
)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
