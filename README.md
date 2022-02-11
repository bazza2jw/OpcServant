OPC Servant

This is a framework for developing OPC UA servers and other data collection and processing applications.

This application provides for desktop (screen) interfaces using wxWidgets and web interfaces using Wt.

OPC UA functionality is based on open62541 and open62541cpp

The framework core services and uses loadable modules. The time to develop an OPC UA server is greatly reduced.

There is the general concept of data collection modules (Equipment) and data export via Publishers.

Publishers include MQTT, SQL. MQTT supports command processing.

Reports can be generated, displayed and printed using the desktop and web interfaces.

The application framework builds and runs on single board computers like the Raspberry Pi 4.

When combined with Webmin fully headless systems can be deployed. 

The VisualScriptEngine (https://github.com/kovacsv/VisualScriptEngine/wiki) has been adapted to behave like Node Red flows and included as a module. This is WIP. This is a way of doing visual programming and visual configuration.

Using Node Red with the Dashboard and SVG nodes, plus the "SCADA" nodes provided in the OpcServant distribution, allows for customisable user interfaces.

Wordpress is another option.

Hopefully this is useful. If only as a range of examples for the use of wxWidgets, Wt and open62541 (OPC UA)

