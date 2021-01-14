/*
 * Copyright 2014-18 Andre M Maree / KSS Technologies (Pty) Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/*
 * x_terminal.h
 */

#pragma once

#include	"hal_usart.h"
#include	<stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ###################################### BUILD : CONFIG definitions ##############################

#define	buildTERMINAL_CONTROLS_CURSOR		1

// ##################################### MACRO definitions #########################################

#define	TERMINAL_DEFAULT_MAX_X				80
#define	TERMINAL_DEFAULT_MAX_Y				25

// ####################################### structures ##############################################

typedef union {
	struct {
		uint8_t	CurX ;
		uint8_t	CurY ;
		uint8_t	MaxX ;
		uint8_t	MaxY ;
	} ;
	uint8_t		x8[4] ;
	uint32_t	x32 ;
} terminfo_t ;

// ###################################### Global variables #########################################


// ###################################### global functions ######################################

char *	pcANSIattrib(char * pBuf, uint8_t FG, uint8_t BG) ;
void	vANSIattrib(uint8_t FG, uint8_t BG) ;

char *	pcANSIlocate(char * pBuf, uint8_t Row, uint8_t Col) ;
void	vANSIlocate(uint8_t x, uint8_t y) ;

void 	vANSIhome(void) ;

void	vTerminalClear(void) ;
void	vANSIclear2EOL(void) ;
void	vANSIclear2BOL(void) ;
void	vANSIclear_line(void) ;
void	vANSIclearhome(void) ;

void	vTerminalSetSize(uint16_t x, uint16_t y) ;
void	vTerminalGetInfo(terminfo_t * psTermInfo) ;

void    vTerminalActivityShow(void) ;
void	vTerminalActivityLoop(void) ;

void	vANSItestcode(void) ;

#ifdef __cplusplus
}
#endif
