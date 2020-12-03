/*
 * Copyright 2014-18 Andre M Maree / KSS Technologies (Pty) Ltd.
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
 * x_utilties.h
 */

#pragma once

#include	"stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

// ########################################### Macros ##############################################


// #################################################################################################

void	vUtilPrintCharacterSet(void) ;

uint64_t mac2int(uint8_t * hwaddr) ;
void	int2mac(uint64_t mac, uint8_t * hwaddr) ;
void	MemDump(uint8_t ** pMemAddr, int32_t cChr, size_t Size) ;

int32_t	xDigitsInI32(int32_t I32, bool grouping) ;
int32_t	xDigitsInU32(uint32_t U32, bool grouping) ;
int32_t	xDigitsInU64(uint64_t U64, bool grouping) ;

void	xGenerateUUID(char * pBuf) ;
uint32_t u32pow(uint32_t base, uint32_t exp) ;

#ifdef __cplusplus
}
#endif
