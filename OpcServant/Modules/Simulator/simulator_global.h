#ifndef SIMULATOR_GLOBAL_H
#define SIMULATOR_GLOBAL_H

#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif

#if defined(SIMULATOR_LIBRARY)
#  define SIMULATORSHARED_EXPORT  __declspec(dllexport)
#else
#  define SIMULATORSHARED_EXPORT  __declspec(dllimport)
#endif

#endif // SIMULATOR_GLOBAL_H
