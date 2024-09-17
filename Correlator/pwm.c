#include "global.h"
// initialise the PWM on 
void InitPwm()
{
	T2CON = 0;
	//
	PR2 = 0xFFFF; // 10 bits 
	T2CONbits.TON = 1; // enable timer	
	//
	OC2CON = 0; // intialise - timer 2
	OC2R = 0x100;
	OC2RS = 0x100; // start at 1 /4 scale
	OC2CONbits.OCM = 6; // enable PWM mode 
	//
};
