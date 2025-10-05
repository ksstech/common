// stdioX.h

#pragma once

#include "FreeRTOS_Support.h"

#include "stddef.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

// ##################################### MACRO definitions #########################################

#if (CONFIG_LIBC_STDIN_LINE_ENDING_CRLF == 1)
	#define TST_STDIN_TERM(cChr) (cChr == CHR_CR || cChr == CHR_LF)
	#define termSTDIN_TERM	CHR_CR
#elif (CONFIG_LIBC_STDIN_LINE_ENDING_CR == 1)
	#define TST_STDIN_TERM(cChr) (cChr == CHR_CR)
	#define termSTDIN_TERM	CHR_CR
#elif (CONFIG_LIBC_STDIN_LINE_ENDING_LF == 1)
	#define TST_STDIN_TERM(cChr) (cChr == CHR_LF)
	#define termSTDIN_TERM	CHR_LF
#else
	#error "Invalid STDIN line termination"
#endif

// ################################# Terminal MACRO definitions ####################################

#define stdioBUILD_TERMIO			1						// build terminal IO support

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

// ################################ Low level Terminal IO support ##################################

/**
 * @brief		input a string directly from UART/terminal channel specified
 * @param[in]	fd UART/USB channel to use
 * @param[in]	pcStr pointer to buffer to store characters
 * @param[in]	Len size of buffer
 * @param[in]	Match character/key to be treated as end-of-string
 * @return		number of characters in the buffer, terminator (if there) excluded
 */
int xStdioReadStringMatchUnlocked(int fd, char * pcStr, size_t Len, int Match);

int xStdioReadStringMatch(int fd, char * pcStr, size_t Len, int Match);

/**
 * @brief
 * @param[in]	fd UART/USB number 
 * @return
 * @note	https://invisible-island.net/xterm/ctlseqs/ctlseqs.html
 * @note	1B	5B	3F	31	3B	32	63	"\e[?1;2c"	idf.py, Serial, Xterm/VT100/Linux emulation all same
 */
int xStdioReadTerminalType(int fd);

/**
 * @brief
 * @return
 */
int xStdioGetTerminalType(void);

int xStdioSyncCursor(int sd, char * pcStr, i16_t * pRowY, i16_t * pColX);

/**
 * @brief		Request, receive and parse current cursor location in a single lock/unlock operation
 * @param[in]	fd UART/USB number 
 * @return		Number of parameters parsed (should be 2) or erFAILURE if incomplete/malformed packet
 */
int xStdioSyncCursorNow(int fd, terminfo_t * psTI);

/**
 * @brief		Determine current terminal window size by locating to 999,999 and reading actual value
 * @param[in]	fd UART/USB number 
 * @return
 */
int xStdioSyncCursorMax(int fd, terminfo_t * psTI);

/**
 * @brief		set the current UART/USB console status
 * @param[in]	state 0=inactive, 1=active
 */
void vStdioConsoleSetStatus(bool state);

/**
 * @brief		obtain current console UART/USB status 
 * @return		0=inactive, 1=active
 */
bool bStdioConsoleGetStatus(void);

// ################################ Generic terminal IO support #################################

int xStdioGetCurColX(terminfo_t * psTI);
int xStdioGetSavColX(terminfo_t * psTI);
int xStdioGetMaxColX(terminfo_t * psTI);

int xStdioGetCurRowY(terminfo_t * psTI);
int xStdioGetSavRowY(terminfo_t * psTI);
int xStdioGetWaxRowY(terminfo_t * psTI);

void vStdioPushCurRowYColX(terminfo_t * psTI);
void vStdioPushMaxRowYColX(terminfo_t * psTI);

void vStdioSetCurRowYColX(terminfo_t * psTI, i16_t RowY, i16_t ColX);
void vStdioSetMaxRowYColX(terminfo_t * psTI, i16_t RowY, i16_t ColX);

void vStdioPullCurRowYColX(terminfo_t * psTI);
void vStdioPullMaxRowYColX(terminfo_t * psTI);

/**
 * @brief	Update row and/or column tracking values based on the specific character being processed
 * @param[in]	psTI pointer to terminal status structure to be updated
 * @param[in]	pBuf pointer to buffer containing characters to be processed
 * @param[in]	Size number of characters in the buffer to be processed
 */
void vStdioUpdateCursor(terminfo_t * psTI, char * pBuf, size_t Size);

/**
 * @brief	set terminal row & column size (0 = reset to default)
 * @param[in]	Rows - number of rows/lines
 * @param[in]	Columns - number of columns
 */
void vStdioSetSize(terminfo_t * psTI, u16_t Rows, u16_t Columns) ;

// #################################### Global/public functions ####################################

int xStdioRead(int sd, char * pBuf, size_t Size);

/**
 * @brief		read a terminated text string from a file and store it into the buffer provided
 * @param[in]	sd - handle of file to be read from
 * @param[in]	pcBuf - buffer to store the text string read into
 * @param[in]	Size - size of the buffer ie maximum string length
 * @param[in]	bEcho - flag indicating whether the contents read should be echoed
 * @return 	number of characters read & stored in buffer
 */
int	xStdioGetString(int sd, char * pBuf, size_t Size, bool bHide);

/**
 * @brief		read a terminated text string from a stream and store it into the buffer provided
 * @param[in]	sd - stream to be read from
 * @param[in]	pcBuf - buffer to store the text string read into
 * @param[in]	Size - size of the buffer ie maximum string length
 * @return 	number of characters read & stored in buffer
 */
int xStdioGetS(int fd, char * pcStr, size_t Size);

int xStdioGetC(int fd);

int xStdioWrite(int fd, char * pBuf, size_t Size);

int xStdioPutS(int fd, char * pcStr);

int xStdioPutC(int fd, int iChr);

void xStdioPutHex(int fd, char * pcStr);

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
