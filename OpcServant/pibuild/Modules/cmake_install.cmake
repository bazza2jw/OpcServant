# Install script for directory: /Development/Projects/MRL/OpcServant/OpcServant/Modules

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Development/sysroot/usr/local/MRL5/OpcServant")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/Simulator/cmake_install.cmake")
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/MariaDbPublisher/cmake_install.cmake")
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/ICPCON/cmake_install.cmake")
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/Values/cmake_install.cmake")
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/Alarms/cmake_install.cmake")
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/MQTT/cmake_install.cmake")
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/SbcSupport/cmake_install.cmake")
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/AerisNOx/cmake_install.cmake")
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/LwsApex/cmake_install.cmake")
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/OPCN3/cmake_install.cmake")
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/BME280/cmake_install.cmake")
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/SPS30/cmake_install.cmake")
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/Mics6814/cmake_install.cmake")
  include("/Development/Projects/MRL/OpcServant/OpcServant/pibuild/Modules/GasCells/cmake_install.cmake")

endif()

