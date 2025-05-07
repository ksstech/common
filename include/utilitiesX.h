// utilitiesX.h - Copyright (c) 2014-25 Andre M. Maree / KSS Technologies (Pty) Ltd.

#pragma once

#include "struct_union.h"
#include "report.h"

#ifdef __cplusplus
extern "C" {
#endif

// ########################################### Macros ##############################################

// ########################################## Structures ###########################################

typedef struct ba_t {									// 1/2/4 bit field array
	u8_t * pvBA;
	size_t ByteSize;
	size_t Count;
	u8_t BitSize;									// size (in bits) of each entry
	u8_t Fields;
	u8_t Mask;
	u8_t Spare;
} ba_t;

// #################################################################################################

/**
 * @brief		Find a matching value in formatted charater buffer
 * @param[in]	Value - the code/value to try and match
 * @param[in]	pcBuf - buffer to search in for the value
 * @param[in]	Size - size of buffer in bytes
 * @return		pointer the the NULL terminated stricg immediately following the matching value
 * @note		const char Buffer[] = { "\001String1\0" "\002String2\0" "\023String19\0" }
 */
const char * pcFindValueMatch(u8_t Value, const char * pcBuf, size_t Size);

void vShowActivity(int i);

void vUtilPrintCharacterSet(void);

u64_t mac2int(u8_t * hwaddr);
void int2mac(u64_t mac, u8_t * hwaddr);
void MemDump(u8_t ** pMemAddr, int cChr, size_t Size);

void xGenerateUUID(char * pBuf);

void vBuildRandomSXX(u8_t * pu8, int len, int set);
void vBuildRandomStr(u8_t * pu8, int len, int set);

x8_t xBuildRandomX8(void);
x16_t xBuildRandomX16(void);
x32_t xBuildRandomX32(void);
x64_t xBuildRandomX64(void);

u32_t u32pow(u32_t base, u32_t exp);
u64_t u64pow(u32_t base, u32_t exp);

/**
 * @brief	Calculate number of trailing 0's in U32 value
 * @param	u32V value to evaluate
 * @return	number of trailing 0's
 */
int u32Trailing0(u32_t u32V);

/**
 * @brief	Calculate number of trailing 0's in U64 value
 * @param	U64val value to evaluate
 * @return	number of trailing 0's
 */
int u64Trailing0(u64_t U64val);

/**
 * @brief	Calculate number of digits (including sign) in I32 value
 * @param	i32V - value of which length (number of digits) is to be calculated
 * @param	grouping - 1 if number of digits calculated should include SI separators
 * @return	number of digits that value (if  printed) would consist of
 */
int	xDigitsInI32(i32_t i32V, bool grouping);

/**
 * @brief	Calculate number of digits in U32 value
 * @param	u32V - value of which length (number of digits) is to be calculated
 * @param	grouping - 1 if number of digits calculated should include SI separators
 * @return	number of digits that value (if  printed) would consist of
 */
int	xDigitsInU32(u32_t u32V, bool grouping);

/**
 * @brief	Calculate number of digits in U64 value
 * @param	U64val - value of which length (number of digits) is to be calculated
 * @param	grouping - 1 if number of digits calculated should include SI separators
 * @return	number of digits that value (if  printed) would consist of
 */
int	xDigitsInU64(u64_t U64val, bool grouping);

int	xU32ToDecStr(u32_t Value, char *);

/**
 * @brief	Round VAL up or down to closest multiple of MULT
 * @param[in]	u32V value to be rounded down/up
 * @param[in]	u32M roundup multiple
 * @return	value after rounded up and adjusted
 */
u32_t u32Round(u32_t u32V, u32_t u32M);

/**
 * @brief	Round VAL up to closest multiple of MULT
 * @param[in]	u32V value to be rounded up
 * @param[in]	u32M roundup multiple
 * @return	value after rounded up and adjusted
 */
u32_t u32RoundUP(u32_t u32V, u32_t u32M);

/**
 * @brief	Increment the value and return the modulus/wrapped result
 * @param	x32V - value to be incremented
 * @param	x32L - limit to be used for wrapping
 * @return	modulus of the incremented value
 */
inline __attribute__((always_inline)) u32_t u32IncrementWrap(u32_t u32V, u32_t u32L) { ++u32V; return u32V % u32L; }
inline __attribute__((always_inline)) i32_t i32IncrementWrap(i32_t i32V, i32_t i32L) { ++i32V; return i32V % i32L; }

// ################################### 1/2/4 bit field array support ###############################

ba_t * pvBitArrayCreate(size_t Count, size_t Size);
void xBitArrayDelete(ba_t * psBA);
int	xBitArraySet(ba_t * psBA, int baI, u8_t baV);
int	xBitArrayGet(ba_t * psBA, int baI);

// #################################### audit buffer support #######################################

/**
 * @brief	allocate SL_MAX_LEN_MESSAGE buffer and initialise report_t structure with related values
 * @return  pointer to the allocated * initialized buffer
 */
report_t * psAuditOpen(void);

/**
 * @brief	if used, log buffer content to console and/or host then free allocated memory
 * @param[in]	psR - pointer to report structure
 * @param[in]	flag - 1=anything there, log it
 * @return		NULL
 */
void vAuditClose(report_t * psR, bool flag);

#ifdef __cplusplus
}
#endif
