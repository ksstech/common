// stdioX.c - Copyright (c) 2014-25 Andre M. Maree / KSS Technologies (Pty) Ltd.

#include "hal_platform.h"
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
static bool uart_active = 0;							// console active (during period < TIMEOUT)

// ################################ Low level Terminal IO support ##################################

#include "esp_rom_sys.h"
#include "esp_rom_uart.h"

#ifdef CONFIG_ESP_CONSOLE_USB_CDC_SUPPORT_ETS_PRINTF
#include "usb_console.h"

/* Used as an output function by esp_rom_printf.
 * The LF->CRLF replacement logic replicates the one in esp_rom_uart_putc.
 * Not static to allow placement into IRAM by ldgen.
 */
void esp_usb_console_write_char(char c){
    char cr = '\r';
    char lf = '\n';

    if (c == lf) {
        esp_usb_console_write_buf(&cr, 1);
        esp_usb_console_write_buf(&lf, 1);
    } else if (c == '\r') {
    } else {
        esp_usb_console_write_buf(&c, 1);
    }
}

/**
 * @brief	Switch the low level output channel
 * @param[in]	eCh channel to switch to:
 */
void vStdioSelectChannel(int eCh) {
	IF_myASSERT(debugPARAM, OUTSIDE(0, eCh, CONFIG_SOC_UART_NUM));
	int NewCh;
	void (*NewHdlr)(char);
	switch (eCh) {
	#if defined(CONFIG_ESP_ROM_USB_OTG_NUM)
		case -1:
			NewCh = CONFIG_ESP_ROM_USB_OTG_NUM; 
			NewHdlr = &esp_usb_console_write_char;
			break;
	#elif defined(CONFIG_ESP_ROM_USB_SERIAL_DEVICE_NUM)
		case -1:
			NewCh = CONFIG_ESP_ROM_USB_SERIAL_DEVICE_NUM; 
			NewHdlr = &esp_usb_console_write_char;
			break;
	#endif
	#if defined(CONFIG_SOC_UART_NUM) && (CONFIG_SOC_UART_NUM > 0)
		case 0: 
			NewCh = 1;									// Ch 1 by default connected to UART0
			NewHdlr = esp_rom_output_putc; 
			break;	
		#if (CONFIG_SOC_UART_NUM > 1)
		case 1: 
			NewCh = 2; 
			NewHdlr = esp_rom_output_putc; 
			break;
		#endif
		#if (CONFIG_SOC_UART_NUM > 2)
		case 2:
			NewCh = 3; 
			NewHdlr = esp_rom_output_putc;
			break;
		#endif
	#endif
		default: assert(0);
	}
    esp_rom_output_set_as_console(NewCh);
	//esp_rom_install_channel_putc(1, NewHdlr);
}
#endif

/**
 * @brief		input a string directly from UART/terminal channel specified
 * @param[in]	fd UART/USB channel to use
 * @param[in]	pcStr pointer to buffer to store characters
 * @param[in]	Len size of buffer
 * @param[in]	Match character/key to be treated as end-of-string
 * @return		number of characters in the buffer, terminator (if there) excluded
 */
static int xStdioReadStringMatch(int sd, char * pcStr, size_t Len, int Match) {
	IF_myASSERT(debugPARAM, sd != STDOUT_FILENO && sd != STDERR_FILENO);
	TickType_t tNow = 0, tStart = xTaskGetTickCount();
	int Count = 0;
	char cChr;
	do {
		if (read(sd, &cChr, 1) == 1) {
			pcStr[Count++] = cChr;
			if (Count == Len || cChr == Match)			// buffer full or match found?
				break;									// get out of here
		} else {
			taskYIELD();
			tNow = xTaskGetTickCount() - tStart;
		}
	} while (tNow < pdMS_TO_TICKS(500));
	if (Count < (Len - 1))								// if less than buffer size
		pcStr[Count] = CHR_NUL;							// add terminator
	return Count;
}

