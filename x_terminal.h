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

#define termBUILD_CTRL(LO,UL,WAIT) ((termctrl_t) { .Lock = LO, .Unlock = UL, .Wait = WAIT } )

#define termUSE_7BIT			0						// C0 ~ ESC [
#define termUSE_8BIT			1						// C1 ~ 0x80 -> 0x9F
#define termUSE_TYPE			termUSE_7BIT

#if (termUSE_TYPE == termUSE_7BIT)
	#define termCSI	"\033["
	#define termST	"\033\134"
	#define termOSC "\033]"
#else
	#define termCSI	"\233"
	#define termST	"\234"
	#define termOSC "\235"
#endif

#define getCURSOR_POS	termCSI "6n"
#define setCURSOR_HOME	termCSI "1;1H"
#define setCURSOR_SAVE	termCSI "7"			// termCSI "s"
#define setCURSOR_REST	termCSI "8"			// termCSI "u"

#define setCLRLIN_RIGHT	termCSI "0K"
#define setCLRLIN_LEFT	termCSI "1K"
#define setCLRLIN_ALL	termCSI "2K"

#define setCLRDSP_BELOW	termCSI "0J"
#define setCLRDSP_ABOVE	termCSI "1J"
#define setCLRDSP_ALL	termCSI "2J"

#define getDEVICE_ATTR	termCSI "0c"
//#define xterm

// ####################################### structures ##############################################

typedef union {
	struct __attribute__((packed)) {
		u16_t CurX, CurY, SavX, SavY, MaxX, MaxY, Tabs;
	};
} terminfo_t;

typedef struct __attribute__((packed)) termctrl_t {
	union {
		struct { bool Lock; bool Unlock; };
		u16_t u16Val;
	};
	u16_t Wait;
} termctrl_t;

// ###################################### Public variables #########################################


// ###################################### Public functions #########################################

int xTermGetCurColX(void);
int xTermGetCurRowY(void);

int xTermGetMaxColX(void);
int xTermGetMaxRowY(void);

void vTermPushCurRowCol(void);
void vTermPullCurRowCol(void);
void vTermSetCurRowCol(u16_t RowY, u16_t ColX);

void vTermPushMaxRowCol(void);
void vTermPullMaxRowCol(void);
void vTermSetMaxRowCol(u16_t RowY, u16_t ColX);

/**
 * @brief	Update row and/or column tracking values based on the specific character being processed
 * @param	cChr - character to be processed
 */
void xTermProcessChr(int cChr);

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
char * pcTermLocate(char * pBuf, u16_t Row, u16_t Col);

/**
 * @brief	generate text attribute control/ESC string
 * @param	pBuf pointer to buffer where string is to be built
 * @param	a1 - first attribute
 * @param	a2 - second attribute
 * @return	[Adjusted] pointer	
 */
char * pcTermAttrib(char * pBuf, u16_t a1, u16_t a2);

/**
 * @brief	generate & output cursor location control/ESC string
 * @param	Row - 1 relative row value
 * @param	Col - 1 relative column value
 */
void vTermLocate(u16_t Row, u16_t Col);

/**
 * @brief	generate & output cursor location control/ESC string
 * @param	a1 - first attribute
 * @param	a2 - second attribute
 */
void vTermAttrib(u16_t a1, u16_t a2);

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
