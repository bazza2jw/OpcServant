cmake_minimum_required(VERSION 3.10)
set (MODULE ICPCON)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
  icpcon.cpp 
  ICPCON7017/icpcon7017objectmanager.cpp 
  ICPCON7017/icpcon7017objecttype.cpp 
  ICPCON7017/icpcon7017rt.cpp
  ICPCON7017/ICPCON7017SetupDialog.cpp
  ICPCON7017/icpcon7017setupwebdialog.cpp
  ICPCON7065/icpcon7065objectmanager.cpp
  ICPCON7065/icpcon7065objecttype.cpp
  ICPCON7065/icpcon7065rt.cpp
  ICPCON7065/icpcon7065nodecontext.cpp
)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
