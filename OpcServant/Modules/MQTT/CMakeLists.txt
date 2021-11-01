cmake_minimum_required(VERSION 3.10)
set (MODULE MQTT)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    mqtt.cpp 
    mqttobjectmanager.cpp 
    mqttrt.cpp
    MqttSetupDialog.cpp 
    MQTTUI_mqtt_bitmaps.cpp 
    MQTTUI.cpp
    mqttwebsetupdialog.cpp
    mqttcommand.cpp
)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
