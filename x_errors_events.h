/*
 * x_errors_events.h
 */

#pragma	once

#ifdef __cplusplus
extern "C" {
#endif

// ########################################### Macros ##############################################

#define	TABLE_ENTRY_INT(x,y,z)		{ .iVal1=x , .iVal2=y , .pMess=z }

// ######################################## Error and Event codes ##################################

enum {
	erFIRSTCODE						= (-3100),	// Check overlap with other error codes
// Script engine (Rules/Sense)
	erSCRIPT_INV_KEYWORD,
	erSCRIPT_INV_INDEX,
	erSCRIPT_INV_VALUE,
	erSCRIPT_INV_PARA,
	erSCRIPT_INV_OPERATION,
	erSCRIPT_INV_MODE,				// no message
	erSCRIPT_INV_SYNTAX,
	erSCRIPT_INV_ENDPOINT,
	erSCRIPT_INCOM_SENSE,			// incomplete sense instruction
	erSCRIPT_INCOM_RULES,			// incomplete rules instruction
	erSCRIPT_INCOM_IDRES,			// incomplete identity/resource entry
	erINV_VAR_TYPE,
	erINV_STATS_CONFIG,
	erSCRIPT_TOO_MANY,

	erTX_QUEUE_SEND,
	erSENSOR_NOT_READABLE,			// no message
	erPAYLOAD_MISSING,				// no payload
	erPAYLOAD_TOO_LARGE,
	erPAYLOAD_INVALID,				// invalid content in payload
// tftp client
 	erTFTP_ERRORREPLY,
 	erTFTP_RESOURCES,
	erTFTP_FILE0BYTES,				// no message
	erTFTP_FAILED,
// Insert additional application error codes above here
	erLASTCODE,
/* based on the range of error codes used by any HAL (and other) modules
 * insert their error code list at the most suitable location in the enumerated list
 */

// Indicates the last ie general FAILURE and neutral SUCCESS code
	erFAILURE						= -1,
	erSUCCESS						= 0,
// From here on all positive numbers indicating events
	evLASTCODE,						// Insert all new codes ABOVE here
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
