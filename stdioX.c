// stdioX.c - Copyright (c) 2014-25 Andre M. Maree / KSS Technologies (Pty) Ltd.

#include "stdioX.h"
#include "FreeRTOS_Support.h"
#include "errors_events.h"
#include "hal_memory.h"
#include "hal_usart.h"
#include "syslog.h"
#include "utilitiesX.h"

#include <string.h>
#include <unistd.h>
#include <errno.h>

#if		defined(__ARM_CC)
	#include "rt_misc.h"
#elif	defined( __TI_ARM__ )
	// nothing for now...
#elif	defined( __GNUC__ )
	// nothing for now...
#endif

// ############################################### Macros ##########################################

#define	debugFLAG					0xF000

#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

// ###################################### BUILD : CONFIG definitions ###############################

// ######################################## Enumerations ###########################################

// ###################################### Private constants ########################################

const char cBS[3] = { CHR_BS, CHR_SPACE, CHR_BS };

// ###################################### Private variables ########################################

#if		defined(__ARM_CC)
	// nothing to be done
#elif	defined( __TI_ARM__ )
	FILE	__stdin 	= { .fd = 0 , .flags = 0	} ;
	FILE	__stdout	= {	.fd = 1 , .flags = 0	} ;
	FILE	__stderr	= { .fd = 2 , .flags = 0	} ;
#elif	defined( __GNUC__ )
	// nothing to be done
#endif

// ###################################### Global variables #########################################

terminfo_t sTI = {
	.mux = NULL, .CurX = 1, .CurY = 1, .SavX = 1, .SavY = 1,
	.MaxX = TERMINAL_DFLT_X, .MaxY = TERMINAL_DFLT_Y, .Tabs = TERMINAL_DFLT_TAB,
};

static int TermType = 0;								// 0=unknown, -1=Error, rest = type

// ################################ Low level Terminal IO support ##################################

int serial_read_terminal_type(int fd) {
	char caType[16];
	serial_write_string(fd, getDEVICE_ATTR);			// send query string
	int iRV = serial_read_string_match(fd, caType, sizeof(caType), 'c');
	if (iRV < 5 || OUTSIDE(CHR_0, caType[3], CHR_9))	// invalid response 
		return erFAILURE;
	if (caType[4] == CHR_SEMICOLON) {
		if (caType[3] == CHR_1) {						// VT100/101
			return (caType[5] == CHR_2) ? 100 : (caType[5] == CHR_0) ? 101 : erFAILURE;
		} else if (caType[3] == CHR_4 && caType[5] == CHR_6) {	// VT132
			return 132;
		} else {
			return erFAILURE;
		}
	} else if (caType[4] == CHR_c) {					// VT102/131
		return (caType[3] == CHR_6) ? 102 : (caType[3] == CHR_7) ? 131 : erFAILURE;
	} else if (caType[5] == CHR_SEMICOLON) {
		if (caType[3] == CHR_1 && caType[4] == CHR_2) {	// VT125
			return 125;
		} else if (caType[3] == CHR_6) {				// VT 220/320/420/520
			return (caType[4] == CHR_2) ? 220 : (caType[4] == CHR_3) ? 320 :
					(caType[4] == CHR_4) ? 420 : (caType[4] == CHR_5) ? 520 : erFAILURE;
		}
	}
	return erFAILURE;
}

int serial_get_terminal_type(void) { return TermType; }

int serial_query_cursor(int fd, char * pcStr, i16_t * pRowY, i16_t * pColX) {
	char caType[16];
	serial_write_string(fd, pcStr);						// send query string
	int iRV = serial_read_string_match(fd, caType, sizeof(caType), 'R');
	if (iRV < 6)
		return erFAILURE;
	return sscanf(caType, termCSI "%hu;%huR", pRowY, pColX);
}

int serial_query_cursor_now(int fd) {
	return serial_query_cursor(fd, getCURSOR_POS,  &sTI.CurY, &sTI.CurX);
}

int serial_query_cursor_max(int fd) {
	return serial_query_cursor(fd, getCURSOR_MAX,  &sTI.MaxY, &sTI.MaxX);
}

// ################################# Generic terminal IO support ###################################

int stdio_get_cur_colX(terminfo_t * psTI) { return psTI->CurX; };
int stdio_get_sav_colX(terminfo_t * psTI) { return psTI->SavX; };
int stdio_get_max_colX(terminfo_t * psTI) { return psTI->MaxX; };

