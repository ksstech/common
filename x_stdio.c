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
 * x_stdio.c
 */

#include	"hal_config.h"
#include	"hal_usart.h"
#include	"hal_rtc.h"
#include	"hal_debug.h"
#include	"hal_nvs.h"

#include	"FreeRTOS_Support.h"
#include	"x_errors_events.h"
#include 	"printfx.h"
#include	"x_stdio.h"
#include	"x_time.h"
#include	"x_ubuf.h"
#include	"x_definitions.h"
#include	"esp_attr.h"

#include	<string.h>
#include	<sys/errno.h>
#include	<sys/types.h>
#include	<sys/stat.h>

// ############################################### Macros ##########################################

#define	debugFLAG					0xC000

#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

// ###################################### BUILD : CONFIG definitions ##############################

#define	retargetNONE				0
#define	retargetUART				1
#define	retargetITM					2
#define	retargetRTT					3
#define	retargetTNET				4
#define	retargetCUSTOM				5

#define	retargetCHAN				retargetNONE

#if 	(retargetCHAN == retargetRTT)					// ALL to RTT
	#define	retargetSTDIN			retargetRTT
	#define	retargetSTDOUT			retargetRTT
	#define	retargetSTDERR			retargetRTT

#elif	(retargetCHAN == retargetUART)					// ALL to UART
	#define	retargetSTDIN			retargetUART
	#define	retargetSTDOUT			retargetUART
	#define	retargetSTDERR			retargetUART

#elif	(retargetCHAN == retargetITM)					// ALL to ITM
	#define	retargetSTDIN			retargetITM
	#define	retargetSTDOUT			retargetITM
	#define	retargetSTDERR			retargetITM

#elif	(retargetCHAN == retargetTNET)					// ALL to Telnet
	#define	retargetSTDIN			retartgetTNET
	#define	retargetSTDOUT			retartgetTNET
	#define	retargetSTDERR			retartgetTNET

#elif	(retargetCHAN == retargetCUSTOM)				// custom
	#define	retargetSTDIN			retargetNONE
	#define	retargetSTDOUT			retargetNONE
	#define	retargetSTDERR			retargetNONE

#else
	#define	retargetSTDIN			retargetUART
	#define	retargetSTDOUT			retargetUART
	#define	retargetSTDERR			retargetUART
#endif

// ##################################### Configuration tests #######################################

#if		(retargetCHAN == retargetTNET)  && (configCONSOLE_TELNET == 0)
	#error "Conflicting configuration for retargetTNET and configCONSOLE_TELNET"
#endif

// ###################################### Private variables ########################################

static uint32_t StdioBufFlag	= 0 ;

// ###################################### Global variables #########################################


// ################################ RTC Slow RAM buffer support ####################################

/* This code should ONLY run under very specific conditions being:
 * a) the first time on a new mote; or
 * b) after a power failure when RTC data has been wiped; or
 * c) if the allocation of RTC slow RAM has changed and the location of the pointers are in a different place.
 */
int32_t	xStdioBufInit(void) {
	ubuf_t * psBuf	= &sRTCvars.sRTCbuf ;
	if (psBuf->pBuf != sRTCvars.RTCbuf	||
		psBuf->Size != rtcBUF_SIZE	||
		psBuf->Used > rtcBUF_SIZE	||
		psBuf->IdxWR >= rtcBUF_SIZE	||
		psBuf->IdxRD >= rtcBUF_SIZE) {
		memset(&sRTCvars, 0, sizeof(sRTCvars)) ;
		psBuf->pBuf	= sRTCvars.RTCbuf ;
		psBuf->Size	= rtcBUF_SIZE ;
	}
	/* Since the value of the mux from last time round is of no value the 2nd (or later) times
	 * around the buffer structure will/might be correct but the mux must be re-initialised.
	 * The mux initialization will be done when xStdioBufLock() is called for the first time.
	 */
	if (StdioBufFlag == 0)
		psBuf->mux	= NULL ;
	StdioBufFlag	= 0x12345678 ;
	return erSUCCESS ;
}

int32_t	xStdioBufLock(TickType_t Ticks) {
	if (StdioBufFlag == 0)
		xStdioBufInit() ;
	return xRtosSemaphoreTake(&sRTCvars.sRTCbuf.mux, Ticks) ;
}

