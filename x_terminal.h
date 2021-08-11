/*
 * Copyright 2014-21 Andre M. Maree / KSS Technologies (Pty) Ltd.
 * x_terminal.h
 */

#pragma once

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
	struct __attribute__((packed)) {
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
