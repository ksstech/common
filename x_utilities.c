/*
 * x_utilities.c - Copyright (c) 2014-24 Andre M. Maree / KSS Technologies (Pty) Ltd.
 */

#include <string.h>

#include "hal_config.h"
#include "FreeRTOS_Support.h"
#include "printfx.h"
#include "x_utilities.h"
#include "x_terminal.h"
#include "x_errors_events.h"

#define	debugFLAG					0xF000

#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

// #################################################################################################

void vShowActivity(int i) {
	static char caActivity[] = { '0', '0', '0', '0', 0 };
	IF_myASSERT(debugPARAM, i < (sizeof(caActivity) - 1));
	++caActivity[i];
	if (caActivity[i] == 0x3A) caActivity[i] = CHR_0;
	printfx_lock(NULL);
	vANSIcursorsave();
	vANSIlocate(1, 120);
	vANSIputs(caActivity);
	vANSIcursorback();
	printfx_unlock(NULL);
}

void vUtilPrintCharacterSet(void) {
	u8_t Buffer[256];
	for (int cChr = 0; cChr < sizeof(Buffer); cChr++) Buffer[cChr] = cChr;
	printfx("%!'+hhY", sizeof(Buffer), Buffer);
}

/**
 * @brief	Convert hex mac address to u64_t
 * @param	hwaddr hex mac address
 * @return	mac address as u64_t
 */
u64_t mac2int(u8_t * hwaddr) {
	u64_t iRV = 0;
	for (int i = 5; i >= 0; --i)
		iRV |= (u64_t) *hwaddr++ << (BITS_IN_BYTE * i);
	return iRV;
}

/**
 * Convert u64_t mac address to hex
 * @param[in] mac u64_t mac address
 * @param[out] hwaddr hex mac address
 */
void int2mac(u64_t mac, u8_t * hwaddr) {
	for (s8_t i = 5; i >= 0; --i)
		*hwaddr++ = mac >> (BITS_IN_BYTE * i);
}

void MemDump(u8_t ** pMemAddr, int cChr, size_t Size) {
	printfx("MemDump:\r\n%#'+hhY", Size, *pMemAddr);
	*pMemAddr = (cChr == CHR_PLUS) ? (*pMemAddr + Size) : (cChr == CHR_MINUS) ? (*pMemAddr - Size) : *pMemAddr;
}

// ####################################### UUID support ############################################

// https://stackoverflow.com/questions/7399069/how-to-generate-a-guid-in-c#7399348
void xGenerateUUID(char * pBuf) {
// V4 template = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
	char szHex[] = "0123456789ABCDEF";
	srand(clock());
	for (int t = 0; t < 36; ++t, ++pBuf) {
	    int r = rand() % 16;
	    if (t == 8 || t == 13 || t == 18 || t == 23)
	    	*pBuf = CHR_MINUS;
	    else if (t == 14)
	    	*pBuf = CHR_4;
	    else if (t == 19)
	    	*pBuf = szHex[(r & 0x03) | 0x08];
	    else
	    	*pBuf = szHex[r];
	}
	IF_P(debugRESULT, "%.36s\r\n", pBuf);
}

// ################################ Random number & string support #################################

#define ALPHA_UC 	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define	ALPHA_LC	"abcdefghijklmnopqrstuvwxyz"
#define	NUMERICS	"0123456789"
#define	SYMBOLS1	" !\"#$%&'()*+,-./"
#define	SYMBOLS2	":;<=>?@[\140]^_`{|}~"

#define	ALPHA_UC_SIZE	(sizeof(ALPHA_UC) - 1)
#define	ALPHA_LC_SIZE	(sizeof(ALPHA_LC) - 1)
#define	NUMERICS_SIZE	(sizeof(NUMERICS) - 1)
#define	SYMBOLS1_SIZE	(sizeof(SYMBOLS1) - 1)
#define	SYMBOLS2_SIZE	(sizeof(SYMBOLS2) - 1)

#define BASIC_SIZE	(ALPHA_UC_SIZE + ALPHA_LC_SIZE)
#define	OPTN1_SIZE	(BASIC_SIZE + NUMERICS_SIZE)
#define	OPTN2_SIZE	(OPTN1_SIZE + SYMBOLS1_SIZE)
#define	OPTN3_SIZE	(OPTN2_SIZE + SYMBOLS2_SIZE)

