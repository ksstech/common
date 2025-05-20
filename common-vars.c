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

// ######################################## local constants ########################################

const char * const cvSizeName[4] = { "vs08", "vs16", "vs32", "vs64" };
const char * const cvFormName[4] = { "vfUXX", "vfIXX", "vfFXX", "vfSXX" };
const char * const cvIndexName[cvXXX] = { "U08","U16","U32","U64","I08","I16","I32","I64","F32","F64","SXX" };
const char cvFieldName[cvXXX][4] = { "U08","U16","U32","U64","I08","I16","I32","I64","F32","F64","SXX" };

// ############################# Form / Size / Index to String support ##############################

const char * pcIndex2String(cvi_e cvI) { return (cvI < cvXXX) ? cvFieldName[cvI] : "cv???"; }

const char * pcForm2String(vf_e cvF) { return (cvF <= vfSXX) ? cvFormName[cvF] : "vf???"; }

// ############################# Form / Size / Index to Format support #############################

const char * const cvFormat[cvXXX] = {
	"%hhu",	"%hu",	"%lu",	"%llu",
	"%hhd",	"%hd",	"%ld",	"%lld",
	"%f",	"%lf",	"%s"
};
const char * pccIndex2Format(cvi_e cvI) { return (cvI < cvXXX) ? cvFormat[cvI] : strINV_CVI; }

const char * pccFormSize2Format(vf_e cvF, vs_e cvS) { return pccIndex2Format(xFormSize2Index(cvF, cvS)); }

// return size correct fixed width format string
const char * const cvFormatFix[cvX32num][3] = {
	{ "%lu", "%ld", "%.f" },
	{ "%3lu", "%3ld", "%3.2f" },
	{ "%5lu", "%5ld", "%5.2f" },
	{ "%7lu", "%7ld", "%7.2f" }
};
const char * pccIndex2FormatFix(cvi_e cvI, int Opt) { return (cvI == cvSXX) ? "%s" : (cvI < cvSXX) ? cvFormatFix[Opt%3][xIndex2Form(cvI)] : strINV_CVI; }

// return fixed maximum width format string for x64 value
const char * const cvFormatFix64[cvX64num][3] = {
	{ "%llu", "%lld", "%.lf" },
	{ "%3llu", "%3lld", "%3.2lf" },
	{ "%5llu", "%5lld", "%5.2lf" },
	{ "%7llu", "%7lld", "%7.2lf" }
};
const char * pccIndex2FormatFix64(cvi_e cvI, int Opt) { return (cvI == cvSXX) ? "%s" : (cvI < cvSXX) ? cvFormatFix64[Opt%3][xIndex2Form(cvAdjustX64(cvI))] : strINV_CVI; }

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

// ####################################### Convert support #########################################

x32_t x32ValueConvert(px_t pxSrc, cvi_e cvIS, px_t pxDst, cvi_e cvID, u8_t Scale) {
	if (cvIS == cvID)									// nothing to be changed
		return *pxDst.px32 = *pxSrc.px32;				// copy from source to destination, return value as is
	vf_e vfSrc = xIndex2Form(cvIS);
	vf_e vfDst = xIndex2Form(cvID);
	x32_t X32 = x32ValueFetch(pxSrc, cvIS);
	// if source is FXX and destination is NOT FXX, might have to do some scaling to
	if (vfSrc == vfFXX && vfDst != vfFXX && Scale > 1) {
		X32.f32 *= Scale;
	} else {
		IF_myASSERT(debugPARAM, Scale < 2);
	}
	if (vfSrc != vfDst) {								// format conversion required...
		if (vfDst == vfUXX) {							// -> U32
			X32.u32 = (vfSrc == vfIXX) ? X32.i32 : X32.f32;
		} else if (vfDst == vfIXX) {					// -> I32
			X32.i32 = (vfSrc == vfUXX) ? X32.u32 : X32.f32;
		} else if (vfDst == vfFXX) {					// -> F32
			X32.f32 = (vfSrc == vfUXX) ? X32.u32 : X32.i32;
		} else {
			IF_myASSERT(debugTRACK, 0);
		}
	}
	vx32ValueStore(X32, pxDst, cvID);
	return X32;
}

x64_t x64ValueConvert(px_t pxSrc, cvi_e cvIS, px_t pxDst, cvi_e cvID) {
	if (cvIS == cvID)									// nothing to be changed
		return *pxDst.px64 = *pxSrc.px64;				// copy from source to destination, return value as is
	x64_t X64 = x64ValueFetch(pxSrc, cvIS);
	vf_e vfSrc = xIndex2Form(cvIS);
	vf_e vfDst = xIndex2Form(cvID);
	if (vfSrc != vfDst) {								// format conversion required...
		if (vfDst == vfUXX) {							// -> U64
			X64.u64 = (vfSrc == vfIXX) ? X64.i64 : X64.f64;
		} else if (vfDst == vfIXX) {					// -> I64
			X64.i64 = (vfSrc == vfUXX) ? X64.u64 : X64.f64;
		} else if (vfDst == vfFXX) {					// -> F64
			X64.f64 = (vfSrc == vfUXX) ? X64.u64 : X64.i64;
		} else {
			IF_myASSERT(debugTRACK, 0);
		}
	}
	vx64ValueStore(X64, pxDst, cvID);
	return X64;
}

// ############################### Address manipulation support ####################################

px_t pxAddrNextWithIndex(px_t pX, cvi_e cvI) { pX.pv += (1 << xIndex2Size(cvI)); return pX; }

px_t pxAddrCalcIndexedWithIndex(px_t pX, int xIdx, cvi_e cvI) { pX.pv += (1 << xIndex2Size(cvI) * xIdx); return pX; }
