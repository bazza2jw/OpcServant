#define NCHANNELS (5)
typedef unsigned short SVAL;
typedef unsigned long CVAL;
struct
{
	SVAL S0;
	SVAL S1;
	SVAL S2;
	SVAL S3;
	SVAL S4;
	CVAL C0;
	CVAL C1;
	CVAL C2;
	CVAL C3;
	CVAL C4;
} CC;
void Correlate()
{
	SVAL C;
	C = GETCOUNT;
	CC.C0 += CC.S0 * C;
	CC.C1 += CC.S1 * C;
	CC.C2 += CC.S2 * C;
	CC.C3 += CC.S3 * C;
	CC.C4 += CC.S4 * C;
	CC.S0 = C;
	C = GETCOUNT;
	CC.C0 += CC.S1 * C;
	CC.C1 += CC.S2 * C;
	CC.C2 += CC.S3 * C;
	CC.C3 += CC.S4 * C;
	CC.C4 += CC.S0 * C;
	CC.S1 = C;
	C = GETCOUNT;
	CC.C0 += CC.S2 * C;
	CC.C1 += CC.S3 * C;
	CC.C2 += CC.S4 * C;
	CC.C3 += CC.S0 * C;
	CC.C4 += CC.S1 * C;
	CC.S2 = C;
	C = GETCOUNT;
	CC.C0 += CC.S3 * C;
	CC.C1 += CC.S4 * C;
	CC.C2 += CC.S0 * C;
	CC.C3 += CC.S1 * C;
	CC.C4 += CC.S2 * C;
	CC.S3 = C;
	C = GETCOUNT;
	CC.C0 += CC.S4 * C;
	CC.C1 += CC.S0 * C;
	CC.C2 += CC.S1 * C;
	CC.C3 += CC.S2 * C;
	CC.C4 += CC.S3 * C;
	CC.S4 = C;
};
void ClearCorrelator(void)
{
	memset(&CC,0,sizeof(CC));
};
