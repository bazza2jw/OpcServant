#ifndef ALARMS_GLOBAL_H
#define ALARMS_GLOBAL_H

#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif

#if defined(ALARMS_LIBRARY)
#  define ALARMSSHARED_EXPORT   __declspec(dllexport)
#else
#  define ALARMSSHARED_EXPORT   __declspec(dllimport)
#endif

#endif // ALARMS_GLOBAL_H
