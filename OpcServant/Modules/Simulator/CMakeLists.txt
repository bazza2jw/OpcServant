cmake_minimum_required(VERSION 3.10)
set (MODULE Simulator)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    simulator.cpp
    Simulator.cpp 
    SimulatorConfigurationDialog.cpp
    Simulator_simulator_bitmaps.cpp 
    simulatorconfigurationwebdialog.cpp 
    simulatorrt.cpp
    simulatortypeobject.cpp
    simulatorstartmethod.cpp
    simulatorstopmethod.cpp

)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
