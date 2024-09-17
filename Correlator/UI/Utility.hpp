#ifndef UTILITY_HPP
#define UTILITY_HPP
//$Id$
// this file has the base utility functions
//
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//
// MS Windows depenencies
#ifdef _WINDOWS

#endif
// X11 dependancies
#ifdef UNIX

#endif

#include <QString>
#include <QTextStream>
#include <QtGui>
//
int GetDebugLevel(); // get the current debug level
void SetDebugLevel(int); // set the current debug level
QTextStream & GetDebugStream(); // get the debug output stream
#define GDS GetDebugStream()
//
// These are the trace macros
//
#ifdef DEBUGON
#define DBGTRACE(s) GetDebugStream() << s;
#define DBGTRACELEVEL(n,s) if(GetDebugLevel() < n) GetDebugStream() << s;
#else
#define DBGTRACE(s)
#define DBGTRACELEVEL(n,s)
#endif
// combo box helper
inline void setComboText(QComboBox *p, QString t)
{
	p->setCurrentIndex(p->findText(t));	
};
// this is a pixmap cache
QPixmap getPixmapFromCache(QString t);
#endif
