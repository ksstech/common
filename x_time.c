/*
 * Copyright 2014-20 Andre M Maree / KSS Technologies (Pty) Ltd.
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
 * x_time.c
 */

#include	"x_time.h"
#include 	"printfx.h"
#include	"x_definitions.h"
#include	"x_string_general.h"

#define		erFAILURE				-1
#define		erSUCCESS				0

#include	<string.h>

#define	debugFLAG					0xC000

#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

// #################################################################################################

// Day names lookups
const char * const DayNames[]		= { "Sun",	"Mon",	"Tue",	"Wed",	"Thu",	"Fri",	"Sat", 0 } ;

// Month names lookups
const char * const MonthNames[]	= { "Jan",	"Feb",	"Mar",	"Apr",	"May",	"Jun",	"Jul",	"Aug",	"Sep",	"Oct",	"Nov",	"Dec", 0 } ;

// This array contains the number of days in a year at the beginning of each month of the year, in a non-leap year.
const short	DaysToMonth[12]	= { 0,		31,		59,		90,		120,	151,	181,	212,	243,	273,	304,	334		} ;

// This array contains the number of days in a month for each month of the year, in a non-leap year.
const char DaysPerMonth[]	= { 31,		28,		31,		30,		31,		30,		31,		31,		30,		31,		30,		31		} ;

// #################################################################################################

/*
 * xTimeIsLeapYear
 * \brief	tests if a year is a leap year
 * \param	year to be tested
 * \return	1=yes or 0=no based on evaluation
 */
int32_t	xTimeIsLeapYear(int32_t year) {
	if ((year % 4) == 0) {						// if a multiple of 4
		if ((year % 100) == 0) {				// AND a multiple of 100 (CENTURY year)
			if ((year % 400) == 0) {			// AND a multiple of 400
				return 1 ;						// then IS a leap year
			} else {							// else
				return 0 ;						// NOT a leap year
			}
		} else {								// multiple of 4 but NOT of 100 ?
			return 1 ;							// definitely a leap year
		}
	} else {									// if NOT a multiple of 4
		return 0 ;								// definitely NOT a leap year
	}
}

/*
 * xTimeCountLeapYears() - calculate the number of leap days from epoch start to (year-01-01T00:00:00.000Z
 * \brief		value supplied for year date must not be before epoch start
 * \param[in]	year - ending year for calculation
 * \return		number of leap years, FAILURE if parameter out of range
 */
int32_t	xTimeCountLeapYears(int32_t NowYear) {
	if (OUTSIDE(YEAR_BASE_MIN, NowYear, YEAR_BASE_MAX, seconds_t))
		return erFAILURE ;
	int32_t	Leaps, TestYear ;
	for(TestYear = YEAR_BASE_MIN, Leaps = 0; TestYear < NowYear; TestYear++) {
		if (xTimeIsLeapYear(TestYear))	++Leaps ;
	}
	return Leaps ;
}

/*
 * xTimeGetDayNumber()
 * \brief	calculate the day number based on the 3 char string supplied
 * \param	str - pointer to the name string
 * \return	0 to 6 for Sun to Sat or FAILURE if not matched
 */
int32_t	xTimeGetDayNumber(char * str) { return xstrindex(str, (char * *) DayNames) ; }

/*
 * xTimeGetMonthNumber()
 * \brief	calculate the month number based on the 3 char string supplied
 * \param	str - pointer to the name string
 * \return	0 to 11 for Jan to Dec or FAILURE if not matched
 */
int32_t	xTimeGetMonthNumber(char * str) { return xstrindex(str, (char * *) MonthNames) ; }

char * xTimeGetDayName(int32_t num) { return (num < 7) ? (char *) DayNames[num] : pcFAILURE ; }

char * xTimeGetMonthName(int32_t num) { return (num < 12) ? (char *) MonthNames[num] : pcFAILURE ; }

/*
 * xTimeGMTime() -	Converts epoch seconds to components of date and time.
 * \brief		This function converts a number of seconds since 1970-01-01T00:00:00.000Z
 * \brief		into the equivalent year, month, day, hours, minutes, and seconds representation.
 * \param[in]	timer - epoch seconds.
 * \param[out]	tm - pointer to tm structure to be filled with date & time components.
 * \param[in]	Elapsed - flag indicating if tValue is epoch (zero) or elapsed (non zero) seconds.
 * \return		none
 */
