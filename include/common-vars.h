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

// ################################# Form <> Size <> Index support #################################

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

// ############################### Address manipulation support ####################################

px_t pxAddrNextWithIndex(px_t pX, cvi_e cvI);

px_t pxAddrCalcIndexedWithIndex(px_t pX, int xIdx, cvi_e cvI);

#ifdef __cplusplus
}
#endif