int32_t	xStdioBufUnLock(void) {
	return xRtosSemaphoreGive(&sRTCvars.sRTCbuf.mux) ;
}

int32_t	xStdioBufPutC(int cChr) {
	if (StdioBufFlag == 0)
		xStdioBufInit() ;
	if (cChr == CHR_LF)
		xStdioBufPutC(CHR_CR) ;
	ubuf_t * psBuf = &sRTCvars.sRTCbuf ;
	if (psBuf->Used == psBuf->Size) {					// buffer full ?
		++psBuf->IdxRD ;								// discard oldest (next to be read) char
		psBuf->IdxRD %= psBuf->Size ;					// adjust buffer for wrap
		--psBuf->Used ;									// adjust used counter
	}
	*(psBuf->pBuf + psBuf->IdxWR++) = cChr ;			// store character in buffer, adjust pointer
	psBuf->IdxWR %= psBuf->Size ;						// handle wrap
	++psBuf->Used ;										// & update used counter
	return cChr ;
}

int32_t	xStdioBufGetC(void) {
	if (StdioBufFlag == 0)
		xStdioBufInit() ;
	ubuf_t * psBuf = &sRTCvars.sRTCbuf ;
	if (psBuf->Used == 0) {
		errno = EAGAIN ;
		return EOF ;
	} else if (psBuf->Size == 0) {
		errno = ENOMEM ;
		return EOF ;
	}
	int32_t cChr = *(psBuf->pBuf + psBuf->IdxRD++) ;
	psBuf->IdxRD %= psBuf->Size ;
	if (--psBuf->Used == 0)	{							// buffer now empty
		psBuf->IdxRD = psBuf->IdxWR = 0 ;				// reset both In & Out indexes to start
	}
	return cChr ;
}

// ######################################## global functions #######################################

/**
 * putcharx() - Output a single character to the (possibly redirected) STDOUT
 * @brief		non-blocking behaviour, return immediately
 * @param[in]	cChr - char to output
 * @return		if successful, character sent, else EOF
 */
int		putcharx(int cChr) {
#if		(retargetSTDOUT == retargetUART)
	while(!halUART_TxFifoSpace(configSTDIO_UART_CHAN)) ;
	return halUART_PutChar(cChr, configSTDIO_UART_CHAN) ;

#elif	(retargetSTDOUT == retargetITM)
	return halITM_PutChar(cChr) ;

#elif	(retargetSTDOUT == retargetRTT)
	char c = cChr ;
	SEGGER_RTT_Write(0u, &c, 1u) ;
	return cChr ;

#elif	(retargetSTDOUT == retargetTNET)
	return xTelnetPutChar(cChr) ;

#else

	return EOF ;
#endif
}

int		getcharx(void) {
#if		(retargetSTDIN == retargetUART)
	return halUART_GetChar(configSTDIO_UART_CHAN) ;

#elif	(retargetSTDIN == retargetITM)
	return halITM_GetChar() ;

#elif	(retargetSTDIN == retargetRTT)
	return SEGGER_RTT_Read(0u, 1u) ;
]
#elif	(retargetSTDIN == retargetTNET)
	return xTelnetGetChar() ;

#else

	return EOF ;
#endif
}

int		putcx(int cChr, int ud) {
	if (ud == configSTDIO_UART_CHAN) {
#if		(CONFIG_IRMACS_UART_REDIR == 1)
		return xStdioBufPutC(cChr) ;
#else
		return putcharx(cChr) ;
#endif
	}
	while(!halUART_TxFifoSpace(ud)) ;
	return halUART_PutChar(cChr, ud) ;
}

int		getcx(int ud) {
	if (ud == configSTDIO_UART_CHAN)
		return getcharx() ;
	return halUART_GetChar((uart_port_t) ud) ;
}

int		putsx(char * pStr, int ud) {
	int32_t iRV = 0 ;
	while (*pStr) { putcx(*pStr++, ud) ; ++iRV ; }
	if (iRV) { putcx(CHR_LF, ud) ; ++iRV ; }
	return iRV ;
}

#if 0
#if		defined(__ARM_CC)

	#include	"rt_misc.h"

#elif	defined( __TI_ARM__ )

	FILE	__stdin 	= { .fd = 0 , .flags = 0	} ;
	FILE	__stdout	= {	.fd = 1 , .flags = 0	} ;
	FILE	__stderr	= { .fd = 2 , .flags = 0	} ;

