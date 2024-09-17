#include "global.h"
//
// Initialise ADC 1
//
int autoSampleTime = 0;
int conversionClock = 40; //20us convert clock
void initAdc12()
{
	AD1CON1 = 0;
	AD1CON2 = 0;
	AD1CON3 = 0;
	AD1CON4 = 0;
	AD2CON1 = 0;
	AD2CON2 = 0;
	AD2CON3 = 0;
	AD2CON4 = 0;

        AD1CON1bits.ADON = 0;		// Turn off the A/D converter
	//AD1CON1 Register
        AD1CON1bits.FORM =  0;		// Data Output Format: integer
        AD1CON1bits.SSRC =  7;		// Internal Counter (SAMC) ends sampling and starts convertion
        AD1CON1bits.ASAM =  1;		// ADC Sample Control: Sampling begins immediately after conversion
 		AD1CON1bits.SIMSAM= 0;		// Sequencial Sampling/conversion
		AD1CON1bits.AD12B = 1;		// 12-bit 1 chan
		AD1CON1bits.ADDMABM=1; 		// DMA buffers are built in conversion order mode
		AD1CON2bits.CHPS=0;			// Converts CH0/CH1

    //AD1CON2 Register
        AD1CON2bits.SMPI=0;     	// Increment DMA address every 1 sample/conversion
		AD1CON2bits.BUFM=0;			// one value ping pong - should be enough
		AD1CON2bits.VCFG = 1; 


	//AD1CON3 Register        
		AD1CON3bits.ADRC=0;			    // ADC Clock is derived from Systems Clock
		AD1CON3bits.SAMC=autoSampleTime;// Auto Sample Time = 0*Tad
		AD1CON3bits.ADCS=conversionClock; // ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/40M)*3 = 75ns (13.3Mhz)
									// ADC Conversion Time for 10-bit Tc=12*Tab =  900ns (1.1MHz)						
	//AD1CON4 Register  
		AD1CON4=0;					// Allocate 1 words of buffer to each analog input
									// This register is not used in conversion order mode
									// This is required only in the scatter/gather mode


	//AD1PCFGH/AD1PCFGL: Port Configuration Register
		AD1PCFGL=0xFFFF;
		//AD1PCFGH=0xFFFF;
		AD1CHS0 = 0x0004;
		AD1PCFGLbits.PCFG4 = 0;		// AN5 as Analog Input

 	//AD1CSSH/AD1CSSL: A/D Input Scan Selection Register
    //    AD1CSSH = 0x0000;
		AD1CSSL = 0x0000;			// Channel Scan is disabled, default state
        

        _AD1IF = 0;			// Clear the A/D interrupt flag bit
        _AD1IE = 0;			// Do Not Enable A/D interrupt 	
        //
        AD1CON1bits.DONE = 0;

        AD2CON1bits.ADON = 0;		// Turn off the A/D converter
	//AD1CON1 Register
        AD2CON1bits.FORM =  0;		// Data Output Format: integer
        AD2CON1bits.SSRC =  7;		// Internal Counter (SAMC) ends sampling and starts convertion
        AD2CON1bits.ASAM =  1;		// ADC Sample Control: Sampling begins immediately after conversion
 		AD2CON1bits.SIMSAM= 0;		// Sequencial Sampling/conversion
		AD2CON1bits.AD12B = 1;		// 12-bit 1 chan
		AD2CON1bits.ADDMABM=1; 		// DMA buffers are built in conversion order mode
		AD2CON2bits.CHPS=0;			// Converts CH0/CH1

    //AD1CON2 Register
        AD2CON2bits.SMPI=0;     	// Increment DMA address every 1 sample/conversion
		AD2CON2bits.BUFM=0;			// one value ping pong - should be enough
		AD2CON2bits.VCFG = 1; 


	//AD1CON3 Register        
		AD2CON3bits.ADRC=0;			// ADC Clock is derived from Systems Clock
		AD2CON3bits.SAMC=4; // Auto Sample Time = 0*Tad
		AD2CON3bits.ADCS=8;// ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/40M)*3 = 75ns (13.3Mhz)
									// ADC Conversion Time for 10-bit Tc=12*Tab =  900ns (1.1MHz)						
	//AD1CON4 Register  
		AD2CON4=0;					// Allocate 1 words of buffer to each analog input
									// This register is not used in conversion order mode
									// This is required only in the scatter/gather mode


		 AD2PCFGL = 0xFFFF;
		 AD2CSSL = 0x0000;
	//AD1PCFGH/AD1PCFGL: Port Configuration Register
		AD2CHS0 = 0x0004;
		AD2PCFGLbits.PCFG4 = 0;		// AN4 as Analog Input


        _AD2IF = 0;			// Clear the A/D interrupt flag bit
        _AD2IE = 0;			// Do Not Enable A/D interrupt 	
        //
        AD2CON1bits.DONE = 0;

};


