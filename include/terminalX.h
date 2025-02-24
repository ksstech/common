// terminalX.h

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// ####################################### structures ##############################################

// ###################################### Public variables #########################################

// ###################################### Public functions #########################################

/**
 * @brief
 */
void vTermGetInfo(terminfo_t * psTermInfo) ;

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

/**
 * @brief	Update cursor location into window title bar (XTERM only)
*/
void vTermOpSysCom(char * pStr);

void vTermWinTleCursor(void);

void xTermShowCurRowYColX(i16_t RowY, i16_t ColX);

/**
 * @brief
 */
void vTermTestCode(void) ;

#ifdef __cplusplus
}
#endif
