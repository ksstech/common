// terminalX.c - Copyright (c) 2014-25 Andre M. Maree / KSS Technologies (Pty) Ltd.

#include "hal_stdio.h"
#include "hal_usart.h"
#include "errors_events.h"
#include "printfx.h"
#include "terminalX.h"
#include "utilitiesX.h"

#include <string.h>

// ##################################### MACRO definitions #########################################

// ######################################## Enumerations ###########################################

// ###################################### Global variables #########################################

// ################################# Terminal (VT100) support routines #############################


void vTermGetInfo(terminfo_t * psTI) { memcpy(psTI, &sTI, sizeof(terminfo_t)); }

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
		xTermPuts(Buffer, termBUILD_CTRL(1,1,termWAIT_MS));
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
	if (pcTermLocate(Buffer, RowY, ColX) != Buffer)
		xTermPuts(Buffer, sTC);
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
