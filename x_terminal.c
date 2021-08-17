/*
 * Copyright 2014-21 Andre M. Maree / KSS Technologies (Pty) Ltd.
 * x_terminal.c
 */

#include	"x_terminal.h"
#include	"hal_config.h"
#include	"x_stdio.h"
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

void vANSIputs(char * pStr) { while (*pStr) putcx(*pStr++, configSTDIO_UART_CHAN); }

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

char * pcANSIattrib(char * pBuf, uint8_t a1, uint8_t a2) {
	if (a1 <= colourBG_WHITE && a2 <= colourBG_WHITE) {
		*pBuf++	= 0x1B ;
		*pBuf++	= '[' ;
		pBuf += xU32ToDecStr(a1, pBuf) ;
		if (a2) {
			*pBuf++ = ';' ;
			pBuf += xU32ToDecStr(a2, pBuf) ;
		}
		*pBuf++ = 'm' ;
	}
	*pBuf = 0 ;									// terminate
	return pBuf ;
}

char * pcANSIlocate(char * pBuf, uint8_t Row, uint8_t Col) {
	if (Row > 0 && Col > 0) {
		*pBuf++	= 0x1B ;
		*pBuf++	= '[' ;
		pBuf	+= xU32ToDecStr(Row, pBuf) ;
		*pBuf++	= ';' ;
		pBuf	+= xU32ToDecStr(Col, pBuf) ;
		*pBuf++	= 'H' ;
		sTI.CurX = (Col %= sTI.MaxX) ;
		sTI.CurY = (Row %= sTI.MaxY) ;
	}
	*pBuf = 0;
	return pBuf;
}

void vANSIattrib(uint8_t a1, uint8_t a2) {
	char Buffer[sizeof("E[yyy;xxxH0")] ;
	if (pcANSIattrib(Buffer, a1, a2) != Buffer) vANSIputs(Buffer) ;
}

void vANSIlocate(uint8_t Row, uint8_t Col) {
	char Buffer[sizeof("E[yyy;xxxH0")];
	if (pcANSIlocate(Buffer, Row, Col) != Buffer) vANSIputs(Buffer);
}

/**
 * vTerminalSetSize() - set terminal row & column size (0 = reset to default)
 * @param x		columns
 * @param y		rows
 */
void vTerminalSetSize(uint16_t x, uint16_t y) {
	sTI.MaxX = x ? x : TERMINAL_DEFAULT_MAX_X ;
	sTI.MaxY = y ? y : TERMINAL_DEFAULT_MAX_Y ;
}

void vTerminalGetInfo(terminfo_t * psTI) { psTI->x32 = sTI.x32 ; }

#if 0
// ################################### Graphical plot ##############################################

void	TerminalPlot(int16_t * dat, uint32_t len) {
	uint32_t	i ;
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
