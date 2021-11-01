#ifndef ICPCON_GLOBAL_H
#define ICPCON_GLOBAL_H

#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif

#if defined(ICPCON_LIBRARY)
#  define ICPCONSHARED_EXPORT  __declspec(dllexport)
#else
#  define ICPCONSHARED_EXPORT __declspec(dllimport)
#endif

#endif // ICPCON_GLOBAL_H