int stdio_get_cur_rowY(terminfo_t * psTI) { return psTI->CurY; };
int stdio_get_sav_rowY(terminfo_t * psTI) { return psTI->SavY; };
int stdio_get_max_rowY(terminfo_t * psTI) { return psTI->MaxY; };

void stdio_push_cur_rowY_colX(terminfo_t * psTI) { psTI->SavY = psTI->CurY; psTI->SavX = psTI->CurX; }
void stdio_push_max_rowY_colX(terminfo_t * psTI) { psTI->SavY = psTI->MaxY; psTI->SavX = psTI->MaxX; }

void stdio_set_cur_rowY_colX(terminfo_t * psTI, i16_t RowY, i16_t ColX) { psTI->CurY = RowY; psTI->CurX = ColX; };
void stdio_set_max_rowY_colX(terminfo_t * psTI, i16_t RowY, i16_t ColX) { psTI->MaxY = RowY; psTI->MaxX = ColX; };

void stdio_pull_cur_rowY_colX(terminfo_t * psTI) { psTI->CurY = psTI->SavY; psTI->CurX = psTI->SavX; }
void stdio_pull_max_rowY_colX(terminfo_t * psTI) { psTI->MaxY = psTI->SavY; psTI->MaxX = psTI->SavX; }

//	https://invisible-island.net/xterm/ctlseqs/ctlseqs.html
//	http://docs.smoe.org/oreilly/unix_lib/upt/ch05_05.htm
//	http://www.acm.uiuc.edu/webmonkeys/book/c_guide/2.12.html#gets

/**
 * @brief	Check column and adjust column & row if required
 * @param[in]	psTI pointer to terminal status structure to be updated
 */
 static void stdio_check_cursor(terminfo_t * psTI) {
	if (psTI->CurX >= psTI->MaxX) {
		psTI->CurX = 1;
		if (psTI->CurY < psTI->MaxY)
			++psTI->CurY;
	}
}

void stdio_update_cursor(terminfo_t * psTI, int cChr) {
	BaseType_t btRV = xRtosSemaphoreTake(&psTI->mux, portMAX_DELAY);
	switch(cChr) {
	case CHR_BS: {
		--psTI->CurX;
		if (psTI->CurX == 0) {
			psTI->CurX = psTI->MaxX;
			if (psTI->CurY > 1)
				--psTI->CurY;
		}
		break;
	}
	case CHR_TAB: {
		psTI->CurX = u32RoundUP(psTI->CurX, psTI->Tabs);
		stdio_check_cursor(psTI);
		break;
	}
	case CHR_LF: {
		#if (CONFIG_LIBC_STDOUT_LINE_ENDING_LF == 1 || CONFIG_LIBC_STDOUT_LINE_ENDING_CRLF == 1)
			psTI->CurX = 1;
		#endif
		if (psTI->CurY < psTI->MaxY)
			++psTI->CurY;
		break;
	}
	case CHR_FF: {
		psTI->CurX = psTI->CurY = 1;
		break;
	}
	case CHR_CR: {
		psTI->CurX = 1;
		#if (CONFIG_LIBC_STDOUT_LINE_ENDING_CR == 1 || CONFIG_LIBC_STDOUT_LINE_ENDING_CRLF == 1)
			if (psTI->CurY < psTI->MaxY)
				++psTI->CurY;
		#endif
		break;
	}
	default:
		if (INRANGE(CHR_SPACE, cChr, CHR_TILDE)) {
			++psTI->CurX;
			stdio_check_cursor(psTI);
		}
		break;
	}
	if (btRV == pdTRUE)
		xRtosSemaphoreGive(&psTI->mux);
}

void stdio_set_size(terminfo_t * psTI, u16_t RowY, u16_t ColX) {
    if (RowY && ColX) {
	    psTI->MaxY = (RowY < TERMINAL_MAX_Y) ? RowY : TERMINAL_DFLT_Y;
    	psTI->MaxX = (ColX < TERMINAL_MAX_X) ? ColX : TERMINAL_DFLT_X;
    }
}

// ######################################## global functions #######################################

