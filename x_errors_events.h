/*
 * x_errors_events.h
 */

#pragma	once

#include <errno.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// ########################################### Macros ##############################################

#define	TABLE_ENTRY_INT(x,y,z)		{ .iVal1=x , .iVal2=y , .pMess=z }

// ######################################## Error and Event codes ##################################

enum {
	erFIRSTCODE						= (-3100),	// Check overlap with other error codes
// Script engine (Rules/Sense)
	erINVALID_KEYWORD,
	erINVALID_INDEX,
	erINVALID_VALUE,
	erINVALID_PARA,
	erINVALID_OPERATION,
	erINVALID_MODE,
	erINVALID_SYNTAX,
	erINVALID_ENDPOINT,
	erINVALID_STATE,
	erINVALID_CONFIG,
	erINVALID_SENSOR,
	erOUT_OF_RANGE,

	erTABLE_FULL,
	// Insert additional application error codes above here

	// tftp client
 	erTFTP_ERRORREPLY,
 	erTFTP_RESOURCES,
	erTFTP_FILE0BYTES,				// no message
	erTFTP_FAILED,
	// Below here MUST have messages defined, used in syslog functionality
	erTX_QUEUE_SEND,
	erFAILURE				= -1,	// general failure code
	erSUCCESS				= 0,	// neutral success code
} ;

// ####################################### Structures ##############################################

typedef struct {
	int			iVal1 ;
	int			iVal2 ;
	const char * pMess ;
} eTable_t ;

// ################################### Global/public variable(s) ###################################


// ################################### Global/public functions #####################################

int	ErrorSet(int eCode) ;
int	ErrorGet(void) ;
char * pcCodeToMessage(int eCode, const eTable_t * eTable) ;

#ifdef __cplusplus
}
#endif
