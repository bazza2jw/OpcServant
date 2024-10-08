#define BITS_PER_INT 16
typedef unsigned int CVAL;/* basic integer type */
typedef unsigned int RVAL;/*  Counter type    */
typedef struct
{
	CVAL Shift00;
	CVAL Shift01;
	CVAL Shift02;
	CVAL Shift03;
	CVAL Shift04;
	CVAL Shift05;
	CVAL Shift06;
	CVAL Shift07;
	CVAL Shift08;
	CVAL Shift09;
	CVAL Shift10;
	CVAL Shift11;
	CVAL Shift12;
	CVAL Shift13;
	CVAL Shift14;
	CVAL Shift15;
	CVAL Bit0_00;
	CVAL Bit0_01;
	CVAL Bit0_02;
	CVAL Bit0_03;
	CVAL Bit0_04;
	CVAL Bit0_05;
	CVAL Bit0_06;
	CVAL Bit0_07;
	CVAL Bit0_08;
	CVAL Bit0_09;
	CVAL Bit0_10;
	CVAL Bit0_11;
	CVAL Bit0_12;
	CVAL Bit0_13;
	CVAL Bit0_14;
	CVAL Bit0_15;
	CVAL Bit1_00;
	CVAL Bit1_01;
	CVAL Bit1_02;
	CVAL Bit1_03;
	CVAL Bit1_04;
	CVAL Bit1_05;
	CVAL Bit1_06;
	CVAL Bit1_07;
	CVAL Bit1_08;
	CVAL Bit1_09;
	CVAL Bit1_10;
	CVAL Bit1_11;
	CVAL Bit1_12;
	CVAL Bit1_13;
	CVAL Bit1_14;
	CVAL Bit1_15;
	CVAL Bit2_00;
	CVAL Bit2_01;
	CVAL Bit2_02;
	CVAL Bit2_03;
	CVAL Bit2_04;
	CVAL Bit2_05;
	CVAL Bit2_06;
	CVAL Bit2_07;
	CVAL Bit2_08;
	CVAL Bit2_09;
	CVAL Bit2_10;
	CVAL Bit2_11;
	CVAL Bit2_12;
	CVAL Bit2_13;
	CVAL Bit2_14;
	CVAL Bit2_15;
	CVAL Bit3_00;
	CVAL Bit3_01;
	CVAL Bit3_02;
	CVAL Bit3_03;
	CVAL Bit3_04;
	CVAL Bit3_05;
	CVAL Bit3_06;
	CVAL Bit3_07;
	CVAL Bit3_08;
	CVAL Bit3_09;
	CVAL Bit3_10;
	CVAL Bit3_11;
	CVAL Bit3_12;
	CVAL Bit3_13;
	CVAL Bit3_14;
	CVAL Bit3_15;
	CVAL Bit4_00;
	CVAL Bit4_01;
	CVAL Bit4_02;
	CVAL Bit4_03;
	CVAL Bit4_04;
	CVAL Bit4_05;
	CVAL Bit4_06;
	CVAL Bit4_07;
	CVAL Bit4_08;
	CVAL Bit4_09;
	CVAL Bit4_10;
	CVAL Bit4_11;
	CVAL Bit4_12;
	CVAL Bit4_13;
	CVAL Bit4_14;
	CVAL Bit4_15;
	CVAL Bit5_00;
	CVAL Bit5_01;
	CVAL Bit5_02;
	CVAL Bit5_03;
	CVAL Bit5_04;
	CVAL Bit5_05;
	CVAL Bit5_06;
	CVAL Bit5_07;
	CVAL Bit5_08;
	CVAL Bit5_09;
	CVAL Bit5_10;
	CVAL Bit5_11;
	CVAL Bit5_12;
	CVAL Bit5_13;
	CVAL Bit5_14;
	CVAL Bit5_15;
	CVAL Bit6_00;
	CVAL Bit6_01;
	CVAL Bit6_02;
	CVAL Bit6_03;
	CVAL Bit6_04;
	CVAL Bit6_05;
	CVAL Bit6_06;
	CVAL Bit6_07;
	CVAL Bit6_08;
	CVAL Bit6_09;
	CVAL Bit6_10;
	CVAL Bit6_11;
	CVAL Bit6_12;
	CVAL Bit6_13;
	CVAL Bit6_14;
	CVAL Bit6_15;
	CVAL Bit7_00;
	CVAL Bit7_01;
	CVAL Bit7_02;
	CVAL Bit7_03;
	CVAL Bit7_04;
	CVAL Bit7_05;
	CVAL Bit7_06;
	CVAL Bit7_07;
	CVAL Bit7_08;
	CVAL Bit7_09;
	CVAL Bit7_10;
	CVAL Bit7_11;
	CVAL Bit7_12;
	CVAL Bit7_13;
	CVAL Bit7_14;
	CVAL Bit7_15;
	RVAL Counts[256];
} CORRELATOR;
CORRELATOR CC;
int Run = 0;
// ********************* FUNCTION *************************************
void Correlate()
{
	while(Run)
	{
		if(getClipped())
		{
			CVAL R;
			CVAL C;
			R = CC.Bit0_00 ^ CC.Shift00;
			C = CC.Bit0_00 & CC.Shift00;
			CC.Bit0_00 = R;
			if(!C)goto End00;
			R = CC.Bit1_00 ^ C;
			C = CC.Bit1_00 & C;
			CC.Bit1_00 = R;
			if(!C)goto End00;
			R = CC.Bit2_00 ^ C;
			C = CC.Bit2_00 & C;
			CC.Bit2_00 = R;
			if(!C)goto End00;
			R = CC.Bit3_00 ^ C;
			C = CC.Bit3_00 & C;
			CC.Bit3_00 = R;
			if(!C)goto End00;
			R = CC.Bit4_00 ^ C;
			C = CC.Bit4_00 & C;
			CC.Bit4_00 = R;
			if(!C)goto End00;
			R = CC.Bit5_00 ^ C;
			C = CC.Bit5_00 & C;
			CC.Bit5_00 = R;
			if(!C)goto End00;
			R = CC.Bit6_00 ^ C;
			C = CC.Bit6_00 & C;
			CC.Bit6_00 = R;
			if(!C)goto End00;
			R = CC.Bit7_00 ^ C;
			C = CC.Bit7_00 & C;
			CC.Bit7_00 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[0]++;
				if(C & 0x2) CC.Counts[1]++;
				if(C & 0x4) CC.Counts[2]++;
				if(C & 0x8) CC.Counts[3]++;
				if(C & 0x10) CC.Counts[4]++;
				if(C & 0x20) CC.Counts[5]++;
				if(C & 0x40) CC.Counts[6]++;
				if(C & 0x80) CC.Counts[7]++;
				if(C & 0x100) CC.Counts[8]++;
				if(C & 0x200) CC.Counts[9]++;
				if(C & 0x400) CC.Counts[10]++;
				if(C & 0x800) CC.Counts[11]++;
				if(C & 0x1000) CC.Counts[12]++;
				if(C & 0x2000) CC.Counts[13]++;
				if(C & 0x4000) CC.Counts[14]++;
				if(C & 0x8000) CC.Counts[15]++;
			};
			End00:;
			R = CC.Bit0_01 ^ CC.Shift01;
			C = CC.Bit0_01 & CC.Shift01;
			CC.Bit0_01 = R;
			if(!C)goto End01;
			R = CC.Bit1_01 ^ C;
			C = CC.Bit1_01 & C;
			CC.Bit1_01 = R;
			if(!C)goto End01;
			R = CC.Bit2_01 ^ C;
			C = CC.Bit2_01 & C;
			CC.Bit2_01 = R;
			if(!C)goto End01;
			R = CC.Bit3_01 ^ C;
			C = CC.Bit3_01 & C;
			CC.Bit3_01 = R;
			if(!C)goto End01;
			R = CC.Bit4_01 ^ C;
			C = CC.Bit4_01 & C;
			CC.Bit4_01 = R;
			if(!C)goto End01;
			R = CC.Bit5_01 ^ C;
			C = CC.Bit5_01 & C;
			CC.Bit5_01 = R;
			if(!C)goto End01;
			R = CC.Bit6_01 ^ C;
			C = CC.Bit6_01 & C;
			CC.Bit6_01 = R;
			if(!C)goto End01;
			R = CC.Bit7_01 ^ C;
			C = CC.Bit7_01 & C;
			CC.Bit7_01 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[16]++;
				if(C & 0x2) CC.Counts[17]++;
				if(C & 0x4) CC.Counts[18]++;
				if(C & 0x8) CC.Counts[19]++;
				if(C & 0x10) CC.Counts[20]++;
				if(C & 0x20) CC.Counts[21]++;
				if(C & 0x40) CC.Counts[22]++;
				if(C & 0x80) CC.Counts[23]++;
				if(C & 0x100) CC.Counts[24]++;
				if(C & 0x200) CC.Counts[25]++;
				if(C & 0x400) CC.Counts[26]++;
				if(C & 0x800) CC.Counts[27]++;
				if(C & 0x1000) CC.Counts[28]++;
				if(C & 0x2000) CC.Counts[29]++;
				if(C & 0x4000) CC.Counts[30]++;
				if(C & 0x8000) CC.Counts[31]++;
			};
			End01:;
			R = CC.Bit0_02 ^ CC.Shift02;
			C = CC.Bit0_02 & CC.Shift02;
			CC.Bit0_02 = R;
			if(!C)goto End02;
			R = CC.Bit1_02 ^ C;
			C = CC.Bit1_02 & C;
			CC.Bit1_02 = R;
			if(!C)goto End02;
			R = CC.Bit2_02 ^ C;
			C = CC.Bit2_02 & C;
			CC.Bit2_02 = R;
			if(!C)goto End02;
			R = CC.Bit3_02 ^ C;
			C = CC.Bit3_02 & C;
			CC.Bit3_02 = R;
			if(!C)goto End02;
			R = CC.Bit4_02 ^ C;
			C = CC.Bit4_02 & C;
			CC.Bit4_02 = R;
			if(!C)goto End02;
			R = CC.Bit5_02 ^ C;
			C = CC.Bit5_02 & C;
			CC.Bit5_02 = R;
			if(!C)goto End02;
			R = CC.Bit6_02 ^ C;
			C = CC.Bit6_02 & C;
			CC.Bit6_02 = R;
			if(!C)goto End02;
			R = CC.Bit7_02 ^ C;
			C = CC.Bit7_02 & C;
			CC.Bit7_02 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[32]++;
				if(C & 0x2) CC.Counts[33]++;
				if(C & 0x4) CC.Counts[34]++;
				if(C & 0x8) CC.Counts[35]++;
				if(C & 0x10) CC.Counts[36]++;
				if(C & 0x20) CC.Counts[37]++;
				if(C & 0x40) CC.Counts[38]++;
				if(C & 0x80) CC.Counts[39]++;
				if(C & 0x100) CC.Counts[40]++;
				if(C & 0x200) CC.Counts[41]++;
				if(C & 0x400) CC.Counts[42]++;
				if(C & 0x800) CC.Counts[43]++;
				if(C & 0x1000) CC.Counts[44]++;
				if(C & 0x2000) CC.Counts[45]++;
				if(C & 0x4000) CC.Counts[46]++;
				if(C & 0x8000) CC.Counts[47]++;
			};
			End02:;
			R = CC.Bit0_03 ^ CC.Shift03;
			C = CC.Bit0_03 & CC.Shift03;
			CC.Bit0_03 = R;
			if(!C)goto End03;
			R = CC.Bit1_03 ^ C;
			C = CC.Bit1_03 & C;
			CC.Bit1_03 = R;
			if(!C)goto End03;
			R = CC.Bit2_03 ^ C;
			C = CC.Bit2_03 & C;
			CC.Bit2_03 = R;
			if(!C)goto End03;
			R = CC.Bit3_03 ^ C;
			C = CC.Bit3_03 & C;
			CC.Bit3_03 = R;
			if(!C)goto End03;
			R = CC.Bit4_03 ^ C;
			C = CC.Bit4_03 & C;
			CC.Bit4_03 = R;
			if(!C)goto End03;
			R = CC.Bit5_03 ^ C;
			C = CC.Bit5_03 & C;
			CC.Bit5_03 = R;
			if(!C)goto End03;
			R = CC.Bit6_03 ^ C;
			C = CC.Bit6_03 & C;
			CC.Bit6_03 = R;
			if(!C)goto End03;
			R = CC.Bit7_03 ^ C;
			C = CC.Bit7_03 & C;
			CC.Bit7_03 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[48]++;
				if(C & 0x2) CC.Counts[49]++;
				if(C & 0x4) CC.Counts[50]++;
				if(C & 0x8) CC.Counts[51]++;
				if(C & 0x10) CC.Counts[52]++;
				if(C & 0x20) CC.Counts[53]++;
				if(C & 0x40) CC.Counts[54]++;
				if(C & 0x80) CC.Counts[55]++;
				if(C & 0x100) CC.Counts[56]++;
				if(C & 0x200) CC.Counts[57]++;
				if(C & 0x400) CC.Counts[58]++;
				if(C & 0x800) CC.Counts[59]++;
				if(C & 0x1000) CC.Counts[60]++;
				if(C & 0x2000) CC.Counts[61]++;
				if(C & 0x4000) CC.Counts[62]++;
				if(C & 0x8000) CC.Counts[63]++;
			};
			End03:;
			R = CC.Bit0_04 ^ CC.Shift04;
			C = CC.Bit0_04 & CC.Shift04;
			CC.Bit0_04 = R;
			if(!C)goto End04;
			R = CC.Bit1_04 ^ C;
			C = CC.Bit1_04 & C;
			CC.Bit1_04 = R;
			if(!C)goto End04;
			R = CC.Bit2_04 ^ C;
			C = CC.Bit2_04 & C;
			CC.Bit2_04 = R;
			if(!C)goto End04;
			R = CC.Bit3_04 ^ C;
			C = CC.Bit3_04 & C;
			CC.Bit3_04 = R;
			if(!C)goto End04;
			R = CC.Bit4_04 ^ C;
			C = CC.Bit4_04 & C;
			CC.Bit4_04 = R;
			if(!C)goto End04;
			R = CC.Bit5_04 ^ C;
			C = CC.Bit5_04 & C;
			CC.Bit5_04 = R;
			if(!C)goto End04;
			R = CC.Bit6_04 ^ C;
			C = CC.Bit6_04 & C;
			CC.Bit6_04 = R;
			if(!C)goto End04;
			R = CC.Bit7_04 ^ C;
			C = CC.Bit7_04 & C;
			CC.Bit7_04 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[64]++;
				if(C & 0x2) CC.Counts[65]++;
				if(C & 0x4) CC.Counts[66]++;
				if(C & 0x8) CC.Counts[67]++;
				if(C & 0x10) CC.Counts[68]++;
				if(C & 0x20) CC.Counts[69]++;
				if(C & 0x40) CC.Counts[70]++;
				if(C & 0x80) CC.Counts[71]++;
				if(C & 0x100) CC.Counts[72]++;
				if(C & 0x200) CC.Counts[73]++;
				if(C & 0x400) CC.Counts[74]++;
				if(C & 0x800) CC.Counts[75]++;
				if(C & 0x1000) CC.Counts[76]++;
				if(C & 0x2000) CC.Counts[77]++;
				if(C & 0x4000) CC.Counts[78]++;
				if(C & 0x8000) CC.Counts[79]++;
			};
			End04:;
			R = CC.Bit0_05 ^ CC.Shift05;
			C = CC.Bit0_05 & CC.Shift05;
			CC.Bit0_05 = R;
			if(!C)goto End05;
			R = CC.Bit1_05 ^ C;
			C = CC.Bit1_05 & C;
			CC.Bit1_05 = R;
			if(!C)goto End05;
			R = CC.Bit2_05 ^ C;
			C = CC.Bit2_05 & C;
			CC.Bit2_05 = R;
			if(!C)goto End05;
			R = CC.Bit3_05 ^ C;
			C = CC.Bit3_05 & C;
			CC.Bit3_05 = R;
			if(!C)goto End05;
			R = CC.Bit4_05 ^ C;
			C = CC.Bit4_05 & C;
			CC.Bit4_05 = R;
			if(!C)goto End05;
			R = CC.Bit5_05 ^ C;
			C = CC.Bit5_05 & C;
			CC.Bit5_05 = R;
			if(!C)goto End05;
			R = CC.Bit6_05 ^ C;
			C = CC.Bit6_05 & C;
			CC.Bit6_05 = R;
			if(!C)goto End05;
			R = CC.Bit7_05 ^ C;
			C = CC.Bit7_05 & C;
			CC.Bit7_05 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[80]++;
				if(C & 0x2) CC.Counts[81]++;
				if(C & 0x4) CC.Counts[82]++;
				if(C & 0x8) CC.Counts[83]++;
				if(C & 0x10) CC.Counts[84]++;
				if(C & 0x20) CC.Counts[85]++;
				if(C & 0x40) CC.Counts[86]++;
				if(C & 0x80) CC.Counts[87]++;
				if(C & 0x100) CC.Counts[88]++;
				if(C & 0x200) CC.Counts[89]++;
				if(C & 0x400) CC.Counts[90]++;
				if(C & 0x800) CC.Counts[91]++;
				if(C & 0x1000) CC.Counts[92]++;
				if(C & 0x2000) CC.Counts[93]++;
				if(C & 0x4000) CC.Counts[94]++;
				if(C & 0x8000) CC.Counts[95]++;
			};
			End05:;
			R = CC.Bit0_06 ^ CC.Shift06;
			C = CC.Bit0_06 & CC.Shift06;
			CC.Bit0_06 = R;
			if(!C)goto End06;
			R = CC.Bit1_06 ^ C;
			C = CC.Bit1_06 & C;
			CC.Bit1_06 = R;
			if(!C)goto End06;
			R = CC.Bit2_06 ^ C;
			C = CC.Bit2_06 & C;
			CC.Bit2_06 = R;
			if(!C)goto End06;
			R = CC.Bit3_06 ^ C;
			C = CC.Bit3_06 & C;
			CC.Bit3_06 = R;
			if(!C)goto End06;
			R = CC.Bit4_06 ^ C;
			C = CC.Bit4_06 & C;
			CC.Bit4_06 = R;
			if(!C)goto End06;
			R = CC.Bit5_06 ^ C;
			C = CC.Bit5_06 & C;
			CC.Bit5_06 = R;
			if(!C)goto End06;
			R = CC.Bit6_06 ^ C;
			C = CC.Bit6_06 & C;
			CC.Bit6_06 = R;
			if(!C)goto End06;
			R = CC.Bit7_06 ^ C;
			C = CC.Bit7_06 & C;
			CC.Bit7_06 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[96]++;
				if(C & 0x2) CC.Counts[97]++;
				if(C & 0x4) CC.Counts[98]++;
				if(C & 0x8) CC.Counts[99]++;
				if(C & 0x10) CC.Counts[100]++;
				if(C & 0x20) CC.Counts[101]++;
				if(C & 0x40) CC.Counts[102]++;
				if(C & 0x80) CC.Counts[103]++;
				if(C & 0x100) CC.Counts[104]++;
				if(C & 0x200) CC.Counts[105]++;
				if(C & 0x400) CC.Counts[106]++;
				if(C & 0x800) CC.Counts[107]++;
				if(C & 0x1000) CC.Counts[108]++;
				if(C & 0x2000) CC.Counts[109]++;
				if(C & 0x4000) CC.Counts[110]++;
				if(C & 0x8000) CC.Counts[111]++;
			};
			End06:;
			R = CC.Bit0_07 ^ CC.Shift07;
			C = CC.Bit0_07 & CC.Shift07;
			CC.Bit0_07 = R;
			if(!C)goto End07;
			R = CC.Bit1_07 ^ C;
			C = CC.Bit1_07 & C;
			CC.Bit1_07 = R;
			if(!C)goto End07;
			R = CC.Bit2_07 ^ C;
			C = CC.Bit2_07 & C;
			CC.Bit2_07 = R;
			if(!C)goto End07;
			R = CC.Bit3_07 ^ C;
			C = CC.Bit3_07 & C;
			CC.Bit3_07 = R;
			if(!C)goto End07;
			R = CC.Bit4_07 ^ C;
			C = CC.Bit4_07 & C;
			CC.Bit4_07 = R;
			if(!C)goto End07;
			R = CC.Bit5_07 ^ C;
			C = CC.Bit5_07 & C;
			CC.Bit5_07 = R;
			if(!C)goto End07;
			R = CC.Bit6_07 ^ C;
			C = CC.Bit6_07 & C;
			CC.Bit6_07 = R;
			if(!C)goto End07;
			R = CC.Bit7_07 ^ C;
			C = CC.Bit7_07 & C;
			CC.Bit7_07 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[112]++;
				if(C & 0x2) CC.Counts[113]++;
				if(C & 0x4) CC.Counts[114]++;
				if(C & 0x8) CC.Counts[115]++;
				if(C & 0x10) CC.Counts[116]++;
				if(C & 0x20) CC.Counts[117]++;
				if(C & 0x40) CC.Counts[118]++;
				if(C & 0x80) CC.Counts[119]++;
				if(C & 0x100) CC.Counts[120]++;
				if(C & 0x200) CC.Counts[121]++;
				if(C & 0x400) CC.Counts[122]++;
				if(C & 0x800) CC.Counts[123]++;
				if(C & 0x1000) CC.Counts[124]++;
				if(C & 0x2000) CC.Counts[125]++;
				if(C & 0x4000) CC.Counts[126]++;
				if(C & 0x8000) CC.Counts[127]++;
			};
			End07:;
			R = CC.Bit0_08 ^ CC.Shift08;
			C = CC.Bit0_08 & CC.Shift08;
			CC.Bit0_08 = R;
			if(!C)goto End08;
			R = CC.Bit1_08 ^ C;
			C = CC.Bit1_08 & C;
			CC.Bit1_08 = R;
			if(!C)goto End08;
			R = CC.Bit2_08 ^ C;
			C = CC.Bit2_08 & C;
			CC.Bit2_08 = R;
			if(!C)goto End08;
			R = CC.Bit3_08 ^ C;
			C = CC.Bit3_08 & C;
			CC.Bit3_08 = R;
			if(!C)goto End08;
			R = CC.Bit4_08 ^ C;
			C = CC.Bit4_08 & C;
			CC.Bit4_08 = R;
			if(!C)goto End08;
			R = CC.Bit5_08 ^ C;
			C = CC.Bit5_08 & C;
			CC.Bit5_08 = R;
			if(!C)goto End08;
			R = CC.Bit6_08 ^ C;
			C = CC.Bit6_08 & C;
			CC.Bit6_08 = R;
			if(!C)goto End08;
			R = CC.Bit7_08 ^ C;
			C = CC.Bit7_08 & C;
			CC.Bit7_08 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[128]++;
				if(C & 0x2) CC.Counts[129]++;
				if(C & 0x4) CC.Counts[130]++;
				if(C & 0x8) CC.Counts[131]++;
				if(C & 0x10) CC.Counts[132]++;
				if(C & 0x20) CC.Counts[133]++;
				if(C & 0x40) CC.Counts[134]++;
				if(C & 0x80) CC.Counts[135]++;
				if(C & 0x100) CC.Counts[136]++;
				if(C & 0x200) CC.Counts[137]++;
				if(C & 0x400) CC.Counts[138]++;
				if(C & 0x800) CC.Counts[139]++;
				if(C & 0x1000) CC.Counts[140]++;
				if(C & 0x2000) CC.Counts[141]++;
				if(C & 0x4000) CC.Counts[142]++;
				if(C & 0x8000) CC.Counts[143]++;
			};
			End08:;
			R = CC.Bit0_09 ^ CC.Shift09;
			C = CC.Bit0_09 & CC.Shift09;
			CC.Bit0_09 = R;
			if(!C)goto End09;
			R = CC.Bit1_09 ^ C;
			C = CC.Bit1_09 & C;
			CC.Bit1_09 = R;
			if(!C)goto End09;
			R = CC.Bit2_09 ^ C;
			C = CC.Bit2_09 & C;
			CC.Bit2_09 = R;
			if(!C)goto End09;
			R = CC.Bit3_09 ^ C;
			C = CC.Bit3_09 & C;
			CC.Bit3_09 = R;
			if(!C)goto End09;
			R = CC.Bit4_09 ^ C;
			C = CC.Bit4_09 & C;
			CC.Bit4_09 = R;
			if(!C)goto End09;
			R = CC.Bit5_09 ^ C;
			C = CC.Bit5_09 & C;
			CC.Bit5_09 = R;
			if(!C)goto End09;
			R = CC.Bit6_09 ^ C;
			C = CC.Bit6_09 & C;
			CC.Bit6_09 = R;
			if(!C)goto End09;
			R = CC.Bit7_09 ^ C;
			C = CC.Bit7_09 & C;
			CC.Bit7_09 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[144]++;
				if(C & 0x2) CC.Counts[145]++;
				if(C & 0x4) CC.Counts[146]++;
				if(C & 0x8) CC.Counts[147]++;
				if(C & 0x10) CC.Counts[148]++;
				if(C & 0x20) CC.Counts[149]++;
				if(C & 0x40) CC.Counts[150]++;
				if(C & 0x80) CC.Counts[151]++;
				if(C & 0x100) CC.Counts[152]++;
				if(C & 0x200) CC.Counts[153]++;
				if(C & 0x400) CC.Counts[154]++;
				if(C & 0x800) CC.Counts[155]++;
				if(C & 0x1000) CC.Counts[156]++;
				if(C & 0x2000) CC.Counts[157]++;
				if(C & 0x4000) CC.Counts[158]++;
				if(C & 0x8000) CC.Counts[159]++;
			};
			End09:;
			R = CC.Bit0_10 ^ CC.Shift10;
			C = CC.Bit0_10 & CC.Shift10;
			CC.Bit0_10 = R;
			if(!C)goto End10;
			R = CC.Bit1_10 ^ C;
			C = CC.Bit1_10 & C;
			CC.Bit1_10 = R;
			if(!C)goto End10;
			R = CC.Bit2_10 ^ C;
			C = CC.Bit2_10 & C;
			CC.Bit2_10 = R;
			if(!C)goto End10;
			R = CC.Bit3_10 ^ C;
			C = CC.Bit3_10 & C;
			CC.Bit3_10 = R;
			if(!C)goto End10;
			R = CC.Bit4_10 ^ C;
			C = CC.Bit4_10 & C;
			CC.Bit4_10 = R;
			if(!C)goto End10;
			R = CC.Bit5_10 ^ C;
			C = CC.Bit5_10 & C;
			CC.Bit5_10 = R;
			if(!C)goto End10;
			R = CC.Bit6_10 ^ C;
			C = CC.Bit6_10 & C;
			CC.Bit6_10 = R;
			if(!C)goto End10;
			R = CC.Bit7_10 ^ C;
			C = CC.Bit7_10 & C;
			CC.Bit7_10 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[160]++;
				if(C & 0x2) CC.Counts[161]++;
				if(C & 0x4) CC.Counts[162]++;
				if(C & 0x8) CC.Counts[163]++;
				if(C & 0x10) CC.Counts[164]++;
				if(C & 0x20) CC.Counts[165]++;
				if(C & 0x40) CC.Counts[166]++;
				if(C & 0x80) CC.Counts[167]++;
				if(C & 0x100) CC.Counts[168]++;
				if(C & 0x200) CC.Counts[169]++;
				if(C & 0x400) CC.Counts[170]++;
				if(C & 0x800) CC.Counts[171]++;
				if(C & 0x1000) CC.Counts[172]++;
				if(C & 0x2000) CC.Counts[173]++;
				if(C & 0x4000) CC.Counts[174]++;
				if(C & 0x8000) CC.Counts[175]++;
			};
			End10:;
			R = CC.Bit0_11 ^ CC.Shift11;
			C = CC.Bit0_11 & CC.Shift11;
			CC.Bit0_11 = R;
			if(!C)goto End11;
			R = CC.Bit1_11 ^ C;
			C = CC.Bit1_11 & C;
			CC.Bit1_11 = R;
			if(!C)goto End11;
			R = CC.Bit2_11 ^ C;
			C = CC.Bit2_11 & C;
			CC.Bit2_11 = R;
			if(!C)goto End11;
			R = CC.Bit3_11 ^ C;
			C = CC.Bit3_11 & C;
			CC.Bit3_11 = R;
			if(!C)goto End11;
			R = CC.Bit4_11 ^ C;
			C = CC.Bit4_11 & C;
			CC.Bit4_11 = R;
			if(!C)goto End11;
			R = CC.Bit5_11 ^ C;
			C = CC.Bit5_11 & C;
			CC.Bit5_11 = R;
			if(!C)goto End11;
			R = CC.Bit6_11 ^ C;
			C = CC.Bit6_11 & C;
			CC.Bit6_11 = R;
			if(!C)goto End11;
			R = CC.Bit7_11 ^ C;
			C = CC.Bit7_11 & C;
			CC.Bit7_11 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[176]++;
				if(C & 0x2) CC.Counts[177]++;
				if(C & 0x4) CC.Counts[178]++;
				if(C & 0x8) CC.Counts[179]++;
				if(C & 0x10) CC.Counts[180]++;
				if(C & 0x20) CC.Counts[181]++;
				if(C & 0x40) CC.Counts[182]++;
				if(C & 0x80) CC.Counts[183]++;
				if(C & 0x100) CC.Counts[184]++;
				if(C & 0x200) CC.Counts[185]++;
				if(C & 0x400) CC.Counts[186]++;
				if(C & 0x800) CC.Counts[187]++;
				if(C & 0x1000) CC.Counts[188]++;
				if(C & 0x2000) CC.Counts[189]++;
				if(C & 0x4000) CC.Counts[190]++;
				if(C & 0x8000) CC.Counts[191]++;
			};
			End11:;
			R = CC.Bit0_12 ^ CC.Shift12;
			C = CC.Bit0_12 & CC.Shift12;
			CC.Bit0_12 = R;
			if(!C)goto End12;
			R = CC.Bit1_12 ^ C;
			C = CC.Bit1_12 & C;
			CC.Bit1_12 = R;
			if(!C)goto End12;
			R = CC.Bit2_12 ^ C;
			C = CC.Bit2_12 & C;
			CC.Bit2_12 = R;
			if(!C)goto End12;
			R = CC.Bit3_12 ^ C;
			C = CC.Bit3_12 & C;
			CC.Bit3_12 = R;
			if(!C)goto End12;
			R = CC.Bit4_12 ^ C;
			C = CC.Bit4_12 & C;
			CC.Bit4_12 = R;
			if(!C)goto End12;
			R = CC.Bit5_12 ^ C;
			C = CC.Bit5_12 & C;
			CC.Bit5_12 = R;
			if(!C)goto End12;
			R = CC.Bit6_12 ^ C;
			C = CC.Bit6_12 & C;
			CC.Bit6_12 = R;
			if(!C)goto End12;
			R = CC.Bit7_12 ^ C;
			C = CC.Bit7_12 & C;
			CC.Bit7_12 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[192]++;
				if(C & 0x2) CC.Counts[193]++;
				if(C & 0x4) CC.Counts[194]++;
				if(C & 0x8) CC.Counts[195]++;
				if(C & 0x10) CC.Counts[196]++;
				if(C & 0x20) CC.Counts[197]++;
				if(C & 0x40) CC.Counts[198]++;
				if(C & 0x80) CC.Counts[199]++;
				if(C & 0x100) CC.Counts[200]++;
				if(C & 0x200) CC.Counts[201]++;
				if(C & 0x400) CC.Counts[202]++;
				if(C & 0x800) CC.Counts[203]++;
				if(C & 0x1000) CC.Counts[204]++;
				if(C & 0x2000) CC.Counts[205]++;
				if(C & 0x4000) CC.Counts[206]++;
				if(C & 0x8000) CC.Counts[207]++;
			};
			End12:;
			R = CC.Bit0_13 ^ CC.Shift13;
			C = CC.Bit0_13 & CC.Shift13;
			CC.Bit0_13 = R;
			if(!C)goto End13;
			R = CC.Bit1_13 ^ C;
			C = CC.Bit1_13 & C;
			CC.Bit1_13 = R;
			if(!C)goto End13;
			R = CC.Bit2_13 ^ C;
			C = CC.Bit2_13 & C;
			CC.Bit2_13 = R;
			if(!C)goto End13;
			R = CC.Bit3_13 ^ C;
			C = CC.Bit3_13 & C;
			CC.Bit3_13 = R;
			if(!C)goto End13;
			R = CC.Bit4_13 ^ C;
			C = CC.Bit4_13 & C;
			CC.Bit4_13 = R;
			if(!C)goto End13;
			R = CC.Bit5_13 ^ C;
			C = CC.Bit5_13 & C;
			CC.Bit5_13 = R;
			if(!C)goto End13;
			R = CC.Bit6_13 ^ C;
			C = CC.Bit6_13 & C;
			CC.Bit6_13 = R;
			if(!C)goto End13;
			R = CC.Bit7_13 ^ C;
			C = CC.Bit7_13 & C;
			CC.Bit7_13 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[208]++;
				if(C & 0x2) CC.Counts[209]++;
				if(C & 0x4) CC.Counts[210]++;
				if(C & 0x8) CC.Counts[211]++;
				if(C & 0x10) CC.Counts[212]++;
				if(C & 0x20) CC.Counts[213]++;
				if(C & 0x40) CC.Counts[214]++;
				if(C & 0x80) CC.Counts[215]++;
				if(C & 0x100) CC.Counts[216]++;
				if(C & 0x200) CC.Counts[217]++;
				if(C & 0x400) CC.Counts[218]++;
				if(C & 0x800) CC.Counts[219]++;
				if(C & 0x1000) CC.Counts[220]++;
				if(C & 0x2000) CC.Counts[221]++;
				if(C & 0x4000) CC.Counts[222]++;
				if(C & 0x8000) CC.Counts[223]++;
			};
			End13:;
			R = CC.Bit0_14 ^ CC.Shift14;
			C = CC.Bit0_14 & CC.Shift14;
			CC.Bit0_14 = R;
			if(!C)goto End14;
			R = CC.Bit1_14 ^ C;
			C = CC.Bit1_14 & C;
			CC.Bit1_14 = R;
			if(!C)goto End14;
			R = CC.Bit2_14 ^ C;
			C = CC.Bit2_14 & C;
			CC.Bit2_14 = R;
			if(!C)goto End14;
			R = CC.Bit3_14 ^ C;
			C = CC.Bit3_14 & C;
			CC.Bit3_14 = R;
			if(!C)goto End14;
			R = CC.Bit4_14 ^ C;
			C = CC.Bit4_14 & C;
			CC.Bit4_14 = R;
			if(!C)goto End14;
			R = CC.Bit5_14 ^ C;
			C = CC.Bit5_14 & C;
			CC.Bit5_14 = R;
			if(!C)goto End14;
			R = CC.Bit6_14 ^ C;
			C = CC.Bit6_14 & C;
			CC.Bit6_14 = R;
			if(!C)goto End14;
			R = CC.Bit7_14 ^ C;
			C = CC.Bit7_14 & C;
			CC.Bit7_14 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[224]++;
				if(C & 0x2) CC.Counts[225]++;
				if(C & 0x4) CC.Counts[226]++;
				if(C & 0x8) CC.Counts[227]++;
				if(C & 0x10) CC.Counts[228]++;
				if(C & 0x20) CC.Counts[229]++;
				if(C & 0x40) CC.Counts[230]++;
				if(C & 0x80) CC.Counts[231]++;
				if(C & 0x100) CC.Counts[232]++;
				if(C & 0x200) CC.Counts[233]++;
				if(C & 0x400) CC.Counts[234]++;
				if(C & 0x800) CC.Counts[235]++;
				if(C & 0x1000) CC.Counts[236]++;
				if(C & 0x2000) CC.Counts[237]++;
				if(C & 0x4000) CC.Counts[238]++;
				if(C & 0x8000) CC.Counts[239]++;
			};
			End14:;
			R = CC.Bit0_15 ^ CC.Shift15;
			C = CC.Bit0_15 & CC.Shift15;
			CC.Bit0_15 = R;
			if(!C)goto End15;
			R = CC.Bit1_15 ^ C;
			C = CC.Bit1_15 & C;
			CC.Bit1_15 = R;
			if(!C)goto End15;
			R = CC.Bit2_15 ^ C;
			C = CC.Bit2_15 & C;
			CC.Bit2_15 = R;
			if(!C)goto End15;
			R = CC.Bit3_15 ^ C;
			C = CC.Bit3_15 & C;
			CC.Bit3_15 = R;
			if(!C)goto End15;
			R = CC.Bit4_15 ^ C;
			C = CC.Bit4_15 & C;
			CC.Bit4_15 = R;
			if(!C)goto End15;
			R = CC.Bit5_15 ^ C;
			C = CC.Bit5_15 & C;
			CC.Bit5_15 = R;
			if(!C)goto End15;
			R = CC.Bit6_15 ^ C;
			C = CC.Bit6_15 & C;
			CC.Bit6_15 = R;
			if(!C)goto End15;
			R = CC.Bit7_15 ^ C;
			C = CC.Bit7_15 & C;
			CC.Bit7_15 = R;
			if(C)
			{
				if(C & 0x1) CC.Counts[240]++;
				if(C & 0x2) CC.Counts[241]++;
				if(C & 0x4) CC.Counts[242]++;
				if(C & 0x8) CC.Counts[243]++;
				if(C & 0x10) CC.Counts[244]++;
				if(C & 0x20) CC.Counts[245]++;
				if(C & 0x40) CC.Counts[246]++;
				if(C & 0x80) CC.Counts[247]++;
				if(C & 0x100) CC.Counts[248]++;
				if(C & 0x200) CC.Counts[249]++;
				if(C & 0x400) CC.Counts[250]++;
				if(C & 0x800) CC.Counts[251]++;
				if(C & 0x1000) CC.Counts[252]++;
				if(C & 0x2000) CC.Counts[253]++;
				if(C & 0x4000) CC.Counts[254]++;
				if(C & 0x8000) CC.Counts[255]++;
			};
			End15:;
			CC.Shift15 <<= 1;
			if(CC.Shift14 & 0x8000) CC.Shift15 |= 1;
			CC.Shift14 <<= 1;
			if(CC.Shift13 & 0x8000) CC.Shift14 |= 1;
			CC.Shift13 <<= 1;
			if(CC.Shift12 & 0x8000) CC.Shift13 |= 1;
			CC.Shift12 <<= 1;
			if(CC.Shift11 & 0x8000) CC.Shift12 |= 1;
			CC.Shift11 <<= 1;
			if(CC.Shift10 & 0x8000) CC.Shift11 |= 1;
			CC.Shift10 <<= 1;
			if(CC.Shift09 & 0x8000) CC.Shift10 |= 1;
			CC.Shift09 <<= 1;
			if(CC.Shift08 & 0x8000) CC.Shift09 |= 1;
			CC.Shift08 <<= 1;
			if(CC.Shift07 & 0x8000) CC.Shift08 |= 1;
			CC.Shift07 <<= 1;
			if(CC.Shift06 & 0x8000) CC.Shift07 |= 1;
			CC.Shift06 <<= 1;
			if(CC.Shift05 & 0x8000) CC.Shift06 |= 1;
			CC.Shift05 <<= 1;
			if(CC.Shift04 & 0x8000) CC.Shift05 |= 1;
			CC.Shift04 <<= 1;
			if(CC.Shift03 & 0x8000) CC.Shift04 |= 1;
			CC.Shift03 <<= 1;
			if(CC.Shift02 & 0x8000) CC.Shift03 |= 1;
			CC.Shift02 <<= 1;
			if(CC.Shift01 & 0x8000) CC.Shift02 |= 1;
			CC.Shift01 <<= 1;
			if(CC.Shift00 & 0x8000) CC.Shift01 |= 1;
			CC.Shift00 <<= 1;
			CC.Shift00  |= 1;
		}
		else
		{
			CC.Shift15 <<= 1;
			if(CC.Shift14 & 0x8000) CC.Shift15 |= 1;
			CC.Shift14 <<= 1;
			if(CC.Shift13 & 0x8000) CC.Shift14 |= 1;
			CC.Shift13 <<= 1;
			if(CC.Shift12 & 0x8000) CC.Shift13 |= 1;
			CC.Shift12 <<= 1;
			if(CC.Shift11 & 0x8000) CC.Shift12 |= 1;
			CC.Shift11 <<= 1;
			if(CC.Shift10 & 0x8000) CC.Shift11 |= 1;
			CC.Shift10 <<= 1;
			if(CC.Shift09 & 0x8000) CC.Shift10 |= 1;
			CC.Shift09 <<= 1;
			if(CC.Shift08 & 0x8000) CC.Shift09 |= 1;
			CC.Shift08 <<= 1;
			if(CC.Shift07 & 0x8000) CC.Shift08 |= 1;
			CC.Shift07 <<= 1;
			if(CC.Shift06 & 0x8000) CC.Shift07 |= 1;
			CC.Shift06 <<= 1;
			if(CC.Shift05 & 0x8000) CC.Shift06 |= 1;
			CC.Shift05 <<= 1;
			if(CC.Shift04 & 0x8000) CC.Shift05 |= 1;
			CC.Shift04 <<= 1;
			if(CC.Shift03 & 0x8000) CC.Shift04 |= 1;
			CC.Shift03 <<= 1;
			if(CC.Shift02 & 0x8000) CC.Shift03 |= 1;
			CC.Shift02 <<= 1;
			if(CC.Shift01 & 0x8000) CC.Shift02 |= 1;
			CC.Shift01 <<= 1;
			if(CC.Shift00 & 0x8000) CC.Shift01 |= 1;
			CC.Shift00 <<= 1;
		};
	};
};
