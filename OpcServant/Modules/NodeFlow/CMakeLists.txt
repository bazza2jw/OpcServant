cmake_minimum_required(VERSION 3.10)
set (MODULE NodeFlowSet)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    nodeflowplugin.cpp
    nodeflowobjectmanager.cpp
    nodeflowrt.cpp
    nodeflowtypeobject.cpp
    nodeflownodes.cpp
    NodeFlowConfiguration.cpp
    NodeFlowUI.cpp
    NodeFlowUI_nodeflow_bitmaps.cpp
    FlowEditorDialog.cpp
    nodeflowwebconfiguration.cpp
    )

add_definitions(-DUSE_WT)
set(PROJECT_LIB_FLAGS Common MrlLib NodeFlow ${NODEFLOWLIBS})
add_library(${MODULE} SHARED ${LIB_SOURCES})
target_link_libraries(${MODULE} ${wxWidgets_LIBRARIES}  ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS})
install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
