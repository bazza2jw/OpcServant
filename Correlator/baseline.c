#include "global.h"
//
int baseline; // the current baseline
int noise; // the current noise value
int mnoise; // the negative noise value
int noise3; // three times the noise value - optimise
int relativeNoise = 5; // relativeNoise used in auto threshold detect
int fixedNoise = 0; // flag if using fixed noise
int fixedBaseline = 0; // the fixed baseline
int fixedBaselineValue = 0; // flag if using fixed base line - infers no DC offset
int fixedNoiseValue = 5; // the 
//
//
void initPeak(int base, int n)
{
	baseline = base;
	noise = n; // noise 
	noise3 = n * 3; // limit at which we do not bother to corellate
	mnoise = -noise; // negitve noise value 
};
//
// determine the base line
// and DC offset
//
static int peakc; // the peak channel
//
void DetermineBaseline()
{
	// take 10,000 samples (50ms)
	int sum;
	int i;
	peakc = 0; // the peak channel
	int max = 0;
	int lim20 = 0;
	//
	if(relativeNoise < 1) relativeNoise = 5;
	//
	//
	dmaOff();
	mode = BASE_MODE; // select calibrate mode
	sampleTime = 100; // 100 ms sample
	sampleCount = 0;
	//
	memset(&Counts,0,sizeof(Counts));
	//
	dmaOn();
	//
	while(isDmaOn()){ asm("CLRWDT");}; // wait for data
	//	
	// we have the data 
	for(i = 0; i < N_BASELINE_CHAN; i++)
	{
		if(max < Counts.uCounts[i]) 
		{
			peakc = i;
			max = Counts.uCounts[i];	
		};
	};
	
	//printf("peakc %d max %d"EOL,peakc,max);
	//
	// the peak channel is the baseline
	// The problem here is the noise may not be gaussian. For example 
	// the base line may be negitive with the peak coming above it 
	//
	if(peakc > 0)
	{
		max = max / relativeNoise; 
		lim20 = peakc;
		for(i = peakc; i > 0; i--)
		{
			if(Counts.uCounts[i] <= max)
			{
				lim20 = (peakc - i + 1); 
				break;	
			};
		};
					
	}
	else
	{
		max = max / relativeNoise; 
		for(i = peakc; i < N_BASELINE_CHAN; i++)
		{
			if(Counts.uCounts[i] <= max)
			{
				lim20 = (i - peakc + 1); 
				break;	
			};
		};			
	};
#ifndef TEN_BIT_ADC	
	peakc *= N_BASELINE_CHAN / N_CHAN;
	lim20 *= N_BASELINE_CHAN / N_CHAN;
#endif
	//printf("noise %d"EOL,lim20);
	if(fixedBaseline) peakc = fixedBaselineValue;
	if(fixedNoise) lim20 = fixedNoiseValue;
	initPeak(peakc,lim20);
};


//
// we should only call this at the start of a measurement
// it takes about 1 second to go full scale
// 
void SetupOffset()
{
	if(!fixedBaseline)
	{
		int i,d;
		//
#ifdef INVERTED_OFFSET
// for the Liquilaz interface
		OC2RS = 0x8000;
	
#else
// we assume positve pulses with positive baseline
		OC2RS = 0;
#endif
		asm("CLRWDT");
		for(i = 0; i < 20; i++)
		{
				T1CONbits.TCKPS = 2;
				TMR1 = 0;
				PR1 = 10;
	   			T1CONbits.TON = 1;
				IFS0bits.T1IF = 0;
				while(!_T1IF); // wait 1ms per ADC step
		};
		
		//
		for(i = 0; i < 50; i++) // try 20 times
		{
			long j;
			// this is to keep the offset correct
			DetermineBaseline(); // work out where the base line is
			//
			//printf("OC2RS %d peakc %d\n\r",OC2RS,peakc);
			//
			d = (peakc *4)/ (N_BASELINE_CHAN / N_CHAN);
			j = OC2RS; // watch for unsigned / signed problems
#ifdef INVERTED_OFFSET
			if(j > d) 
			{
					j -= d; //  * (0xFFFF /20480);
			}
			else
			{
					j = 0;	
			};
#else
			j += d; //  * (0xFFFF /20480);
#endif
			if( peakc < 10) return;
			OC2RS = (unsigned)j;
			//
			// The wait time is about 1ms / mV to allow it to settle
			//
			for(;j > 0; j--)
			{
				T1CONbits.TCKPS = 2;
				TMR1 = 0;
				PR1 = 10;
	   			T1CONbits.TON = 1;
				IFS0bits.T1IF = 0;
				while(!_T1IF); // wait 1ms per ADC step
				asm("CLRWDT");
			};
		};
	}
	else
	{
		OC2RS = 0; // disable DC offset
	};
};

