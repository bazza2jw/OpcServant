// We load and save from the second page (page 1)
// configuration data is save in 64 word blocks
// We use the Table instructions to load and save
#include "global.h"
// 
// Read from flash
void ReadE2()
{
	//
	// get first byte - check if not blank	
	// otherwise stick with defaults
	//
	int ArrayinRAM[16];
	if(ArrayinRAM[0] == MAGIC_ID)
	{
		autoSampleTime = ArrayinRAM[1];
		conversionClock = ArrayinRAM[2];
		peakMin = ArrayinRAM[3];
		peakMax = ArrayinRAM[4];	
	};
};
//
// Write to E2 - must write 64 words
//
void WriteE2()
{
	int ArrayinRAM[16];
	memset(ArrayinRAM,0,sizeof(ArrayinRAM));
	ArrayinRAM[0] = MAGIC_ID;
	ArrayinRAM[1] = autoSampleTime;
	ArrayinRAM[2] = conversionClock;
	ArrayinRAM[3] = peakMin;
	ArrayinRAM[4] = peakMax;	
};
