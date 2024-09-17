#include "global.h"
// Wait for half the conversion time
void Wait1us()
{
	T1CON = 0;
	TMR1 = 0;
	PR1 = 37 * 40 / 4;
    T1CONbits.TON = 1;
	IFS0bits.T1IF = 0;
	while(!IFS0bits.T1IF); // wait for 1us
};
// wait for 32k clocks
void Wait32k()
{
	T1CONbits.TCKPS = 2;
	TMR1 = 0;
	PR1 = 0x8000;
   	T1CONbits.TON = 1;
	IFS0bits.T1IF = 0;
	while(!_T1IF); // wait for 32ms
};
