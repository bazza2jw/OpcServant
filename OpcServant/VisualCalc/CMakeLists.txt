cmake_minimum_required(VERSION 3.10)
project (VisualScriptEngine)

set(CMAKE_INSTALL_PREFIX ${PIROOT}/usr/local/OpcServant)

set(CMAKE_CXX_STANDARD 17)
add_definitions (-DUNICODE -D_UNICODE)

## build settings
set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})

#
set(wxWidgets_USE_UNICODE ON)
set(wxWidgets_CONFIGURATION gtk3u)
find_package(wxWidgets REQUIRED html net core base xrc adv xml propgrid richtext)
include(${wxWidgets_USE_FILE})
#

set (NodeEngineSourcesFolder NodeEngine)
file (GLOB NodeEngineHeaderFiles ${NodeEngineSourcesFolder}/*.hpp)
file (GLOB NodeEngineSourceFiles ${NodeEngineSourcesFolder}/*.cpp)

set (NodeUIEngineSourcesFolder NodeUIEngine)
file (GLOB NodeUIEngineHeaderFiles ${NodeUIEngineSourcesFolder}/*.hpp)
file (GLOB NodeUIEngineSourceFiles ${NodeUIEngineSourcesFolder}/*.cpp)

set (BuiltInNodesSourcesFolder BuiltInNodes)
file (GLOB BuiltInNodesHeaderFiles ${BuiltInNodesSourcesFolder}/*.hpp)
file (GLOB BuiltInNodesSourceFiles ${BuiltInNodesSourcesFolder}/*.cpp)

set (wxWidgetsAppSupportFolder wxWidgetsAppSupport)
file (GLOB wxWidgetsAppSupportHeaderFiles ${wxWidgetsAppSupportFolder}/*.hpp)
file (GLOB wxWidgetsAppSupportSourceFiles ${wxWidgetsAppSupportFolder}/*.cpp)

# Example UI 
#set  (VisualCalcFolder VisualCalc)
#file (GLOB VisualCalcHeaderFiles ${VisualCalcFolder}/*.hpp)
#file (GLOB VisualCalcSourceFiles ${VisualCalcFolder}/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/wxWidgetsAppSupport)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/NodeEngine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/NodeUIEngine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/BuiltInNodes)

add_library(VisualScriptEngine SHARED ${NodeEngineSourceFiles} ${NodeUIEngineSourceFiles}
    ${BuiltInNodesSourceFiles} ${wxWidgetsAppSupportSourceFiles} ${VisualCalcSourceFiles})

target_link_libraries(VisualScriptEngine ${wxWidgets_LIBRARIES}  ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS})

install(TARGETS VisualScriptEngine DESTINATION bin)

install(FILES ${NodeEngineHeaderFiles} DESTINATION include/VisualScriptEngine)
install(FILES ${NodeUIEngineHeaderFiles} DESTINATION include/VisualScriptEngine)
install(FILES ${BuiltInNodesHeaderFiles} DESTINATION include/VisualScriptEngine)
install(FILES ${wxWidgetsAppSupportHeaderFiles} DESTINATION include/VisualScriptEngine)
#install(FILES ${VisualCalcHeaderFiles} DESTINATION include/VisualScriptEngine)



