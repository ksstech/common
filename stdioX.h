// x_stdio.h - Copyright (c) 2014-24 Andre M Maree / KSS Technologies (Pty) Ltd.

#pragma once

#include "definitions.h"
#include "stddef.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

// ###################################### BUILD : CONFIG definitions ##############################

// ##################################### MACRO definitions #########################################

// #################################### Global/public functions ####################################

/// @brief		read a termianted text string from a file and store it into the buffer provided
/// @param[in]	sd - handle of file to be read from
/// @param[in]	pcBuf - buffer to store the text string read into
/// @param[in]	Size - size of the buffer ie maximum string length
/// @param[in]	bEcho - flag indicating whether the contents read should be echoed
/// @return 	number of characters read & stored in buffer
int	xReadString(int sd, char * pcBuf, size_t Size, bool bEcho);

#ifdef __cplusplus
} /* extern "C" */
#endif
