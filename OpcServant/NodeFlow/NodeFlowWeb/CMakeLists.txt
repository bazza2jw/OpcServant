cmake_minimum_required(VERSION 3.10)
#
# NB - Only a start - does not work
#
set(MODULE NodeFlowWeb)
project(${MODULE})


set(CMAKE_CXX_STANDARD 17)

file (GLOB SourceFiles *.cpp)

set(LIB_SOURCES
    ${SourceFiles}
)

add_executable(${MODULE} ${LIB_SOURCES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#
target_link_libraries(${MODULE} NodeFlow ${wxWidgets_LIBRARIES}  ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS} ${WTLIBS}  MrlLib  )
#
install( TARGETS ${MODULE} DESTINATION ${EXE_DIR})
