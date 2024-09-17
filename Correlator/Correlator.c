// Code generator for a clipped auto correlator
//
// for a 16bit word processor a  256 channel correlator has a mean of 20 instructions per cycle
// Note this is a mean time.
//
// It a pre-requisit that the counts are measured and clipped on a timer interrupt and
// then written to a FIFO.
//
//The length of the FIFO is estimated from the longest possible processing
// time. For the above example 32.
//
// With Microchip dsPIC33s only Timer0 supports async counting.
//
#include <stdio.h>
#include <stdlib.h>
//
// invoke as
// Correlator  <number of channels> <processor word size>
// eg Correlator  256 16
//
int main(int argc, char **av)
{
	if(ac == 3)
	{
		//
		int nchan = atoi(av[1]);           // number of channels must be multiple of  of bits per int
		int bits_per_int = atoi(av[2]);  // size of native int  8, 16, 32 or 64
		int nword = nchan / bits_per_int;
		//
		printf("typedef unsigned int CVAL;\n");
		printf("#define BITS_PER_INT %d\n",bits_per_int);
		//
		printf("typedef struct\n{\n");
		for(int i = 0; i < nword; i++) // generate the shift register
		{
			printf("\tCVAL Shift%02d;\n",i);
		};
		//
		// now generate the bitmaps
		//
		for(int i = 0; i < 8; i++) // 8 bits
		{
			for(int j = 0; j < nword; j++)
			{
				printf("\tCVAL Bit%d_%02d;\n",i,j);
			};
		};
		//
		printf("CVAL Counts[%d];\n",nchan); // where to put the counts
		printf("\n} CORRELATOR;\n");
		printf("CORRELATOR CC;\n"); // we call the correlator CC (for clipped correlator)
		//
		//
		printf(" // ********************* FUNCTION *************************************\n");
		//
		// we unwind everything
		//
		printf("\nif(getClipped())\n{\n");
		// generate the ripple counter code
		printf("CVAL R;\n");
		printf("CVAL C;\n");
		//
		//
		// we ripple each bank one at a time
		for(int i = 0; i < nword; i++)
		{
			printf("R = CC.Bit0_%02d ^ CC.Shift%02d;\n",i,i);
			printf("C = CC.Bit0_%02d & CC.Shift%02d;\n",i,i,i); 
			printf("CC.Bit0_%02d = R;\n",i);
			//
			for(int j = 0; j < 8; j++)
			{
				printf("if(C)\n{\n");
				printf("R = CC.Bit%d_%02d ^ C;",j,i);
				printf("C = CC.Bit%d_%02d & C;",j,i);
				printf("CC.Bit%d_%02d = R;",j,i);
				printf("}\nelse\n{\ngoto End%02d;\n};",i);
			};
			//
			// bit 7 has rippled over
			// accumulate to counters
			printf("if(C)\n{\n");
			
			int m = 1;
			for(int k = 0; k < bits_per_int; k++, m <<= 1)
			{
				printf("if(C & 0x%X) CC.Counts[%d]++;\n",m,k);
			};
			printf("/n}\nEnd%0d:;\n",i);
			//
		};
		//
		// Generate the shift register code
		//
		for(int i = (nword - 1); i >= 0; i--)
		{
			printf("\t\tShift%02d <<= 1;\n", i);
			if(i) printf("\t\tif(Shift%02d & 0x8000) Shift%02d |= 1;\n", i, i - 1, i);
		};
		//
		//
		printf("\t\tShift0  |= 1;\n");
		printf("}\nelse\n{\n"); // close
		//
		//
		for(int i = (nword - 1); i >= 0; i--)
		{
			printf("\t\tShift%02d <<= 1;\n", i);
			if(i) printf("\t\tif(Shift%02d & 0x8000) Shift%02d |= 1;\n", i, i - 1, i);
		};
		printf("\t};\n");
	};
	return 0;
};
