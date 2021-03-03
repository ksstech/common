/*
 * x_struct_union.h
 */

#pragma	once

#include	"x_time.h"						// x_definitions stdint.h time.h

#ifdef __cplusplus
extern "C" {
#endif

// ############################# common complex data types/ structures #############################

typedef union __attribute__((packed)) ow_rom_u {
	uint64_t	Value ;
	uint8_t		HexChars[8] ;
	struct {
		uint8_t		Family ;
		uint8_t		TagNum[6] ;
		uint8_t		CRC ;
	} ;
} ow_rom_t ;
DUMB_STATIC_ASSERT( sizeof(ow_rom_t) == 8) ;

typedef union mac_addr_t {
	struct __attribute__((packed)) {
		uint8_t		str6hex[6] ;
		uint8_t		filler1[2] ;
	} ;
	struct __attribute__((packed)) {
		uint64_t	value	: 48 ;
		uint16_t	filler2 : 16 ;
	} ;
} mac_addr_t ;
DUMB_STATIC_ASSERT(sizeof(mac_addr_t) == 8) ;

typedef	union xVer_u {									// Version numbers
	struct __attribute__((packed)) {
		uint8_t		ver_rev ;			// Revision of Sub
		uint8_t		ver_sub ;			// Sub version
		uint8_t		ver_min ;			// Minor version
		uint8_t		ver_maj ;			// Major version
	} ;
	uint32_t	val ;
}  xVer_t ;
DUMB_STATIC_ASSERT(sizeof(xVer_t) == 4) ;
#define	MAKE_VERSION(Maj, Min, Sub, Rev)	((Maj << 24) + (Min << 16) + (Sub << 8) + Rev)

// ######################################### 8 bit types ###########################################

typedef union x8_u {
	uint8_t		u8 ;
	int8_t		i8 ;
	char		c8 ;
} x8_t ;
DUMB_STATIC_ASSERT(sizeof(x8_t) == 1) ;

// ######################################### 16 bit types ##########################################

typedef union x16_u {
	uint16_t	u16 ;
	int16_t		i16 ;
	x8_t		x8[2] ;
} x16_t ;
DUMB_STATIC_ASSERT(sizeof(x16_t) == 2) ;

// ########################################## x32 types ############################################

typedef union x32_u {
	uint32_t	u32 ;
	int32_t		i32 ;
	float		f32 ;
	x16_t		x16[2] ;
	x8_t		x8[4] ;
#ifdef ESP_PLATFORM
	char *		pc8 ;
	const char *pcc8 ;
#endif
} x32_t ;
DUMB_STATIC_ASSERT(sizeof(x32_t) == 4) ;

// ########################################## x64 types ############################################

typedef union x64_u {
	uint64_t	u64 ;
	int64_t		i64 ;
 	double		f64 ;
 	x32_t		x32[2] ;
	x16_t		x16[4] ;
	x8_t		x8[8] ;
} x64_t ;

// ########################################## p32 pointer types ####################################

typedef union p32_u {
	void *		pvoid ;
	void * *	pPVoid ;
// pointers to x64
	x64_t * 	px64 ;
	uint64_t *	pu64 ;
	int64_t *	pi64 ;
	double *	pf64 ;
// pointers to x32
	x32_t * 	px32 ;
	uint32_t *	pu32 ;
	int32_t *	pi32 ;
	float *		pf32 ;
// pointer to x16
	x16_t *		px16 ;
	uint16_t *	pu16 ;
	int16_t *	pi16 ;
// pointers to x08
	x8_t *		px8 ;
	uint8_t *	pu8 ;
	int8_t *	pi8 ;
	const char *pcc8 ;
	seconds_t *	psec ;
	char *		pc8 ;
	char **		ppc8 ;
} p32_t ;

// ################################## Structure pointer container ##################################

typedef union ps_u {
	struct	x32mma_s *	pMMA ;
	struct	x32mmab_s *	pMMAB ;
	struct	x32stat_s *	pSTAT ;
	struct	TSZ_s *		pTSZ ;
	struct	cli_t *		psCLI ;
	struct	complex_s * psCX ;
	union	ow_rom_u *	pOW_ROM ;
} ps_t ;

// #################################### All-In-One container #######################################

typedef	union z32_u {
	void *	pvoid ;
	x32_t	x32 ;
	p32_t	p32 ;
	ps_t	ps ;
} z32_t ;

typedef	union z64_u {
	x32_t		x32[2] ;
	p32_t		p32[2] ;
	z32_t		z32[2] ;
	ps_t		ps[2] ;
	x64_t		x64 ;
} z64_t ;

// ##################################### CLI related structures ####################################

typedef struct __attribute__((packed)) cli_s {
	char *			pcBeg ;								// Buffer beginning
	char *			pcStore ;							// Buffer position
	char *			pcParse ;
	struct cmnd_s *	pasList ;							// Command List
	z64_t			z64Var ;
	uint8_t			u8BSize ;
	uint8_t			u8LSize ;							// Command List Size
	uint8_t			bMode	: 1 ;						// Long mode
	uint8_t			bEcho	: 1 ;
	uint8_t			bForce	: 1 ;						// force flags display
} cli_t ;

typedef	struct	cmnd_s {
	char	cmnd[4] ;
	int32_t	(* hdlr) (cli_t *) ;
} cmnd_t ;

enum {													// {flags}{counter}
	maskCOUNT	= 0x000FFFFF,							// counter value or mask
	mfbCOUNT	= 1 << 20,								// Prefix 24x LSB uCount
	mfbCOLOR	= 1 << 21,								// Use colours where applicable
	mfbPRIOX	= 1 << 22,
	mfbSTATE	= 1 << 23,
	mfbSTACK	= 1 << 24,
	mfbCORE		= 1 << 25,
	mfbXTRAS	= 1 << 26,
	mfbSPARE1	= 1 << 27,
	mfbSPARE2	= 1 << 28,
	mfbSPARE3	= 1 << 29,
	mfbNL		= 1 << 30,								// PostFix 'n'
	mfbRT		= 1 << 31,								// prefix RunTime
} ;

typedef	union flagmask_u {
	struct {											// Generic structure for init
		uint32_t	m		: 20 ;
		uint8_t		l		: 1 ;
		uint8_t		k		: 1 ;
		uint8_t		j		: 1 ;
		uint8_t		i		: 1 ;
		uint8_t		h		: 1 ;
		uint8_t		g		: 1 ;
		uint8_t		f		: 1 ;
		uint8_t		e		: 1	;
		uint8_t		d		: 1 ;
		uint8_t		c		: 1 ;
		uint8_t		b		: 1 ;
		uint8_t		a		: 1 ;
	} ;
	struct {											// Printing control
		uint32_t	uCount	: 20 ;
		uint8_t		bCount	: 1 ;
		uint8_t		bColor	: 1 ;
		uint8_t		bPrioX	: 1 ;						// Priorities
		uint8_t		bState	: 1 ;						// Task state RBPS
		uint8_t		bStack	: 1 ;						// Low Stack value
		uint8_t		bCore	: 1 ;						// MCU 01X
		uint8_t		bXtras	: 1 ;						// Ticks, Stack & TCB
		uint8_t		bSpare1	: 1	;
		uint8_t		bSpare2	: 1 ;
		uint8_t		bSpare3	: 1 ;
		uint8_t		bNL		: 1 ;						// terminating NL
		uint8_t		bRT		: 1 ;						// Runtime
	} ;
	uint32_t	u32Val ;
} flagmask_t ;
DUMB_STATIC_ASSERT( sizeof(flagmask_t) == 4) ;

#define	makeMASKFLAG(A,B,C,D,E,F,G,H,I,J,K,L,M) (flagmask_t) \
	{ .a=A, .b=B, .c=C, .d=D, .e=E, .f=F, .g=G, .h=H, .i=I, .j=J, .k=K, .l=L, .m=M }

// ################################### complex VAR related structures ##############################

typedef enum varsize_e { vs08B, vs16B, vs32B, vs64B } varsize_t ;

typedef enum varform_e { vfUXX, vfIXX, vfFXX, vfSXX } varform_t ;

typedef enum vartype_e {			// HANDLE WITH CARE - SEQUENCE IS CRITICAL !!!
	vtNULL	= 0,					// first ie ZERO and unused !!!!
	vtVALUE,						// single value, direct or indirect
	vtMMA,							// MinMaxAvg structure
	vtMMAB,							// MinMaxAvgBuf structure
	vtSTAT,							// Statistics structure
	vtARRAY,						// pointer to array of values/strings
	vtCLOCK,						// epoch time
	vtTIMER,						// uSec based time ie RunTime
	vtTABLE,						// table of rows/records and columns/fields
	vtCOMPLEX,						// primarily 1-Wire thermometers for now
} vartype_t ;

typedef	union vardef_u {
	struct {
		uint8_t		varcount 	: 8 ;		// number of items in array type ?
		uint8_t		varindex 	: 8 ;		// CVARS: index for value to get/set ?
		varsize_t	varsize 	: 3 ;		// 2 + 1 storage size of an individual element
		vartype_t	vartype		: 5 ;		// 4 + 1 VALUE, MMA[B], STAT, ARRAY, CLOCK, TABLE etc
		varform_t	varform		: 3 ;		// 2 + 1 UXX, IXX, FXX or SXX
		uint8_t		pntr		: 1 ;		// indirect/pointer to type
		uint8_t		sumX		: 1 ;		// treat as a sum, add value on update
		uint8_t		sense		: 1 ;		// indicate when sense command has changed default config
		uint8_t		spare		: 2 ;
	} cv ;
	struct {
		uint8_t		varcount 	: 8 ;		// number of items in array type ?
		uint8_t		varindex 	: 8 ;		// STATS: specify MMA_IDX??? value to be logged
		varsize_t	varsize 	: 3 ;		// 2 + 1 storage size of an individual element
		vartype_t	vartype		: 5 ;		// 4 + 1 VALUE, MMA[B], STAT, ARRAY, CLOCK, TABLE etc
		varform_t	varform		: 3 ;		// 2 + 1 UXX, IXX, FXX or SXX
		uint8_t		rst			: 1 ;		// used to indicate AVGL must be reset with this value...
		uint8_t		init		: 1 ;		// set in statistics after first run/update done
		uint8_t		step		: 1 ;		// stats updated AND value added to buffer
		uint8_t		zero		: 1 ;		// if set, value=0 will not update AVGx, DEVx, CNTx or SUMx
		uint8_t		alloc		: 1 ;		// set if memory allocated, 0 if provided
	} st ;
	uint32_t		val ;
} vardef_t ;
DUMB_STATIC_ASSERT(sizeof(vardef_t) == 4) ;

/* Similar to the basic 4 byte structure but including 4 byte (1 word) space for
 * the actual variable or pointer to variable,array or complex structure */
typedef	union var_t {
	struct {
		vardef_t	varDef ;
		z32_t		varVal ;
	} ;
	uint64_t		varValue ;
} var_t ;

typedef	union x64var_t {
	struct __attribute__((packed)) {
		vardef_t	varDef ;
		z64_t		varVal ;
	} ;
	uint32_t		varValues[3] ;
} x64var_t ;

// ##################################### endpoint related structures ###############################

typedef enum {
	epSCALE_NANO	= -3,
	epSCALE_MICRO	= -2,
	epSCALE_MILLI	= -1,
	epSCALE_NONE	= 0,
	epSCALE_KILO	= 1,
	epSCALE_MEGA	= 2,
	epSCALE_GIGA	= 3,
} ep_scale_t ;

typedef	union epid_u {									// endpoint ID value
	struct {
		uint8_t		devclass ;							// Device category ie ACTxxx, BMA222, BMP180, TMPxxx etc
		uint8_t		subclass ;							// Device SubCategory ie subxxxx, A_xxxx, D_xxxx, R_xxxx etc
		uint8_t		epuri ;
		uint8_t		epunit ;
	} ;
	uint32_t	val ;
} epid_t ;
DUMB_STATIC_ASSERT(sizeof(epid_t) == 4) ;

typedef union event_u {
	struct {
		union {
			x32_t		var ;
			z32_t		pVar ;
		} ;
		uint8_t		Event ;								// kwPRESSED, kwRELEASE etc..
		uint8_t		Idx ;								// mostly 0, 1->n for multiple buttons
		uint8_t		epuri ;
		uint8_t		epunit ;
	} ;
	uint64_t		val ;
} event_t ;

/*
 * Definition of a table entry containing all the DYNAMIC (SRAM based)
 * info related to an endpoint.
 * FIXED sequence, tied to macros to build table!!!
 */
typedef	struct __attribute__((__packed__)) ep_work_s {
	var_t		Var ;
	uint32_t	tSeconds ;								// timestamp when last read ie latest value
	uint32_t	Tsns ;									// SNS reload value
	uint32_t	Rsns ;									// SNS countdown value, 0 to stop
	uint32_t	Tlog ;									// LOG reload value
	uint32_t	Rlog ;									// LOG countdown value, 0 to stop
	uint32_t	Tadd ;									// LOG to buffer (if buffered) countdown
	struct  {
		union {
			struct {
				int8_t		fScale	: 3 ;
				uint8_t		fSpare	: 2 ;
				uint8_t		fSECsns	: 1 ;				// 0=Tsns in EWP, 1=Tsns in EWS
				uint8_t		fAlert	: 1 ;
				uint8_t		fSense	: 1 ;
			} ;
			uint8_t		flag ;							// SENSE vs ALERT (vs OBSERVE for CoAP)
		} ;
		uint8_t		idx ;								// EPP = logged #, EWS = enumerated #
		uint8_t		uri ;
		uint8_t		eChan ;
	} ;
} ep_work_t ;

typedef union alert_u {
	struct {
		void *		pvValue ;
		uint8_t		Type ;
		uint8_t 	Level ;
		uint8_t		Field ;
		uint8_t		Chan ;
	} ;
	uint64_t		Value ;
} alert_t ;

typedef struct complex_s {								// vtCOMPLEX handlers
	ep_work_t * (* work) (int32_t) ;
	void	(* reset) (ep_work_t *, ep_work_t *) ;
	void	(* sense) (ep_work_t *, ep_work_t *) ;
	float	(* get) (ep_work_t *) ;
} complex_t ;

// ###################################### Public functions #########################################

#ifdef __cplusplus
}
#endif