const char charset[] = { ALPHA_UC ALPHA_LC NUMERICS SYMBOLS1 SYMBOLS2 };
DUMB_STATIC_ASSERT(sizeof(charset) == (OPTN3_SIZE + 1));

void vBuildRandomSXX(u8_t * pu8, int len, int set) {
	if (len && pu8) {
		int size = (set >= 3) ?	OPTN3_SIZE :
					(set == 2) ? OPTN2_SIZE :
					(set == 1) ? OPTN1_SIZE : BASIC_SIZE;
		for (int n = 0; n < len; pu8[n++] = charset[rand() % size]);
	}
}

void vBuildRandomStr(u8_t * pu8, int len, int set) {
	if (len && pu8) {
		vBuildRandomSXX(pu8, --len, set);
		pu8[len] = CHR_NUL;
	}
}

x8_t xBuildRandomX8(void) {
	x8_t X8;
	X8.u8 = rand() & 0xFF;
	return X8;
}

x16_t xBuildRandomX16(void) {
	x16_t X16;
	X16.u16 = rand() & 0xFFFF;
	return X16;
}

x32_t xBuildRandomX32(void) {
	x32_t	X32;
	X32.i32 = rand();
	if (rand() % 2) X32.i32 *= -1;						// ensure some (-) values
	return X32;
}

x64_t xBuildRandomX64(void) {
	x64_t X64;
	X64.x32[0] = xBuildRandomX32();
	X64.x32[1] = xBuildRandomX32();
	return X64;
}

// ############################## Ixx and Uxx manipulation support #################################

u32_t u32pow(u32_t base, u32_t exp) {
	u32_t res;
	for(res = 1; exp > 0; res *= base, --exp);
	return res;
}

u64_t u64pow(u32_t base, u32_t exp) {
	u64_t res;
	for(res = 1; exp > 0; res *= base, --exp);
	return res;
}

int u32Trailing0(u32_t U32val) {
	int iRV = 0;
	while (U32val > 0) {
		if (U32val % 10) break;
		++iRV;
		U32val /= 10;
	}
	return iRV;
}

int u64Trailing0(u64_t U64val) {
	int iRV = 0;
	while (U64val > 0) {
		if (U64val % 10) break;
		++iRV;
		U64val /= 10;
	}
	return iRV;
}

int	xDigitsInI32(i32_t I32val, bool grouping) {
	int x;
	if (I32val == INT32_MIN) {
		x = 10 + 1;
	} else {	// if value is negative, and '-' counts as a digit....
		if (I32val < 0)
			return xDigitsInI32(-I32val, grouping) + 1;
		if (I32val >= 10000) {
			if (I32val >= 10000000) {
		        if (I32val >= 100000000)
		        	x = (I32val >= 1000000000) ? 10 : 9;
		        else
		        	x = 8;
			} else {
				if (I32val >= 100000)
					x = (I32val >= 1000000) ? 7 : 6;
				else
					x = 5;
			}
	    } else {
		    if (I32val >= 100)
		    	x = (I32val >= 1000) ? 4 : 3;
		    else
		    	x = (I32val >= 10) ? 2 : 1;
	    }
	}
	return x + ((x - 1) / 3);
}

int	xDigitsInU32(u32_t U32val, bool grouping) {
	int x;
	if (U32val >= 10000) {
		if (U32val >= 10000000) {
	        if (U32val >= 100000000)
	        	x = (U32val >= 1000000000)? 10 : 9;
	        else
	        	x = 8;
		} else {
			if (U32val >= 100000)
				x = (U32val >= 1000000) ? 7 : 6;
			else
				x = 5;
		}
    } else {
        if (U32val >= 100)
        	x = (U32val >= 1000) ? 4 : 3;
        else
        	x = (U32val >= 10) ? 2 : 1;
    }
    return x + ((x - 1) / 3);
}

