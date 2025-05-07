// timeX.c - Copyright (c) 2014-25 Andre M. Maree / KSS Technologies (Pty) Ltd.

#include "hal_platform.h"
#include "string_general.h"
#include "timeX.h"
#include "errors_events.h"

#include <string.h>

// ########################################## Macros ###############################################

#define	debugFLAG					0xF000
#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

// #################################################################################################

const char * const DayNames[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri",	"Sat", 0 };
const char * const MonthNames[]	= { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",	"Sep", "Oct", "Nov", "Dec", 0 };
// Number of days in a year at the beginning of each month of the year, in a non-leap year.
const short	DaysToMonth[12]	= { 0,	31,	59,	90,120,151,181,212,243,273,304,334 };
// Number of days in a month for each month of the year, in a non-leap year.
const char DaysPerMonth[]	= { 31,	28,	31,	30,	31,	30,	31,	31,	30,	31,	30,	31 };

// #################################################################################################

/**
 * @brief	tests if a year is a leap year
 * @param	year to be tested
 * @return	1=yes or 0=no based on evaluation
 */
int	xTimeIsLeapYear(int year) {
	if ((year % 4) == 0) {						// if a multiple of 4
		if ((year % 100) == 0) {				// AND a multiple of 100 (CENTURY year)
			if ((year % 400) == 0)
				return 1;			// AND a multiple of 400 then IS a leap year
			else
				return 0;			// NOT a leap year
		} else
			return 1;				// multiple of 4 but NOT of 100,definitely a leap year
	} else
		return 0;					// NOT a multiple of 4, definitely NOT a leap year
}

/**
 * @brief	calculate the number of leap days from epoch start to (year-01-01T00:00:00.000Z
 * @brief	value supplied for year date must not be before epoch start
 * @param	year ending for calculation
 * @return	number of leap years, FAILURE if parameter out of range
 */
int	xTimeCountLeapYears(int NowYear) {
	if (OUTSIDE(YEAR_BASE_MIN, NowYear, YEAR_BASE_MAX))
		return erFAILURE;
	int	Leaps = 0;
	for (int TestYear = YEAR_BASE_MIN; TestYear < NowYear; TestYear++) {
		if (xTimeIsLeapYear(TestYear))
			++Leaps;
	}
	return Leaps;
}

/**
 * @brief	calculate the day number based on the 3 char string supplied
 * @param	str - pointer to the name string
 * @return	0 to 6 for Sun to Sat or FAILURE if not matched
 */
int	xTimeGetDayNumber(char * str) { return xstrindex(str, (char * *) DayNames); }

/**
 * @brief	calculate the month number based on the 3 char string supplied
 * @param	str - pointer to the name string
 * @return	0 to 11 for Jan to Dec or FAILURE if not matched
 */
int	xTimeGetMonthNumber(char * str) { return xstrindex(str, (char * *) MonthNames); }

char * xTimeGetDayName(int num) { return (num < 7) ? (char *) DayNames[num] : pcFAILURE; }

char * xTimeGetMonthName(int num) { return (num < 12) ? (char *) MonthNames[num] : pcFAILURE; }

/**
 * @brief	Converts seconds to date/time components based on epoch selected..
 * @brief	U32NTP 1900-01-01T00:00:00.000Z onwards
 * 			I32UNIX 1970-01-01T00:00:00.000Z before and after
 * 			U32UNIX 1970-01-01T00:00:00.000Z onwards
 * @param	tValue - epoch seconds.
 * @param	psTM - pointer to tm structure to be filled with date & time components.
 * @param	Elapsed - flag indicating if tValue is epoch (zero) or elapsed (non zero) seconds.
 * @return	none
 */
void xTimeGMTime(seconds_t tValue, tm_t * psTM, int fElapsed) {
    memset(psTM, 0, sizeof(tm_t));
	#if (timexEPOCH_SELECTED == timexEPOCH_I32UNIX)
	if (tValue < 0)
		return;											// time prior to 1970 not supported
	#endif
    psTM->tm_sec = tValue % 60;							// calculate seconds
    tValue /= 60;
    psTM->tm_min = tValue % 60;							// calculate minutes
    tValue /= 60;
    psTM->tm_hour = tValue % 24;						// calculate hours
    tValue /= 24;
    if (fElapsed) {
    	psTM->tm_mday = tValue;							// elapsed time, leave as is
    	return;
    }
    // calculate day# of week (0 -> 6)
	psTM->tm_wday = (tValue + timeEPOCH_DAY_0_NUM) % 7;
	// Calculate the current year
	int	leap;
	int year = YEAR_BASE_MIN;
    while (tValue >= (DAYS_IN_YEAR + (leap = xTimeIsLeapYear(year)))) {	// sufficient days left for a full year?
		tValue -= (DAYS_IN_YEAR + leap);				// yes, adjust remaining days
		++year;											// and step to next year
    }
    psTM->tm_year = year - YEAR_BASE_MIN;				// Adjust year for correct epoch
	psTM->tm_yday = tValue;								// and store the day # of the year
	// calculate the month of the year
    while (1) {
    	int	DaysInMonth = DaysPerMonth[psTM->tm_mon];
    	// if date is in Feb and year is a leap year, add day for 29th Feb
    	if (psTM->tm_mon == 1 && xTimeIsLeapYear(year))
			++DaysInMonth;
    	if (tValue < DaysInMonth)
			break;
   		tValue -= DaysInMonth;
   		++psTM->tm_mon;
    }
    // remaining days are day of month
	psTM->tm_mday = tValue + 1;							// range is 1 -> 31
}

int xTimeCalcDaysMonth(int Year, int Month) {			// handle February of a leap year
	return DaysPerMonth[Month] + ((xTimeIsLeapYear(Year + YEAR_BASE_MIN) && (Month == 1)) ? 1 : 0);
}

/**
 * @brief
 * @param psTM
 * @return
 */
int	xTimeCalcDaysInMonth(tm_t * psTM) { return xTimeCalcDaysMonth(psTM->tm_year, psTM->tm_mon); }

/**
 * @brief
 * @param psTM
 * @return
 */
int	xTimeCalcDaysYTD(tm_t *psTM) {				// handle day AFTER February of leap year
	return DaysToMonth[psTM->tm_mon] + (psTM->tm_mday-1) + ((xTimeIsLeapYear(psTM->tm_year+YEAR_BASE_MIN) && (psTM->tm_mon > 1)) ? 1 : 0);
}

/**
 * @brief
 * @param psTM
 * @return
 */
int	xTimeCalcDaysToDate(tm_t *psTM) {
	return (psTM->tm_year*DAYS_IN_YEAR) + xTimeCountLeapYears(psTM->tm_year+YEAR_BASE_MIN) + xTimeCalcDaysYTD(psTM);
}

/**
 * @brief	Convert time component values into [epoch] seconds
 * @param	psTM - structure containing time components
 * @param	fElapsed - flag indicating if structure represent elapsed time
 * @return	time in seconds
 */
seconds_t xTimeCalcSeconds(tm_t *psTM, int fElapsed) {
	seconds_t Seconds;
	if ((psTM->tm_sec < 60) && (psTM->tm_min < 60) && (psTM->tm_hour < 24) &&
		(psTM->tm_mon < 12) && (psTM->tm_mday <= 31)) {
		Seconds = psTM->tm_sec + (psTM->tm_min * SECONDS_IN_MINUTE) + (psTM->tm_hour * SECONDS_IN_HOUR);
		Seconds	+= (DaysToMonth[psTM->tm_mon] + (psTM->tm_mday - (fElapsed ? 0 : 1))) * SECONDS_IN_DAY;
		if (fElapsed) {
			Seconds	+= (psTM->tm_year * SECONDS_IN_YEAR_AVG);
		} else {
			// calc # of leap years
			int Leap;
			Leap = xTimeIsLeapYear(psTM->tm_year + YEAR_BASE_MIN) && (psTM->tm_mon > 1) ? 1 : 0;
			Leap += xTimeCountLeapYears(psTM->tm_year + YEAR_BASE_MIN);
			// add seconds in prev years
			Seconds	+= (psTM->tm_year * SECONDS_IN_YEAR365) + (Leap * SECONDS_IN_DAY);
		}
	} else {
		Seconds = 0;
	}
	return Seconds;
}

/**
 * @brief
 * @param	psTSZ
 * @return	Timestamp as seconds
 */
seconds_t xTimeCalcLocalTimeSeconds(tsz_t * psTSZ) {
	return xTimeStampSeconds(psTSZ->usecs) + psTSZ->pTZ->timezone + (int) psTSZ->pTZ->daylight;
}

u32_t xTimeStampSeconds(u64_t Timestamp) { return (u32_t) (Timestamp / MICROS_IN_SECOND); }

u64_t xTimeMakeTimeStamp(u32_t Sec, u32_t uSec) { return ((u64_t) Sec * (u64_t) MICROS_IN_SECOND) + (u64_t) uSec; }

seconds_t xTimeReport(tm_t *psTM) {
	seconds_t sRV = xTimeCalcSeconds(psTM, 0);
	report(psR, "%d/%02d/%02d %d:%02d:%02d DoW=%d DoY=%d (%lu)\r\n", psTM->tm_year+1970, psTM->tm_mon+1,
		psTM->tm_mday, psTM->tm_hour, psTM->tm_min, psTM->tm_sec, psTM->tm_wday, psTM->tm_yday, sRV);
	return sRV;
}

// ##################################### functional tests ##########################################

//	http://www.timeanddate.com/countdown/to?p0=198&year=1900&month=1&day=1&hour=0&min=0&sec=0

void xTimeTest(void) {
#if		(timexTZTYPE_SELECTED == timexTZTYPE_POINTER)
	tz_t	sTZ = {	.daylight = 0, .timezone = 0, .pcTZName = "Africa/Johannesburg", .pcDSTName = "South Africa Standard Time" };
#elif	(timexTZTYPE_SELECTED == timexTZTYPE_FOURCHARS)
	tz_t	sTZ = {	.daylight = 0, .timezone = 0, .tzname = { 'S', 'A', 'S', 'T' }, .dstname = { 'N', 'O', 'N', 'E' } };
#endif

	tsz_t	sTSZtemp;
	tz_t	sTZtemp;
	sTSZtemp.pTZ = &sTZtemp;

#if		(timexEPOCH_SELECTED == timexEPOCH_U32NTP)
	sTSZtemp.usecs = xTimeMakeTimeStamp(SECONDS_IN_EPOCH_PAST, 0);
	PX("1900-01-01 00:00:00.000Z (+0.00 UTC) = %Z" strNL, &sTSZtemp);

	sTSZtemp.unit = CurSecs.unit;
	PX("1968-01-20 03:14:07.000Z (+0.00 UTC) = %Z" strNL, &sTSZtemp);

	sTSZtemp.usecs = xTimeMakeTimeStamp(SECONDS_IN_EPOCH_FUTURE, 0);
	PX("2036-02-07 06:28:15.000Z (+0.00 UTC) = %Z" strNL, &sTSZtemp);

#elif	(timexEPOCH_SELECTED == timexEPOCH_I32UNIX)
	sTSZtemp.usecs = xTimeMakeTimeStamp(SECONDS_IN_EPOCH_PAST, 0);
	PX("1970-01-01 00:00:00.000Z (+0.00 UTC) = %Z" strNL, &sTSZtemp);

	sTSZtemp.usecs	= xTimeMakeTimeStamp(946684799, 0);
	PX("1999-12-31 23:59:59.000Z (+0.00 UTC) = %Z" strNL, &sTSZtemp);

	sTSZtemp.usecs += MICROS_IN_SECOND;
	PX("2000-01-01 00:00:00.000Z (+0.00 UTC) = %Z" strNL, &sTSZtemp);

	sTSZtemp.usecs = xTimeMakeTimeStamp(SECONDS_IN_EPOCH_FUTURE, 0);
	PX("2038-01-19 03:14:07.000Z (+0.00 UTC) = %Z" strNL, &sTSZtemp);

#elif	(timexEPOCH_SELECTED == timexEPOCH_U32UNIX)
	sTSZtemp.usecs = xTimeMakeTimeStamp(SECONDS_IN_EPOCH_PAST, 0);
	PX("1970-01-01 00:00:00.000Z (+0.00 UTC) = %Z" strNL, &sTSZtemp);

	sTSZtemp.usecs	= xTimeMakeTimeStamp(315532800, 0);
	PX("1980-01-01 00:00:00.000Z (+0.00 UTC) = %Z" strNL, &sTSZtemp);

	sTSZtemp.usecs = xTimeMakeTimeStamp(SECONDS_IN_EPOCH_FUTURE, 0);
	PX("2106-02-07 06:28:15.000Z (+0.00 UTC) = %Z" strNL, &sTSZtemp);
#endif
}