void	xTimeGMTime(seconds_t tValue, struct tm * psTM, int32_t fElapsed) {
    memset(psTM, 0, sizeof(struct tm)) ;
#if 	(buildTIME_EPOCH_SELECTED == buildTIME_EPOCH_I32UNIX)
	if (tValue < 0)	{									// if time prior to 1970
		return ;
	}
#endif
	// calculate seconds
    psTM->tm_sec	= tValue % SECONDS_IN_MINUTE ;
    tValue		/= SECONDS_IN_MINUTE ;
    // calculate minutes
    psTM->tm_min	= tValue % MINUTES_IN_HOUR ;
    tValue		/= MINUTES_IN_HOUR ;
    // calculate hours
    psTM->tm_hour	= tValue % HOURS_IN_DAY ;
    tValue		/= HOURS_IN_DAY ;
    if (fElapsed) {
    	psTM->tm_mday = tValue ;						// elapsed time, leave as is
    	return ;
    }
    // calculate day# of week (0 -> 6)
	psTM->tm_wday = (tValue + timeEPOCH_DAY_0_NUM) % DAYS_IN_WEEK ;
	// Calculate the current year
	int32_t	leap ;
	int32_t year = YEAR_BASE_MIN ;
    while (tValue >= (DAYS_IN_YEAR + (leap = xTimeIsLeapYear(year)))) {	// sufficient days left for a full year?
		tValue -= (DAYS_IN_YEAR + leap) ;				// yes, adjust remaining days
		++year ;										// and step to next year
    }
    psTM->tm_year = year - YEAR_BASE_MIN ;				// Adjust year for correct epoch
	psTM->tm_yday = tValue ;							// and store the day # of the year
	// calculate the month of the year
    while (true) {
    	int32_t	DaysInMonth = DaysPerMonth[psTM->tm_mon] ;
    	// if date is in Feb and year is a leap year, add day for 29th Feb
    	if (psTM->tm_mon == 1 && xTimeIsLeapYear(year))
    		++DaysInMonth ;
    	if (tValue < DaysInMonth) {
    		break ;
    	}
   		tValue -= DaysInMonth ;
   		++psTM->tm_mon ;
    }
    // remaining days are day of month
	psTM->tm_mday = tValue + 1 ;						// range is 1 -> 31
}

/*
 * xTimeCalcDaysInMonth()
 */
int32_t	xTimeCalcDaysInMonth(struct tm *psTM) {			// handle February of a leap year
	return DaysPerMonth[psTM->tm_mon] + ((xTimeIsLeapYear(psTM->tm_year + YEAR_BASE_MIN) && (psTM->tm_mon == 1)) ? 1 : 0) ;
}

/*
 * xTimeCalcDaysYTD()
 */
int32_t	xTimeCalcDaysYTD(struct tm *psTM) {				// handle day AFTER February of leap year
	return DaysToMonth[psTM->tm_mon] + (psTM->tm_mday-1) + ((xTimeIsLeapYear(psTM->tm_year+YEAR_BASE_MIN) && (psTM->tm_mon > 1)) ? 1 : 0) ;
}

/*
 * xTimeCalcDaysToDate()
 */
int32_t	xTimeCalcDaysToDate(struct tm *psTM) {
	return (psTM->tm_year*DAYS_IN_YEAR) + xTimeCountLeapYears(psTM->tm_year+YEAR_BASE_MIN) + xTimeCalcDaysYTD(psTM) ;
}

/*
 * xTimeCalcSeconds()
 */
seconds_t	xTimeCalcSeconds(struct tm *psTM, int32_t fElapsed) {
	// calculate seconds for hh:mm:ss portion
	seconds_t Seconds	= psTM->tm_sec + (psTM->tm_min * SECONDS_IN_MINUTE) + (psTM->tm_hour * SECONDS_IN_HOUR) ;

	// Then add seconds for MM/DD values (check elapsed time/not, to handle DoM correctly 0/1 relative)
	Seconds	+= (DaysToMonth[psTM->tm_mon] + (psTM->tm_mday - (fElapsed ? 0 : 1))) * SECONDS_IN_DAY ;

	// lastly, handle the years
	if (fElapsed) {
		Seconds	+= (psTM->tm_year * SECONDS_IN_YEAR_AVG) ;
	} else {
		int32_t Leap = xTimeIsLeapYear(psTM->tm_year + YEAR_BASE_MIN) && (psTM->tm_mon > 1) ? 1 : 0 ;
		int32_t Count = xTimeCountLeapYears(psTM->tm_year + YEAR_BASE_MIN) + Leap ;		// calculate the number of leap years
		Seconds	+= (psTM->tm_year * SECONDS_IN_YEAR365) + (Count * SECONDS_IN_DAY) ; // add seconds in previous years
	}
	return Seconds ;
}

