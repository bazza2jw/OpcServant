cmake_minimum_required(VERSION 3.10)
set (MODULE MimicDisplay)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
        MimicDisplayUI.cpp
        MimicDisplayUI_mimicdisplay_bitmaps.cpp
        MimicDisplayConfigurationDialog.cpp
        MimicEditorDialog.cpp
        mimicdisplayplugin.cpp
        mimicdisplayobjectmanager.cpp
        mimicdisplayrt.cpp
        MimicTabDisplay.cpp
        opcservantobjects.cpp
        InputViewer.cpp
        InputViewerHistory.cpp
    )

set(PROJECT_LIB_FLAGS Common MrlLib Mimic ${NODEFLOWLIBS})
add_library(${MODULE} SHARED ${LIB_SOURCES})
target_link_libraries(${MODULE} ${wxWidgets_LIBRARIES}  ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS})
install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
