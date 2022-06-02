/*
 * Copyright 2014-22 (c) Andre M. Maree / KSS Technologies (Pty) Ltd.
 * x_terminal.c
 */

#include	"hal_config.h"
#include	"hal_usart.h"

#include	"x_terminal.h"
#include	"x_utilities.h"

// #################################### Global/public variables ####################################

terminfo_t	sTI = {
	.CurX=0,
	.CurY=0,
	.MaxX=TERMINAL_DEFAULT_MAX_X,
	.MaxY=TERMINAL_DEFAULT_MAX_Y
} ;

// ################################# Terminal (VT100) support routines #############################

int	vANSIgets(char * pcBuf) { return 0 ; }

void vANSIputs(char * pStr) { while (*pStr) putcharX(*pStr++, configSTDIO_UART_CHAN); }

void vANSIcursorsave(void) { vANSIputs("\033[s") ; }

void vANSIcursorback(void) { vANSIputs("\033[u") ; }

void vANSIcursorread(void) {
	char Buf[16] ;
	vANSIputs("\033[6n") ;
	vANSIgets(Buf);
	// Add logic to read the buffer and return a string
	// see http://www.acm.uiuc.edu/webmonkeys/book/c_guide/2.12.html#gets
}

void vANSIclear2EOL(void) { vANSIputs("\033[0K") ; }

void vANSIclear2BOL(void) { vANSIputs("\033[1K") ; }

void vANSIclearline(void) { vANSIputs("\033[2K") ; }

void vANSIclearscreen(void) { vANSIputs("\033[2J") ; }

void vANSIclearhome(void) { vANSIclearscreen() ; vANSIhome() ; }

void vANSIhome(void) { vANSIputs("\033[1;1H") ; sTI.CurX = sTI.CurY = 1 ; }

char * pcANSIattrib(char * pBuf, u8_t a1, u8_t a2) {
	if (a1 <= colourBG_WHITE && a2 <= colourBG_WHITE) {
		*pBuf++	= CHR_ESC;
		*pBuf++	= CHR_L_SQUARE;
		pBuf += xU32ToDecStr(a1, pBuf);
		if (a2) {
			*pBuf++ = CHR_SEMICOLON;
			pBuf += xU32ToDecStr(a2, pBuf);
		}
		*pBuf++ = CHR_m;
	}
	*pBuf = 0;									// terminate
	return pBuf;
}

char * pcANSIlocate(char * pBuf, u8_t Row, u8_t Col) {
	if (Row > 0 && Col > 0) {
		*pBuf++	= CHR_ESC;
		*pBuf++	= CHR_L_SQUARE;
		pBuf	+= xU32ToDecStr(Row, pBuf);
		*pBuf++	= CHR_SEMICOLON;
		pBuf	+= xU32ToDecStr(Col, pBuf);
		*pBuf++	= CHR_H;
		sTI.CurX = (Col %= sTI.MaxX);
		sTI.CurY = (Row %= sTI.MaxY);
	}
	*pBuf = 0;
	return pBuf;
}

int xANSIupdatecursor(int cChr) {
	if (cChr == CHR_FF) {
		sTI.CurX = sTI.CurY = 0;
	} else if (cChr == CHR_CR) {
		sTI.CurX = 0;
	} else if (cChr == CHR_LF) {
		++sTI.CurY;
		if (sTI.CurY == sTI.MaxY)
			sTI.CurY = sTI.MaxY - 1;
	} else if (cChr != CHR_NUL) {
		++sTI.CurX;
		if (sTI.CurX == sTI.MaxX) {
			sTI.CurX = 0;
			++sTI.CurY;
			if (sTI.CurY == sTI.MaxY)
				sTI.CurY = sTI.MaxY - 1;
		}
	}
	return cChr;
}

void vANSIattrib(u8_t a1, u8_t a2) {
	char Buffer[sizeof("E[yyy;xxxH0")] ;
	if (pcANSIattrib(Buffer, a1, a2) != Buffer) vANSIputs(Buffer) ;
}

void vANSIlocate(u8_t Row, u8_t Col) {
	char Buffer[sizeof("E[yyy;xxxH0")];
	if (pcANSIlocate(Buffer, Row, Col) != Buffer) vANSIputs(Buffer);
}

/**
 * vTerminalSetSize() - set terminal row & column size (0 = reset to default)
 * @param x		columns
 * @param y		rows
 */
void vTerminalSetSize(u16_t x, u16_t y) {
	sTI.MaxX = x ? x : TERMINAL_DEFAULT_MAX_X ;
	sTI.MaxY = y ? y : TERMINAL_DEFAULT_MAX_Y ;
}

void vTerminalGetInfo(terminfo_t * psTI) { psTI->x32 = sTI.x32 ; }

int xTerminalAttached(void) {
	int iRV = 0;
	if (halUART_RxFifoUsed(configSTDIO_UART_CHAN) == 0) {
		halUART_Flush(configSTDIO_UART_CHAN) ;			// nothing to read, ensure TX buffer is empty
		putcharX(CHR_ENQ, configSTDIO_UART_CHAN) ;		// send ENQ to illicit a response
		for (int i = 0; i < 5; ++i) {
			vTaskDelay(pdMS_TO_TICKS(1)) ;				// wait a short while for a response
			if (getcharX(configSTDIO_UART_CHAN) == CHR_ENQ) {
				iRV = 1;
			}
		}
	}
	// add code to determine specific terminal type.
	return (iRV == 0) ? iRV : false;
}

#if 0
// ################################### Graphical plot ##############################################

void	TerminalPlot(int16_t * dat, u32_t len) {
	u32_t	i ;
	TerminalClear() ;
	for (i = 0; i < len; i++) { }
}
#endif

// ##################################### functional tests ##########################################

void vANSItestcode(void) {
	vANSIputs("Waiting to clear the screen...") ;

	vTaskDelay(5000) ;

	vANSIclearhome() ;
	vANSIlocate(5, 5) ;
	vANSIputs("Normal text at 5,5") ;

	vANSIlocate(7, 7) ;
	vANSIattrib(colourFG_WHITE, colourBG_BLACK) ;
	vANSIputs("White text on Black background at 7,7") ;

	vANSIlocate(9, 9) ;
	vANSIattrib(attrREV_ON, attrULINE_ON) ;
	vANSIputs("Normal underlined text at 9,9") ;

	vTaskDelay(5000) ;
	vANSIlocate(9, 19) ;
	vANSIputs("!!! OVERPRINTED TEXT !!!") ;

	vTaskDelay(5000) ;
	vANSIlocate(9, 25) ;
	vANSIclear2EOL() ;

	vANSIattrib(attrRESET, 0) ;
}