void initAdc10()
{
	AD1CON1 = 0;
	AD1CON2 = 0;
	AD1CON3 = 0;
	AD1CON4 = 0;
	AD2CON1 = 0;
	AD2CON2 = 0;
	AD2CON3 = 0;
	AD2CON4 = 0;
	
        AD1CON1bits.ADON = 0;		// Turn off the A/D converter
	//AD1CON1 Register
        AD1CON1bits.FORM =  0;		// Data Output Format: integer
        AD1CON1bits.SSRC =  7;		// Internal Counter (SAMC) ends sampling and starts convertion
        AD1CON1bits.ASAM =  1;		// ADC Sample Control: Sampling begins immediately after conversion
 		AD1CON1bits.SIMSAM= 0;		// Sequencial Sampling/conversion
		AD1CON1bits.AD12B = 0;		// 10-bit 1 chan
		AD1CON1bits.ADDMABM=1; 		// DMA buffers are built in conversion order mode
		AD1CON2bits.CHPS=0;			// Converts CH0/CH1

    //AD1CON2 Register
        AD1CON2bits.SMPI=0;     	// Increment DMA address every 1 sample/conversion
		AD1CON2bits.BUFM=0;			// one value ping pong - should be enough
		AD1CON2bits.VCFG = 1; 


	//AD1CON3 Register        
		AD1CON3bits.ADRC=0;			    // ADC Clock is derived from Systems Clock
		AD1CON3bits.SAMC=0;// Auto Sample Time = 0*Tad
		AD1CON3bits.ADCS=3; // ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/40M)*3 = 75ns (13.3Mhz)
									// ADC Conversion Time for 10-bit Tc=12*Tab =  900ns (1.1MHz)						
	//AD1CON4 Register  
		AD1CON4=0;					// Allocate 1 words of buffer to each analog input
									// This register is not used in conversion order mode
									// This is required only in the scatter/gather mode


	//AD1PCFGH/AD1PCFGL: Port Configuration Register
		AD1PCFGL=0xFFFF;
		//AD1PCFGH=0xFFFF;
		AD1CHS0 = 0x0004;
		AD1PCFGLbits.PCFG4 = 0;		// AN5 as Analog Input

 	//AD1CSSH/AD1CSSL: A/D Input Scan Selection Register
    //    AD1CSSH = 0x0000;
		AD1CSSL = 0x0000;			// Channel Scan is disabled, default state
        

        _AD1IF = 0;			// Clear the A/D interrupt flag bit
        _AD1IE = 0;			// Do Not Enable A/D interrupt 	
        //
        AD1CON1bits.DONE = 0;

        AD2CON1bits.ADON = 0;		// Turn off the A/D converter
	//AD1CON1 Register
        AD2CON1bits.FORM =  0;		// Data Output Format: integer
        AD2CON1bits.SSRC =  7;		// Internal Counter (SAMC) ends sampling and starts convertion
        AD2CON1bits.ASAM =  1;		// ADC Sample Control: Sampling begins immediately after conversion
 		AD2CON1bits.SIMSAM= 0;		// Sequencial Sampling/conversion
		AD2CON1bits.AD12B = 0;		// 12-bit 1 chan
		AD2CON1bits.ADDMABM=1; 		// DMA buffers are built in conversion order mode
		AD2CON2bits.CHPS=0;			// Converts CH0/CH1

    //AD1CON2 Register
        AD2CON2bits.SMPI=0;     	// Increment DMA address every 1 sample/conversion
		AD2CON2bits.BUFM=0;			// one value ping pong - should be enough
		AD2CON2bits.VCFG = 1; 


	//AD1CON3 Register        
		AD2CON3bits.ADRC=0;			// ADC Clock is derived from Systems Clock
		AD2CON3bits.SAMC=4; // Auto Sample Time = 0*Tad
		AD2CON3bits.ADCS=8;// ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/40M)*3 = 75ns (13.3Mhz)
									// ADC Conversion Time for 10-bit Tc=12*Tab =  900ns (1.1MHz)						
	//AD1CON4 Register  
		AD2CON4=0;					// Allocate 1 words of buffer to each analog input
									// This register is not used in conversion order mode
									// This is required only in the scatter/gather mode


		 AD2PCFGL = 0xFFFF;
		 AD2CSSL = 0x0000;
	//AD1PCFGH/AD1PCFGL: Port Configuration Register
		AD2CHS0 = 0x0004;
		AD2PCFGLbits.PCFG4 = 0;		// AN4 as Analog Input


        _AD2IF = 0;			// Clear the A/D interrupt flag bit
        _AD2IE = 0;			// Do Not Enable A/D interrupt 	
        //
        AD2CON1bits.DONE = 0;

};



//
// Start the Adcs
//
void StartAdc()
{
	_AD1IF = 0;			// Clear the A/D interrupt flag bit
	_AD1IE = 0;			// A/D interrupt 
	_AD2IF = 0;			// Clear the A/D interrupt flag bit
	_AD2IE = 0;			// A/D interrupt 
	AD1CON1bits.DONE = 0;
	AD1CON1bits.ADON = 1;
	AD2CON1bits.DONE = 0;
	AD2CON1bits.ADON = 1;
};

