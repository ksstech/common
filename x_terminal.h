/*
 * x_terminal.h - Copyright (c) 2014-23 Andre M. Maree / KSS Technologies (Pty) Ltd.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// ###################################### BUILD : CONFIG definitions ##############################


// ##################################### MACRO definitions #########################################

#define	TERMINAL_DEFAULT_MAX_X				132
#define	TERMINAL_DEFAULT_MAX_Y				48

// ####################################### structures ##############################################

typedef union {
	struct __attribute__((packed)) {
		u8_t CurX;
		u8_t CurY;
		u8_t MaxX;
		u8_t MaxY;
	};
	u8_t x8[4];
	u32_t x32;
} terminfo_t;

// ###################################### Global variables #########################################


// ###################################### global functions ######################################

int	vANSIgets(char * pcBuf);
void vANSIputs(char * pStr);
void vANSIcursorsave(void);
void vANSIcursorback(void);
void vANSIcursorread(void);

void vANSIclear2EOL(void);
void vANSIclear2BOL(void);
void vANSIclearline(void);
void vANSIclearscreen(void);
void vANSIclearhome(void);

void vANSIhome(void) ;

char * pcANSIattrib(char * pBuf, u8_t FG, u8_t BG) ;
char * pcANSIlocate(char * pBuf, u8_t Row, u8_t Col) ;

int xANSIupdatecursor(int cChr);
void vANSIattrib(u8_t FG, u8_t BG);
void vANSIlocate(u8_t x, u8_t y);

void vTerminalSetSize(u16_t x, u16_t y) ;
void vTerminalGetInfo(terminfo_t * psTermInfo) ;

void vANSItestcode(void) ;

#ifdef __cplusplus
}
#endif
