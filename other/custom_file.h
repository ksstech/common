// custom_file.h

#include	<stdint.h>

#pragma once

typedef struct {
	int32_t		fd ;
	uint32_t	flags ;
} __FILE ;

typedef	__FILE FILE ;

/*
 * For ESP IDF __FILE is defined in "sys/reent.h"
 *
 */

//#warning "__FILE and FILE defined in sys/custom_file.h"