int	xDigitsInU64(u64_t U64val, bool grouping) {
	int x;
	if (U64val <= UINT32_MAX)
		return xDigitsInU32((u32_t) U64val, grouping);
	if (U64val >= 100000000000000ULL) {
		if (U64val >= 100000000000000000ULL) {
			if (U64val >= 1000000000000000000ULL)
				x = (U64val >= 10000000000000000000ULL) ? 20  : 19;
			else
				x = 18;
		} else {
			if (U64val >= 1000000000000000ULL)
				x = (U64val >= 10000000000000000ULL) ? 17 : 16;
			else
				x = 15;
		}
	} else {
		if (U64val >= 1000000000000ULL)
			x = (U64val > 10000000000000ULL) ? 14 : 13;
		else
			x = (U64val >= 100000000000ULL) ? 12 : 11;
	}
	return x + ((x - 1) / 3);
}

int	xU32ToDecStr(u32_t U32val, char * pBuf) {
	int	Len = 0;
	if (U32val) {
		u32_t	iTemp, Div = 1000000000UL;
		do {
			iTemp = U32val / Div;
			if (iTemp != 0 || Len > 0) {
				*pBuf++	= iTemp + CHR_0;
				U32val 	-= iTemp * Div;
				++Len;
			}
			Div /= 10;
		} while (Div);
	} else {
		*pBuf++ = CHR_0;									// answer is single '0'
		++Len;
	}
	*pBuf	= 0;										// terminate
	return Len;
}

/**
 * @brief	Round VAL up or down to closest multiple of MULT
 * @param
 * @return
 */
u32_t u32Round(u32_t U32val, u32_t Mult) {
	u32_t Frac = U32val % Mult;
	U32val -= Frac;
	return (Frac >= (Mult / 2)) ? (U32val + Mult) : U32val;
}

/**
 * @brief	Round VAL up to closest multiple of MULT
 * @param
 * @return
 */
u32_t u32RoundUP(u32_t U32val, u32_t Mult) {
	U32val += Mult - 1;
	return (U32val < Mult) ? Mult : (U32val - (U32val % Mult));
}

// ################################### 1/2/4 bit field array support ###############################

ba_t * pvBitArrayCreate(size_t Count, size_t Size) {
	if (Size != 1 || Size != 2 || Size != 4) return pvFAILURE;
	size_t szBA = Count * Size;							// size in total # of bits
	if (szBA & 0x00000007) return pvFAILURE;			// not on a byte boundary
	szBA >>= 3;											// size in bytes
	ba_t * psBA = pvRtosMalloc(sizeof(ba_t) + szBA);
	psBA->ByteSize	= szBA;
	psBA->Count		= Count;
	psBA->BitSize	= Size;
	psBA->Fields	= Size == 1	? 8		: Size == 2 ?	4		: 2;
	psBA->Mask		= Size == 1	? 0x01	: Size == 2 ?	0x03	: 0x0F;
	psBA->pvBA	= (u8_t *) psBA + sizeof(ba_t);
	memset(psBA->pvBA, 0, szBA);
	return psBA;
}

void xBitArrayDelete(ba_t * psBA) {
	IF_myASSERT(debugPARAM, halCONFIG_inSRAM(psBA));
	memset(psBA, 0, sizeof(ba_t) + psBA->ByteSize);
	vRtosFree(psBA);
}

int	xBitArraySet(ba_t * psBA, int baI, u8_t baV) {
	if (baI >= psBA->Count || baV > psBA->Mask)
		return erFAILURE;
	u8_t	Xidx = baI / psBA->Fields;
	u8_t	Sidx = baI % psBA->Fields;
	u8_t Mask = psBA->Mask << Sidx;
	psBA->pvBA[Xidx]	&= ~Mask;						// Remove THIS field bits
	psBA->pvBA[Xidx]	|= (baV << Sidx);				// add in new THIS field bits
	return erSUCCESS;
}

int	xBitArrayGet(ba_t * psBA, int baI) {
	if (baI >= psBA->Count)
		return erFAILURE;
	u8_t	Xidx = baI / psBA->Fields;
	u8_t	Sidx = baI % psBA->Fields;
	u8_t Mask = psBA->Mask << Sidx;
	u8_t u8Val = psBA->pvBA[Xidx] & (Mask << Sidx);	// strip out OTHER field bits
	return u8Val >> Sidx;								// return THIS field value in LSB(s)
}
