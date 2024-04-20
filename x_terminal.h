// x_terminal.h

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// ##################################### MACRO definitions #########################################

#define	TERMINAL_DEFAULT_MAX_X				132
#define	TERMINAL_DEFAULT_MAX_Y				48

// ####################################### structures ##############################################

typedef union {
	struct __attribute__((packed)) {
		u8_t CurX, CurY, SavX, SavY, MaxX, MaxY, Tabs;
	};
} terminfo_t;



// ###################################### Public functions #########################################

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

void xANSIupdatecursor(int cChr);
void vANSIattrib(u8_t FG, u8_t BG);
void vANSIlocate(u8_t x, u8_t y);

void vTerminalSetSize(u16_t x, u16_t y) ;
void vTerminalGetInfo(terminfo_t * psTermInfo) ;

void vANSItestcode(void) ;

#ifdef __cplusplus
}
#endif
