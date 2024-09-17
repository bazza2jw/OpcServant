//
// (c) B. J. Hill 2007 - 
// Purpose: to generate a full correlator
// full unwound 
//
// 
// $Id$
//
#include <stdio.h>
#include <stdlib.h>

int main(int ac, char **av)
{
	if(ac == 2)
	{
		int nc = atoi(av[1]);
		//
		// generate the correlator
		//
		if(nc > 1)
		{
			// define the types
			printf("#define NCHANNELS (%d)\n",nc);
			printf("typedef unsigned short SVAL;\n");
			printf("typedef unsigned long CVAL;\n");
			//
			// define the correlator structure
			//
			printf("struct\n{\n");
			// generate the shift register
			for(int i = 0; i < nc; i++)
			{
				printf("SVAL S%d;\n",i);
			};
			//
			// generate the accumulators
			//
			for(int i = 0; i < nc; i++)
			{
				printf("CVAL C%d;\n",i);
			};
			printf("} CC;\n");
			//
			//
			// define the correlator code
			// current count  - GETCOUNT is a macro defined elsewhere
			printf("void Correlate()\n{\n");
			printf("SVAL C;\n");
			//
			// we generate code for each possible shift register position
			//
			for(int i = 0; i < nc; i++)
			{
				printf("C = GETCOUNT;\n");
				for(int j = 0; j < nc; j++)
				{
					printf("CC.C%d += CC.S%d * C;\n",j,(i + j)% nc);
				};
				printf("CC.S%d = C;\n",i); // assign the last count into the shift reg
			};
			printf("\n};\n");
			// generate utility function 
			printf("void ClearCorrelator(void)\n{\nmemset(&CC,0,sizeof(CC));\n};\n");
		};
	};
	return 0;
};