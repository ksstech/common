/*
 * x_builddefs.h
 */

#pragma once

#include "x_time.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * * Example of __TIMESTAMP__ISO__
 *	0         1         2         3
 *	01234567890123456789012345678901234567
 *	ccyy/mm/ddThh:mm:ssZ
 *	ccyy/mm/ddThh:mm:ss+h:mmZ
 *	ccyy/mm/ddThh:mm:ss+hh:mm(UTC)
 *	ccyy/mm/ddThh:mm:ss.xZ
 *	ccyy/mm/ddThh:mm:ss.xxxZ
 *	ccyy/mm/ddThh:mm:ss.xxxxxxZ
 *	ccyy/mm/ddThh:mm:ss.xxxxxx+hh:mm(SAST)
 *
 * Example of __TIMESTAMP__
 *	0         1         2         3
 *	0123456789012345678901234567890
 *	MMM dd ccyy hh:mm:ss
 *
 * Example of __DATE__
 *	0         1
 *	01234567890
 *	MMM dd ccyy
 *
 * Example of __TIME__
 *	0         1
 *	01234567890
 *	hh:mm:ss
 */

/*
#warning	"Timestamp     : "__TIMESTAMP__
#warning	"Timestamp ISO : "__TIMESTAMP__ISO__
#warning	"Date          : "__DATE__
#warning	"Time          : "__TIME__
*/

// determine the individual DATE characters for YEAR, MONTH and DAY

#define		BUILD_YEAR_CH0		(__DATE__[ 7])
#define 	BUILD_YEAR_CH1		(__DATE__[ 8])
#define 	BUILD_YEAR_CH2		(__DATE__[ 9])
#define 	BUILD_YEAR_CH3		(__DATE__[10])

#define		BUILD_MONTH_IS_JAN	(__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define 	BUILD_MONTH_IS_FEB	(__DATE__[0] == 'F')
#define 	BUILD_MONTH_IS_MAR	(__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define 	BUILD_MONTH_IS_APR	(__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define 	BUILD_MONTH_IS_MAY	(__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define 	BUILD_MONTH_IS_JUN	(__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define 	BUILD_MONTH_IS_JUL	(__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define 	BUILD_MONTH_IS_AUG	(__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define 	BUILD_MONTH_IS_SEP	(__DATE__[0] == 'S')
#define 	BUILD_MONTH_IS_OCT	(__DATE__[0] == 'O')
#define 	BUILD_MONTH_IS_NOV	(__DATE__[0] == 'N')
#define 	BUILD_MONTH_IS_DEC	(__DATE__[0] == 'D')

#define		BUILD_MONTH_CH0		((BUILD_MONTH_IS_OCT || BUILD_MONTH_IS_NOV || BUILD_MONTH_IS_DEC) ? '1' : '0')
#define		BUILD_MONTH_CH1		((BUILD_MONTH_IS_JAN) ? '1' : (BUILD_MONTH_IS_FEB) ? '2' : (BUILD_MONTH_IS_MAR) ? '3' : (BUILD_MONTH_IS_APR) ? '4' : \
								(BUILD_MONTH_IS_MAY) ? '5' : (BUILD_MONTH_IS_JUN) ? '6' : (BUILD_MONTH_IS_JUL) ? '7' : (BUILD_MONTH_IS_AUG) ? '8' : \
								(BUILD_MONTH_IS_SEP) ? '9' : (BUILD_MONTH_IS_OCT) ? '0' : (BUILD_MONTH_IS_NOV) ? '1' : (BUILD_MONTH_IS_DEC) ? '2' : '?' )

#define 	BUILD_DAY_CH0 		((__DATE__[4] >= '0') ? (__DATE__[4]) : '0')
#define 	BUILD_DAY_CH1 		(__DATE__[ 5])

// determine the DATE components as numeric values COMPUTE_BUILD_xxxxx

#define		COMPUTE_BUILD_YEAR	(uint32_t)							\
								(((__DATE__[ 7] - '0') * 1000) +	\
								((__DATE__[ 8] - '0') * 100) +		\
								((__DATE__[ 9] - '0') * 10) + 		\
								(__DATE__[10] - '0'))

#define		COMPUTE_BUILD_MONTH	(uint32_t) 						\
								((BUILD_MONTH_IS_JAN) ?  1 : 	\
								(BUILD_MONTH_IS_FEB) ?  2 : 	\
								(BUILD_MONTH_IS_MAR) ?  3 : 	\
								(BUILD_MONTH_IS_APR) ?  4 : 	\
								(BUILD_MONTH_IS_MAY) ?  5 : 	\
								(BUILD_MONTH_IS_JUN) ?  6 : 	\
								(BUILD_MONTH_IS_JUL) ?  7 : 	\
								(BUILD_MONTH_IS_AUG) ?  8 : 	\
								(BUILD_MONTH_IS_SEP) ?  9 : 	\
								(BUILD_MONTH_IS_OCT) ? 10 : 	\
								(BUILD_MONTH_IS_NOV) ? 11 : 	\
								(BUILD_MONTH_IS_DEC) ? 12 : 99 )

