// x_stdio.h - Copyright (c) 2014-24 Andre M Maree / KSS Technologies (Pty) Ltd.

#pragma once

#include "FreeRTOS_SUpport.h"

#include "stddef.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

// ###################################### BUILD : CONFIG definitions ##############################

// ##################################### MACRO definitions #########################################

#define stdioBUILD_TERMIO			0						// build terminal IO support

#define	TERMINAL_DFLT_X				160
#define	TERMINAL_DFLT_Y				48
#define TERMINAL_DFLT_TAB			8

#define	TERMINAL_MAX_X				255
#define	TERMINAL_MAX_Y				63
#define TERMINAL_MAX_TAB			15

#define	termWAIT_MS					100

#define termBUILD_CTRL(LO,UL,WAIT) ((termctrl_t) { .Lock = LO, .Unlock = UL, .Wait = WAIT } )

#define termUSE_7BIT				0						// C0 ~ ESC [
#define termUSE_8BIT				1						// C1 ~ 0x80 -> 0x9F
#define termUSE_TYPE				termUSE_7BIT

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
#define setCURSOR_SAVE	termCSI "s"							// "7" or "s"
#define setCURSOR_REST	termCSI "u"							// "8" or "u"
#define setCURSOR_HOME	termCSI "1;1H"
#define setCURSOR_MAX	termCSI "999;999H"
#define getCURSOR_MAX	setCURSOR_SAVE setCURSOR_MAX getCURSOR_POS setCURSOR_REST

#define setCLRLIN_RIGHT	termCSI "0K"
#define setCLRLIN_LEFT	termCSI "1K"
#define setCLRLIN_ALL	termCSI "2K"

#define setCLRDSP_BELOW	termCSI "0J"
#define setCLRDSP_ABOVE	termCSI "1J"
#define setCLRDSP_ALL	termCSI "2J"

#define getDEVICE_ATTR	termCSI "0c"

// ####################################### Structures ##############################################

typedef struct __attribute__((packed)) terminfo_t {
	SemaphoreHandle_t mux;
	i16_t CurX, CurY, SavX, SavY, MaxX, MaxY, Tabs;
} terminfo_t;

typedef struct __attribute__((packed)) termctrl_t {
	union {
		struct { bool Lock; bool Unlock; };
		u16_t u16Val;
	};
	u16_t Wait;
} termctrl_t;

// ###################################### Public variables #########################################

extern terminfo_t sTI;

// ################################ Low level Terminal IO support ##################################

/**
 * @brief
 * @param[in]	fd UART/USB number 
 * @return
 * @note	https://invisible-island.net/xterm/ctlseqs/ctlseqs.html
 * @note	1B	5B	3F	31	3B	32	63	"\e[?1;2c"	idf.py, Serial, Xterm/VT100/Linux emulation all same
 */
int serial_read_terminal_type(int fd);

/**
 * @brief
 * @return
 */
int serial_get_terminal_type(void);

/**
 * @brief		Request, receive and parse current cursor location in a single lock/unlock operation
 * @param[in]	fd UART/USB number 
 * @return		Number of parameters parsed (should be 2) or erFAILURE if incomplete/malformed packet
 */
int serial_query_cursor_now(int fd);

/**
 * @brief		Determine current terminal window size by locating to 999,999 and reading actual value
 * @param[in]	fd UART/USB number 
 * @return
 */
int serial_query_cursor_max(int fd);

// ################################ Generic terminal IO support #################################

int stdio_get_cur_colX(terminfo_t * psTI);
int stdio_get_sav_colX(terminfo_t * psTI);
int stdio_get_max_colX(terminfo_t * psTI);

int stdio_get_cur_rowY(terminfo_t * psTI);
int stdio_get_sav_rowY(terminfo_t * psTI);
int stdio_get_max_rowY(terminfo_t * psTI);

void stdio_push_cur_rowY_colX(terminfo_t * psTI);
void stdio_push_max_rowY_colX(terminfo_t * psTI);

void stdio_set_cur_rowY_colX(terminfo_t * psTI, i16_t RowY, i16_t ColX);
void stdio_set_max_rowY_colX(terminfo_t * psTI, i16_t RowY, i16_t ColX);

void stdio_pull_cur_rowY_colX(terminfo_t * psTI);
void stdio_pull_max_rowY_colX(terminfo_t * psTI);

/**
 * @brief	Update row and/or column tracking values based on the specific character being processed
 * @param[in]	psTI pointer to terminal status structure to be updated
 * @param[in]	iChr character to be processed
 */
void stdio_update_cursor(terminfo_t * psTI, int iChr);

/**
 * @brief	set terminal row & column size (0 = reset to default)
 * @param[in]	Rows - number of rows/lines
 * @param[in]	Columns - number of columns
 */
void stdio_set_size(terminfo_t * psTI, u16_t Rows, u16_t Columns) ;

// #################################### Global/public functions ####################################

/// @brief		read a termianted text string from a file and store it into the buffer provided
/// @param[in]	sd - handle of file to be read from
/// @param[in]	pcBuf - buffer to store the text string read into
/// @param[in]	Size - size of the buffer ie maximum string length
/// @param[in]	bEcho - flag indicating whether the contents read should be echoed
/// @return 	number of characters read & stored in buffer
int	xReadString(int sd, char * pcBuf, size_t Size, bool bEcho);

/**
 * @brief
 */
void vTermGetInfo(terminfo_t * psTermInfo) ;

/**
 * @brief	output a string to the terminal
 * @param	pBuf - string to be output
 * @return	Number of characters processed or negative error code
 */
ssize_t xStdioWriteS(char * pBuf);
ssize_t xStdioWriteSctrl(char * pBuf, termctrl_t Ctrl);

/**
 * @brief	generate cursor location control/ESC string
 * @param	pBuf pointer to buffer where string is to be built
 * @param	Row - 1 relative row value
 * @param	Col - 1 relative column value
 * @return	[Adjusted] pointer	
 */
char * pcTermLocate(char * pBuf, i16_t Row, i16_t Col);

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
void vTermLocate(i16_t Row, i16_t Col);

/**
 * @brief	output a string at the specified cursor location
 * @param	RowY - 1 relative row value
 * @param	Col - 1 relative column value
 * @return	Number of characters processed
 */
ssize_t xTermLocatePuts(i16_t RowY, i16_t ColX, char * pBuf);

/**
 * @brief	generate & output cursor location control/ESC string
 * @param	a1 - first attribute
 * @param	a2 - second attribute
 */
void vTermAttrib(u8_t a1, u8_t a2);

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

ssize_t xTermWinTleCursor(void);

ssize_t xTermShowCurRowYColX(i16_t RowY, i16_t ColX);

/**
 * @brief	output a string directly to the UART/terminal
 * @param	pStr - string to be output
 * @param	flag - control flags specifying [un]lock actions, wait periods etc.. 
 * @return	Number of characers processed
 */
int xTermPuts(char * pStr, termctrl_t flag);

/**
 * @brief	Update cursor location into window title bar (XTERM only)
*/
ssize_t xTermOpSysCom(char * pStr);

/**
 * @brief
 */
void vTermTestCode(void) ;

#ifdef __cplusplus
} /* extern "C" */
#endif
