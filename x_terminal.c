// x_terminal.c - Copyright (c) 2014-24 Andre M. Maree / KSS Technologies (Pty) Ltd.

#include "hal_platform.h"
#include "hal_stdio.h"
#include "hal_usart.h"
#include "x_errors_events.h"
#include "printfx.h"
#include "x_terminal.h"
#include "x_utilities.h"

// ##################################### MACRO definitions #########################################

#define termUSE_7BIT			0						// C0 ~ ESC [
#define termUSE_8BIT			1						// C1 ~ 0x80 -> 0x9F
#define termUSE_TYPE			termUSE_7BIT

#if (termUSE_TYPE == termUSE_7BIT)
	#define termCSI	"\033["
	#define termST	"\033\134"
	#define termOSC "\033]"
#else
	#define termCSI	"\233"
	#define termST	"\234"
	#define termOSC "\235"
#endif

// ######################################## Enumerations ###########################################
// ###################################### Global variables #########################################

terminfo_t sTI = {
	.CurX = 0,
	.CurY = 0,
	.SavX = 0,
	.SavY = 0,
	.MaxX = TERMINAL_DFLT_X,
	.MaxY = TERMINAL_DFLT_Y,
	.Tabs = TERMINAL_DFLT_TAB,
};

// ################################# Terminal (VT100) support routines #############################
//	https://invisible-island.net/xterm/ctlseqs/ctlseqs.html
//	http://docs.smoe.org/oreilly/unix_lib/upt/ch05_05.htm
//	http://www.acm.uiuc.edu/webmonkeys/book/c_guide/2.12.html#gets

int	xTermGets(char * pcBuf, size_t Size, int Match, termctrl_t Ctrl) { 
	int iRV, Len = 0;
	TickType_t tNow = 0, tStart = xTaskGetTickCount();
	if (Ctrl.Lock) {
		xRtosSemaphoreTake(&shUARTmux, pdMS_TO_TICKS(Ctrl.Wait));
	}
	do {
		iRV = getchar();
		if (iRV != EOF) {
			pcBuf[Len++] = iRV;
			if (Len == Size || iRV == Match) {
				break;
			}
			continue;
		}
		taskYIELD();
		tNow = xTaskGetTickCount() - tStart;
	} while (tNow < pdMS_TO_TICKS(Ctrl.Wait));
	if (Len < (Size - 1)) {
		pcBuf[Len] = CHR_NUL;
	}
	if (Ctrl.Unlock) {
		xRtosSemaphoreGive(&shUARTmux);
	}
	return Len;
}

int xTermPuts(char * pStr, termctrl_t Ctrl) {
	int iRV = 0;
	if (Ctrl.Lock) {
		xRtosSemaphoreTake(&shUARTmux, pdMS_TO_TICKS(Ctrl.Wait));
	}
	while (*pStr) {
		putchar_cursor(*pStr++, Ctrl.NoCursor);
		++iRV;
	}
	if (Ctrl.Unlock) {
		xRtosSemaphoreGive(&shUARTmux);
	}
	return iRV;
}

/**
 * @brief	generate Term terminal cursor locate string (values are 1 relative)
 * @param	
 * @return	[Adjusted] pointer	
 */
char * pcTermLocate(char * pBuf, u8_t Row, u8_t Col) {
	if (Row > 0 && Col > 0) {
		pBuf = stpcpy(pBuf, termCSI);
		pBuf += xU32ToDecStr(Row, pBuf);
		*pBuf++	= CHR_SEMICOLON;
		pBuf += xU32ToDecStr(Col, pBuf);
		*pBuf++	= CHR_H;
		sTI.CurX = (Col %= sTI.MaxX);
		sTI.CurY = (Row %= sTI.MaxY);
	}
	*pBuf = 0;
	return pBuf;
}

char * pcTermAttrib(char * pBuf, u8_t a1, u8_t a2) {
	if (a1 <= colourBG_WHITE && a2 <= colourBG_WHITE) {
		pBuf = stpcpy(pBuf, termCSI);
		pBuf += xU32ToDecStr(a1, pBuf);
		*pBuf++ = CHR_SEMICOLON;
		pBuf += xU32ToDecStr(a2, pBuf);
		*pBuf++ = CHR_m;
	}
	*pBuf = 0;									// terminate
	return pBuf;
}

