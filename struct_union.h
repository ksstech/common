/*
 * struct_union.h
 */

#pragma	once

#include "x_time.h"				// time x_definitions

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// See http://www.catb.org/esr/structure-packing/
// Also http://c0x.coding-guidelines.com/6.7.2.1.html

// ###################################### enumerations #############################################

typedef enum {
	epSCALE_NANO	= -3,
	epSCALE_MICRO	= -2,
	epSCALE_MILLI	= -1,
	epSCALE_NONE	= 0,
	epSCALE_KILO	= 1,
	epSCALE_MEGA	= 2,
	epSCALE_GIGA	= 3,
} ep_scale_t;

// ############################# common complex data types/ structures #############################

typedef union va_fake_t { char * pa; va_list va; } va_fake_t;

typedef union mac_addr_t {
	struct __attribute__((packed)) { u8_t str6hex[6], filler1[2]; };
	struct __attribute__((packed)) { u64_t value:48; u16_t filler2:16; };
} mac_addr_t;
DUMB_STATIC_ASSERT(sizeof(mac_addr_t) == 8);

typedef	union xVer_u {				// Version numbers
	struct __attribute__((packed)) {
		u8_t	ver_rev;			// Revision of Sub
		u8_t	ver_sub;			// Sub version
		u8_t	ver_min;			// Minor version
		u8_t	ver_maj;			// Major version
	};
	u32_t val;
}  xVer_t;
DUMB_STATIC_ASSERT(sizeof(xVer_t) == 4);
#define	MAKE_VERSION(Maj, Min, Sub, Rev)	((Maj << 24) + (Min << 16) + (Sub << 8) + Rev)

enum {								// {flags}{counter}
	maskCOUNT	= 0x007FFFFF,		// counter value or mask
	mfbCOUNT	= (1 << 23),		// Prefix 23x LSB uCount
	mfbCOLOR	= (1 << 24),		// Use colours where applicable
	mfbPRIOX	= (1 << 25),
	mfbSTATE	= (1 << 26),
	mfbSTACK	= (1 << 27),
	mfbCORE		= (1 << 28),
	mfbXTRAS	= (1 << 29),
	mfbNL		= (1 << 30),		// PostFix 'n'
	mfbRT		= (1 << 31),		// prefix RunTime
};

