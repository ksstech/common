/*
 * Copyright 2014-21 Andre M Maree / KSS Technologies (Pty) Ltd.
 *
 * x_terminal.c
 */

#include	"x_terminal.h"
#include	"x_definitions.h"
#include	"x_values_to_string.h"

#include	"hal_config.h"
#include	"hal_usart.h"

// #################################### Global/public variables ####################################

terminfo_t	sTI = {
	.CurX=0,
	.CurY=0,
	.MaxX=TERMINAL_DEFAULT_MAX_X,
	.MaxY=TERMINAL_DEFAULT_MAX_Y
} ;

// ################################# Terminal (VT100) support routines #############################

		}
	}
	return false ;
}

void	vTermSetForeground(uint8_t Colour) {
	char	Buffer[12] ;
	char * pTmp = Buffer ;
	*pTmp++	= CHR_ESC ;
	*pTmp++	= CHR_L_SQUARE ;
	pTmp += xU32ToDecStr(Colour + colourFOREGND, pTmp) ;
	*pTmp++ = CHR_m ;
	*pTmp = CHR_NUL ;									// terminate
	puts(Buffer) ;
}

void	vTermSetBackground(uint8_t Colour) {
char	Buffer[12] ;
char * pTmp = Buffer ;
	*pTmp++	= CHR_ESC ;
	*pTmp++	= CHR_L_SQUARE ;
	pTmp += xU32ToDecStr(Colour + colourBACKGND, pTmp) ;
	*pTmp++ = CHR_m ;
	*pTmp = CHR_NUL ;									// terminate
	puts(Buffer) ;
}

void	vTermSetForeBackground(uint8_t ColFG, uint8_t ColBG) {
char	Buffer[12] ;
char * pTmp = Buffer ;
	*pTmp++	= CHR_ESC ;
	*pTmp++	= CHR_L_SQUARE ;
	pTmp 	+= xU32ToDecStr(ColFG + colourFOREGND, pTmp) ;
	*pTmp++ = CHR_SEMICOLON ;
	pTmp 	+= xU32ToDecStr(ColBG + colourBACKGND, pTmp) ;
	*pTmp++ = CHR_m ;
	*pTmp = CHR_NUL ;									// terminate
	puts(Buffer) ;
}

//              1           2
//	0  12345678901234  567890
//	Esc[yyyyy;xxxxxHNul
void 	vTerminalLocate(uint16_t x, uint16_t y) {
char	Buffer[16] ;
char * pTmp = Buffer ;
	*pTmp++	= CHR_ESC ;
	*pTmp++	= CHR_L_SQUARE ;
	TermInfo.CurX = (x %= TermInfo.MaxX) ;
	TermInfo.CurY = (y %= TermInfo.MaxY) ;
	pTmp += xU32ToDecStr(y + 1, pTmp) ;
	*pTmp++ = ';' ;
	pTmp += xU32ToDecStr(x + 1, pTmp) ;
	*pTmp++ = 'H' ;
	*pTmp = 0 ;					// terminate
	puts(Buffer) ;
}

void 	vTerminalClear(void) { puts("\033[2J") ; }

void 	vTerminalHome(void) { puts("\033[1;1H" ) ; TermInfo.CurX = TermInfo.CurY = 0 ; }

void 	vTerminalClear2EOL(void) { puts("\033[s\033[0K\033[u" ) ; }			// save cursor, then erase to EOL & restore cursor

void 	vTerminalClear2BOL(void) { puts("\033[1K" ) ; }

void 	vTerminalClearLine(void) { puts("\033[2K" ) ; }

void 	vTerminalClearHome(void) { vTerminalClear() ; vTerminalHome() ; }

/**
 * vTerminalSetSize() - set terminal row & column size (0 = reset to default)
 * @param x		columns
 * @param y		rows
 */
void	vTerminalSetSize(uint16_t x, uint16_t y) {
	sTI.MaxX = x ? x : TERMINAL_DEFAULT_MAX_X ;
	sTI.MaxY = y ? y : TERMINAL_DEFAULT_MAX_Y ;
}

void	vTerminalGetInfo(terminfo_t * psTI) { psTI->x32 = sTI.x32 ; }

#if 0
// ###################################### terminal IO related ######################################

static	char		ActivityChr[]   = { ' ', '*', 'x', '+', 'X' } ;
static 	int32_t		ActivityIdx     = 0 ;

/*
 * vTerminalActivityShow
 * \brief	sequentially (per call) display a character to indicate activity
 *			before each character comes a CR to ensure cursor remain at Column 0 of current line
 * \param	None
 * \return	None
 */
void	vTerminalActivityShow(void) {
	putchar('\r') ;
    putchar(ActivityChr[ActivityIdx++]) ;
    ActivityIdx %= sizeof(ActivityChr) ;
    putchar('\r') ;
}

/*
 * vTerminalActivityLoop
 * \brief	flush the console buffer then continually display the activity chars
 *			whilst waiting for a key from the console before returning
 * \param	None
 * \return	None
 */
void	vTerminalActivityLoop(void) {
  	while (gotchar() != EOF) ;						// first flush the UART receive buffer
	while (getchar() == 0) {						// then start waiting for another key to be pressed
		vTerminalActivityShow() ;
	}
}

// ################################### Graphical plot ##############################################

int32_t	TerminalCursorGet(uint8_t * CurX, uint8_t * CurY) {
uint8_t		Buf[16] ;
	puts("\033[6n") ;
	gets(buf);
	// Add logic to read the buffer and return a string
	// see http://www.acm.uiuc.edu/webmonkeys/book/c_guide/2.12.html#gets
	return SUCCESS ;
}

void	TerminalPlot(int16_t * dat, uint32_t len) {
uint32_t	i ;
//	TerminalClear() ;
//	for (i = 0; i < len; i++) { }
}
#endif

// ##################################### functional tests ##########################################

void	vANSItestcode(void) {
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
