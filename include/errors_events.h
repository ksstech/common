// x_errors_events.h

#pragma	once

#include "definitions.h"

#include <errno.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// ########################################### Macros ##############################################

#define	TABLE_ENTRY_INT(c,m)		{ .code=c, .pMess=m }

#define	MARK_M(M)					{ ErrLine = __LINE__; ErrPtr = M; }
#define	MARK_MX(M,X)				{ MARK_M(M); iRV = X; }
#define	MARK_MP(M,P)				{ MARK_M(M); pcRV = P; }

#define	BREAK_M(M)					{ MARK_M(M); break; }
#define	BREAK_X(X)					{ iRV = X; break; }
#define	BREAK_MX(M,X)				{ MARK_M(M); iRV = X; break; }

#define	RETURN_M(M)					{ MARK)M(M); return;
#define	RETURN_X(X)					return X;
#define	RETURN_MX(M,X)				{ MARK_M(M); return X; }
#define	RETURN_MP(M,P)				{ MARK_M(M); return P; }

#define	EXIT_M(M)					{ MARK_M(M); goto exit; }
#define	EXIT_X(X)					{ iRV = X; goto exit; }
#define	EXIT_MX(M,X)				{ MARK_M(M); iRV = X; goto exit; }
#define	EXIT_MP(M,P)				{ MARK_M(M); pcRV = P; goto exit; }

#define	IF_MARK_M(C,M)				if (C) MARK_M(M)
#define	IF_MARK_MX(C,M,X)			if (C) MARK_MX(M,X)
#define	IF_MARK_MP(C,M,P)			if (C) MARK_MP(M,P)

#define IF_EXIT(C)					if (C) goto exit;
#define IF_GOTO_L(C,L)				if (C) goto L;
#define IF_EXIT_M(C,M)				if (C) EXIT_M(M)
#define IF_EXIT_X(C,X)				if (C) { iRV = X; goto exit; }
#define IF_EXIT_MX(C,M,X)			if (C) EXIT_MX(M,X)
#define IF_EXIT_MP(C,M,P)			if (C) EXIT_MP(M,P)

#define IF_RETURN(C)				if (C) return;
#define IF_RETURN_X(C,X)			if (C) RETURN_X(X)
#define IF_RETURN_M(C,M)			if (C) RETURN_M(X)
#define IF_RETURN_MX(C,M,X)			if (C) RETURN_MX(M,X)
#define IF_RETURN_MP(C,M,P)			if (C) RETURN_MP(M,P)

#define IF_BREAK(C)					if (C) break
#define IF_BREAK_M(C,M)				if (C) BREAK_M(M)
#define IF_BREAK_X(C,X)				if (C) BREAK_X(X)
#define IF_BREAK_MX(C,M,X)			if (C) BREAK_MX(M,X)

/* ###################################### MBED error codes #########################################
 *	High level error codes
 *	======================
 *	ssl.h			-0x5E80	-0x7F80	
 *	cipher.h		-0x6080	-0x6380		Gap in ssl.h above
 *	pkcs7.h			-0x5300	-0x5880
 *	md.h			-0x5080	-0x5200
 *	ecp.h			-0x4B00	-0x4F80
 *	rsa.h			-0x4080	-0x4480
 *	pk.h			-0x3880	-0x3F80
 *	dhm.h			-0x3080	-0x3580
 *	pkcs5			-0x2e00	-0x2F80		Gap in x509.h below
 *	x509.h			-0x2080	-0x2980		And	-0x3000
 *	pkcs12.h		-0x1E00	-0x1F80
 *	pem.h			-0x1080	-0x1480
 *
 *	Low level error codes
 *	=====================
 * Bottom 7 digits (0->7F) of the complete code
 */

#define	mbedERROR_SMALLEST	(-0x7F80)	// MBEDTLS_ERR_SSL_HW_ACCEL_FAILED
#define	mbedERROR_BIGGEST	(-0x1080)	// MBEDTLS_ERR_PEM_NO_HEADER_FOOTER_PRESENT

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
	erINV_SIZE,
    erINV_CREDLEN,
	erINV_APNUM,
	erINV_COMMAND,
	erINV_ADDRESS,
	erINV_FLDNUM,
	erINV_DEVICE,
	erINV_VERSION,
	erINV_WHOAMI,
	erINV_HOST,
	erFILE_READ,
	erFILE_WRITE,
	erOUT_OF_RANGE,

	erWL_CFG_STA,
	erWL_CFG_MODE,
	erWL_CFG_SAVE,

	erTO_L1toL2,
	erTO_L2toL3,
	erTO_L3toLX,
	erTIMEOUT,

	erNO_SPACE,
	erNO_MEM,
	erTABLE_FULL,
	erISR_BLOCK,
	erINV_NUM_BITS,
	// Insert additional application error codes above here

	// tftp client
 	erTFTP_ERRORREPLY,
 	erTFTP_RESOURCES,
	erTFTP_FILE0BYTES,				// no message
	erTFTP_FAILED,
	// Below here MUST have messages defined, used in syslog functionality
	erTX_QUEUE_SEND,
	// Sequence is important, used to determine seriousness/priority 
	erACT_INV_CH = -4,
	erACT_NOT_CFG = -3,
	erACT_BLOCKED = -2,
	erFAILURE = -1,					// general failure code
	erSUCCESS = 0,					// neutral success code
} ;

// ####################################### Structures ##############################################

typedef struct {
	int code;
	const char * pMess;
} eTable_t;

// ################################### Global/public variable(s) ###################################

extern u32_t ErrLine;
extern char * ErrPtr;

// ################################### Global/public functions #####################################

int	ErrorSet(int eCode);
int	ErrorGet(void);
const char * pcCodeToMessage(int eCode);
const char * pcStrError(int eCode);

#ifdef __cplusplus
}
#endif
