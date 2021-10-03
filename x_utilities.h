/*
 * Copyright 2014-21 Andre M. Maree / KSS Technologies (Pty) Ltd.
 */

#pragma once

#include	"struct_union.h"

#include	<stdbool.h>
#include	<stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ########################################### Macros ##############################################


// ########################################## Structures ###########################################

typedef struct ba_t {									// 1/2/4 bit field array
	uint8_t * pvBA ;
	size_t	ByteSize ;
	size_t	Count ;
	uint8_t	BitSize ;									// size (in bits) of each entry
	uint8_t	Fields ;
	uint8_t Mask ;
	uint8_t	Spare ;
} ba_t;

// #################################################################################################

void vShowActivity(int i) ;
void vUtilPrintCharacterSet(void) ;

uint64_t mac2int(uint8_t * hwaddr) ;
void int2mac(uint64_t mac, uint8_t * hwaddr) ;
void MemDump(uint8_t ** pMemAddr, int cChr, size_t Size) ;

void xGenerateUUID(char * pBuf) ;

void vBuildRandomSXX(uint8_t * pu8, int len, int set) ;
void vBuildRandomStr(uint8_t * pu8, int len, int set) ;

x8_t	xBuildRandomX8(void) ;
x16_t	xBuildRandomX16(void) ;
x32_t	xBuildRandomX32(void) ;
x64_t	xBuildRandomX64(void) ;

uint32_t u32pow(uint32_t base, uint32_t exp) ;
uint64_t u64pow(uint32_t base, uint32_t exp) ;

int u32Trailing0(uint32_t U32) ;
int u64Trailing0(uint64_t U64) ;

int	xDigitsInI32(int32_t I32, bool grouping) ;
int	xDigitsInU32(uint32_t U32, bool grouping) ;
int	xDigitsInU64(uint64_t U64, bool grouping) ;

int	xU32ToDecStr(uint32_t Value, char *) ;
uint32_t xU32Round(uint32_t u32V, uint32_t u32P);

// ################################### 1/2/4 bit field array support ###############################

ba_t * pvBitArrayCreate(size_t Count, size_t Size) ;
void xBitArrayDelete(ba_t * psBA) ;
int	xBitArraySet(ba_t * psBA, int baI, uint8_t baV) ;
int	xBitArrayGet(ba_t * psBA, int baI) ;

#ifdef __cplusplus
}
#endif
