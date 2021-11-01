#ifndef MQTT_GLOBAL_H
#define MQTT_GLOBAL_H

#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif

#if defined(MQTT_LIBRARY)
#  define MQTTSHARED_EXPORT __declspec(dllexport)
#else
#  define MQTTSHARED_EXPORT __declspec(dllimport)
#endif

#endif // MQTT_GLOBAL_H
