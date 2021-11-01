#ifndef MARIADBPUBLISHER_GLOBAL_H
#define MARIADBPUBLISHER_GLOBAL_H

#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif

#if defined(MARIADBPUBLISHER_LIBRARY)
#  define MARIADBPUBLISHERSHARED_EXPORT __declspec(dllexport)
#else
#  define MARIADBPUBLISHERSHARED_EXPORT __declspec(dllimport)
#endif

#endif // MARIADBPUBLISHER_GLOBAL_H
