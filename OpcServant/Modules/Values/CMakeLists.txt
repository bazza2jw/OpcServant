cmake_minimum_required(VERSION 3.10)
set (MODULE Values)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    values.cpp 
    ValuesUI_values_bitmaps.cpp 
    ValuesUI.cpp 
    ValueConfigurationDialog.cpp
    valueobjectmanager.cpp
    valuert.cpp
    valuetypeobject.cpp
    valuesetupwebdialog.cpp
    ValueSelectAliasDialog.cpp
    ValueGroupConfigurationDialog.cpp
    valuegroupconfigurationwebdialog.cpp
    ValueGroupPanel.cpp
    valuegroupwebpanel.cpp
)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})
target_link_libraries(Values ${wxWidgets_LIBRARIES} ${WTLIBS} ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS})
install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
