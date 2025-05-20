// common-vars.h

#pragma	once

#include "struct_union.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ########################################## Macros ###############################################

// ####################################### Enumerations ############################################

/*		Typ	8	16	32	64
Siz			0	1	2	3
Uxx		0	X	X	X	X
Ixx		1	X	X	X	X
Fxx		2			X	X
Sxx		3	X
*/

typedef enum  __attribute__((packed)) cvi_e { cvU08,cvU16,cvU32,cvU64,cvI08,cvI16,cvI32,cvI64,cvF32,cvF64,cvSXX,cvXXX,cvINV=15 }cvi_e;

typedef enum   __attribute__((packed)) vs_e { vs08B, vs16B, vs32B, vs64B } vs_e;

typedef enum   __attribute__((packed)) vf_e { vfUXX, vfIXX, vfFXX, vfSXX } vf_e;

// ####################################### Structures ##############################################

// ######################################### Global variables ######################################

extern const char * const cvSizeName[];
extern const char * const cvFormName[];
extern const char * const cvIndexName[];

// ############################# Form / Size / Index to String support ##############################

const char * pcIndex2String(cvi_e cvI);
const char * pcForm2String(vf_e cvF);

// ############################# Form / Size / Index to Format support #############################

const char * pccIndex2Format(cvi_e cvI);
const char * pccFormSize2Format(vf_e cvF, vs_e cvS);

enum { cvX32free, cvX32fix3, cvX32fix5, cvX32fix7, cvX32num };
const char * pccIndex2FormatFix(cvi_e cvI, int Opt);

enum { cvX64free, cvX64fix3, cvX64fix5, cvX64fix7, cvX64num };
const char * pccIndex2FormatFix64(cvi_e cvI, int Opt);

// ################################# Form <> Size <> Index support #################################

/**
 * @brief	
 * @param[in]	
 * @return			
 */
vs_e xIndex2Size(cvi_e cvI);
u32_t xIndex2Bytes(cvi_e cvI);
vf_e xIndex2Form(cvi_e cvI);
cvi_e xFormSize2Index(vf_e cvF, vs_e cvS);
int	xFormField2Index(vf_e cvF, u8_t fS);

u8_t xIndex2Width(cvi_e cvI);
u8_t xIndex2Field(cvi_e cvI);
u8_t xIndexField2Field(cvi_e cvI, u8_t fS);

// ################################# Form <> Size <> Index support #################################

/**
 * @brief		Check form and size specifiers to be a valid combination 
 * @param[in]	cvF - form specifier
 * @param[in]	cvS - size specifier
 * @return		0/false (vfFXX + vs08B/vs16B), 1/true for all other combinations
 */
bool xFormSizeVerify(vf_e cvF, vs_e cvS);

cvi_e cvAdjustX64(cvi_e cvI);

bool bCVI_CheckSameForm(cvi_e cvI1, cvi_e cvI2);
bool bCVI_CheckSameSize(cvi_e cvI1, cvi_e cvI2);

// ##################################### Fetch / Store support #####################################

/**
 * @brief	Read [and upscale 8/16 bit] variable to 32 bit using cvI type specifier
 * @param	px - source address
 * @param	cvI - source format
 * @return	32 bit value in requested format
 */
x32_t x32ValueFetch(px_t px, cvi_e cvI);

/**
 * @brief	Read [and upscale 8/16/32 bit] variable to 64 bit using cvI type specifier
 * @param	px - pointer to source variable
 * @param	cvI - type specifier
 * @return	64 bit value in requested format
 */
x64_t x64ValueFetch(px_t px, cvi_e cvI);

void vx32ValueStore(x32_t X32, px_t px, cvi_e cvI);

void vx64ValueStore(x64_t x64Val, px_t px, cvi_e cvI);

/**
 * @brief	Convert (up/same/down size) source value/format to x32_t destination value/format
 * @param	pxSrc - source address
 * @param	cvIS - source value format
 * @param	pxDst - destination address
 * @param	cviD - destination value format
 * @param	Scale - optional scale factor for FXX -> IXX/UXX conversion, preserve some fraction
 * @return	32 bit value in requested format
 */
x32_t x32ValueConvert(px_t pxSrc, cvi_e cvIS, px_t pxDst, cvi_e cvID, u8_t Scale);

/**
 * @brief	Convert (up/same/down size) source value/format to x64_t destination value/format
 * @param	pxSrc - source address
 * @param	cvIS - source value format
 * @param	pxDst - destination address
 * @param	cviD - destination value format
 * @return	64 bit value in requested format
 */
x64_t x64ValueConvert(px_t pxSrc, cvi_e cvIS, px_t pxDst, cvi_e cvID);

// ############################### Address manipulation support ####################################

px_t pxAddrNextWithIndex(px_t pX, cvi_e cvI);

px_t pxAddrCalcIndexedWithIndex(px_t pX, int xIdx, cvi_e cvI);

#ifdef __cplusplus
}
#endif
