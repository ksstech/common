/*
 * Copyright 2014-20 Andre M Maree / KSS Technologies (Pty) Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/*
 * x_utilities.c
 */

#include 	"printfx.h"

#if		defined(ESP_PLATFORM)
	#include	"hal_config.h"
#else
	#include	"alt_config.h"
#endif

#include	<limits.h>
#include	<stdint.h>

#define	debugFLAG					0xC000

#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

// #################################################################################################

void	vUtilPrintCharacterSet(void) {
	uint8_t Buffer[256] ;
	for (int32_t cChr = 0; cChr < sizeof(Buffer); cChr++) {
		Buffer[cChr] = cChr ;
	}
	PRINT("%!'+b", sizeof(Buffer), Buffer) ;
}

/**
 * Convert hex mac address to uint64_t
 * @param[in] hwaddr hex mac address
 * @return mac address as uint64_t
 */
uint64_t mac2int(uint8_t * hwaddr) {
	uint64_t iRV = 0 ;
	for (int8_t i = 5; i >= 0; --i) {
		iRV |= (uint64_t) *hwaddr++ << (CHAR_BIT * i);
	}
	return iRV;
}

/**
 * Convert uint64_t mac address to hex
 * @param[in] mac uint64_t mac address
 * @param[out] hwaddr hex mac address
 */
void int2mac(uint64_t mac, uint8_t * hwaddr) {
	for (int8_t i = 5; i >= 0; --i) {
		*hwaddr++ = mac >> (CHAR_BIT * i);
	}
}

void	MemDump(uint8_t ** pMemAddr, int32_t cChr, size_t Size) {
	PRINT("MemDump:\n%#'+b", Size, *pMemAddr) ;
	*pMemAddr = (cChr == CHR_PLUS) ? (*pMemAddr + Size) : (cChr == CHR_MINUS) ? (*pMemAddr - Size) : *pMemAddr ;
}

int32_t	xDigitsInI32(int32_t I32, bool grouping) {
	int x ;
	if (I32 == INT32_MIN) {
		x = 10 + 1 ;
	} else {
		// if value is negative, and '-' counts as a digit....
		if (I32 < 0)
			return xDigitsInI32(-I32, grouping) + 1 ;
		if (I32 >= 10000) {
			if (I32 >= 10000000) {
		        if (I32 >= 100000000) {
	                x = (I32 >= 1000000000)	? 10 : 9 ;
	            }
		        x = 8 ;
			}
			if (I32 >= 100000) {
				x = (I32 >= 1000000) ? 7 : 6 ;
			}
	        x = 5 ;
	    }
	    if (I32 >= 100) {
	        x = (I32 >= 1000) ? 4 : 3 ;
	    }
	    x = (I32 >= 10) ? 2 : 1 ;
	}
	return x + ((x - 1) / 3) ;
}

int32_t	xDigitsInU32(uint32_t U32, bool grouping) {
#if 1
	int32_t x ;
	if (U32 >= 10000) {
		if (U32 >= 10000000) {
	        if (U32 >= 100000000) {
                x = (U32 >= 1000000000)? 10 : 9 ;
            }
	        x = 8 ;
		}
		if (U32 >= 100000) {
			x = (U32 >= 1000000) ? 7 : 6 ;
		}
        x = 5 ;
    }
    if (U32 >= 100) {
        x = (U32 >= 1000) ? 4 : 3 ;
    }
    x = U32 >= 10 ? 2 : 1 ;
    return x + ((x - 1) / 3) ;

#else	// Iterative
	int32_t x = 0 ;
	while (U32 != 0) {
		U32 /= 10 ;
		++x ;
	}
	return x + ((x - 1) / 3) ;
#endif
}

int32_t	xDigitsInU64(uint64_t U64, bool grouping) {
#if 1
	int x ;
	if (U64 <= UINT32_MAX)
		return xDigitsInU32((uint32_t) U64, grouping) ;
	if (U64 >= 100000000000000ULL) {
		if (U64 >= 100000000000000000ULL) {
			if (U64 >= 1000000000000000000ULL) {
				x = (U64 >= 10000000000000000000ULL) ? 20  : 19 ;
			}
			x = 18 ;
		}
		if (U64 >= 1000000000000000ULL) {
			x = (U64 >= 10000000000000000ULL) ? 17 : 16 ;
		}
		x = 15 ;
	}
	if (U64 >= 1000000000000ULL) {
		x = (U64 > 10000000000000ULL) ? 14 : 13 ;
	}
	x = (U64 >= 100000000000ULL) ? 12 : 11 ;
	return x + ((x - 1) / 3) ;
#else	// Iterative
	int32_t x = 0 ;
	while (U64 != 0) {
		U64 /= 10 ;
		++x ;
	}
    return x + ((x - 1) / 3) ;
#endif
}

// https://stackoverflow.com/questions/7399069/how-to-generate-a-guid-in-c#7399348
void	xGenerateUUID(char * pBuf) {
// V4 template = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx" ;
	char szHex[] = "0123456789ABCDEF" ;
	srand(clock()) ;
	for (int t = 0; t < 36; ++t, ++pBuf) {
	    int r = rand() % 16;
	    if (t == 8 || t == 13 || t == 18 || t == 23)
	    	*pBuf = CHR_MINUS ;
	    else if (t == 14)
	    	*pBuf = CHR_4 ;
	    else if (t == 19)
	    	*pBuf = szHex[(r & 0x03) | 0x08] ;
	    else
	    	*pBuf = szHex[r] ;
	}
	IF_PRINT(debugRESULT, "%.36s\n", pBuf) ;
}

const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" ;

void	vBuildRandomBuffer(uint8_t * pu8, int32_t len) {
	if (len && pu8) {
		for (int n = 0; n < len; ++n) {
			int key = rand() % sizeof(charset) ;          // per-iteration instantiation
			pu8[n] = charset[key] ;
	    }
	}
}

void	vBuildRandomString(uint8_t * pu8, int32_t len) {
	if (len && pu8) {
		vBuildRandomBuffer(pu8, --len) ;
		pu8[len] = CHR_NUL ;
	}
}

x8_t	xBuildRandomX8(void) {
	int8_t I8 = rand() >> 23 ;
	return (x8_t) I8 ;
}

x16_t	xBuildRandomX16(void) {
	int16_t I16 = rand() >> 7 ;
	return (x16_t) I16 ;
}

x32_t	xBuildRandomX32(void) {
	x32_t	X32 ;
	X32.i32 = rand() ;
	if (rand() %  2)
		X32.i32 *= -1 ;
	return X32 ;
}

x64_t	xBuildRandomX64(void) {
	x64_t	X64 ;
	X64.x32[0] = xBuildRandomX32() ;
	X64.x32[1] = xBuildRandomX32() ;
	return X64 ;
}

uint32_t u32pow(uint32_t base, uint32_t exp) {
	uint32_t res ;
	for(res = 1; exp > 0; res *= base, --exp) ;
	return res ;
}