/*
 * xTimeCalcLocalTimeSeconds()
 */
seconds_t	xTimeCalcLocalTimeSeconds(TSZ_t * psTSZ) {
	return xTimeStampAsSeconds(psTSZ->usecs) + psTSZ->pTZ->timezone + (int32_t) psTSZ->pTZ->daylight ;
}

// ##################################### functional tests ##########################################

//	http://www.timeanddate.com/countdown/to?p0=198&year=1900&month=1&day=1&hour=0&min=0&sec=0

void	xTimeTest(void) {
#if		(buildTIME_TZTYPE_SELECTED == buildTIME_TZTYPE_POINTER)
	TZ_t	sTZ = {	.daylight = 0, .timezone = 0, .pcTZName = "Africa/Johannesburg", .pcDSTName = "South Africa Standard Time" } ;
#elif	(buildTIME_TZTYPE_SELECTED == buildTIME_TZTYPE_FOURCHARS)
	TZ_t	sTZ = {	.daylight = 0, .timezone = 0, .tzname = { 'S', 'A', 'S', 'T' }, .dstname = { 'N', 'O', 'N', 'E' } } ;
#endif

	TSZ_t	sTSZtemp ;
	TZ_t	sTZtemp ;
	sTSZtemp.pTZ = &sTZtemp ;

#if		(buildTIME_EPOCH_SELECTED == buildTIME_EPOCH_U32NTP)
	sTSZtemp.usecs = xTimeMakeTimestamp(SECONDS_IN_EPOCH_PAST, 0) ;
	printfx("1900-01-01 00:00:00.000Z (+00.00 UTC) = %Z\n", &sTSZtemp) ;

	sTSZtemp.unit = CurSecs.unit ;
	printfx("1968-01-20 03:14:07.000Z (+00.00 UTC) = %Z\n", &sTSZtemp) ;

	sTSZtemp.usecs = xTimeMakeTimestamp(SECONDS_IN_EPOCH_FUTURE, 0) ;
	printfx("2036-02-07 06:28:15.000Z (+00.00 UTC) = %Z\n", &sTSZtemp) ;

#elif	(buildTIME_EPOCH_SELECTED == buildTIME_EPOCH_I32UNIX)
	sTSZtemp.usecs = xTimeMakeTimestamp(SECONDS_IN_EPOCH_PAST, 0) ;
	printfx("1970-01-01 00:00:00.000Z (+00.00 UTC) = %Z\n", &sTSZtemp) ;

	sTSZtemp.usecs	= xTimeMakeTimestamp(946684799, 0) ;
	printfx("1999-12-31 23:59:59.000Z (+00.00 UTC) = %Z\n", &sTSZtemp) ;

	sTSZtemp.usecs += MICROS_IN_SECOND ;
	printfx("2000-01-01 00:00:00.000Z (+00.00 UTC) = %Z\n", &sTSZtemp) ;

	sTSZtemp.usecs = xTimeMakeTimestamp(SECONDS_IN_EPOCH_FUTURE, 0) ;
	printfx("2038-01-19 03:14:07.000Z (+00.00 UTC) = %Z\n", &sTSZtemp) ;

#elif	(buildTIME_EPOCH_SELECTED == buildTIME_EPOCH_U32UNIX)
	sTSZtemp.usecs = xTimeMakeTimestamp(SECONDS_IN_EPOCH_PAST, 0) ;
	printfx("1970-01-01 00:00:00.000Z (+00.00 UTC) = %Z\n", &sTSZtemp) ;

	sTSZtemp.usecs	= xTimeMakeTimestamp(315532800, 0) ;
	printfx("1980-01-01 00:00:00.000Z (+00.00 UTC) = %Z\n", &sTSZtemp) ;

	sTSZtemp.usecs = xTimeMakeTimestamp(SECONDS_IN_EPOCH_FUTURE, 0) ;
	printfx("2106-02-07 06:28:15.000Z (+00.00 UTC) = %Z\n", &sTSZtemp) ;

#endif
}
