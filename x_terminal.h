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
		uint16_t	CurX ;
		uint16_t	CurY ;
		uint16_t	MaxX ;
		uint16_t	MaxY ;
	} ;
	uint16_t	x16[4] ;
	uint64_t	x64 ;
} terminfo_t ;

// ###################################### Global variables #########################################


// ###################################### global functions ######################################

bool	bTerminalConnected(uart_port_t) ;
void	vTermSetForeground(uint8_t Colour) ;
void	vTermSetBackground(uint8_t Colour) ;
void	vTermSetForeBackground(uint8_t ColFG, uint8_t ColBG) ;

void	vTerminalLocate(uint16_t x, uint16_t y) ;
void 	vTerminalHome(void) ;

void	vTerminalClear(void) ;
void	vTerminalClear2EOL(void) ;
void	vTerminalClear2BOL(void) ;
void	vTerminalClearLine(void) ;
void	vTerminalClearHome(void) ;

void	vTerminalSetSize(uint16_t x, uint16_t y) ;
void	vTerminalGetInfo(terminfo_t * psTermInfo) ;

void    vTerminalActivityShow(void) ;
void	vTerminalActivityLoop(void) ;

#ifdef __cplusplus
}
#endif
