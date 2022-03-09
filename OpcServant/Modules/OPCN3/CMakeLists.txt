cmake_minimum_required(VERSION 3.10)
set (MODULE OPCN3)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    opcn3.cpp
    OPCN3plugin.cpp
    OPCN3UI.cpp
    OPCN3rt.cpp
    OPCN3ConfigurationDialog.cpp
    OPCN3Panel.cpp
    OPCN3UI_opcn3_bitmaps.cpp
    OPCN3objectmanager.cpp
    OPCN3typeobject.cpp
    OPCN3webconfiguration.cpp
    OPCN3webpanel.cpp
)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})
target_link_libraries(${MODULE} ${wxWidgets_LIBRARIES}  ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
