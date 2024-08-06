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

/**
 * @brief	input a string directly from the UART/terminal
 * @param	pBuf - buffer to store characters
 * @param	Size - size of buffer
 * @param	Match - character/key to be treated as end-of-line EOL
 * @param	flag - control flags specifying [un]lock actions, wait periods etc.. 
 * @return	number of characters in the buffer, terminator (if there) excluded
 */
int	xTermGets(char * pcBuf, size_t Size, int Match, termctrl_t flag);

/**
 * @brief	output a string directly to the UART/terminal
 * @param	pStr - string to be output
 * @param	flag - control flags specifying [un]lock actions, wait periods etc.. 
 * @return	Number of characers processed
 */
int xTermPuts(char * pStr, termctrl_t flag);

/**
 * @brief	generate cursor location control/ESC string
 * @param	pBuf pointer to buffer where string is to be built
 * @param	Row - 1 relative row value
 * @param	Col - 1 relative column value
 * @return	[Adjusted] pointer	
 */
char * pcTermLocate(char * pBuf, u8_t Row, u8_t Col);

/**
 * @brief	generate text attribute control/ESC string
 * @param	pBuf pointer to buffer where string is to be built
 * @param	a1 - first attribute
 * @param	a2 - second attribute
 * @return	[Adjusted] pointer	
 */
char * pcTermAttrib(char * pBuf, u8_t a1, u8_t a2);

/**
 * @brief	generate & output cursor location control/ESC string
 * @param	Row - 1 relative row value
 * @param	Col - 1 relative column value
 */
void vTermLocate(u8_t Row, u8_t Col);

/**
 * @brief	generate & output cursor location control/ESC string
 * @param	a1 - first attribute
 * @param	a2 - second attribute
 */
void vTermAttrib(u8_t a1, u8_t a2);

/**
 * @brief	Request, receive and parse current cursor location in a single lock/unlock operation
 * @return	Number of parameters parsed (should be 2) or erFAILURE if incomplete/malformed packet
*/
int xTermCursorRead(void);

/**
 * @brief
 */
void vTermCursorSave(void);

/**
 * @brief
 */
void vTermCursorBack(void);

/**
 * @brief
 */
void vTermClear2EOL(void);

/**
 * @brief
 */
void vTermClear2BOL(void);

/**
 * @brief
 */
void vTermClearline(void);

/**
 * @brief
 */
void vTermClearScreen(void);

/**
 * @brief
 */
void vTermHome(void);

/**
 * @brief
 */
void vTermClearHome(void);

/**
 * @brief	Update cursor location into window title bar (XTERM only)
*/
void vTermOpSysCom(char * pStr);

void vTermWinTleCursor(void);

void vTermDisplayLocation(void);

/**
 * @brief	Check column and adjust column & row if required
 */
void vTermCheckCursor(void);

/**
 * @brief	Update row and/or column tracking values based on the specific character being processed
 * @param	cChr - character to be processed
 */
void xTermProcessChr(int cChr);

/**
 * @brief	vTermSetSize() - set terminal row & column size (0 = reset to default)
 * @param	Rows - number of rows/lines
 * @param	Columns - number of columns
 */
void vTermSetSize(u16_t Rows, u16_t Columns) ;

/**
 * @brief
 */
void vTermGetInfo(terminfo_t * psTermInfo) ;

/**
 * @brief
 */
int xTermIdentify(void);

int xTermAttached(void);

/**
 * @brief
 */
void vTermTestCode(void) ;

#ifdef __cplusplus
}
#endif
