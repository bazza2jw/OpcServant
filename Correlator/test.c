#include "global.h"

// test harnesses
void Test(void);
char traceBuf[32];
int main(int ac, char **av)
{
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
	TRISD = 0;
	PORTD = 0;
	InitSerial();
	SendString1(EOL);
	SendString2(EOL);
	initAdc();
	initDma0();
	while(1)
	{
		int i;
		printf("Get Wave Form"EOL);
		GetChar1();
		GetWaveForm();		
			
		for(i = 1; i < N_CHAN; i++)
		{
			printf("%X ",Counts.uCounts[i]);
			if(!(i % 8)) printf(EOL);	
		};
		printf("Got Wave Form"EOL);
		GetChar1();
	};
};




