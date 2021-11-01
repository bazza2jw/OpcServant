cmake_minimum_required(VERSION 3.10)
project(ReportDocument)

set(CMAKE_CXX_STANDARD 17)

set(LIB_SOURCES
        filehandlers.cpp
        pageitems.cpp
        pagelayout.cpp
        printout.cpp
        reportdocument.cpp
        shapeitems.cpp
        styles.cpp
        tableelements.cpp
        textelements.cpp
        )

add_library(ReportDocument SHARED ${LIB_SOURCES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ReportDocument ${wxWidgets_LIBRARIES} ${WTLIBS} ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS})
install(TARGETS ReportDocument DESTINATION ${EXE_DIR})

file(GLOB DAQ_HEADERS "*.h")
install(FILES ${DAQ_HEADERS} DESTINATION include/reportdocument)



