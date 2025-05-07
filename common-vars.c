// common-vars.c - Copyright (c) 2025 André M. Maree / KSS Technologies (Pty) Ltd.

#include "hal_platform.h"
#include "common-vars.h"

#include "errors_events.h"

// ############################################### Macros ##########################################

#define	debugFLAG					0xF000
#define	debugTIMING					(debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG & 0x8000)

// ################################# Form <> Size <> Index support #################################

const u8_t cvIndexSize[] = { vs08B, vs16B, vs32B, vs64B, vs08B, vs16B, vs32B, vs64B, vs32B, vs64B };
vs_e xIndex2Size(cvi_e cvI) {
	IF_myASSERT(debugTRACK, cvI < cvSXX);
	return cvIndexSize[cvI];
}

u32_t xIndex2Bytes(cvi_e cvI) { return (1UL << xIndex2Size(cvI)); }

vf_e xIndex2Form(cvi_e cvI) {
	IF_myASSERT(debugTRACK, cvI < cvXXX);
	return (cvI > cvF64) ? vfSXX : (cvI > cvI64) ? vfFXX : (cvI > cvU64) ? vfIXX : vfUXX;
}

cvi_e xFormSize2Index(vf_e cvF, vs_e cvS) {
	IF_myASSERT(debugPARAM, cvF < vfSXX);
	IF_myASSERT(debugPARAM && cvF == vfFXX, cvS > vs16B);
	return (cvF * 4) + cvS - (cvF == vfFXX ? 2 : 0);
}

int	xFormField2Index(vf_e cvF, u8_t fS) {
	if ((cvF == vfSXX && fS == 0) ||					// SXX size must be > 0
		(cvF == vfFXX && (fS != 4 || fS != 8)) ||		// FXX size must be 4/8
		(cvF < vfFXX && (fS != 1 && fS != 2 && fS != 4 && fS != 8))) {	// U/IXX size must be 1/2/4/8
		return erFAILURE;
	}
	if (cvF == vfSXX)								return cvSXX;
	if (cvF == vfFXX)								return (fS == 4) ? cvF32 : cvF64;
	fS -= (fS == 8) ? 2 : 0;							// 1-2-4-8 -> 1-2-4-6
	fS >>= 1;											// 1-2-4-6 -> 0-1-2-3
	return fS + ((cvF == vfUXX) ? 0 : 4);				// 0-1-2-3 or 4-5-6-7
}

const u8_t cvIndexWidth[cvXXX]	= {	4,	6,	12,	21,	4,	6,	12,	21,	18,	21,	255 };
u8_t xIndex2Width(cvi_e cvI) {
	IF_myASSERT(debugTRACK, cvI < cvXXX);
	return cvIndexWidth[cvI];
}

const u8_t cvIndexSizeOf[cvXXX]	= {	1,	2,	4,	8,	1,	2,	4,	8,	4,	8,	255 };
u8_t xIndex2Field(cvi_e cvI) {
	IF_myASSERT(debugTRACK, cvI < cvXXX);
	return cvIndexSizeOf[cvI];
}

u8_t xIndexField2Field(cvi_e cvI, u8_t fS) {
	IF_myASSERT(debugTRACK, cvI < cvXXX);
	return (cvI < cvSXX) ? xIndex2Field(cvI) : fS;
}

bool xFormSizeVerify(vf_e cvF, vs_e cvS) { return ((cvF == vfFXX) && (cvS == vs08B || cvS == vs16B)) ? 0 : 1; }

cvi_e cvAdjustX64(cvi_e cvI) { return (cvI <= cvU64) ? cvU64 : (cvI <= cvI64) ? cvI64 : (cvI <= cvF64) ? cvF64 : cvINV; }

bool bCVI_CheckSameForm(cvi_e cvI1, cvi_e cvI2) { return xIndex2Form(cvI1) == xIndex2Form(cvI2) ? 1 : 0; }

bool bCVI_CheckSameSize(cvi_e cvI1, cvi_e cvI2) {return xIndex2Size(cvI1) == xIndex2Size(cvI2) ? 1 : 0; }

// ###################################### Fetch / Store support ####################################

x32_t x32ValueFetch(px_t px, cvi_e cvI) {
	x32_t X32 = { 0 };
	switch(cvI) {
	case cvU08: X32.u32 = *px.pu8; break;
	case cvU16: X32.u32 = *px.pu16; break;
	case cvU32: X32.u32 = *px.pu32; break;

	case cvI08: X32.i32 = *px.pi8; break;
	case cvI16: X32.i32 = *px.pi16; break;
	case cvI32: X32.i32 = *px.pi32; break;

	case cvF32: X32.f32 = *px.pf32; break;
	default: IF_myASSERT(debugTRACK,0); break;
	}
	return X32;
}

x64_t x64ValueFetch(px_t px, cvi_e cvI) {
	x64_t X64 = { 0 };
	switch(cvI) {
	case cvU08: X64.u64 = *px.pu8; break;
	case cvU16: X64.u64 = *px.pu16; break;
	case cvU32: X64.u64 = *px.pu32; break;
	case cvU64: X64.u64 = *px.pu64; break;

	case cvI08: X64.i64 = *px.pi8; break;
	case cvI16: X64.i64 = *px.pi16; break;
	case cvI32: X64.i64 = *px.pi32; break;
	case cvI64: X64.i64 = *px.pi64; break;

	case cvF32: X64.f64 = *px.pf32; break;
	case cvF64: X64.f64 = *px.pf64; break;
	default: IF_myASSERT(debugTRACK,0); break;
	}
	return X64;
}

void vx32ValueStore(x32_t X32, px_t px, cvi_e cvI) {
	switch(cvI) {
	case cvU08: *px.pu8	= (u8_t) X32.u32; break;
	case cvU16: *px.pu16 = (u16_t) X32.u32; break;
	case cvU32: *px.pu32 = X32.u32; break;

	case cvI08: *px.pi8	= (s8_t) X32.i32; break;
	case cvI16: *px.pi16 = (i16_t) X32.i32; break;
	case cvI32: *px.pi32 = X32.i32; break;

	case cvF32: *px.pf32 = X32.f32; break;
	default: assert(0);
	}
}

void vx64ValueStore(x64_t X64, px_t px, cvi_e cvI) {
	switch(cvI) {
	case cvU08: *px.pu8	= X64.u64; break;
	case cvU16: *px.pu16 = X64.u64; break;
	case cvU32: *px.pu32 = X64.u64; break;
	case cvU64: *px.pu64 = X64.u64; break;

	case cvI08: *px.pi8	= X64.i64; break;
	case cvI16: *px.pi16 = X64.i64; break;
	case cvI32: *px.pi32 = X64.i64; break;
	case cvI64: *px.pi64 = X64.i64; break;

	case cvF32: *px.pf32 = X64.f64; break;
	case cvF64: *px.pf64 = X64.f64; break;
	default: assert(0);
	}
}

// ############################### Address manipulation support ####################################

px_t pxAddrNextWithIndex(px_t pX, cvi_e cvI) { pX.pv += (1 << xIndex2Size(cvI)); return pX; }

px_t pxAddrCalcIndexedWithIndex(px_t pX, int xIdx, cvi_e cvI) { pX.pv += (1 << xIndex2Size(cvI) * xIdx); return pX; }
