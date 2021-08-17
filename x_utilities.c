/*
 * Copyright 2014-21 Andre M. Maree / KSS Technologies (Pty) Ltd.
 */

#include	"hal_config.h"
#include 	"printfx.h"
#include	"x_utilities.h"
#include	"x_terminal.h"
#include	"x_errors_events.h"

#include	<stdlib.h>
#include	<string.h>

#define	debugFLAG					0xC000

#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

// #################################################################################################

void vShowActivity(int i) {
	static char caActivity[] = { '0', '0', '0', '0', 0 } ;
	IF_myASSERT(debugPARAM, i < (sizeof(caActivity) - 1)) ;
	++caActivity[i] ;
	if (caActivity[i] == 0x3A) caActivity[i] = '0' ;
	printfx_lock();
	vANSIcursorsave();
	vANSIlocate(1, 120);
	vANSIputs(caActivity) ;
	vANSIcursorback();
	printfx_unlock();
}

void vUtilPrintCharacterSet(void) {
	uint8_t Buffer[256] ;
	for (int cChr = 0; cChr < sizeof(Buffer); cChr++) Buffer[cChr] = cChr;
	printfx("%!'+B", sizeof(Buffer), Buffer) ;
}

/**
 * Convert hex mac address to uint64_t
 * @param[in] hwaddr hex mac address
 * @return mac address as uint64_t
 */
uint64_t mac2int(uint8_t * hwaddr) {
	uint64_t iRV = 0 ;
	for (int8_t i = 5; i >= 0; --i) iRV |= (uint64_t) *hwaddr++ << (BITS_IN_BYTE * i);
	return iRV;
}

/**
 * Convert uint64_t mac address to hex
 * @param[in] mac uint64_t mac address
 * @param[out] hwaddr hex mac address
 */
void int2mac(uint64_t mac, uint8_t * hwaddr) {
	for (int8_t i = 5; i >= 0; --i) *hwaddr++ = mac >> (BITS_IN_BYTE * i);
}

void MemDump(uint8_t ** pMemAddr, int32_t cChr, size_t Size) {
	printfx("MemDump:\n%#'+B", Size, *pMemAddr) ;
	*pMemAddr = (cChr == '+') ? (*pMemAddr + Size) : (cChr == '-') ? (*pMemAddr - Size) : *pMemAddr ;
}

int	xDigitsInI32(int32_t I32, bool grouping) {
	int x ;
	if (I32 == INT32_MIN) {
		x = 10 + 1 ;
	} else {	// if value is negative, and '-' counts as a digit....
		if (I32 < 0) return xDigitsInI32(-I32, grouping) + 1 ;
		if (I32 >= 10000) {
			if (I32 >= 10000000) {
		        if (I32 >= 100000000) x = (I32 >= 1000000000) ? 10 : 9 ;
		        else x = 8 ;
			} else {
				if (I32 >= 100000) x = (I32 >= 1000000) ? 7 : 6 ;
				else x = 5 ;
			}
	    } else {
		    if (I32 >= 100) x = (I32 >= 1000) ? 4 : 3 ;
		    else x = (I32 >= 10) ? 2 : 1 ;
	    }
	}
	return x + ((x - 1) / 3) ;
}

int	xDigitsInU32(uint32_t U32, bool grouping) {
	int x ;
	if (U32 >= 10000) {
		if (U32 >= 10000000) {
	        if (U32 >= 100000000) x = (U32 >= 1000000000)? 10 : 9 ;
	        else x = 8 ;
		} else {
			if (U32 >= 100000) x = (U32 >= 1000000) ? 7 : 6 ;
			else x = 5 ;
		}
    } else {
        if (U32 >= 100) x = (U32 >= 1000) ? 4 : 3 ;
        else x = (U32 >= 10) ? 2 : 1 ;
    }
    return x + ((x - 1) / 3) ;
}

int	xDigitsInU64(uint64_t U64, bool grouping) {
	int x ;
	if (U64 <= UINT32_MAX) return xDigitsInU32((uint32_t) U64, grouping) ;
	if (U64 >= 100000000000000ULL) {
		if (U64 >= 100000000000000000ULL) {
			if (U64 >= 1000000000000000000ULL) x = (U64 >= 10000000000000000000ULL) ? 20  : 19 ;
			else x = 18 ;
		} else {
			if (U64 >= 1000000000000000ULL) x = (U64 >= 10000000000000000ULL) ? 17 : 16 ;
			else x = 15 ;
		}
	} else {
		if (U64 >= 1000000000000ULL) x = (U64 > 10000000000000ULL) ? 14 : 13 ;
		else x = (U64 >= 100000000000ULL) ? 12 : 11 ;
	}
	return x + ((x - 1) / 3) ;
}

// https://stackoverflow.com/questions/7399069/how-to-generate-a-guid-in-c#7399348
void xGenerateUUID(char * pBuf) {
// V4 template = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx" ;
	char szHex[] = "0123456789ABCDEF" ;
	srand(clock()) ;
	for (int t = 0; t < 36; ++t, ++pBuf) {
	    int r = rand() % 16;
	    if (t == 8 || t == 13 || t == 18 || t == 23) *pBuf = '-' ;
	    else if (t == 14) *pBuf = '4' ;
	    else if (t == 19) *pBuf = szHex[(r & 0x03) | 0x08] ;
	    else *pBuf = szHex[r] ;
	}
	IF_PRINT(debugRESULT, "%.36s\n", pBuf) ;
}

