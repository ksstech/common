/*
 * x_errors_events.c
 */

#include	"x_errors_events.h"

const	eTable_t	ErrorTable[] = {
#if		(!defined(NDEBUG) || defined(DEBUG))
	TABLE_ENTRY_INT( erSCRIPT_INV_KEYWORD,		0,	"Inv KEYWORD"),
	TABLE_ENTRY_INT( erSCRIPT_INV_INDEX,		0,	"Inv INDEX"),
	TABLE_ENTRY_INT( erSCRIPT_INV_VALUE,		0,	"Inv VALUE"),
	TABLE_ENTRY_INT( erSCRIPT_INV_PARA,			0,	"Inv PARA"),
	TABLE_ENTRY_INT( erSCRIPT_INV_OPERATION,	0,	"Inv Ep Oper"),
	TABLE_ENTRY_INT( erSCRIPT_INV_MODE,			0,	"Inv MODE"),
	TABLE_ENTRY_INT( erSCRIPT_INV_SYNTAX,		0,	"Inv SYNTAX"),
	TABLE_ENTRY_INT( erSCRIPT_INV_ENDPOINT,		0,	"Inv ENDPOINT"),
	TABLE_ENTRY_INT( erSCRIPT_INCOM_SENSE,		0,	"Incom SENSE "),
	TABLE_ENTRY_INT( erSCRIPT_INCOM_RULES,		0,	"Incom RULES"),
	TABLE_ENTRY_INT( erSCRIPT_INCOM_IDRES,		0,	"Incom IDENT "),
	TABLE_ENTRY_INT( erINV_VAR_TYPE,			0,	"Inv VAR Type"),
	TABLE_ENTRY_INT( erINV_STATS_CONFIG,		0,	"Inv STATS Config"),
	TABLE_ENTRY_INT( erSCRIPT_TOO_MANY,			0,	"Too Many"),
#endif

	// Messages below here will ALWAYS be in the firmware, so as to be syslog'd
	TABLE_ENTRY_INT(erTX_QUEUE_SEND, 0, "TX Queue Send Err"),

	// Insert additional error codes above here !!!!!!!
	TABLE_ENTRY_INT(erFAILURE, erFAILURE, "Undef eCode"),
} ;

static int	LastError ;

int	ErrorSet(int eCode) { LastError = eCode ; return LastError ; }

int	ErrorGet(void) { return LastError ; }

/**
 * pcCodeToMessage() - locates the provided eCode within the table specified and return the message
 * @param[in]	eCode - (error) code to lookup
 * @param[in]	eTable - pointer to message table within which to look
 * @return		pointer to correct/matched or default (not found) message
 */
char * pcCodeToMessage(int eCode, const eTable_t * eTable) {
	int	i = 0 ;
	while ((eTable[i].iVal1 != -1) && (eTable[i].iVal2 != -1)) {
	// If an exact match with 1st value in table, return pointer to the message
		if (eCode == eTable[i].iVal1) break ;
	// If we have a 2nd (ie range) value, then test differently for positive & negative ranges
		if (eTable[i].iVal2 != 0) {
		// code is negative (ie an error code) and within this (negative) range, return pointer to message
			if ((eCode < 0) && (eCode < eTable[i].iVal1) && (eCode >= eTable[i].iVal2)) break ;

		// if code is positive (ie a normal) and within the positive range, return pointer to message
			if ((eCode > 0) && (eCode > eTable[i].iVal1) && (eCode <= eTable[i].iVal2)) break ;
		}
		++i ;											// no match found, try next entry
	}
	return (char *) eTable[i].pMess ;
}

#if		defined(cc3200)
char * strerror(int _errno) { return pcCodeToMessage(_errno, ErrorTable) ; }
#endif