void vTermLocate(u8_t Row, u8_t Col) {
	char Buffer[sizeof("E[yyy;xxxH0")];
	if (pcTermLocate(Buffer, Row, Col) != Buffer) {
		xTermPuts(Buffer, termBUILD_CTRL(1,1,1,termWAIT_MS));
	}
}

void vTermAttrib(u8_t a1, u8_t a2) {
	char Buffer[sizeof("E[yyy;xxxH0")];
	if (pcTermAttrib(Buffer, a1, a2) != Buffer) {
		xTermPuts(Buffer, termBUILD_CTRL(1,1,1,termWAIT_MS));
	}
}

/**
 * @brief	Request, receive and parse current cursor location is a single lock/unlock operation
 * @return	Number of parameters parsed (should be 2) or erFAILURE if incomplete/malformed packet
*/
int xTermCursorRead(void) {
	char Buf[16];
	xTermPuts(termCSI "6n", termBUILD_CTRL(1,1,0,termWAIT_MS));
	int iRV = xTermGets(Buf, sizeof(Buf), 'R', termBUILD_CTRL(1,0,1,termWAIT_MS));
	if (iRV < 6) return erFAILURE;
	int row = 0, col = 0;
	iRV = sscanf(Buf, termCSI "%d;%dR", &row, &col);
//	RPL(" [iRV=%d row=%d col=%d num=%d]\r\n", iRV, row, col, num);
	sTI.CurY = sTI.SavY = row;
	sTI.CurX = sTI.SavX = col;
	return iRV;
}

void vTermCursorSave(void) { xTermPuts(termCSI "s", termBUILD_CTRL(1,1,1,termWAIT_MS)); }

void vTermCursorBack(void) { xTermPuts(termCSI "u", termBUILD_CTRL(1,1,1,termWAIT_MS)); }

void vTermclear2EOL(void) { xTermPuts(termCSI "0K", termBUILD_CTRL(1,1,1,termWAIT_MS)); }

void vTermclear2BOL(void) { xTermPuts(termCSI "1K", termBUILD_CTRL(1,1,1,termWAIT_MS)); }

void vTermclearline(void) { xTermPuts(termCSI "2K", termBUILD_CTRL(1,1,1,termWAIT_MS)); }

void vTermclearscreen(void) { xTermPuts(termCSI "2J", termBUILD_CTRL(1,1,1,termWAIT_MS)); }

void vTermhome(void) { xTermPuts(termCSI "1;1H", termBUILD_CTRL(1,1,1,termWAIT_MS)); }

void vTermclearhome(void) { vTermclearscreen(); vTermhome(); }

/**
 * @brief	Update cursor location into window title bar (XTERM only)
*/
void vTermOpSysCom(char * pStr) {
	xTermPuts(termOSC, termBUILD_CTRL(1,1,0,termWAIT_MS));
	xTermPuts(pStr, termBUILD_CTRL(1,0,0,0));
	xTermPuts(termST, termBUILD_CTRL(1,0,1,0));
}

void vTermWinTleCursor(void) {
	char Buffer[16];
	char * pPos = Buffer;
	pPos = stpcpy(pPos, "0;R=");
	pPos += xU32ToDecStr(sTI.CurY, pPos);
	pPos = stpcpy(pPos, " C=");
	pPos += xU32ToDecStr(sTI.CurX, pPos);
	vTermOpSysCom(Buffer);
}

void vTermDisplayLocation(void) {
	xTermCursorRead();
	vTermLocate(1,110);
	char Buffer[16];
	char * pPos = Buffer;
	pPos = stpcpy(pPos, " [");
	pPos += xU32ToDecStr(sTI.SavY, pPos);
	pPos = stpcpy(pPos, ",");
	pPos += xU32ToDecStr(sTI.SavX, pPos);
	pPos = stpcpy(pPos, "] ");
	xTermPuts(Buffer, termBUILD_CTRL(1,1,1,termWAIT_MS));
	vTermLocate(sTI.SavY, sTI.SavX);
}

