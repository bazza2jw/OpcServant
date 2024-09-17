#include "global.h"
// (c) Micro Research Ltd 2007 -
void Measure()
{
	dmaOn();
	while(!RxCharReady() && isDmaOn())
	{
		asm("CLRWDT");		
	};
	dmaOff();
	//printf("Baseline %d"EOL,baseline);
};
