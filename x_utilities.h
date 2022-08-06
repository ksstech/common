/*
 * Copyright (c) 2014-22 Andre M. Maree / KSS Technologies (Pty) Ltd.
 */

#pragma once

#include "struct_union.h"

#ifdef __cplusplus
extern "C" {
#endif

// ########################################### Macros ##############################################


// ########################################## Structures ###########################################

typedef struct ba_t {									// 1/2/4 bit field array
	u8_t * pvBA ;
	size_t ByteSize ;
	size_t Count ;
	u8_t BitSize ;									// size (in bits) of each entry
	u8_t Fields ;
	u8_t Mask ;
	u8_t Spare ;
} ba_t;

// #################################################################################################

void vShowActivity(int i) ;
void vUtilPrintCharacterSet(void) ;

u64_t mac2int(u8_t * hwaddr) ;
void int2mac(u64_t mac, u8_t * hwaddr) ;
void MemDump(u8_t ** pMemAddr, int cChr, size_t Size) ;

void xGenerateUUID(char * pBuf) ;

void vBuildRandomSXX(u8_t * pu8, int len, int set) ;
void vBuildRandomStr(u8_t * pu8, int len, int set) ;

x8_t	xBuildRandomX8(void) ;
x16_t	xBuildRandomX16(void) ;
x32_t	xBuildRandomX32(void) ;
x64_t	xBuildRandomX64(void) ;

u32_t u32pow(u32_t base, u32_t exp) ;
u64_t u64pow(u32_t base, u32_t exp) ;

int u32Trailing0(u32_t U32) ;
int u64Trailing0(u64_t U64) ;

int	xDigitsInI32(s32_t I32, bool grouping) ;
int	xDigitsInU32(u32_t U32, bool grouping) ;
int	xDigitsInU64(u64_t U64, bool grouping) ;

int	xU32ToDecStr(u32_t Value, char *);

u32_t u32Round(u32_t u32V, u32_t u32P);
u32_t u32RoundUP(u32_t u32V, u32_t u32P);

// ################################### 1/2/4 bit field array support ###############################

ba_t * pvBitArrayCreate(size_t Count, size_t Size) ;
void xBitArrayDelete(ba_t * psBA) ;
int	xBitArraySet(ba_t * psBA, int baI, u8_t baV) ;
int	xBitArrayGet(ba_t * psBA, int baI) ;

#ifdef __cplusplus
}
#endif
