#ifndef HTTPSEND_GLOBAL_H
#define HTTPSEND_GLOBAL_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif

#if defined(HTTPSEND_LIBRARY)
#  define HTTPSENDSHARED_EXPORT __declspec(dllexport)
#else
#  define HTTPSENDSHARED_EXPORT __declspec(dllimport)
#endif

#endif // HTTPSEND_GLOBAL_H
