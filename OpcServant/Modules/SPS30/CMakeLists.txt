cmake_minimum_required(VERSION 3.10)
set (MODULE SPS30)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    sps30.cpp
    sps30objectmanager.cpp
    sps30rt.cpp
    sps30typeobject.cpp
    sps30/sensirion_common.c
    sps30/sensirion_hw_i2c_implementation.c
    sps30/sps30.c
    sps30/sps_git_version.c
)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
