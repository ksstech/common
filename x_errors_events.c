/*
 * x_errors_events.c
 */

#include	"x_errors_events.h"
#include	"x_definitions.h"
#include	"x_string_general.h"

const	eTable_t	ErrorTable[] = {
	#include		"hal_errormessages.cla"					// Platform SDK defined error codes

#if		(!defined(NDEBUG) || defined(DEBUG))
	TABLE_ENTRY_INT( erSCRIPT_INV_KEYWORD,							0,	"Inv KEYWORD")
	TABLE_ENTRY_INT( erSCRIPT_INV_INDEX,							0,	"Inv INDEX")
	TABLE_ENTRY_INT( erSCRIPT_INV_VALUE,							0,	"Inv VALUE")
	TABLE_ENTRY_INT( erSCRIPT_INV_PARA,								0,	"Inv PARA")
	TABLE_ENTRY_INT( erSCRIPT_INV_OPERATION,						0,	"Inv Ep Oper")
#endif

/*
 * Messages below here will ALWAYS be in the firmware, so as to be syslog'd
 */
// Cloud service related
	TABLE_ENTRY_INT(erTX_QUEUE_SEND, 0, "TX Queue Send Err")

// Insert additional error codes above here !!!!!!!
	TABLE_ENTRY_INT(erFAILURE, erFAILURE, "Undef eCode")
} ;

static int32_t	LastError ;

int32_t	ErrorSet(int32_t eCode) { LastError = eCode ; return LastError ; }
int32_t	ErrorGet(void) { return LastError ; }

#if		defined(cc3200)
char * strerror(int _errno) { return pcCodeToMessage(_errno, ErrorTable) ; }
#endif