typedef	union {
	struct __attribute__((packed)) {	// 8:24 Generic
		union {
			struct __attribute__((packed)) { u32_t z00:1, z01:1, z02:1, z03:1, zv04:1, z05:1, z06:1, z07:1, z08:1, z09:1, z10:1, z11:1, z12:1, z13:1, z14:1, z15:1, z16:1, z17:1, z18:1, z19:1, z20:1, z21:1, z22:1, z23:1; };
			struct __attribute__((packed)) { u32_t y00:2, y01:1, y02:1, y03:1, yv04:1, y05:1, y06:1, y07:1, y08:1, y09:1, y10:1, y11:1, y12:1, y13:1, y14:1, y15:1, y16:1, y17:1, y18:1, y19:1, y20:1, y21:1, y22:1; };
			struct __attribute__((packed)) { u32_t x00:3, x01:1, x02:1, x03:1, xv04:1, x05:1, x06:1, x07:1, x08:1, x09:1, x10:1, x11:1, x12:1, x13:1, x14:1, x15:1, x16:1, x17:1, x18:1, x19:1, x20:1, x21:1; };
			struct __attribute__((packed)) { u32_t w00:4, w01:1, w02:1, w03:1, wv04:1, w05:1, w06:1, w07:1, w08:1, w09:1, w10:1, w11:1, w12:1, w13:1, w14:1, w15:1, w16:1, w17:1, w18:1, w19:1, w20:1; };
			struct __attribute__((packed)) { u32_t v00:5, v01:1, v02:1, v03:1, vv04:1, v05:1, v06:1, v07:1, v08:1, v09:1, v10:1, v11:1, v12:1, v13:1, v14:1, v15:1, v16:1, v17:1, v18:1, v19:1; };
			struct __attribute__((packed)) { u32_t u00:6, u01:1, u02:1, u03:1, uv04:1, u05:1, u06:1, u07:1, u08:1, u09:1, u10:1, u11:1, u12:1, u13:1, u14:1, u15:1, u16:1, u17:1, u18:1; };
			struct __attribute__((packed)) { u32_t t00:7, t01:1, t02:1, t03:1, tv04:1, t05:1, t06:1, t07:1, t08:1, t09:1, t10:1, t11:1, t12:1, t13:1, t14:1, t15:1, t16:1, t17:1; };
			struct __attribute__((packed)) { u32_t s00:8, s01:1, s02:1, s03:1, sv04:1, s05:1, s06:1, s07:1, s08:1, s09:1, s10:1, s11:1, s12:1, s13:1, s14:1, s15:1, s16:1; };
			struct __attribute__((packed)) { u32_t r00:9, r01:1, r02:1, r03:1, rv04:1, r05:1, r06:1, r07:1, r08:1, r09:1, r10:1, r11:1, r12:1, r13:1, r14:1, r15:1; };
			struct __attribute__((packed)) { u32_t q00:10, q01:1, q02:1, q03:1, qv04:1, q05:1, q06:1, q07:1, q08:1, q09:1, q10:1, q11:1, q12:1, q13:1, q14:1; };
			struct __attribute__((packed)) { u32_t p00:11, p01:1, p02:1, p03:1, pv04:1, p05:1, p06:1, p07:1, p08:1, p09:1, p10:1, p11:1, p12:1, p13:1; };
			struct __attribute__((packed)) { u32_t o00:12, o01:1, o02:1, o03:1, ov04:1, o05:1, o06:1, o07:1, o08:1, o09:1, o10:1, o11:1, o12:1; };
			struct __attribute__((packed)) { u32_t n00:13, n01:1, n02:1, n03:1, nv04:1, n05:1, n06:1, n07:1, n08:1, n09:1, n10:1, n11:1; };
			struct __attribute__((packed)) { u32_t m00:14, m01:1, m02:1, m03:1, mv04:1, m05:1, m06:1, m07:1, m08:1, m09:1, m10:1; };
			struct __attribute__((packed)) { u32_t l00:15, l01:1, l02:1, l03:1, lv04:1, l05:1, l06:1, l07:1, l08:1, l09:1; };
			struct __attribute__((packed)) { u32_t k00:16, k01:1, k02:1, k03:1, kv04:1, k05:1, k06:1, k07:1, k08:1; };
			struct __attribute__((packed)) { u32_t h00:17, h01:1, h02:1, h03:1, hv04:1, h05:1, h06:1, h07:1; };
			struct __attribute__((packed)) { u32_t g00:18, g01:1, g02:1, g03:1, gv04:1, g05:1, g06:1; };
			struct __attribute__((packed)) { u32_t f00:19, f01:1, f02:1, f03:1, fv04:1, f05:1; };
			struct __attribute__((packed)) { u32_t e00:20, e01:1, e02:1, e03:1, ev04:1; };
			struct __attribute__((packed)) { u32_t d00:21, d01:1, d02:1, d03:1; };
			struct __attribute__((packed)) { u32_t c00:22, c01:1, c02:1; };
			struct __attribute__((packed)) { u32_t b00:23, b01:1; };
			struct __attribute__((packed)) { u32_t a00:24; };
		};
		u32_t	h:1, g:1, f:1, e:1, d:1, c:1, b:1, a:1;
	};
	struct __attribute__((packed)) {	// 9:23 Printing tasks
		u32_t	uCount:23;			// Task # mask
		u32_t	bTskNum:1;			// Task #
		u32_t	bColor:1;			// Use colour
		u32_t	bPrioX:1;			// Priorities
		u32_t	bState:1;			// Task state RBPS
		u32_t	bStack:1;			// Low Stack value
		u32_t	bCore:1;			// MCU 01X
		u32_t	bXtras:1;
		u32_t	bNL:1;
		u32_t	bRT:1;
	};
	struct __attribute__((packed)) {	// 11:21 xRtosReportMemory & halMCU_ReportMemory
		u32_t	rmSpare:21;
		u32_t	rmDefault:1;
		u32_t	rmInternal:1;
		u32_t	rmPSram:1;
		u32_t	rmColor:1;			// same as bColor
		u32_t	rmIram:1;
		u32_t	rmExec:1;
		u32_t	rmDma:1;
		u32_t	rm8b:1;
		u32_t 	rm32b:1;
		u32_t	rmTotal:1;
		u32_t	rmSmall:1;
	};
	struct __attribute__((packed)) {	// 12:20 xRtosSensorsReport
		u32_t	senFree2:20;
		u32_t	senNL:1;
		u32_t 	senFree1:2;
		u32_t	senTlog:1;
		u32_t	senTsen:1;
		u32_t	senColor:1;			// same as bColor
		u32_t 	senDev:1;
		u32_t 	senAvg:1;
		u32_t 	senSum:1;
		u32_t 	senMMP:1;
		u32_t	senUnit:1;
		u32_t	senURI:1;
	};
	u32_t	u32Val;
} fm_t;
DUMB_STATIC_ASSERT(sizeof(fm_t) == 4);

