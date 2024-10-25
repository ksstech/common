// x_terminal.c - Copyright (c) 2014-24 Andre M. Maree / KSS Technologies (Pty) Ltd.

#include "hal_platform.h"
#include "hal_stdio.h"
#include "hal_usart.h"
#include "x_errors_events.h"
#include "printfx.h"
#include "x_terminal.h"
#include "x_utilities.h"

// ##################################### MACRO definitions #########################################

// ######################################## Enumerations ###########################################
// ###################################### Global variables #########################################

static terminfo_t sTI = {
	.CurX = 0, .CurY = 0, .SavX = 0, .SavY = 0,
	.MaxX = TERMINAL_DFLT_X, .MaxY = TERMINAL_DFLT_Y, .Tabs = TERMINAL_DFLT_TAB,
};

// ################################# Terminal (VT100) support routines #############################
//	https://invisible-island.net/xterm/ctlseqs/ctlseqs.html
//	http://docs.smoe.org/oreilly/unix_lib/upt/ch05_05.htm
//	http://www.acm.uiuc.edu/webmonkeys/book/c_guide/2.12.html#gets

inline int xTermGetCurColX(void) { return sTI.CurX; };
inline int xTermGetCurRowY(void) { return sTI.CurY; };

inline int xTermGetMaxColX(void) { return sTI.MaxX; };
inline int xTermGetMaxRowY(void) { return sTI.MaxY; };

inline void vTermPushCurColRow(void) { sTI.SavY = sTI.CurY; sTI.SavX = sTI.CurX; }
inline void vTermPullCurColRow(void) { sTI.CurY = sTI.SavY; sTI.CurX = sTI.SavX; }
inline void vTermSetCurColRow(u16_t ColX, u16_t RowY) { sTI.CurY = RowY; sTI.CurX = ColX; };

inline void vTermPushMaxColRow(void) { sTI.SavY = sTI.MaxY; sTI.SavX = sTI.MaxX; }
inline void vTermPullMaxColRow(void) { sTI.MaxY = sTI.SavY; sTI.MaxX = sTI.SavX; }
inline void vTermSetMaxColRow(u16_t ColX, u16_t RowY) { sTI.MaxY = RowY; sTI.MaxX = ColX; };

void vTermSetSize(u16_t RowY, u16_t ColX) {
    if (RowY && ColX) {
    	sTI.MaxX = (ColX < TERMINAL_MAX_X) ? ColX : TERMINAL_DFLT_X;
	    sTI.MaxY = (RowY < TERMINAL_MAX_Y) ? RowY : TERMINAL_DFLT_Y;
    }
}

void vTermGetInfo(terminfo_t * psTI) { memcpy(psTI, &sTI, sizeof(terminfo_t)); }

/**
 * @brief	Check column and adjust column & row if required
 */
void vTermCheckCursor(void) {
	if (sTI.CurX >= sTI.MaxX) {
		sTI.CurX = 1;
		if (sTI.CurY < sTI.MaxY) ++sTI.CurY;
	}
}

void xTermProcessChr(int cChr) {
	switch(cChr) {
	case CHR_BS:
		--sTI.CurX;
		if (sTI.CurX == 0) {
			sTI.CurX = sTI.MaxX;
			if (sTI.CurY > 1) --sTI.CurY;
		}
		break;
	case CHR_TAB:
		sTI.CurX = u32RoundUP(sTI.CurX, sTI.Tabs);
		vTermCheckCursor();
		break;
	case CHR_LF:
		#if (CONFIG_NEWLIB_STDOUT_LINE_ENDING_LF == 1)
			sTI.CurX = 0;
		#endif
		++sTI.CurY;
		if (sTI.CurY == sTI.MaxY) sTI.CurY = sTI.MaxY - 1;
		break;
	case CHR_FF:
		sTI.CurX = sTI.CurY = 0;
		break;
	case CHR_CR:
		sTI.CurX = 0;
		#if (CONFIG_NEWLIB_STDOUT_LINE_ENDING_CR == 1)
			++sTI.CurY;
			if (sTI.CurY == sTI.MaxY) sTI.CurY = sTI.MaxY - 1;
		#endif
		break;
	default:
		if (INRANGE(CHR_SPACE, cChr, CHR_TILDE)) {
			++sTI.CurX;
			vTermCheckCursor();
		}
		break;
	}
}

