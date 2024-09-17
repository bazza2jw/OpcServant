//
#include "global.h"
// This is the command interpreter - the processor needs 100% to do the peak detect
// So it either does commands or it collects, not both
//
#define SEND(s) TxLine(s)
void Correlate();
void Cli()
{
	// one letter commands:
	//
	int c;
	//printf("Enter CLI"EOL);
	while((c = RxChar()) < 0)
	{
		asm("CLRWDT");	
	}; // whait for a character
	
	//printf("Got Char %X"EOL,c);
	//	
	switch(c)
	{
		case 'A': // Start
		{
			printf("Start\r\n");
			Correlate();
			printf("Stop\r\n");
		};		
		break;
		
		case 'B': // Stop
		{
			
		};
		break;
		
		case 'C': // send counts
		{
			
		};
		break;

		default:
		break;
	};	
};
