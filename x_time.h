/*
 * Copyright 2014-21 Andre M Maree / KSS Technologies (Pty) Ltd.
 *	x_time.h
 */

#pragma once

#include	<time.h>									// struct tm
#include	<stdint.h>
#include	"x_definitions.h"							// no nested includes

#ifdef __cplusplus
	extern "C" {
#endif

// ############################## BUILD: TimeZone structure support ################################

#define	timexEPOCH_U32NTP			1
#define	timexEPOCH_I32UNIX			2
#define	timexEPOCH_U32UNIX			3
#define	timexEPOCH_SELECTED			timexEPOCH_U32UNIX

#define	timexTZTYPE_POINTER			1
#define	timexTZTYPE_FOURCHARS		2
#define	timexTZTYPE_RFC3164			3
#define	timexTZTYPE_SELECTED		timexTZTYPE_RFC3164

// ##################### BUILD: Unix I32 vs NTP U32 vs proprietary U32 epoch #######################

#if		(timexEPOCH_SELECTED == timexEPOCH_U32NTP)
	#define	SECONDS_IN_EPOCH_PAST			0UL				// 00:00.00 UTC 1900/01/01
	#define	SECONDS_IN_EPOCH_FUTURE			UINT32_MAX		// ??h??m??s on 2036/02/07
	#define	YEAR_BASE_MIN					1900UL
	#define	YEAR_BASE_MAX					2036UL
	#define	EPOCH_SECONDS_DIFFERENCE		0UL				// difference between selected epoch & NTP epoch
	#define	timeEPOCH_DAY_0_NUM				1				// 1900/01/01 was Monday, hence day 1
	typedef	uint32_t						seconds_t ;

#elif	(timexEPOCH_SELECTED == timexEPOCH_I32UNIX)
	#define	SECONDS_IN_EPOCH_PAST			0				// No support for dates prior to 1 Jan 1970
	#define	SECONDS_IN_EPOCH_FUTURE			INT32_MAX		// 03:14:07 UTC 2038/01/19 Tue
	#define	YEAR_BASE_MIN					1970L
	#define	YEAR_BASE_MAX					2038L
	#define	EPOCH_SECONDS_DIFFERENCE		2208988800L		// difference between selected epoch & NTP epoch
	#define	timeEPOCH_DAY_0_NUM				4				// 1970/01/01 was Thursday, hence day 4
	typedef	int32_t							seconds_t ;
	#warning	"UNTESTED configuration !!"

#elif	(timexEPOCH_SELECTED == timexEPOCH_U32UNIX)
/* Non standard interpretation for seconds based on U32 but with the base
 * date & time (0seconds) being 1970/01/01T00:00.00.000, same as I32 Unix */
	#define	SECONDS_IN_EPOCH_PAST			0UL				// 00:00.00 UTC 1970/01/01
	#define	SECONDS_IN_EPOCH_FUTURE			UINT32_MAX
	#define	YEAR_BASE_MIN					1970			// 00:00.00 UTC 1970/01/01
	#define	YEAR_BASE_MAX					2106			// ??:??.?? UTC 2106/02/07
	#define	EPOCH_SECONDS_DIFFERENCE		2208988800L		// difference between selected epoch & NTP epoch
	#define	timeEPOCH_DAY_0_NUM				4				// 1970/01/01 was Thursday, hence day 4
	typedef	uint32_t						seconds_t ;

#else
	#error "Cannot determine epoch, invalid base year specified"

#endif

// ################################## TZ DST UTC and time related ##################################

#if		(timexTZTYPE_SELECTED == timexTZTYPE_POINTER)
	#define	configTZ_EST	{-1*SECONDS_IN_HOUR,	-5*SECONDS_IN_HOUR, 	"EST",		"EST"	}
	#define	configTZ_UTC	{ 0, 					0, 						"UTC",		0 		}
	#define	configTZ_SAST	{ 0,					2*SECONDS_IN_HOUR,		"SAST", 	0		}
	#define	configTZ_ACT	{-1*SECONDS_IN_HOUR, 	10.5*SECONDS_IN_HOUR,	"ACT", 		"ACT"	}

	#define	configTIME_MAX_LEN_DSTNAME			48		// "Australian Central Daylight Savings Time" = 42
	#define	configTIME_MAX_LEN_TZNAME			48		// "America/Argentina/ComodRivadavia" == 33
	#define	configTIME_MAX_LEN_TZINFO			(sizeof("+12h34()") + configTIME_MAX_LEN_DSTNAME)

#elif	(timexTZTYPE_SELECTED == timexTZTYPE_FOURCHARS)
	#define	configTZ_EST	{-1*SECONDS_IN_HOUR,	-5*SECONDS_IN_HOUR, 	{'E','S','T',0},	{'E','S','T',0} }
	#define	configTZ_UTC	{ 0, 					0, 						{'U','T','C',0},	{'N','/','A',0} }
	#define	configTZ_SAST	{ 0,					2*SECONDS_IN_HOUR,		{'S','A','S','T'}, 	{'N','/','A',0} }
	#define	configTZ_ACT	{-1*SECONDS_IN_HOUR, 	10.5*SECONDS_IN_HOUR,	{'A','C','T',0}, 	{'A','C','T',0} }

	#define	configTIME_MAX_LEN_DSTNAME			4
	#define	configTIME_MAX_LEN_TZNAME			4
	#define	configTIME_MAX_LEN_TZINFO			(sizeof("+12h34()") + configTIME_MAX_LEN_DSTNAME)

#elif	(timexTZTYPE_SELECTED == timexTZTYPE_RFC3164)
	#define	configTZ_EST	{-1*SECONDS_IN_HOUR,	-5*SECONDS_IN_HOUR,		0,	0	}
	#define	configTZ_UTC	{ 0, 					0, 						0,	0 	}
	#define	configTZ_SAST	{ 0,					2*SECONDS_IN_HOUR,		0,	0	}
	#define	configTZ_ACT	{-1*SECONDS_IN_HOUR, 	10.5*SECONDS_IN_HOUR,	0,	0	}

	#define	configTIME_MAX_LEN_DSTNAME			48		// "Australian Central Daylight Savings Time" = 42
	#define	configTIME_MAX_LEN_TZNAME			48		// "America/Argentina/ComodRivadavia" == 33
	#define	configTIME_MAX_LEN_TZINFO			sizeof("+12h34")

#else
	#error	"Invalid or undefined 'timexTZTYPE_SELECTED' value"

#endif

// ############################### fractional seconds manipulation #################################

#define		FRACTIONS_PER_MILLISEC			( 0x0000000100000000ULL / 1000ULL)
#define		FRACTIONS_PER_MICROSEC			( 0x0000000100000000ULL / 1000000ULL)

/*
 * xTimeFractionToMillis()
 * \brief		convert NTP fractions of a second to milliseconds
 * \param[in]	fraction of a second
 * \param[out]	none
 * \return		fraction of a second as a number of milliseconds
 */
#define	xTimeFractionToMillis(x)	((uint32_t) x / (uint32_t) FRACTIONS_PER_MILLISEC)

#define	xTimeMillisToFraction(x)	((uint32_t) x * (uint32_t) FRACTIONS_PER_MILLISEC)

/*
 * xTimeFractionToMicros()
 * \brief		convert NTP fractions of a second to microseconds
 * \param[in]	fraction of a second
 * \param[out]	none
 * \return		fraction of a second as a number of microseconds
 */
#define	xTimeFractionToMicros(x)	((uint32_t) x / (uint32_t) FRACTIONS_PER_MICROSEC)

#define	xTimeMicrosToFraction(x)	((uint32_t) x * (uint32_t) FRACTIONS_PER_MICROSEC)

// ####################################### structures & unions #####################################

/*
 * TIME ZONE STRUCTURE DEFINITION
 */
typedef	struct __attribute__((__packed__)) TZ_s {
	short	daylight ;
	int32_t	timezone ;
#if		(timexTZTYPE_SELECTED == timexTZTYPE_POINTER || timexTZTYPE_SELECTED == timexTZTYPE_RFC3164)
	char *	pcTZName ;
	char *	pcDSTName ;
#elif	(timexTZTYPE_SELECTED == timexTZTYPE_FOURCHARS)
	char	tzname[4] ;
	char	dstname[4] ;
#endif
} TZ_t ;

typedef	struct TSZ_s {
	uint64_t	usecs ;
	TZ_t	*	pTZ ;
} TSZ_t ;

// ###################################### x_time related ###########################################

int32_t	xTimeIsLeapYear(int32_t Year) ;
int32_t	xTimeCountLeapYears(int32_t Year) ;
int32_t	xTimeGetDayNumber(char * pName) ;
int32_t	xTimeGetMonthNumber(char * pName) ;
int32_t	xTimeCalcDaysInMonth(struct tm *psTM) ;
int32_t	xTimeCalcDaysYTD(struct tm * psTM) ;
int32_t	xTimeCalcDaysToDate(struct tm * psTM) ;
char * xTimeGetDayName(int32_t) ;
char * xTimeGetMonthName(int32_t) ;
void	xTimeGMTime(seconds_t tValue, struct tm * psTM, int32_t fElapsed) ;
seconds_t xTimeCalcSeconds(struct tm *psTM, int32_t fElapsed) ;
seconds_t xTimeCalcLocalTimeSeconds(TSZ_t * psTSZ) ;

inline uint32_t xTimeStampAsSeconds(uint64_t Timestamp) { return (uint32_t) (Timestamp / (uint64_t) MICROS_IN_SECOND) ; }
inline uint64_t xTimeMakeTimestamp(uint32_t Seconds, uint32_t Micros) { return ((uint64_t) Seconds * (uint64_t) MICROS_IN_SECOND) + (uint64_t) Micros ; }

void	xTime_Test(void) ;

#ifdef __cplusplus
}
#endif
