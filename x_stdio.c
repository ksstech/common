// x_stdio.c - Copyright (c) 2014-24 Andre M. Maree / KSS Technologies (Pty) Ltd.

#include <errno.h>
#include <sys/stat.h>

#include "hal_platform.h"
#include "x_stdio.h"
#include "x_ubuf.h"
#include "printfx.h"
#include "x_errors_events.h"
#include "x_time.h"

#include "esp_attr.h"

// ############################################### Macros ##########################################

#define	debugFLAG					0xF000

#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

// ###################################### BUILD : CONFIG definitions ##############################


// ###################################### Private variables ########################################


// ###################################### Global variables #########################################


// ######################################## global functions #######################################

#if		defined(__ARM_CC)

	#include "rt_misc.h"

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
		putchar(ch_saved);							// echo to STDOUT
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
	if (name[0] == CHR_COLON) {
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
		putchar(ch_saved);								// echo to STDOUT
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

