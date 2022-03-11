cmake_minimum_required(VERSION 3.10)
set(MODULE Common)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)


set(LIB_SOURCES
    common.cpp 
    database.cpp
    plugin.cpp 
    Opc/opccommon.cpp 
    datastore.cpp
    datastoretree.cpp
    displaypropertytree.cpp 
    objectmanager.cpp 
    bobject.cpp
    Web/webdatamodel.cpp
    Web/webaddobjectdialog.cpp
    Web/webreportpane.cpp
    Web/webaliasconfigurationdialog.cpp
    Gui/AddObjectDialog.cpp
    Gui/BourbonCommonUI.cpp
    Gui/BourbonCommonUI_bourboncommon_bitmaps.cpp 
    objectdataitem.cpp 
    objectdatavalidator.cpp 
    Opc/opcserver.cpp 
    Opc/readonlycontext.cpp 
    Opc/readwritecontext.cpp
    Daq/daqcommon.cpp
    Daq/rtobject.cpp
    Daq/serial.cpp
    Daq/daq.cpp 
    messages.cpp
    storagedatabase.cpp 
    Daq/serialinterface.cpp
    Daq/serialpacket.cpp 
    Daq/localdb.cpp
    MQTT/mqttconnection.cpp 
    togglebutton.cpp 
    VirtualKeypad.cpp
    VirtualKeypadUI.cpp
    VirtualKeypadUI_common_bitmaps.cpp
    mathplot.cpp
    AngularMeter.cpp
    BmpSwitcher.cpp
    LCDWindow.cpp
    LinearMeter.cpp
    LinearRegulator.cpp
    roundedbutton.cpp
    sliderswitch.cpp
    LCDClock.cpp
    TimeAlarm.cpp
    PanelList.cpp
    reporter.cpp
    reportresultdatabase.cpp
    reportconfig.cpp
    reportdoc.cpp
    reportgraph.cpp
    graph.cpp
    graphwindow.cpp
    Gui/CommonGUI.cpp
    Gui/SystemPropertiesDialog.cpp
    Gui/ReportGeneratorPanel.cpp
    Gui/ReportTablePage.cpp
    Gui/ReportPrintOptions.cpp
    Gui/AliasConfigurationDialog.cpp
    Gui/SelectValueDialog.cpp
    Gui/AddAliasDialog.cpp
    Web/reportselectweb.cpp
    Gui/CommonGUI_bitmaps.cpp
    Gui/ConfigureI2cDialog.cpp
    Web/configurei2cwebdialog.cpp
    Web/webpanel.cpp
    Gui/GridPanel.cpp
    Web/i2cconfigurationwebdialog.cpp
    Web/configurationwebdialog.cpp
    Gui/SetupDialog.cpp
    Gui/i2csetupdialog.cpp
    Gui/I2cConfigurePanel.cpp
    Gui/DatabaseConfigurePanel.cpp
    Gui/NetworkConfigurePanel.cpp
    Gui/SerialConfigurePanel.cpp
    Gui/ScaleOffsetConfigurePanel.cpp
    Web/weblogindialog.cpp
    Gui/UserConfigurationDialog.cpp
    Gui/PinEntryDialog.cpp
    Web/webuserconfigurationdialog.cpp
    Daq/rtobjectscritped.cpp
        )

set(PROJECT_LIB_FLAGS  MrlLib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


set(COMMON_INCLUDE ${INCLUDE_DIR}/${MODULE})

add_library(${MODULE} SHARED ${LIB_SOURCES})
target_link_libraries(${MODULE} ${wxWidgets_LIBRARIES} ${WTLIBS} ${BOOST_FLAGS} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS}  ReportDocument)

file(GLOB COMMON_HEADERS "*.h")
file(GLOB COMMON_HEADERS_CPP "*.hpp")
install(FILES ${COMMON_HEADERS} ${COMMON_HEADERS_CPP} DESTINATION ${COMMON_INCLUDE})

file(GLOB DAQ_HEADERS "Daq/*.h")
file(GLOB DAQ_HEADERS_CPP "Daq/*.hpp")

install(FILES ${DAQ_HEADERS} ${DAQ_HEADERS_CPP} DESTINATION ${COMMON_INCLUDE}/Daq)

file(GLOB GUI_HEADERS "Gui/*.h")
install(FILES ${GUI_HEADERS} DESTINATION ${COMMON_INCLUDE}/Gui)

file(GLOB MQTT_HEADERS "MQTT/*.h")
install(FILES ${MQTT_HEADERS} DESTINATION ${COMMON_INCLUDE}/MQTT)

file(GLOB OPC_HEADERS "Opc/*.h")
install(FILES ${OPC_HEADERS} DESTINATION ${COMMON_INCLUDE}/Opc)

file(GLOB WEB_HEADERS "Web/*.h")
install(FILES ${WEB_HEADERS} DESTINATION ${COMMON_INCLUDE}/Web)


install(TARGETS Common DESTINATION ${LIB_DIR})