#elif	defined( __GNUC__ )
	// nothing to be done for GNU
#endif

static int		ch_saved ;

/**
   Obtains the next character (if present) as an uint8_t converted to
   an int, from the input stream pointed to by stream, and advances the
   associated file position indicator (if defined).
  \param[in] stream  Stream handle
  \return    The next character from the input stream pointed to by stream.
             If the stream is at end-of-file, the end-of-file indicator is
             set and fgetc returns EOF. If a read error occurs, the error
             indicator is set and fgetc returns EOF.
*/
int		__fgetc (FILE * stream) {
	if (stream == 0) {
		return EOF ;
	}
	if (stream == stdin) {
		if (FF_STCHK(stream, FF_UNGETC)) {			// yes, do we have an ungetc'd char ?
			FF_UNSET(stream, FF_UNGETC) ;			// yes, flag as empty
			return ch_saved ;						// and re-return last character
		}
		ch_saved = getchar_stdin() ; 				// nothing there, read & save as last char
    	putcharx(ch_saved) ;					// echo to STDOUT
    	return ch_saved ;							// and return...
	}
#if		(buildSTDIO_FILEIO == 1)
	uint8_t cChr ;
	#if	defined(ESP_PLATFORM)
	if (_read(stream->_file, &cChr, 1, 0) == 1) {
		return cChr ;
	}
	#else
	if (_read(stream->fd, &cChr, 1, 0) == 1) {
		return cChr ;
	}
	#endif
#endif
	return EOF ;
}

/**
	Returns the last error code (if any) for a specific file pointer
*/
//int		_ferror(FILE * stream) { return (EOF) ; }

/**
 * _flock() -  simple bit flag based file un/locking
 */
#define	configSTDIO_LOCK_INTERVAL		5

int		_flock(FILE * stream, int mSec) {
	int32_t	xTicks = pdMS_TO_TICKS(mSec) ;
	while (FF_STCHK(stream, FF_LOCFILE) == true) {
		if ((xTicks <= 0) ||
			(xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED)) {
			return erFAILURE ;
		} else {
			vTaskDelay(pdMS_TO_TICKS(configSTDIO_LOCK_INTERVAL)) ;
			xTicks -= configSTDIO_LOCK_INTERVAL ;
		}
	}
	FF_SET(stream, FF_LOCFILE) ;
	return erSUCCESS ;
}

/**
 * _funlock() -  simple bit flag based file un/locking
 */
int		_funlock(FILE * stream) {
	if (FF_STCHK(stream, FF_LOCFILE) == true) {
		FF_UNSET(stream, FF_LOCFILE) ;
		return erSUCCESS ;
	}
	return erFAILURE ;
}

/**
   The function __backspace() is used by the scanf family of functions, and must
   be re-implemented if you retarget the stdio arrangements at the fgetc() level.
  \param[in] stream  Stream handle
  \return    The value returned by __backspace() is either 0 (success) or EOF
             (failure). It returns EOF only if used incorrectly, for example,
             if no characters have been read from the stream. When used
             correctly, __backspace() must always return 0, because the scanf
             family of functions do not check the error return.
*/
int		__backspace(FILE * stream) {
	if (stream == stdin) {
		if (FF_STCHK(stream, FF_UNGETC)) {				// already have 1 ungetc'd saved ?
			return EOF ;
		}
		FF_SET(stream, FF_UNGETC) ;						// set flag to indicate ungetc'c
		return 0 ;
	}
	return EOF ;
}

int 	_open (const char * name, int openmode) {
	if (name == NULL) {
		return EOF ;
	}
	if (name[0] == ':') {
		if (strcmp(name, ":STDIN") == 0) {
			return (FH_STDIN) ;
		}
		if (strcmp(name, ":STDOUT") == 0) {
			return (FH_STDOUT) ;
		}
		if (strcmp(name, ":STDERR") == 0) {
			return (FH_STDERR) ;
		}
		return EOF ;
	}
#if		(buildSTDIO_FILEIO == 1)
	myASSERT(0) ;
#endif
	return (0);
}

