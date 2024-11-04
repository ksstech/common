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

int	xReadString(int sd, char * pcBuf, size_t Size, bool bEcho);

#ifdef __cplusplus
} /* extern "C" */
#endif
