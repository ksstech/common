// x_terminal.h

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// ##################################### MACRO definitions #########################################

#define	TERMINAL_DFLT_X				160
#define	TERMINAL_DFLT_Y				48
#define TERMINAL_DFLT_TAB			8

#define	TERMINAL_MAX_X				255
#define	TERMINAL_MAX_Y				63
#define TERMINAL_MAX_TAB			15

#define	termWAIT_MS					100

#define termFLAG_CURSOR				(1 << 0)
#define termFLAG_LOCK				(1 << 1)
#define termFLAG_UNLOCK				(1 << 2)

#define termFLAG_ALL				(termFLAG_CURSOR|termFLAG_LOCK|termFLAG_UNLOCK)

#define termBUILD_CTRL(NC,LO,UL,WAIT) ((termctrl_t) { .NoCursor = NC, .Lock = LO, .Unlock = UL, .Wait = WAIT } )

// ####################################### structures ##############################################

typedef union {
	struct __attribute__((packed)) {
		u8_t CurX, CurY, SavX, SavY, MaxX, MaxY, Tabs;
	};
} terminfo_t;

typedef struct __attribute__((packed)) termctrl_t {
	union {
		struct { bool NoCursor; bool Lock; bool Unlock; u16_t Spare:13; };
		u16_t u16Val;
	};
	u16_t Wait;
} termctrl_t;

// ###################################### Public variables #########################################


// ###################################### Public functions #########################################

int	xTermGets(char * pcBuf, size_t Size, int Match, termctrl_t flag);
int xTermPuts(char * pStr, termctrl_t flag);
void vTermCursorSave(void);
void vTermCursorBack(void);
int xTermCursorRead(void);

void vTermclear2EOL(void);
void vTermclear2BOL(void);
void vTermclearline(void);
void vTermclearscreen(void);
void vTermclearhome(void);

void vTermhome(void) ;

char * pcTermAttrib(char * pBuf, u8_t FG, u8_t BG) ;
char * pcTermLocate(char * pBuf, u8_t Row, u8_t Col) ;

void xTermProcessChr(int cChr);
void vTermAttrib(u8_t FG, u8_t BG);
void vTermLocate(u8_t x, u8_t y);

void vTermSetSize(u16_t x, u16_t y) ;
void vTermGetInfo(terminfo_t * psTermInfo) ;
int xTermIdentify(void);

void vTermtestcode(void) ;

#ifdef __cplusplus
}
#endif