#define 	COMPUTE_BUILD_DAY	(uint32_t) (((__DATE__[4] >= '0') ? (__DATE__[4] - '0') * 10 : 0) + (__DATE__[5] - '0'))

// check if date was valid to start with, and set final values accordingly...

#define		BUILD_DATE_IS_BAD	(__DATE__[0] == '?')
#define 	BUILD_YEAR  		((BUILD_DATE_IS_BAD) ? 99UL : COMPUTE_BUILD_YEAR)
#define 	BUILD_MONTH 		((BUILD_DATE_IS_BAD) ? 99UL : COMPUTE_BUILD_MONTH)
#define 	BUILD_DAY   		((BUILD_DATE_IS_BAD) ? 99UL : COMPUTE_BUILD_DAY)

// calculate the values for hours, minutes, seconds

#define 	BUILD_HOUR_CH0		(__TIME__[0])
#define 	BUILD_HOUR_CH1		(__TIME__[1])

#define 	BUILD_MIN_CH0		(__TIME__[3])
#define 	BUILD_MIN_CH1		(__TIME__[4])

#define 	BUILD_SEC_CH0		(__TIME__[6])
#define 	BUILD_SEC_CH1		(__TIME__[7])

#define		COMPUTE_BUILD_HOUR	(((__TIME__[0] - '0') * 10) + (__TIME__[1] - '0'))
#define 	COMPUTE_BUILD_MIN	(((__TIME__[3] - '0') * 10) + (__TIME__[4] - '0'))
#define 	COMPUTE_BUILD_SEC	(((__TIME__[6] - '0') * 10) + (__TIME__[7] - '0'))

#define		BUILD_TIME_IS_BAD	(__TIME__[0] == '?')
#define 	BUILD_HOUR  		((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_HOUR)
#define 	BUILD_MIN   		((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_MIN)
#define 	BUILD_SEC			((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_SEC)

// Calculate number of days in February for a given year

#define		COMPUTE_DAYS_IN_FEB(year) (uint32_t)	\
			(((year % 400UL) == 0) 	? 29UL :		\
			((year % 100UL) == 0)	? 28UL :		\
			((year % 4UL) == 0) 	? 29UL : 28UL)

// Calculate number of days in year so far, for given year/month/day

#define COMPUTE_YEAR_DAY(year, month, day)	(uint32_t) 			\
    ( (day) +													\
	( (month) >=  2UL ? 31UL : 0 ) +							\
	( (month) >=  3UL ? COMPUTE_DAYS_IN_FEB(year) : 0 ) +		\
	( (month) >=  4UL ? 31UL : 0 ) +							\
	( (month) >=  5UL ? 30UL : 0 ) +							\
	( (month) >=  6UL ? 31UL : 0 ) +							\
	( (month) >=  7UL ? 30UL : 0 ) +							\
	( (month) >=  8UL ? 31UL : 0 ) +							\
	( (month) >=  9UL ? 31UL : 0 ) +							\
	( (month) >= 10UL ? 30UL : 0 ) +							\
	( (month) >= 11UL ? 31UL : 0 ) +							\
	( (month) >= 12UL ? 30UL : 0 ) )

// calculate different EPOCH timestamps

#define COMPUTE_TS_I32UNIX(yr, mon, day, hr, min, sec)				\
	( (sec) + 													\
	( (min) * SECONDS_IN_MINUTE ) +								\
	( (hr) * SECONDS_IN_HOUR) +									\
	( (COMPUTE_YEAR_DAY(yr, mon, day) - 1L ) * SECONDS_IN_DAY) +\
	( (  (yr) - 1970L ) * SECONDS_IN_YEAR365 ) +				\
	( ( ((yr) - 1969L ) / 4L ) * SECONDS_IN_DAY ) -				\
	( ( ((yr) - 1901L ) / 100L ) * SECONDS_IN_DAY ) +			\
	( ( ((yr) - 1601L ) / 400L ) * SECONDS_IN_DAY ) )

#define COMPUTE_TS_U32UNIX(yr, mon, day, hr, min, sec)	(uint32_t)	\
	( (uint32_t) (sec) + 											\
	( (uint32_t) (min) * SECONDS_IN_MINUTE ) +						\
	( (uint32_t) (hr) * SECONDS_IN_HOUR) +							\
	( (COMPUTE_YEAR_DAY(yr, mon, day) - 1L ) * SECONDS_IN_DAY) +	\
	( ((uint32_t)  (yr) - 1970UL ) * SECONDS_IN_YEAR365 ) +			\
	( ((uint32_t) ((yr) - 1969UL ) / 4UL ) * SECONDS_IN_DAY ) -		\
	( ((uint32_t) ((yr) - 1901UL ) / 100UL ) * SECONDS_IN_DAY ) +	\
	( ((uint32_t) ((yr) - 1601UL ) / 400UL ) * SECONDS_IN_DAY ) )

