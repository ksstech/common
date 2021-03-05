/*
 * Copyright 2014-21 Andre M. Maree / KSS Technologies (Pty) Ltd.
 */

/*
 * x_utilties.h
 */

#pragma once

#include	"x_struct_union.h"

#include	<stddef.h>
#include	<stdint.h>
#include	<stdbool.h>

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

void	vUtilPrintCharacterSet(void) ;

uint64_t mac2int(uint8_t * hwaddr) ;
void	int2mac(uint64_t mac, uint8_t * hwaddr) ;
void	MemDump(uint8_t ** pMemAddr, int32_t cChr, size_t Size) ;

int32_t	xDigitsInI32(int32_t I32, bool grouping) ;
int32_t	xDigitsInU32(uint32_t U32, bool grouping) ;
int32_t	xDigitsInU64(uint64_t U64, bool grouping) ;

void	xGenerateUUID(char * pBuf) ;

void	vBuildRandomSXX(uint8_t * pu8, int32_t len) ;
void	vBuildRandomStr(uint8_t * pu8, int32_t len) ;

x8_t	xBuildRandomX8(void) ;
x16_t	xBuildRandomX16(void) ;
x32_t	xBuildRandomX32(void) ;
x64_t	xBuildRandomX64(void) ;

uint32_t u32pow(uint32_t base, uint32_t exp) ;

// ################################### 1/2/4 bit field array support ###############################

ba_t *	pvBitArrayCreate(size_t Count, size_t Size) ;
void	xBitArrayDelete(ba_t * psBA) ;
int32_t	xBitArraySet(ba_t * psBA, int32_t baI, uint8_t baV) ;
int32_t	xBitArrayGet(ba_t * psBA, int32_t baI) ;

#ifdef __cplusplus
}
#endif