#define	makeMASK08x24(A,B,C,D,E,F,G,H,I)	\
	((u32_t) (A<<31|B<<30|C<<29|D<<28|E<<27|F<<26|G<<25|H<<24|(I&0x00FFFFFF)))
#define	makeMASK09x23(A,B,C,D,E,F,G,H,I,J)	\
	((u32_t) (A<<31|B<<30|C<<29|D<<28|E<<27|F<<26|G<<25|H<<24|I<<23|(J&0x007FFFFF)))
#define	makeMASK10x22(A,B,C,D,E,F,G,H,I,J,K) \
	((u32_t) (A<<31|B<<30|C<<29|D<<28|E<<27|F<<26|G<<25|H<<24|I<<23|J<<22|(K&0x03FFFFF)))
#define	makeMASK11x21(A,B,C,D,E,F,G,H,I,J,K,L) \
	((u32_t) (A<<31|B<<30|C<<29|D<<28|E<<27|F<<26|G<<25|H<<24|I<<23|J<<22|K<<21|(L&0x01FFFFF)))
#define	makeMASK12x20(A,B,C,D,E,F,G,H,I,J,K,L,M) \
	((u32_t) (A<<31|B<<30|C<<29|D<<28|E<<27|F<<26|G<<25|H<<24|I<<23|J<<22|K<<21|L<<20|(M&0x00FFFFF)))

typedef struct report_t { char * pcBuf; size_t Size; fm_t sFM; } report_t;

// ######################################### 8 bit types ###########################################

typedef union { u8_t u8; s8_t s8; i8_t i8; char c8; } x8_t;
DUMB_STATIC_ASSERT(sizeof(x8_t) == 1);

// ######################################### 16 bit types ##########################################

typedef union { u16_t u16; s16_t i16; x8_t x8[2]; } x16_t;
DUMB_STATIC_ASSERT(sizeof(x16_t) == 2);

// ########################################## x32 types ############################################

typedef union { int	iX; u32_t u32; i32_t i32; float	f32; x16_t x16[2]; x8_t x8[4]; } x32_t;
DUMB_STATIC_ASSERT(sizeof(x32_t) == 4);

// ########################################## x64 types ############################################

typedef union { u64_t u64; i64_t i64; double f64; x32_t x32[2]; x16_t x16[4]; x8_t x8[8]; } x64_t;
DUMB_STATIC_ASSERT(sizeof(x64_t) == 8);

// ########################################## Other types ##########################################

typedef struct __attribute__((packed)) {
	int	res1 : 4;					// LSB
	int fract : 4;
	int integ : 8;
} Q8dot4_t;

typedef struct __attribute__((packed)) {
	int	res1 : 4;					// LSB
	int fract : 2;
	int integ : 18;
} Q18dot2_t;

// ###################################### px (32/64) pointer types #################################

typedef union {
	void * pv;
	void * * ppv;
	int * piX;
// pointers to x64
	x64_t * px64;
	u64_t *	pu64;
	i64_t *	pi64;
	double * pf64;
// pointers to x32
	x32_t * px32;
	u32_t *	pu32;
	i32_t *	pi32;
	float *	pf32;
// pointer to x16
	x16_t *	px16;
	u16_t *	pu16;
	s16_t *	pi16;
// pointers to x08
	x8_t *	px8;
	u8_t *	pu8;
	s8_t *	pi8;
	const char *pcc8;
	seconds_t *	psec;
	char * pc8;
	char ** ppc8;
} px_t;
DUMB_STATIC_ASSERT(sizeof(px_t) == sizeof(void *));

// ################################## Structure pointer container ##################################

struct x32mma_t;
struct x32mmab_t;
struct x32stat_t;
struct pcnt_t;
struct tsz_s;
struct vt_enum_t;
union ow_rom_t;
struct mb_srv_t;

typedef union {
	struct x32mma_t *	pMMA;
	struct x32mmab_t *	pMMAB;
	struct x32stat_t *	pSTAT;
	struct pcnt_t *		psPCNT;
	struct tsz_s *		pTSZ;
	const struct vt_enum_t * psCX;
	union ow_rom_t *	pOW_ROM;
	struct mb_srv_t *	psMBSrv;
} ps_t;
DUMB_STATIC_ASSERT(sizeof(ps_t) == sizeof(void *));

// #################################### All-In-One container #######################################

typedef	union { x32_t x32; px_t px; ps_t ps; } z32_t;
DUMB_STATIC_ASSERT(sizeof(z32_t) == 4);

typedef	union { x64_t x64; px_t px; ps_t ps; } z64_t;
DUMB_STATIC_ASSERT(sizeof(z64_t) == 8);

#ifdef __cplusplus
}
#endif
