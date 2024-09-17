//$Id$
// these are the base utility functions 
//
#include "Utility.hpp"
//
// Trace function
//

static QTextStream DebugStream(stderr); // a stream to standard error
static int debugLevel = 0;

//
//Function      : GetDebugLevel
// Description: retuns current debug level
// Inputs   : none
// Outputs: none
// Returns: debug level
//
int GetDebugLevel() { return debugLevel;}; // get the current debug level
//
//Function      : Set DebugLevel
// Description: sets the current debug level
// Inputs   : new debug level
// Outputs: none
// Returns: none
//
void SetDebugLevel(int n) { debugLevel = n;};; // set the current debug level

//
// Function      : GetDebugStream
// Description: Gets the current debug stream
// Inputs   : none
// Outputs: none
// Returns: the debug stream
//
QTextStream & GetDebugStream()
{
	return DebugStream;
}; // get the debug output stream


//
// Function :
// Description:
// Inputs   : none
// Outputs: none
// Returns: none
//
QPixmap getPixmapFromCache(QString t)
{
	static QMap<QString,QPixmap> Pixmaps;
	if(Pixmaps.contains(t)) return Pixmaps[t];
	QString f = QString(":/Types/%1.png").arg(t);
	QPixmap p(f);
	if(!p.isNull())
	{
		Pixmaps[t] = p;
		return p;
	}
	else
	{
		f = QString("Icons/%1.png").arg(t);
		p.load(f);
		if(!p.isNull())
		{
			Pixmaps[t] = p;
			return p;
		};
	};
	return getPixmapFromCache("Base"); // fall back
};

