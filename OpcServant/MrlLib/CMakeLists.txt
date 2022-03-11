cmake_minimum_required(VERSION 3.10)
set(MODULE MrlLib)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)

set(LIB_SOURCES
    mrllib.cpp 
    sqlitedb.cpp 
    stats.cpp 
    variant.cpp
    states.cpp 
    StatesIcons.cpp 
    anypropertytree.cpp 
    variantpropertytree.cpp 
    StatesIcons_bitmaps.cpp 
    json/json_reader.cpp 
    json/json_value.cpp 
    json/json_writer.cpp 
    scrypt.cpp 
    modbusClass.cpp 
    i2cinterface.cpp
    scaleoffset.cpp
    luascript.cpp
        )

add_library(${MODULE} SHARED ${LIB_SOURCES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${MODULE} ${wxWidgets_LIBRARIES} ${WTLIBS} ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS})
install(TARGETS ${MODULE} DESTINATION MRL/lib)

file(GLOB MRLLIB_HEADERS "*.h")
file(GLOB MRLLIB_HEADERS_CPP "*.hpp")
install(FILES ${MRLLIB_HEADERS} ${MRLLIB_HEADERS_CPP} trace.hpp DESTINATION MRL/include/MrlLib)

file(GLOB JSON_HEADERS "json/*.h")
install(FILES ${JSON_HEADERS} DESTINATION MRL/include/MrlLib/json)


