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
	erINV_KEYWORD,
	erINV_INDEX,
	erINV_VALUE,
	erINV_PARA,
	erINV_OPERATION,
	erINV_MODE,
	erINV_SYNTAX,
	erINV_ENDPOINT,
	erINV_STATE,
	erINV_CONFIG,
	erINV_SENSOR,
	erINV_RGSTR,
	erINV_SCALE,
	erINV_UNIT,
	erINV_SSID,
	erINV_PSWD,
	erINV_COMMAND,
	erINV_ADDRESS,
	erHOST_NOTFOUND,
	erINV_HOST,
	erDUP_HOST,
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
