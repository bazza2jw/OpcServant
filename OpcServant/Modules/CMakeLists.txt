cmake_minimum_required(VERSION 3.10)
project(Modules)

set(CMAKE_CXX_STANDARD 17)
#
# Add each of the modules
#
add_subdirectory(Simulator)
add_subdirectory(MariaDbPublisher)
add_subdirectory(ICPCON)
add_subdirectory(Values)
add_subdirectory(MQTT)
#add_subdirectory(SbcSupport)
add_subdirectory(LwsApex)
add_subdirectory(OPCN3)
add_subdirectory(BME280)
add_subdirectory(SPS30)
add_subdirectory(Mics6814)
add_subdirectory(Link)
#add_subdirectory(VisualCalc)