/**
 * @brief
 * @param[in]	fd UART/USB number 
 * @return
 * @note	https://invisible-island.net/xterm/ctlseqs/ctlseqs.html
 * @note	1B	5B	3F	31	3B	32	63	"\e[?1;2c"	idf.py, Serial, Xterm/VT100/Linux emulation all same
 */
static int xStdioReadTerminalType(int sd) {
	char caType[16];
	int sdWR = (sd == STDIN_FILENO) ? STDOUT_FILENO : sd;
	write(sdWR, getDEVICE_ATTR, strlen(getDEVICE_ATTR));
	int iRV = xStdioReadStringMatch(sd, caType, sizeof(caType), 'c');
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

int xStdioGetTerminalType(void) { return TermType; }

int xStdioSyncCursor(int sd, char * pcStr, i16_t * pRowY, i16_t * pColX) {
	char caType[16];
	int sdWR = (sd == STDIN_FILENO) ? STDOUT_FILENO : sd;
	xStdioPutS(sdWR, pcStr);						// send query string
	int iRV = xStdioReadStringMatch(sd, caType, sizeof(caType), 'R');
	if (iRV < 6)
		return erFAILURE;
	return sscanf(caType, termCSI "%hu;%huR", pRowY, pColX);
}

int xStdioSyncCursorNow(int sd, terminfo_t * psTI) {
	if (psTI == NULL)
		psTI = &sTI;
	return xStdioSyncCursor(sd, getCURSOR_POS,  &psTI->CurY, &psTI->CurX);
}

int xStdioSyncCursorMax(int sd, terminfo_t * psTI) {
	if (psTI == NULL)
		psTI = &sTI;
	return xStdioSyncCursor(sd, getCURSOR_MAX,  &psTI->MaxY, &psTI->MaxX);
}

//	https://invisible-island.net/xterm/ctlseqs/ctlseqs.html
//	http://docs.smoe.org/oreilly/unix_lib/upt/ch05_05.htm
//	http://www.acm.uiuc.edu/webmonkeys/book/c_guide/2.12.html#gets

/**
 * @brief	Check column and adjust column & row if required
 * @param[in]	psTI pointer to terminal status structure to be updated
 */
 static void vStdioCheckCursor(terminfo_t * psTI) {
	if (psTI->CurX >= psTI->MaxX) {
		psTI->CurX = 1;
		if (psTI->CurY < psTI->MaxY)
			++psTI->CurY;
	}
}

void vStdioUpdateCursor(terminfo_t * psTI, char * pBuf, size_t Size) {
	BaseType_t btRV = xRtosSemaphoreTake(&psTI->mux, portMAX_DELAY);
	while (Size--) {
		char cChr = *pBuf++;
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
			vStdioCheckCursor(psTI);
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
				vStdioCheckCursor(psTI);
			}
			break;
		}
	}
	if (btRV == pdTRUE)
		xRtosSemaphoreGive(&psTI->mux);
}

void vStdioSetSize(terminfo_t * psTI, u16_t RowY, u16_t ColX) {
    if (RowY && ColX) {
	    psTI->MaxY = (RowY < TERMINAL_MAX_Y) ? RowY : TERMINAL_DFLT_Y;
    	psTI->MaxX = (ColX < TERMINAL_MAX_X) ? ColX : TERMINAL_DFLT_X;
    }
}

void vStdioConsoleSetStatus(bool state) { uart_active = state; }

bool bStdioConsoleGetStatus(void) { return uart_active; }

// ######################################## global functions #######################################

int xStdioRead(int sd, char * pBuf, size_t Size) {
	if (halMemoryRAM(pBuf) == 0)
		return erINV_PARA;
	int iRV = read(sd, pBuf, Size);
	if (sd == STDIN_FILENO && iRV > 0) {
		if (uart_active == 0) {
			#if (stdioBUILD_TERMIO == 1)
				TermType = xStdioReadTerminalType(sd);
				xStdioSyncCursorNow(sd, NULL);
				xStdioSyncCursorMax(sd, NULL);
			#endif
			vStdioConsoleSetStatus(1);
		}
	}
	return iRV;
}

