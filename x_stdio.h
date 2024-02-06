/*
 * x_stdio.h - Copyright (c) 2014-24 Andre M Maree / KSS Technologies (Pty) Ltd.
 */

#pragma once

#if	defined( __GNUC__ )
// To override the default GNU file IO definitions we MUST include this file
//	#define		__CUSTOM_FILE_IO__
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ###################################### BUILD : CONFIG definitions ##############################


// ##################################### MACRO definitions #########################################

#if defined( __CC_ARM )
	#error "CC ARM: Undefined toolset !!!"

#elif defined( __GNUC__ )
// macros to manipulate the file flags
	#define		FF_STCHK(stream, flag)		((stream->_flags) & (flag))
	#define		FF_UNSET(stream, flag)		((stream->_flags) &= ~(flag))
	#define		FF_SET(stream, flag)		((stream->_flags) |= (flag))
// toolset standard file flag mapping
	#define 	FF_LINBUF					__SLBF					// 0x0001
	#define		FF_NONBUF					__SNBF					// 0x0002
	#define		FF_MODER					__SRD					// 0x0004
	#define		FF_MODEW					__SWR					// 0x0008
	#define		FF_MODERW					__SRW					// 0x0010
	#define		FF_STATEOF					__SEOF					// 0x0020 found EOF
	#define		FF_STATERR					__SERR					// 0x0040 found error
	#define		FF_BUFFALOC					__SMBF					// 0x0080 buffer is from pvRtosMalloc
	#define		FF_MODEA					__SAPP					// 0x0100
	#define		FF_FILBUF					__SSTR					// 0x0200 this is an sprintf/snprintf string
//	#define		FF_							__SOPT					// 0x0400 do fseek() optimisation
//	#define		FF_							__SNPT					// 0x0800 do not do fseek() optimisation
//	#define		FF_							__SOFF					// 0x1000 set iff _offset is in fact correct
//	#define		FF_							__SORD					// 0x2000 true => stream orientation (byte/wide) decided
//	#define 	FF_							__SCLE  				// 0x4000  convert line endings CR/LF <-> NL

// complimentary file flag mapping
	#define		FF_MODEPACK					0x02000000				// Linear buffer ONLY, keep used space at start
	#define		FF_MODEBIN					0x04000000				//
	#define		FF_UNGETC					0x08000000				//
	#define		FF_LOCFILE					0x10000000				// simple stdin/stdout/stderr lock
	#define		FF_CIRCULAR					0x20000000				// control wrapping in/output pointers on circular
	#define		FF_FROMISR					0x40000000				// indicate being called from ISR
	#define		FF_TXRUNNING				0x80000000				// indicate UART/I2C/DMA interrupt running

#elif defined( __TI_ARM__ )
// macros to manipulate the file flags
	#define		FF_STCHK(stream, flag)		(((stream)->flags) & (flag))
	#define		FF_UNSET(stream, flag)		(((stream)->flags) &= ~(flag))
	#define		FF_SET(stream, flag)		(((stream)->flags) |= (flag))
// toolset standard file flag mapping
	#define 	FF_FILBUF					_IOFBF					// 0x0001
	#define 	FF_LINBUF					_IOLBF					// 0x0002
	#define		FF_NONBUF					_IONBF					// 0x0004
	#define		FF_BUFFALOC					_BUFFALOC				// 0x0008
	#define		FF_MODER					_MODER					// 0x0010
	#define		FF_MODEW					_MODEW					// 0x0020
	#define		FF_MODERW					_MODERW					// 0x0040
	#define		FF_MODEA					_MODEA					// 0x0080
	#define		FF_MODEBIN					_MODEBIN				// 0x0100
	#define 	FF_STATEOF					_STATEOF				// 0x0200
	#define 	FF_STATERR					_STATERR				// 0x0400
	#define		FF_UNGETC					_UNGETC					// 0x0800
// complimentary file flag mapping
	#define		FF_MODEPACK					0x02000000				// Linear buffer ONLY, keep used space at start
//	#define		FF_MODEBIN					0x04000000				//
//	#define		FF_UNGETC					0x08000000				//
	#define		FF_LOCFILE					0x10000000				// simple file lock
	#define		FF_CIRCULAR					0x20000000				// control wrapping in/output pointers on circular
	#define		FF_FROMISR					0x40000000				// indicate being called from ISR
	#define		FF_TXRUNNING				0x80000000				// indicate UART/I2C/DMA interrupt running

#else
	#error "Unsupported/no toolset defined !!!"

#endif

// #################################### Global/public functions ####################################

void fDiags(void);

#ifdef __cplusplus
} /* extern "C" */
#endif
