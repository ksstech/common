// terminalX.c - Copyright (c) 2014-25 Andre M. Maree / KSS Technologies (Pty) Ltd.

#include "hal_platform.h"
#include "hal_stdio.h"
#include "hal_usart.h"
#include "errors_events.h"
#include "printfx.h"
#include "terminalX.h"
#include "utilitiesX.h"

// ##################################### MACRO definitions #########################################

// ######################################## Enumerations ###########################################

// ###################################### Global variables #########################################

static terminfo_t sTI = {
	.mux = NULL, .CurX = 1, .CurY = 1, .SavX = 1, .SavY = 1,
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

inline void vTermPushCurRowYColX(void) { sTI.SavY = sTI.CurY; sTI.SavX = sTI.CurX; }
inline void vTermPullCurRowYColX(void) { sTI.CurY = sTI.SavY; sTI.CurX = sTI.SavX; }
inline void vTermSetCurRowYColX(i16_t RowY, i16_t ColX) { sTI.CurY = RowY; sTI.CurX = ColX; };

inline void vTermPushMaxRowYColX(void) { sTI.SavY = sTI.MaxY; sTI.SavX = sTI.MaxX; }
inline void vTermPullMaxRowYColX(void) { sTI.MaxY = sTI.SavY; sTI.MaxX = sTI.SavX; }
inline void vTermSetMaxRowYColX(i16_t RowY, i16_t ColX) { sTI.MaxY = RowY; sTI.MaxX = ColX; };

void vTermSetSize(u16_t RowY, u16_t ColX) {
    if (RowY && ColX) {
	    sTI.MaxY = (RowY < TERMINAL_MAX_Y) ? RowY : TERMINAL_DFLT_Y;
    	sTI.MaxX = (ColX < TERMINAL_MAX_X) ? ColX : TERMINAL_DFLT_X;
    }
}

void vTermGetInfo(terminfo_t * psTI) { memcpy(psTI, &sTI, sizeof(terminfo_t)); }

/**
 * @brief	Check column and adjust column & row if required
 */
static void vTermCheckCursor(void) {
	if (sTI.CurX >= sTI.MaxX) {
		sTI.CurX = 1;
		if (sTI.CurY < sTI.MaxY) ++sTI.CurY;
	}
}

void xTermProcessChr(int cChr) {
	xRtosSemaphoreTake(&sTI.mux, portMAX_DELAY);
	switch(cChr) {
	case CHR_BS:
		--sTI.CurX;
		if (sTI.CurX == 0) {
			sTI.CurX = sTI.MaxX;
			if (sTI.CurY > 1)
				--sTI.CurY;
		}
		break;
	case CHR_TAB:
		sTI.CurX = u32RoundUP(sTI.CurX, sTI.Tabs);
		vTermCheckCursor();
		break;
	case CHR_LF:
		#if (CONFIG_NEWLIB_STDOUT_LINE_ENDING_LF == 1 || CONFIG_NEWLIB_STDOUT_LINE_ENDING_CRLF == 1)
			sTI.CurX = 1;
		#endif
		if (sTI.CurY < sTI.MaxY)
			++sTI.CurY;
		break;
	case CHR_FF:
		sTI.CurX = sTI.CurY = 1;
		break;
	case CHR_CR:
		sTI.CurX = 1;
		#if (CONFIG_NEWLIB_STDOUT_LINE_ENDING_CR == 1 || CONFIG_NEWLIB_STDOUT_LINE_ENDING_CRLF == 1)
			if (sTI.CurY < sTI.MaxY)
				++sTI.CurY;
		#endif
		break;
	default:
		if (INRANGE(CHR_SPACE, cChr, CHR_TILDE)) {
			++sTI.CurX;
			vTermCheckCursor();
		}
		break;
	}
	xRtosSemaphoreGive(&sTI.mux);
}

int xTermPuts(char * pBuf, termctrl_t Ctrl) {
	int iRV = 0;
	if (Ctrl.Lock)
		halUartLock(pdMS_TO_TICKS(Ctrl.Wait));
	while (*pBuf) {
		fputc(*pBuf++, stdout);
		++iRV;
	}
	if (Ctrl.Unlock)
		halUartUnLock();
	return iRV;
}

int	xTermGets(char * pBuf, size_t Size, int Match, termctrl_t Ctrl) { 
	int iRV, Len = 0;
	TickType_t tNow = 0, tStart = xTaskGetTickCount();
	if (Ctrl.Lock)
		halUartLock(pdMS_TO_TICKS(Ctrl.Wait));
	do {
		iRV = fgetc(stdin);
		if (iRV != EOF) {
			pBuf[Len++] = iRV;
			if (Len == Size || iRV == Match)
				break;
			continue;
		}
		taskYIELD();
		tNow = xTaskGetTickCount() - tStart;
	} while (tNow < pdMS_TO_TICKS(Ctrl.Wait));
	if (Len < (Size - 1))
		pBuf[Len] = CHR_NUL;
	if (Ctrl.Unlock)
		halUartUnLock();
	return Len;
}

int xTermQuery(char * pccQuery, char * pBuf, size_t Size, int Match) {
	xTermPuts(pccQuery, termBUILD_CTRL(1, 0, termWAIT_MS));
	return xTermGets(pBuf, Size, Match, termBUILD_CTRL(0, 1, 500));
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
	if (pcTermAttrib(Buffer, a1, a2) != Buffer) xTermPuts(Buffer, termBUILD_CTRL(1,1,termWAIT_MS));
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

void _vTermLocate(i16_t RowY, i16_t ColX, termctrl_t sTC) {
	char Buffer[sizeof("\e[yyy;xxxH\0")];
	if (pcTermLocate(Buffer, RowY, ColX) != Buffer) xTermPuts(Buffer, sTC);
}

void vTermLocate(i16_t RowY, i16_t ColX) { _vTermLocate(RowY, ColX, termBUILD_CTRL(1,1,termWAIT_MS)); }

int xTermLocatePuts(i16_t RowY, i16_t ColX, char * pBuf) {
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

void xTermShowCurRowYColX(i16_t RowY, i16_t ColX) {
	#define fmtCURSOR_DISP setCURSOR_SAVE termCSI "%d;%dH [%d,%d] " setCURSOR_REST
	char Buffer[32];
	if (RowY > sTI.MaxY)	RowY = sTI.MaxY;
	else if (RowY == 0)		RowY = sTI.MaxY;
	else if (RowY == -1)	RowY = sTI.MaxY / 2;		// Middle line/row
	else if (RowY < -1)		RowY = 1;
	int iRV = xDigitsInU32(sTI.CurY, 0) + xDigitsInU32(sTI.CurX, 0) + 5; 
	if (ColX > sTI.MaxX)	ColX = sTI.MaxX;
	else if (ColX == 0)		ColX = sTI.MaxX-iRV;		// furthest right without wrap	
	else if (ColX == -1)	ColX = ((sTI.MaxX-iRV)/2);	// center justified	
	else if (ColX < -1)		ColX = 1;	
	iRV = snprintfx(Buffer, sizeof(Buffer), fmtCURSOR_DISP, RowY, ColX, sTI.CurY, sTI.CurX);
	xTermPuts(Buffer, termBUILD_CTRL(1, 1, termWAIT_MS));
}

int _xTermReadRowYColX(char * pReq, i16_t * pRowY, i16_t * pColX) {
	char Buffer[16];
	int iRV = xTermQuery(pReq, Buffer, sizeof(Buffer), 'R');
	if (iRV < 6)
		return erFAILURE;
	iRV = sscanf(Buffer, termCSI "%hu;%huR", pRowY, pColX);
//	PX("[iRV=%d  RowY=%d  ColX=%d]" strNL, iRV, *pRowY, *pColX);
	return iRV;
}

int xTermReadCurRowYColX(void) { return _xTermReadRowYColX(getCURSOR_POS, &sTI.CurY, &sTI.CurX); }

void xTermReadMaxRowYColX(void) { _xTermReadRowYColX(getCURSOR_MAX, &sTI.MaxY, &sTI.MaxX); }

int xTermIdentify(void) {
	char Buffer[16];
	return xTermQuery(getDEVICE_ATTR, Buffer, sizeof(Buffer), 'c');
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