int xTermPuts(char * pBuf, termctrl_t Ctrl) {
	int iRV = 0;
	if (Ctrl.Lock) halUartLock(pdMS_TO_TICKS(Ctrl.Wait));
	while (*pBuf) {
		__real_putchar(*pBuf++);
		++iRV;
	}
	if (Ctrl.Unlock) halUartUnLock();
	return iRV;
}

int	xTermGets(char * pBuf, size_t Size, int Match, termctrl_t Ctrl) { 
	int iRV, Len = 0;
	TickType_t tNow = 0, tStart = xTaskGetTickCount();
	if (Ctrl.Lock) halUartLock(pdMS_TO_TICKS(Ctrl.Wait));
	do {
		iRV = __real_getchar();
		if (iRV != EOF) {
			pBuf[Len++] = iRV;
			if (Len == Size || iRV == Match) break;
			continue;
		}
		taskYIELD();
		tNow = xTaskGetTickCount() - tStart;
	} while (tNow < pdMS_TO_TICKS(Ctrl.Wait));
	if (Len < (Size - 1)) pBuf[Len] = CHR_NUL;
	if (Ctrl.Unlock) halUartUnLock();
	return Len;
}

int xTermQuery(char * pccQuery, char * pBuf, size_t Size, int Match) {
	int iRV1 = xTermPuts(pccQuery, termBUILD_CTRL(1, 0, termWAIT_MS));
	int iRV2 = xTermGets(pBuf, Size, Match, termBUILD_CTRL(0, 1, 500));
	PX(" [iRV=%d/%d %'+-hhY]" strNL, iRV1, iRV2, iRV2, pBuf);
	return iRV2;
}

