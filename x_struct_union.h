/*
 * x_struct_union.h
 */

#pragma	once

#include	"x_time.h"						// x_definitions time.h stdint.h

#ifdef __cplusplus
extern "C" {
#endif

// ###################################### enumerations #############################################

typedef enum {
	epSCALE_NANO	= -3,
	epSCALE_MICRO	= -2,
	epSCALE_MILLI	= -1,
	epSCALE_NONE	= 0,
	epSCALE_KILO	= 1,
	epSCALE_MEGA	= 2,
	epSCALE_GIGA	= 3,
} ep_scale_t ;

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

typedef union x8_t {
	uint8_t		u8 ;
	int8_t		i8 ;
	char		c8 ;
} x8_t ;
DUMB_STATIC_ASSERT(sizeof(x8_t) == 1) ;

// ######################################### 16 bit types ##########################################

typedef union x16_t {
	uint16_t	u16 ;
	int16_t		i16 ;
	x8_t		x8[2] ;
} x16_t ;
DUMB_STATIC_ASSERT(sizeof(x16_t) == 2) ;

// ########################################## x32 types ############################################

typedef union x32_t {
	uint32_t	u32 ;
	int32_t		i32 ;
	float		f32 ;
	x16_t		x16[2] ;
	x8_t		x8[4] ;
} x32_t ;
DUMB_STATIC_ASSERT(sizeof(x32_t) == 4) ;

// ########################################## x64 types ############################################

typedef union x64_t {
	uint64_t	u64 ;
	int64_t		i64 ;
 	double		f64 ;
 	x32_t		x32[2] ;
	x16_t		x16[4] ;
	x8_t		x8[8] ;
} x64_t ;
DUMB_STATIC_ASSERT(sizeof(x64_t) == 8) ;

// ###################################### px (32/64) pointer types #################################

typedef union px_t {
	void *		pv ;
	void * *	ppv ;
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
} px_t ;
DUMB_STATIC_ASSERT(sizeof(px_t) == __SIZEOF_POINTER__) ;

// ################################## Structure pointer container ##################################

typedef union ps_t {
	struct	x32mma_s *	pMMA ;
	struct	x32mmab_s *	pMMAB ;
	struct	x32stat_s *	pSTAT ;
	struct	TSZ_s *		pTSZ ;
	struct	cli_t *		psCLI ;
	struct	complex_t * psCX ;
	union	ow_rom_u *	pOW_ROM ;
} ps_t ;
DUMB_STATIC_ASSERT(sizeof(ps_t) == __SIZEOF_POINTER__) ;

// #################################### All-In-One container #######################################

typedef	union z32_t {
#if (__SIZEOF_POINTER__ == 4)
	px_t	px ;
	ps_t	ps ;
#endif
	x32_t	x32 ;
} z32_t ;
DUMB_STATIC_ASSERT(sizeof(z32_t) == 4) ;

typedef	union z64_t {
	px_t	px ;
	ps_t	ps ;
	x64_t	x64 ;
} z64_t ;
DUMB_STATIC_ASSERT(sizeof(z64_t) == 8) ;

// ##################################### CLI related structures ####################################

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
DUMB_STATIC_ASSERT(sizeof(flagmask_t) == 4) ;

#define	makeMASKFLAG(A,B,C,D,E,F,G,H,I,J,K,L,M) (flagmask_t) \
	{ .a=A, .b=B, .c=C, .d=D, .e=E, .f=F, .g=G, .h=H, .i=I, .j=J, .k=K, .l=L, .m=M }


// ###################################### Public functions #########################################

#ifdef __cplusplus
}
#endif
