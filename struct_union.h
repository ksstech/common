/*
 * struct_union.h
 */

#pragma	once

#include	"x_time.h"				// stdint time x_definitions

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

typedef union __attribute__((packed)) ow_rom_t {
	uint64_t	Value ;
	uint8_t		HexChars[8] ;
	union {
		struct __attribute__((packed)) {
			uint8_t		Family ;
			uint8_t		TagNum[6] ;
			uint8_t		CRC ;
		} ;
		struct __attribute__((packed)) {
			uint8_t		buf1	: 8 ;
			uint64_t	Tagval	: 48 ;
			uint8_t		buf2	: 8 ;
		} ;
	};
} ow_rom_t ;
DUMB_STATIC_ASSERT( sizeof(ow_rom_t) == 8) ;

typedef union __attribute__((packed)) mac_addr_t {
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

typedef	union __attribute__((packed)) xVer_u {			// Version numbers
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
	u64_t *		pu64 ;
	i64_t *		pi64 ;
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

struct x32mma_t ;
struct x32mmab_t ;
struct x32stat_t ;
struct pcnt_t ;
struct TSZ_t ;
struct vt_enum_t ;

typedef union ps_t {
	struct x32mma_t *	pMMA ;
	struct x32mmab_t *	pMMAB ;
	struct x32stat_t *	pSTAT ;
	struct pcnt_t *		psPCNT ;
	struct TSZ_t *		pTSZ ;
	struct vt_enum_t *	psCX ;
	union ow_rom_t *	pOW_ROM ;
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
	mfbRT		= 1 << 0,								// prefix RunTime
	mfbNL		= 1 << 1,								// PostFix 'n'
	mfbXTRAS	= 1 << 2,
	mfbCORE		= 1 << 3,
	mfbSTACK	= 1 << 4,
	mfbSTATE	= 1 << 5,
	mfbPRIOX	= 1 << 6,
	mfbCOLOR	= 1 << 7,								// Use colours where applicable
	mfbCOUNT	= 1 << 8,								// Prefix 23x LSB uCount
	maskCOUNT	= 0x007FFFFF,							// counter value or mask
} ;

typedef	union {
	struct __attribute__((packed)) {					// Generic structure for init
/*LSB*/	int	a:1;
		int	b:1;
		int	c:1;
		int	d:1;
		int	e:1;
		int	f:1;
		int	g:1;
		int	h:1;
		int	i:1;
/*MSB*/	int	j:23;
	} ;
	struct __attribute__((packed)) {					// Printing control
/*LSB*/	int	bRT		: 1 ;								// Runtime
		int	bNL		: 1 ;								// terminating NL
		int	bXtras	: 1 ;								// Ticks, Stack & TCB
		int	bCore	: 1 ;								// MCU 01X
		int	bStack	: 1 ;								// Low Stack value
		int	bState	: 1 ;								// Task state RBPS
		int	bPrioX	: 1 ;								// Priorities
		int	bColor	: 1 ;								// Use colour
		int	bCount	: 1 ;								// Task #
/*MSB*/	int	uCount	: 23 ;								// Task # mask
	} ;
	uint32_t	u32Val ;
} flagmask_t ;
DUMB_STATIC_ASSERT(sizeof(flagmask_t) == 4) ;

#define	makeMASKFLAG(A,B,C,D,E,F,G,H,I,J) (flagmask_t) \
	{ .a=A, .b=B, .c=C, .d=D, .e=E, .f=F, .g=G, .h=H, .i=I, .j=J }

#ifdef __cplusplus
}
#endif