char * pcTermAttrib(char * pBuf, u16_t a1, u16_t a2) {
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

void vTermAttrib(u16_t a1, u16_t a2) {
	char Buffer[sizeof("E[yyy;xxxH0")];
	if (pcTermAttrib(Buffer, a1, a2) != Buffer) xTermPuts(Buffer, termBUILD_CTRL(1,1,termWAIT_MS));
}

char * pcTermLocate(char * pBuf, u16_t RowY, u16_t ColX) {
	if (RowY && ColX) {
		pBuf = stpcpy(pBuf, termCSI);
		pBuf += xU32ToDecStr(RowY, pBuf);
		*pBuf++	= CHR_SEMICOLON;
		pBuf += xU32ToDecStr(ColX, pBuf);
		*pBuf++	= CHR_H;
		sTI.CurY = RowY % (sTI.MaxY + 1);
		sTI.CurX = ColX % (sTI.MaxX + 1);
	}
	*pBuf = 0;
	return pBuf;
}

void _vTermLocate(u16_t RowY, u16_t ColX, termctrl_t sTC) {
	char Buffer[sizeof("\e[yyy;xxxH\0")];
	if (pcTermLocate(Buffer, RowY, ColX) != Buffer) xTermPuts(Buffer, sTC);
}

void vTermLocate(u16_t RowY, u16_t ColX) { _vTermLocate(RowY, ColX, termBUILD_CTRL(1,1,termWAIT_MS)); }

int xTermLocatePuts(u16_t RowY, u16_t ColX, char * pBuf) {
	_vTermLocate(RowY, ColX, termBUILD_CTRL(1, 0, termWAIT_MS));
	return xTermPuts(pBuf, termBUILD_CTRL(0, 1, termWAIT_MS));
}

void vTermCursorSave(void) { xTermPuts(setCURSOR_SAVE, termBUILD_CTRL(1, 1, termWAIT_MS)); }

void vTermCursorBack(void) { xTermPuts(setCURSOR_REST, termBUILD_CTRL(1, 1, termWAIT_MS)); }

void vTermClear2EOL(void) { xTermPuts(setCLRLIN_RIGHT, termBUILD_CTRL(1, 1, termWAIT_MS)); }

void vTermClear2BOL(void) { xTermPuts(setCLRLIN_LEFT, termBUILD_CTRL(1, 1, termWAIT_MS)); }

void vTermClearline(void) { xTermPuts(setCLRLIN_ALL, termBUILD_CTRL(1, 1, termWAIT_MS)); }

void vTermClearScreen(void) { xTermPuts(setCLRDSP_ALL, termBUILD_CTRL(1, 1, termWAIT_MS)); }

void vTermHome(void) { 
	xTermPuts(setCURSOR_HOME, termBUILD_CTRL(1, 1, termWAIT_MS));
	sTI.CurX = sTI.CurY = 1;
}

void vTermClearHome(void) { vTermClearScreen(); vTermHome(); }

void vTermOpSysCom(char * pBuf) {
	xTermPuts(termOSC, termBUILD_CTRL(1, 0, termWAIT_MS));
	xTermPuts(pBuf, termBUILD_CTRL(0, 0, 0));
	xTermPuts(termST, termBUILD_CTRL(0, 1, 0));
}

void vTermWinTleCursor(void) {
	char Buffer[16];
	char * pBuf = Buffer;
	pBuf = stpcpy(pBuf, "0;R=");
	pBuf += xU32ToDecStr(sTI.CurY, pBuf);
	pBuf = stpcpy(pBuf, " C=");
	pBuf += xU32ToDecStr(sTI.CurX, pBuf);
	vTermOpSysCom(Buffer);
}

void vTermDisplayLocation(void) {
	char Buffer[16];
	char * pBuf = Buffer;
	pBuf = stpcpy(pBuf, " [");
	pBuf += xU32ToDecStr(sTI.SavY, pBuf);
	pBuf = stpcpy(pBuf, ",");
	pBuf += xU32ToDecStr(sTI.SavX, pBuf);
	pBuf = stpcpy(pBuf, "] ");
	int RowY = sTI.MaxY - 10;
	int ColX = sTI.MaxX - (pBuf - Buffer);
//	PX("R=%d  C=%d  %s" strNL, RowY, ColX, Buffer);
	xTermLocatePuts(RowY, ColX, Buffer);
	_vTermLocate(sTI.SavY, sTI.SavX, termBUILD_CTRL(1, 1, termWAIT_MS));
}

int xTermCursorRead(void) {
	char Buffer[16];
	int iRV = xTermQuery(getCURSOR_POS, Buffer, sizeof(Buffer), 'R');
	if (iRV < 6) return erFAILURE;
	u16_t RowY = 0, ColX = 0;
	iRV = sscanf(Buffer, termCSI "%hu;%huR", &RowY, &ColX);
//	PX("[iRV=%d RowY=%d ColX=%d]" strNL, iRV, RowY, ColX);
	sTI.CurY = sTI.SavY = RowY;
	sTI.CurX = sTI.SavX = ColX;
	return iRV;
}

int xTermIdentify(void) {
	char Buffer[16];
	return xTermQuery(getDEVICE_ATTR, Buffer, sizeof(Buffer), 'c');
}

int xTermAttached(void) {
	if ((configCONSOLE_UART >= 0) && halUART_RxFifoUsed(configCONSOLE_UART) == 0) {
		halUART_Flush(configCONSOLE_UART);				// nothing to read, ensure TX buffer is empty
		putchar(CHR_ENQ);
		for (int i = 0; i < 5; ++i) {
			vTaskDelay(pdMS_TO_TICKS(1));				// wait a short while for a response
			if (getchar() == CHR_ENQ) return 1;
		}
	}
	return 0;
}

// ##################################### functional tests ##########################################

void vTermTestCode(void) {
	xTermPuts("Waiting to clear the screen...", termBUILD_CTRL(1,1,termWAIT_MS));
	vTermClearHome();
	vTermLocate(5, 5);
	xTermPuts("Normal text at 5,5", termBUILD_CTRL(1,1,termWAIT_MS));

	vTermLocate(7, 7);
	vTermAttrib(attrRESET, colourFG_WHITE);
	xTermPuts("White text on Black background at 7,7", termBUILD_CTRL(1,1,termWAIT_MS));

	vTermLocate(9, 9);
	vTermAttrib(attrREV_ON, attrULINE_ON);
	xTermPuts("Normal underlined text at 9,9", termBUILD_CTRL(1,1,termWAIT_MS));

	vTaskDelay(5000);
	vTermLocate(9, 19);
	xTermPuts("!!! OVERPRINTED TEXT !!!", termBUILD_CTRL(1,1,termWAIT_MS));

	vTaskDelay(5000);
	vTermLocate(9, 25);
	vTermClear2EOL();

	vTermAttrib(attrRESET, 0);
}
