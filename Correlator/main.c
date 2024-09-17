#include "global.h"
void TestCorrelator(void);
int main(int ac, char **av)
{
	int i;
	PSVPAG = 0;
	CORCONbits.PSV=1;
	// we have a 10MHz crystal - we want 40MIPS
	PLLFBD=32;					// M=40
	CLKDIVbits.PLLPOST=0;		// N1=2
	CLKDIVbits.PLLPRE=0;		// N2=2
	OSCTUN=0;					// Tune FRC oscillator, if FRC is used
// Disable Watch Dog Timer
	RCONbits.SWDTEN=0;
// Wait for PLL to lock
	while(OSCCONbits.LOCK!=1) {};
//
// initialise the serial port
// We set the baud rate to 56k for both channels 
// this means it takes never more than 1 second to download all channels
// this is only necessary when running as a dyanmic PHA
//
	InitSerial();
// Default initialise of ADCs - Aim for 2us conversion time for each one
//	initAdc12(); // get the adcs going
//	initDma0();// dma from adc1
//	StartAdc();// start the adcs
//	Wait32k(); // wait a bit
	printf("\r\nCorrelator Ready (c) MRL 2006 - \r\n");
	//	
	while(1)
	{	
		TestCorrelator();
	};
	return 0;	
};