#define COMPUTE_TS_NTP(yr, mon, day, hr, min, sec)							\
	(seconds_t) ( (sec) + 													\
	(seconds_t) ( (min) * SECONDS_IN_MINUTE ) +								\
	(seconds_t) ( (hr) * SECONDS_IN_HOUR) +									\
	(seconds_t) ( (COMPUTE_YEAR_DAY(yr, mon, day) - 1UL ) * SECONDS_IN_DAY) +\
	(seconds_t) ( (  (yr) - 1900UL ) * SECONDS_IN_YEAR365 ) +				\
	(seconds_t) ( ( ((yr) - 1899UL ) / 4UL ) * SECONDS_IN_DAY ) -			\
	(seconds_t) ( ( ((yr) - 1901UL ) / 100UL ) * SECONDS_IN_DAY ) +			\
	(seconds_t) ( ( ((yr) - 1601UL ) / 400UL ) * SECONDS_IN_DAY ) )

// Only valid if built in UTC+2 timezone.... Convert the calculated UTC+2 value into UTC value..

#define	defsEPOCH_U32UNIX_SECONDS	COMPUTE_TS_U32UNIX(BUILD_YEAR, BUILD_MONTH, BUILD_DAY, BUILD_HOUR,	BUILD_MIN, BUILD_SEC) - (2 * SECONDS_IN_HOUR)

#define	defsEPOCH_I32UNIX_SECONDS	COMPUTE_TS_I32UNIX(BUILD_YEAR, BUILD_MONTH, BUILD_DAY, BUILD_HOUR, BUILD_MIN, BUILD_SEC) - (2 * SECONDS_IN_HOUR)

#define	defsEPOCH_U32NTP_SECONDS	COMPUTE_TS_NTP(BUILD_YEAR, BUILD_MONTH, BUILD_DAY, BUILD_HOUR, BUILD_MIN, BUILD_SEC) - (2 * SECONDS_IN_HOUR)

/*
 * Version MAJOR, MINOR and MICRO determination
 */

#define		MAKESTR1(x)    (x + '0')
#define		MAKESTR2(x)   ((x / 10) + '0'), ((x % 10) + '0')
#define		MAKESTR3(x)  (((x / 100) + '0'), (((x % 100) / 10) + '0'), ((x % 10) + '0')
#define		MAKESTR4(x) ((((x / 1000) + '0'), ((x % 100) / 100) + '0'), (((x % 100) / 10) + '0'), ((x % 10) + '0')

#define		MAKESTR(x)	((x) > 1000)	MAKESTR4(X)

#if		(CONFIG_IRMACS_VER_MAJOR > 100)
	#define 	CONFIG_IRMACS_VER_MAJOR_INIT	((CONFIG_IRMACS_VER_MAJOR / 100) + '0'), (((CONFIG_IRMACS_VER_MAJOR % 100) / 10) + '0'), ((CONFIG_IRMACS_VER_MAJOR % 10) + '0')
#elif 	(CONFIG_IRMACS_VER_MAJOR > 10)
	#define 	CONFIG_IRMACS_VER_MAJOR_INIT	((CONFIG_IRMACS_VER_MAJOR / 10) + '0'), ((CONFIG_IRMACS_VER_MAJOR % 10) + '0')
#else
	#define 	CONFIG_IRMACS_VER_MAJOR_INIT	(CONFIG_IRMACS_VER_MAJOR + '0')
#endif

#if 	(CONFIG_IRMACS_VER_MINOR > 100)
	#define 	CONFIG_IRMACS_VER_MINOR_INIT	((CONFIG_IRMACS_VER_MINOR / 100) + '0'), (((CONFIG_IRMACS_VER_MINOR % 100) / 10) + '0'), ((CONFIG_IRMACS_VER_MINOR % 10) + '0')
#elif 	(CONFIG_IRMACS_VER_MINOR > 10)
	#define 	CONFIG_IRMACS_VER_MINOR_INIT	((CONFIG_IRMACS_VER_MINOR / 10) + '0'), ((CONFIG_IRMACS_VER_MINOR % 10) + '0')
#else
	#define 	CONFIG_IRMACS_VER_MINOR_INIT	(CONFIG_IRMACS_VER_MINOR + '0')
#endif

#if 	(CONFIG_IRMACS_VER_MICRO > 100)
	#define 	CONFIG_IRMACS_VER_MICRO_INIT	((CONFIG_IRMACS_VER_MICRO / 100) + '0'), (((CONFIG_IRMACS_VER_MICRO % 100) / 10) + '0'), ((CONFIG_IRMACS_VER_MICRO % 10) + '0')
#elif 	(CONFIG_IRMACS_VER_MICRO > 10)
	#define		CONFIG_IRMACS_VER_MICRO_INIT	((CONFIG_IRMACS_VER_MICRO / 10) + '0'), ((CONFIG_IRMACS_VER_MICRO % 10) + '0')
#else
	#define		CONFIG_IRMACS_VER_MICRO_INIT	(CONFIG_IRMACS_VER_MICRO + '0')
#endif

#ifdef __cplusplus
}
#endif