/* ################################# Cursor position updating ######################################
 * @brief 
 * @param cChr
 */
void xTermcheckcursor(void) {
	if (sTI.CurX >= sTI.MaxX) {
		sTI.CurX = 0;
		++sTI.CurY;
		if (sTI.CurY == sTI.MaxY) {
			sTI.CurY = sTI.MaxY - 1;
		}
	}
}

void xTermProcessChr(int cChr) {
	switch(cChr) {
	case CHR_LF:
		++sTI.CurY;
		if (sTI.CurY == sTI.MaxY) 
			sTI.CurY = sTI.MaxY - 1;
		break;
	case CHR_FF:
		sTI.CurX = sTI.CurY = 0;
		break;
	case CHR_CR:
		sTI.CurX = 0;
		break;
	case CHR_TAB:
		sTI.CurY = u32RoundUP(sTI.CurX, sTI.Tabs);
		xTermcheckcursor();
		break;
	default:
		if (INRANGE(CHR_SPACE, cChr, CHR_TILDE)) {
			++sTI.CurX;
			xTermcheckcursor();
		}
		break;
	}
}

/**
 * @brief	vTermSetSize() - set terminal row & column size (0 = reset to default)
 * @param	x columns
 * @param	y rows
 */
void vTermSetSize(u16_t x, u16_t y) {
	sTI.MaxX = (x < TERMINAL_MAX_X) ? x : TERMINAL_DFLT_X;
	sTI.MaxY = (y < TERMINAL_MAX_Y) ? y : TERMINAL_DFLT_Y;
}

void vTermGetInfo(terminfo_t * psTI) { memcpy(psTI, &sTI, sizeof(terminfo_t)); }

int xTermIdentify(void) {
	char Buffer[64];
	xTermPuts(termCSI "Z", termBUILD_CTRL(1,1,0,termWAIT_MS));
	int iRV = xTermGets(Buffer, sizeof(Buffer), 0x7F, termBUILD_CTRL(1,0,1,500));
	RPL(" [iRV=%d '%s']\r\n", iRV, Buffer);
	return erSUCCESS;
}

int xTermAttached(void) {
	if ((configCONSOLE_UART >= 0) && halUART_RxFifoUsed(configCONSOLE_UART) == 0) {
		halUART_Flush(configCONSOLE_UART);				// nothing to read, ensure TX buffer is empty
		putchar_cursor(CHR_ENQ, 1);
		for (int i = 0; i < 5; ++i) {
			vTaskDelay(pdMS_TO_TICKS(1));				// wait a short while for a response
			if (getchar() == CHR_ENQ) {
				// add code to determine specific terminal type.
				return 1;
			}
		}
	}
	return 0;
}

#if 0
// ################################### Graphical plot ##############################################

void	TerminalPlot(int16_t * dat, u32_t len) {
	u32_t	i;
	TerminalClear();
	for (i = 0; i < len; i++) { }
}
#endif

// ##################################### functional tests ##########################################

void vTermtestcode(void) {
	xTermPuts("Waiting to clear the screen...", termBUILD_CTRL(1,1,1,termWAIT_MS));
	vTermclearhome();
	vTermLocate(5, 5);
	xTermPuts("Normal text at 5,5", termBUILD_CTRL(1,1,1,termWAIT_MS));

	vTermLocate(7, 7);
	vTermAttrib(colourFG_WHITE, colourBG_BLACK);
	xTermPuts("White text on Black background at 7,7", termBUILD_CTRL(1,1,1,termWAIT_MS));

	vTermLocate(9, 9);
	vTermAttrib(attrREV_ON, attrULINE_ON);
	xTermPuts("Normal underlined text at 9,9", termBUILD_CTRL(1,1,1,termWAIT_MS));

	vTaskDelay(5000);
	vTermLocate(9, 19);
	xTermPuts("!!! OVERPRINTED TEXT !!!", termBUILD_CTRL(1,1,1,termWAIT_MS));

	vTaskDelay(5000);
	vTermLocate(9, 25);
	vTermclear2EOL();

	vTermAttrib(attrRESET, 0);
}
