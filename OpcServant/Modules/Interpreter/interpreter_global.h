#ifndef INTERPRETER_GLOBAL_H
#define INTERPRETER_GLOBAL_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif

#if defined(INTERPRETER_LIBRARY)
#  define INTERPRETERSHARED_EXPORT __declspec(dllexport)
#else
#  define INTERPRETERSHARED_EXPORT __declspec(import)
#endif

#endif // INTERPRETER_GLOBAL_H