int		_close (int fh) {
	switch (fh) {
	case FH_STDIN:
	case FH_STDOUT:
	case FH_STDERR:
		return (0) ;
	}
#if		(buildSTDIO_FILEIO == 1)
	myASSERT(0) ;
#endif
	return (0);
}

int 	_read (int fh, uint8_t * buf, uint32_t len, int mode) {
	int		ch ;
	(void) mode ;

    switch (fh) {
    case FH_STDIN:										// what about looping until full len has been read
    	ch = getchar_stdin();							// what about BS if not 1st char in buffer, do BR + SPC + BS
    	if (ch < 0) {
    		return ((int)(len | 0x80000000U)) ;
    	}
    	*buf++ = (uint8_t) ch ;
    	putcharx(ch);
    	len--;
    	return ((int)(len)) ;
    case FH_STDOUT:
    case FH_STDERR:
    	return (EOF) ;
    }
#if		(buildSTDIO_FILEIO == 1)
	myASSERT(0) ;
#endif
	return (0);
}

int 	_istty (int fh) {
	switch (fh) {
    case FH_STDIN:
    case FH_STDOUT:
    case FH_STDERR:
    	return (1);
	}
	return (0);
}

int 	_seek (int fh, long pos) {
	switch (fh) {
    case FH_STDIN:
    case FH_STDOUT:
    case FH_STDERR:
    	return (EOF);
	}
#if		(buildSTDIO_FILEIO == 1)
	myASSERT(0) ;
#endif
	return (0);
}

long 	_flen (int fh) {
	switch (fh) {
    case FH_STDIN:
    case FH_STDOUT:
    case FH_STDERR:
    	return (0);
	}
#if		(buildSTDIO_FILEIO == 1)
	myASSERT(0) ;
#endif
	return (0);
}

int 	_ensure(int fh) { return 0 ; /* success*/ }

int 	_tmpnam(char * name, int sig, unsigned maxlen) { return 0 ; /* fail, not supported*/ }

#endif

void	vStdioDiagsReportFileInfo(const char * pName, struct stat *psStat) {
	PRINT(" %s : dev=%d  ino=%u  mode=%0x  nlink=%u  uid=%d  gid=%u  rdev=%d  size=%d", pName, psStat->st_dev,
		psStat->st_ino, psStat->st_mode, psStat->st_nlink, psStat->st_uid, psStat->st_gid, psStat->st_rdev, psStat->st_size) ;
	PRINT("  aT=%R  mT=%R  cT=%R  bsize=%d  blocks=%d",
		xTimeMakeTimestamp(psStat->st_atime, 0),
		xTimeMakeTimestamp(psStat->st_mtime, 0),
		xTimeMakeTimestamp(psStat->st_ctime, 0),
		psStat->st_blksize, psStat->st_blocks) ;
	PRINT("  %sFILE  %sBLOCK  %sFIFO  %sCHR\n",
		S_ISREG(psStat->st_mode) ? "" : "non",
		S_ISBLK(psStat->st_mode) ? "" : "non",
		S_ISFIFO(psStat->st_mode) ? "" : "non",
		S_ISCHR(psStat->st_mode) ? "" : "non") ;
}

void	vStdioDiags(void) {
	struct stat fs ;
	int32_t iRV = fstat(fileno(stdin), &fs) ;
	IF_myASSERT(debugRESULT, iRV == 0) ;
	vStdioDiagsReportFileInfo("stdin", &fs) ;

	iRV = stat("stdin", &fs) ;
	IF_myASSERT(debugRESULT, iRV == 0) ;
	vStdioDiagsReportFileInfo("stdin", &fs) ;

	iRV = fstat(fileno(stdout), &fs) ;
	IF_myASSERT(debugRESULT, iRV == 0) ;
	vStdioDiagsReportFileInfo("stdout", &fs) ;

	iRV = stat("STDOUT", &fs) ;
	IF_myASSERT(debugRESULT, iRV == 0) ;
	vStdioDiagsReportFileInfo("stdout", &fs) ;

	iRV = fstat(fileno(stderr), &fs) ;
	IF_myASSERT(debugRESULT, iRV == 0) ;
	vStdioDiagsReportFileInfo("stderr", &fs) ;

	iRV = stat("stderr", &fs) ;
	IF_myASSERT(debugRESULT, iRV == 0) ;
	vStdioDiagsReportFileInfo("stderr", &fs) ;
}
