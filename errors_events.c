// x_errors_events.c

#include <netdb.h>
#include "errors_events.h"
#include "definitions.h"

const eTable_t ErrorTable[] = {
	TABLE_ENTRY_INT(erINV_KEYWORD,		"InvKeyword"),
	TABLE_ENTRY_INT(erINV_INDEX,		"InvIndex"),
	TABLE_ENTRY_INT(erINV_VALUE,		"InvValue"),
	TABLE_ENTRY_INT(erINV_PARA,			"InvParameter"),
	TABLE_ENTRY_INT(erINV_OPERATION,	"InvOperation"),
	TABLE_ENTRY_INT(erINV_MODE, 		"InvMode"),
	TABLE_ENTRY_INT(erINV_SYNTAX,		"InvSyntax"),
	TABLE_ENTRY_INT(erINV_ENDPOINT,		"InvEndPoint"),
	TABLE_ENTRY_INT(erINV_STATE,		"InvState"),
	TABLE_ENTRY_INT(erINV_CONFIG,		"InvConfig"),
	TABLE_ENTRY_INT(erINV_SENSOR,		"InvSensor"),
	TABLE_ENTRY_INT(erINV_RGSTR,		"InvRegistration"),
	TABLE_ENTRY_INT(erINV_SCALE,		"InvScale"),
	TABLE_ENTRY_INT(erINV_UNIT,			"InvUnit"),
	TABLE_ENTRY_INT(erINV_SIZE, 		"InvSize"),
    TABLE_ENTRY_INT(erINV_APNUM,        "InvAP_NUM"),
	TABLE_ENTRY_INT(erINV_COMMAND,		"InvCommand"),
	TABLE_ENTRY_INT(erINV_ADDRESS,		"InvAddress"),
	TABLE_ENTRY_INT(erINV_FLDNUM,		"InvFieldNumber"),
	TABLE_ENTRY_INT(erINV_DEVICE,		"InvDevice"),
	TABLE_ENTRY_INT(erINV_VERSION,		"InvVersion"),
	TABLE_ENTRY_INT(erINV_WHOAMI,		"InvWhoAmI"),
	TABLE_ENTRY_INT(erINV_HOST,			"InvHost"),
	TABLE_ENTRY_INT(erTX_QUEUE_SEND,	"TX Queue Send Err"),
	TABLE_ENTRY_INT(erACT_INV_CH,		"Chan Out of Range "),
	TABLE_ENTRY_INT(erACT_NOT_CFG,		"Chan NOT configured"),
	TABLE_ENTRY_INT(erACT_BLOCKED,		"Chan BLOCKED"),
	TABLE_ENTRY_INT(erSUCCESS,			"Success"),
};

// ####################################### Private Variables #######################################

static int LastError;

// ####################################### Global Variables ########################################

u32_t ErrLine;
char *	ErrPtr;

int	ErrorSet(int eCode) { return LastError = eCode; }

int	ErrorGet(void) { return LastError; }

const char * pcStrError(int eCode) {
#if LWIP_DNS_API_DEFINE_ERRORS	/* https://sourceware.org/glibc/wiki/NameResolver */
	if (INRANGE(EPERM, eCode, ENOTRECOVERABLE) || INRANGE(EAI_NONAME, eCode, TRY_AGAIN))
		return strerror(eCode);
#else
	if (INRANGE(EPERM, eCode, ENOTRECOVERABLE) || INRANGE(EAI_NONAME, eCode, EAI_AGAIN))
		return strerror(eCode);
#endif
	for (int i = 0; i < NO_MEM(ErrorTable); i++) {
		if (eCode == ErrorTable[i].code)
			return ErrorTable[i].pMess;
	}
	return esp_err_to_name(eCode);
}
