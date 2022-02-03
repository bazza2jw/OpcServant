cmake_minimum_required(VERSION 3.10)
set (MODULE VisualCalc)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set  (VisualCalcFolder GUI)
file (GLOB VisualCalcSourceGUIFiles ${VisualCalcFolder}/*.cpp)
file (GLOB VisualCalcSourceFiles *.cpp)

set(LIB_SOURCES
    ${VisualCalcSourceFiles}
    ${VisualCalcSourceGUIFiles}
)

set(PROJECT_LIB_FLAGS  MrlLib Common VisualScriptEngine)

add_library(${MODULE} SHARED ${LIB_SOURCES})
target_link_libraries(${MODULE} ${wxWidgets_LIBRARIES} ${WTLIBS} ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS})
install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