int	xReadString(int sd, char * pcBuf, size_t Size, bool bHide) {
	u8_t Idx = 0, cChr;
#if (CONFIG_LIBC_STDIN_LINE_ENDING_CRLF == 1)
	bool CRflag = 0;
#endif
	if (sd < 0 || halMemoryRAM(pcBuf) == 0)
		return erINV_PARA;
	if (Size < 2) 
		return erINV_SIZE;
	while (1) {
		int iRV = read(sd, &cChr, sizeof(cChr));
		if (iRV != 1) {									// nothing read, what now?
			if (iRV == erFAILURE && errno != EAGAIN)
				return erFAILURE;
			vTaskDelay(50);								// wait a bit ...
			continue;									// and try again...
		}
	#if (CONFIG_LIBC_STDIN_LINE_ENDING_CRLF == 1)
		if (cChr == CHR_CR) {							// ALMOST end of input
			CRflag = 1;									// set flag but do not store in buffer or adjust count
		} else if (cChr == CHR_LF)						// now at end of string
	#elif (CONFIG_LIBC_STDIN_LINE_ENDING_CR == 1)
		if (cChr == CHR_CR)
	#elif (CONFIG_LIBC_STDIN_LINE_ENDING_LF == 1)
		if (cChr == CHR_LF)
	#endif
		{
			pcBuf[Idx] = 0;
			write(sd, strNL, strlen(strNL));
			break;
		} else if (cChr == CHR_BS) {					// correct typo
			if (Idx > 0) {
				--Idx;									// if anything in buffer, step back 1 char
				write(sd, cBS, sizeof(cBS));
			} else {
				cChr = CHR_BEL;							// else buffer empty, ring the bell..
				write(sd, &cChr, sizeof(cChr));
			}
		} else if (Idx < (Size-1)) {					// space left in buffer ?
			// only printable characters stored in buffer, control chars just echo'd
			if (INRANGE(CHR_SPACE, cChr, CHR_TILDE)) {
				pcBuf[Idx++] = cChr;					// yes, if valid char store in buffer
				if (bHide == 0)							// show char in clear?
					cChr = CHR_ASTERISK;				// no, replace with '*'
			}
			write(sd, &cChr, sizeof(cChr));				// echo [modified] character
		} else {										// buffer is full
			break;										// go test what you have...
		}
	}
	return Idx;
}

// ################################# Terminal (VT100) support routines #############################

void vTermGetInfo(terminfo_t * psTI) { memcpy(psTI, &sTI, sizeof(terminfo_t)); }

ssize_t xStdioWriteS(char * pBuf) {
	ssize_t sRV = write(STDOUT_FILENO, pBuf, strlen(pBuf));
	if (sRV < 0)
		return xSyslogError(__FUNCTION__, errno);
	return sRV;
}

ssize_t xStdioWriteSctrl(char * pBuf, termctrl_t Ctrl) {
	if (Ctrl.Lock)
		halUartLock(pdMS_TO_TICKS(Ctrl.Wait));
	int iRV = xStdioWriteS(pBuf);
	if (Ctrl.Unlock)
		halUartUnLock();
	return iRV;
}

char * pcTermAttrib(char * pBuf, u8_t a1, u8_t a2) {
	if (a1 <= colourBG_WHITE) {							// as long as valid
		pBuf = stpcpy(pBuf, termCSI);					// MUST process 1st param
		pBuf += xU32ToDecStr(a1, pBuf);
		if (INRANGE(attrBRIGHT, a2, colourBG_WHITE)) {	// 2nd param is optional
			*pBuf++ = CHR_SEMICOLON;
			pBuf += xU32ToDecStr(a2, pBuf);
		}
		*pBuf++ = CHR_m;
	}
	*pBuf = 0;											// terminate
	return pBuf;
}

void vTermAttrib(u8_t a1, u8_t a2) {
	char Buffer[sizeof("E[yyy;xxxH0")];
	if (pcTermAttrib(Buffer, a1, a2) != Buffer)
		xStdioWriteS(Buffer);
}

char * pcTermLocate(char * pBuf, i16_t RowY, i16_t ColX) {
	if (RowY && ColX) {
		pBuf = stpcpy(pBuf, termCSI);
		pBuf += xU32ToDecStr(RowY, pBuf);
		*pBuf++	= CHR_SEMICOLON;
		pBuf += xU32ToDecStr(ColX, pBuf);
		*pBuf++	= CHR_H;
		sTI.CurY = (RowY % sTI.MaxY) + 1;
		sTI.CurX = (ColX % sTI.MaxX) + 1;
	}
	*pBuf = 0;
	return pBuf;
}

