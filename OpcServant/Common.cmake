# Common CMAKE definitons for where to install Get the common definitons at the
# higher level - BOOST
#
# Common CMAKE definitions
set(CMAKE_CXX_STANDARD 20)
# Set project vaiables
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}) # Set the project source code
                                             # location (location of this file)
# If the install dir changes check Common/common.h
set(CMAKE_INSTALL_PREFIX ${PIROOT}/usr/local)
message("Install Dir: ${CMAKE_INSTALL_PREFIX}")

# where the components go
set(EXE_DIR OpcServant/bin)
set(LIB_DIR OpcServant/lib)
set(PLUGINS_DIR OpcServant/modules)
set(INCLUDE_DIR OpcServant/include)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)
#
# Debug and trace options enable this to insert breakpoints into throws and
# catches
add_definitions(-DTRAP_ENABLE)
# Enable debug trace in build add_definitions(-DTRACE_ON) NB: may on module by
# module basis use - target_compile_options(my_library PRIVATE -Werror -Wall
# -Wextra)
set(CMAKE_CXX_FLAGS "-Wno-psabi ${CMAKE_CXX_FLAGS}")
#
if(DEFINED CROSSBUILD)
  set(WX_VERSION "3.2")
  # when building for cross compiling wxWidgets cannot be auto detected
  message("RASPBERRY PI BUILD ENTER")
  include_directories(${PIROOT}/usr/local/include)
  include_directories(
    ${PIROOT}/usr/local/lib/wx/include/gtk3-unicode-${WX_VERSION})
  include_directories(${PIROOT}/usr/local/include/wx-${WX_VERSION})
  link_directories(${PIROOT}/usr/local/lib)
  add_definitions(-DWXUSINGDLL -D__WXGTK3__ -D__WXGTK__)
  set(wxWidgets_LIBRARIES
      wx_gtk3u_xrc-${WX_VERSION}
      wx_gtk3u_html-${WX_VERSION}
      wx_gtk3u_qa-${WX_VERSION}
      wx_gtk3u_core-${WX_VERSION}
      wx_baseu_xml-${WX_VERSION}
      wx_baseu_net-${WX_VERSION}
      wx_baseu-${WX_VERSION}
      wx_gtk3u_propgrid-${WX_VERSION}
      wx_gtk3u_richtext-${WX_VERSION}
      wx_gtk3u_webview-${WX_VERSION})

  # Find BOOST seems to have problem with sysroots so explicitly set the
  # libraries
  set(Boost_LIBRARIES -lboost_system -lboost_thread -lboost_program_options)
  set(BOOST_ROOT ${PIROOT}/usr)
  set(BOOST_DIR ${BOOST_ROOT})
  set(BOOST_PREFIX ${BOOST_DIR})
  set(BOOST_LIBRARYDIR ${PIROOT}/usr/lib/arm-linux-gnueabihf)
  set(BOOST_INCLUDEDIR ${PIROOT}/usr/include/boost)
  set(BOOST_Found ON)     

  # Force the linker to look in the sysroot where the libraries actually are -
  # needed for some of the cmake tests
  set(PI_LINKER_FLAGS "-Wl,--no-as-needed -lcrypt")
  set(CMAKE_EXE_LINKER_FLAGS "${PI_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS} ")
  set(CMAKE_SHARED_LINKER_FLAGS
      "${PI_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS} ")

  add_definitions(-Wall)
  add_definitions(-DwxSIZE_T_IS_UINT)
  add_definitions(-DHAVE_VSNPRINTF_DECL)
  add_definitions(-DHAVE_VSSCANF_DECL)
  add_definitions(-DHAVE_SNPRINTF_DECL)
  add_definitions(-D_FILE_OFFSET_BITS=64)
  add_definitions(-DRASPBERRY_PI_BUILD)
  add_definitions(-DMYSQLPP_MYSQL_HEADERS_BURIED)
  message("RASPBERRY PI BUILD EXIT")

else(DEFINED CROSSBUILD)
  # Include wxWidgets directory
  message("NATIVE BUILD")
  set(wxWidgets_USE_UNICODE ON)
  set(wxWidgets_CONFIGURATION gtk3u)
  find_package(
    wxWidgets
    REQUIRED
    html
    net
    core
    base
    xrc
    adv
    xml
    propgrid
    richtext
    webview)
  include(${wxWidgets_USE_FILE})
  add_definitions(-DMYSQLPP_MYSQL_HEADERS_BURIED)
  # Set a variable with all the required wxWdiget library linker flags
  set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
  #
  set(Boost_USE_STATIC_LIBS OFF)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_RUNTIME OFF)

  find_package(Boost REQUIRED COMPONENTS system thread program_options)

  # add boost to target

  set(LINKER_FLAGS "-lcrypt")
  set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS} ")
  set(CMAKE_SHARED_LINKER_FLAGS "${LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS} ")
endif(DEFINED CROSSBUILD)


  if(Boost_FOUND)
    include_directories(${BOOST_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
    add_definitions(${Boost_DEFINITIONS})
    include_directories(${Boost_INCLUDE_DIRS})
  endif()


# flag a unix build
add_definitions(-D_UNIX -DBOOST_BIND_GLOBAL_PLACEHOLDERS)
# enable debug tracing add_definitions(-DTRACE_ON)
#
set(WTLIBS -lwt -lwthttp)
include_directories(${CMAKE_SOURCE_DIR})

# LUA include directory
include_directories(${PIROOT}/usr/include/lua5.3)

# add mysql headers
include_directories(${PIROOT}/usr/include/mysql)
include_directories(${PIROOT}/usr/local/include/open62541cpp)

#
# Enable the GUI build
add_definitions(-DUSE_GUI)
add_definitions(-DUSE_WT)
set(EXTRA_LIB_FLAGS
    open62541cpp
    sqlite3
    mysqlpp
    modbus
    mosquittopp
    socket++
    atomic
    pthread
    rt
    m
    lua5.3)
