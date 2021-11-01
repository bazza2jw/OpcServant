#ifndef MRLLIB_GLOBAL_H
#define MRLLIB_GLOBAL_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif

#ifdef WIN32
#if defined(MRLLIB_LIBRARY)
#  define MRLLIBSHARED_EXPORT __declspec(dllexport)
#else
#  define MRLLIBSHARED_EXPORT __declspec(dllimport)
#endif
#else
    #define MRLLIBSHARED_EXPORT
#endif

#endif // MRLLIB_GLOBAL_H