void vTermLocate(i16_t RowY, i16_t ColX) {
	char Buffer[sizeof("\e[yyy;xxxH\0")];
	if (pcTermLocate(Buffer, RowY, ColX) != Buffer)
		xStdioWriteS(Buffer);
}

ssize_t xTermLocatePuts(i16_t RowY, i16_t ColX, char * pBuf) {
	vTermLocate(RowY, ColX);
	return xStdioWriteS(pBuf);
}

void vTermCursorSave(void) { xStdioWriteS(setCURSOR_SAVE); }

void vTermCursorBack(void) { xStdioWriteS(setCURSOR_REST); }

void vTermClear2EOL(void) { xStdioWriteS(setCLRLIN_RIGHT); }

void vTermClear2BOL(void) { xStdioWriteS(setCLRLIN_LEFT); }

void vTermClearline(void) { xStdioWriteS(setCLRLIN_ALL); }

void vTermClearScreen(void) { xStdioWriteS(setCLRDSP_ALL); }

void vTermHome(void) { 
	xStdioWriteS(setCURSOR_HOME);
	sTI.CurX = sTI.CurY = 1;
}

void vTermClearHome(void) { vTermClearScreen(); vTermHome(); }

ssize_t xTermOpSysCom(char * pBuf) {
	xStdioWriteSctrl(termOSC, termBUILD_CTRL(1, 0, termWAIT_MS));
	int iRV = xStdioWriteSctrl(pBuf, termBUILD_CTRL(0, 0, 0));
	xStdioWriteSctrl(termST, termBUILD_CTRL(0, 1, 0));
	return iRV;
}

ssize_t xTermWinTleCursor(void) {
	char Buffer[16];
	char * pBuf = Buffer;
	pBuf = stpcpy(pBuf, "0;R=");
	pBuf += xU32ToDecStr(sTI.CurY, pBuf);
	pBuf = stpcpy(pBuf, " C=");
	pBuf += xU32ToDecStr(sTI.CurX, pBuf);
	return xTermOpSysCom(Buffer);
}

ssize_t xTermShowCurRowYColX(i16_t RowY, i16_t ColX) {
	char Buffer[32];
	// scale requested RowY to fit in range of terminal height
	if (RowY > sTI.MaxY)	RowY = sTI.MaxY;
	else if (RowY == 0)		RowY = sTI.MaxY;
	else if (RowY == -1)	RowY = sTI.MaxY / 2;		// Middle line/row
	else if (RowY < -1)		RowY = 1;
	// scale requested ColX to fit in range of terminal width
	int iRV = xDigitsInU32(sTI.CurY, 0) + xDigitsInU32(sTI.CurX, 0) + 5;
	if (ColX > sTI.MaxX)	ColX = sTI.MaxX;
	else if (ColX == 0)		ColX = sTI.MaxX-iRV;		// furthest right without wrap	
	else if (ColX == -1)	ColX = ((sTI.MaxX-iRV)/2);	// center justified	
	else if (ColX < -1)		ColX = 1;
	// build the combined save/display/restore string
	#define fmtCURSOR_DISP setCURSOR_SAVE termCSI "%d;%dH [%d,%d] " setCURSOR_REST
	iRV = snprintfx(Buffer, sizeof(Buffer), fmtCURSOR_DISP, RowY, ColX, sTI.CurY, sTI.CurX);
	return xStdioWriteS(Buffer);
}

// ##################################### functional tests ##########################################

void vTermTestCode(void) {
	xStdioWriteS("Waiting to clear the screen...");
	vTermClearHome();
	vTermLocate(5, 5);
	xStdioWriteS("Normal text at 5,5");

	vTermLocate(7, 7);
	vTermAttrib(attrRESET, colourFG_WHITE);
	xStdioWriteS("White text on Black background at 7,7");

	vTermLocate(9, 9);
	vTermAttrib(attrREV_ON, attrULINE_ON);
	xStdioWriteS("Normal underlined text at 9,9");

	vTaskDelay(5000);
	vTermLocate(9, 19);
	xStdioWriteS("!!! OVERPRINTED TEXT !!!");

	vTaskDelay(5000);
	vTermLocate(9, 25);
	vTermClear2EOL();

	vTermAttrib(attrRESET, 0);
}

#if 0

static int ch_saved ;

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
		ch_saved = halSTDIO_GetChar_stdin() ; 		// nothing there, read & save as last char
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
    	ch = halSTDIO_GetChar_stdin();					// what about BS if not 1st char in buffer, do BR + SPC + BS
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
#endif