const char charset[] = {
	'0','1','2','3','4','5','6','7','8','9',
	'A','B','C','D','E','F','G','H','I','J','K','L','M',
	'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'a','b','c','d','e','f','g','h','i','j','k','l','m',
	'n','o','p','q','r','s','t','u','v','w','x','y','z',
} ;

void vBuildRandomSXX(uint8_t * pu8, int len) {
	if (len && pu8) for (int n = 0; n < len; pu8[n++] = charset[rand() % sizeof(charset)]) ;
}

void vBuildRandomStr(uint8_t * pu8, int32_t len) {
	if (len && pu8) {
		vBuildRandomSXX(pu8, --len) ;
		pu8[len] = CHR_NUL ;
	}
}

x8_t xBuildRandomX8(void) {
	x8_t X8 ;
	X8.u8 = rand() & 0xFF ;
	return X8 ;
}

x16_t xBuildRandomX16(void) {
	x16_t X16 ;
	X16.u16 = rand() & 0xFFFF ;
	return X16 ;
}

x32_t xBuildRandomX32(void) {
	x32_t	X32 ;
	X32.i32 = rand() ;
	if (rand() % 2) X32.i32 *= -1 ;						// ensure some (-) values
	return X32 ;
}

x64_t xBuildRandomX64(void) {
	x64_t X64 ;
	X64.x32[0] = xBuildRandomX32() ;
	X64.x32[1] = xBuildRandomX32() ;
	return X64 ;
}

uint32_t u32pow(uint32_t base, uint32_t exp) {
	uint32_t res ;
	for(res = 1; exp > 0; res *= base, --exp) ;
	return res ;
}

uint64_t u64pow(uint32_t base, uint32_t exp) {
	uint64_t res ;
	for(res = 1; exp > 0; res *= base, --exp) ;
	return res ;
}

int u32Trailing0(uint32_t U32) {
	int iRV = 0 ;
	while (U32 > 0) {
		if (U32 % 10) break;
		++iRV ;
		U32 /= 10 ;
	}
	return iRV ;
}

int u64Trailing0(uint64_t U64) {
	int iRV = 0 ;
	while (U64 > 0) {
		if (U64 % 10) break ;
		++iRV ;
		U64 /= 10 ;
	}
	return iRV ;
}

/* ################################ Decimal number conversions ################################## */

int	xU32ToDecStr(uint32_t Value, char * pBuf) {
	int	Len = 0 ;
	if (Value) {
		uint32_t	iTemp, Div = 1000000000UL ;
		do {
			iTemp = Value / Div ;
			if (iTemp != 0 || Len > 0) {
				*pBuf++	= iTemp + '0' ;
				Value 	-= iTemp * Div ;
				++Len ;
			}
			Div /= 10 ;
		} while (Div) ;
	} else {
		*pBuf++ = '0' ;									// answer is single '0'
		++Len ;
	}
	*pBuf	= 0 ;										// terminate
	return Len ;
}

// ################################### 1/2/4 bit field array support ###############################

ba_t *pvBitArrayCreate(size_t Count, size_t Size) {
	if (Size != 1 || Size != 2 || Size != 4) return pvFAILURE;
	size_t szBA = Count * Size ;						// size in total # of bits
	if (szBA & 0x00000007) return pvFAILURE;			// not on a byte boundary
	szBA >>= 3 ;										// size in bytes
	ba_t * psBA = malloc(sizeof(ba_t) + szBA) ;
	psBA->ByteSize	= szBA ;
	psBA->Count		= Count ;
	psBA->BitSize	= Size ;
	psBA->Fields	= Size == 1	? 8		: Size == 2 ?	4		: 2 ;
	psBA->Mask		= Size == 1	? 0x01	: Size == 2 ?	0x03	: 0x0F ;
	psBA->pvBA	= (uint8_t *) psBA + sizeof(ba_t) ;
	memset(psBA->pvBA, 0, szBA) ;
	return psBA ;
}

void xBitArrayDelete(ba_t * psBA) {
	IF_myASSERT(debugPARAM, halCONFIG_inSRAM(psBA)) ;
	memset(psBA, 0, sizeof(ba_t) + psBA->ByteSize) ;
	free(psBA) ;
}

int	xBitArraySet(ba_t * psBA, int32_t baI, uint8_t baV) {
	if (baI >= psBA->Count || baV > psBA->Mask) return erFAILURE;
	uint8_t	Xidx = baI / psBA->Fields ;
	uint8_t	Sidx = baI % psBA->Fields ;
	uint8_t Mask = psBA->Mask << Sidx ;
	psBA->pvBA[Xidx]	&= ~Mask ;						// Remove THIS field bits
	psBA->pvBA[Xidx]	|= (baV << Sidx) ;				// add in new THIS field bits
	return erSUCCESS ;
}

int	xBitArrayGet(ba_t * psBA, int32_t baI) {
	if (baI >= psBA->Count) return erFAILURE;
	uint8_t	Xidx = baI / psBA->Fields ;
	uint8_t	Sidx = baI % psBA->Fields ;
	uint8_t Mask = psBA->Mask << Sidx ;
	uint8_t u8Val = psBA->pvBA[Xidx] & (Mask << Sidx) ;	// strip out OTHER field bits
	return u8Val >> Sidx ;								// return THIS field value in LSB(s)
}
