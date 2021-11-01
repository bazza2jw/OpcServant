#ifndef VALUES_GLOBAL_H
#define VALUES_GLOBAL_H

#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif

#if defined(VALUES_LIBRARY)
#  define VALUESSHARED_EXPORT  __declspec(dllexport)
#else
#  define VALUESSHARED_EXPORT  __declspec(dllimport)
#endif

#endif // VALUES_GLOBAL_H