int	xStdioGetString(int sd, char * pcBuf, size_t Size, bool bHide) {
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
		} else
		if (cChr == CHR_LF)								// now at end of string
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

int xStdioGetS(int sd, char * pcStr, size_t Size) { return xStdioGetString(sd, pcStr, Size, 0); }

int xStdioGetC(int sd) {
	char cChr = 0;
	int iRV = xStdioRead(sd, &cChr, 1);
	return (iRV == 1) ? cChr : iRV;
}

int xStdioWrite(int sd, char * pBuf, size_t Size) {
#if (cmakeWRAP_STDIO == 1)
	if (sd == STDOUT_FILENO && uart_active == 0) {	// destined for STDOUT & console UART inactive ?
		return xStdOutBufWrite(pBuf, Size);			// save to buffer
	} else {
		int iRV = write(sd, pBuf, Size);
		if (iRV)
			vStdioUpdateCursor(&sTI, pBuf, iRV);	// update cursor tracking
		return iRV;
	}
#else
	return write(sd, pBuf, Size);
#endif
}

int xStdioPutS(int sd, char * pcStr) { return xStdioWrite(sd, pcStr, strlen(pcStr)); }

int xStdioPutC(int sd, int iChr) {
	char cChr = iChr;
	return xStdioWrite(sd, &cChr, 1) == 1 ? iChr : erFAILURE; 
}

void xStdioPutHex(int sd, char * pcStr) {
	char caXlate[16] = "0123456789ABCDEF";
	while(*pcStr) {
		xStdioPutC(sd, caXlate[*pcStr >> 4]);
		xStdioPutC(sd, caXlate[*pcStr++ & 0xF]);
		xStdioPutC(sd, ' ');
	}
	xStdioPutS(sd, strNL);
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

// ################################# Generic terminal IO support ###################################

int xStdioGetCurColX(terminfo_t * psTI) { return psTI ? psTI->CurX : sTI.CurX; }

int xStdioGet_savColX(terminfo_t * psTI) { return psTI ? psTI->SavX : sTI.SavX; }

int xStdioGetMaxColX(terminfo_t * psTI) { return psTI ? psTI->MaxX : sTI.MaxX; };

int xStdioGetCurRowY(terminfo_t * psTI) { return psTI ? psTI->CurY : sTI.CurY; }

int xStdioGet_savRowY(terminfo_t * psTI) { return psTI ? psTI->SavY : sTI.SavY; }

int xStdioGetMaxRowY(terminfo_t * psTI) { return psTI ? psTI->MaxY : sTI.MaxY; }

void vStdioPushCurRowYColX(terminfo_t * psTI) {
	if (psTI == NULL)
		psTI = &sTI;
	psTI->SavY = psTI->CurY;
	psTI->SavX = psTI->CurX;
}

void vStdioPushMaxRowYColX(terminfo_t * psTI) {
	if (psTI == NULL)
		psTI = &sTI;
	psTI->SavY = psTI->MaxY;
	psTI->SavX = psTI->MaxX; 
}

void vStdioSetCurRowYColX(terminfo_t * psTI, i16_t RowY, i16_t ColX) {
	if (psTI == NULL)
		psTI = &sTI;
	psTI->CurY = RowY;
	psTI->CurX = ColX;
}

void vStdioSetMaxRowYColX(terminfo_t * psTI, i16_t RowY, i16_t ColX) {
	if (psTI == NULL)
		psTI = &sTI;
	psTI->MaxY = RowY;
	psTI->MaxX = ColX;
}

void vStdioPullCurRowYColX(terminfo_t * psTI) { 
	if (psTI == NULL)
		psTI = &sTI;
	psTI->CurY = psTI->SavY; 
	psTI->CurX = psTI->SavX;
}

void vStdioPullMaxRowYColX(terminfo_t * psTI) { 
	if (psTI == NULL)
		psTI = &sTI;
	psTI->MaxY = psTI->SavY; 
	psTI->MaxX = psTI->SavX;
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
