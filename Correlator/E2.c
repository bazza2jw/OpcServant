//
// Read / write a configuration to / from flash
// used Micorchip routines and they work like this
// Can save upto 1k of config
#include "global.h"
#include "rtspApi.h"
#include "testData.h"
// 
// Read from flash
//
void ReadE2()
{
	//
	// get first byte - check if not blank	
	// otherwise stick with defaults
	int i;
	int *ArrayinRAM = (int *)Counts.uCounts;
	
	u16 nvmAdr,nvmAdru,nvmAdrPageAligned;
	nvmAdru=__builtin_tblpage(&MyRowData5InFlash[0]);
	nvmAdr=__builtin_tbloffset(&MyRowData5InFlash[0]);
	nvmAdrPageAligned=nvmAdr & 0xFC00;			// Get the Flash Page Aligned address
	//
	dmaOff(); // if there was any counting switch it off
	memset(&Counts,0,sizeof(Counts));
	//
	flashPageRead(nvmAdru,nvmAdrPageAligned, ArrayinRAM);
	//
	//	
	if(ArrayinRAM[0] == MAGIC_ID)
	{
		autoSampleTime = ArrayinRAM[1];
		conversionClock = ArrayinRAM[2];
		peakMin = ArrayinRAM[3];
		peakMax = ArrayinRAM[4];	
		deadTime = ArrayinRAM[5];
		relativeNoise = ArrayinRAM[6]; // relativeNoise used in auto threshold detect
		fixedNoise = ArrayinRAM[7]; // flag if using fixed noise
		fixedBaseline = ArrayinRAM[8]; // the fixed baseline
		fixedBaseline = ArrayinRAM[9]; // flag if using fixed base line - infers no DC offset
		fixedNoiseValue = ArrayinRAM[10];  
		//
		for(i = 0; i < N_REDUCED; i++)
		{
			Reduced[i] = ArrayinRAM[30 + i];		
		};
		//
	};
};
//
// Write to E2 - must write 64 words
//
void WriteE2()
{
	//
	int i;
	int *ArrayinRAM;
	u16 nvmAdr,nvmAdru,nvmAdrPageAligned;
	nvmAdru=__builtin_tblpage(&MyRowData5InFlash[0]);
	nvmAdr=__builtin_tbloffset(&MyRowData5InFlash[0]);
	nvmAdrPageAligned=nvmAdr & 0xFC00;			// Get the Flash Page Aligned address
	//
	// we use the Counting array as the buffer
	dmaOff(); // if there was any counting switch it off
	memset(&Counts,0,sizeof(Counts));
	//
	ArrayinRAM = (int *)Counts.uCounts;
	memset(ArrayinRAM,0,sizeof(ArrayinRAM));
	ArrayinRAM[0] = MAGIC_ID;
	ArrayinRAM[1] = autoSampleTime;
	ArrayinRAM[2] = conversionClock;
	ArrayinRAM[3] = peakMin;
	ArrayinRAM[4] = peakMax;
	ArrayinRAM[5] = deadTime;
	ArrayinRAM[6] =	relativeNoise; // relativeNoise used in auto threshold detect
	ArrayinRAM[7] =	fixedNoise; // flag if using fixed noise
	ArrayinRAM[8] =	fixedBaseline; // the fixed baseline
	ArrayinRAM[9] = fixedBaseline; // flag if using fixed base line - infers no DC offset
	ArrayinRAM[10] = fixedNoiseValue; // the 
	//
	for(i = 0; i < N_REDUCED; i++)
	{
		ArrayinRAM[30 + i] = Reduced[i];		
	};
	//
	// we write a hell of a lot of stuff at once to flash
	// erase the page
// Erase the page in Flash
	flashPageErase(nvmAdru,nvmAdrPageAligned);

// Program the page with modified data
	flashPageWrite(nvmAdru,nvmAdrPageAligned,ArrayinRAM);
	//
		
};
