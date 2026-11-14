// struct_union.h

#pragma	once

#include "timeX.h"

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ########################################### Macros ##############################################

// ######################################## Enumerations ###########################################

// ############################# common complex data types/ structures #############################

// See http://www.catb.org/esr/structure-packing/
// Also http://c0x.coding-guidelines.com/6.7.2.1.html

typedef union u64rt_t {	struct { u32_t LSW, MSW; }; u64_t U64val; } u64rt_t; // LSW then MSW sequence critical

//	va_list:	sz=12	pntr	pntr	ofst	
//	va_fake_t:	sz=16
typedef union va_fake_t { char * pa; va_list va; } va_fake_t;

typedef union mac_addr_t {
	struct __attribute__((packed)) { u8_t str6hex[6], filler1[2]; };
	struct __attribute__((packed)) { u64_t value:48; u16_t filler2:16; };
} mac_addr_t;
DUMB_STATIC_ASSERT(sizeof(mac_addr_t) == 8);

typedef	union ver_t {				// Version numbers
	struct __attribute__((packed)) {
		u8_t rev;					// Revision of Sub
		u8_t sub;					// Sub version
		u8_t min;					// Minor version
		u8_t maj;					// Major version
	};
	u32_t val;
} ver_t;
DUMB_STATIC_ASSERT(sizeof(ver_t) == 4);

/* 1/2/3/4/8-bit option variables, sized at compile time (options_size.h). Each width is a little-endian byte array:
 * option n of a width occupies bits [n * width, n * width + width - 1] counted from bit 0 of byte 0 - the same bit
 * positions the former u64_t unions gave (byte 0 = least significant byte), so the full set is byte-identical to
 * the old layout and existing NVS blobs read unchanged. Access only through options.c (xOptionGet/xOptionSet). */
typedef struct __attribute__((packed)) ioset_t {
#if (optB1_BYTES > 0)
	u8_t ioB1[optB1_BYTES];							// 1-bit options
#endif
#if (optB2_BYTES > 0)
	u8_t ioB2[optB2_BYTES];							// 2-bit options
#endif
#if (optB3_BYTES > 0)
	u8_t ioB3[optB3_BYTES];							// 3-bit options (top bit of the last byte unused at 21)
#endif
#if (optB4_BYTES > 0)
	u8_t ioB4[optB4_BYTES];							// 4-bit options
#endif
#if (optB8_BYTES > 0)
	u8_t ioB8[optB8_BYTES];							// 8-bit options
#endif
} ioset_t;
DUMB_STATIC_ASSERT(sizeof(ioset_t) == optBYTES);

//#pragma GCC diagnostic ignored "-Wpacked-bitfield-compat"
//#pragma GCC diagnostic pop

// ######################################### 8 bit types ###########################################

typedef union { u8_t u8; s8_t s8; i8_t i8; char c8; } x8_t;
DUMB_STATIC_ASSERT(sizeof(x8_t) == 1);

// ######################################### 16 bit types ##########################################

typedef union { u16_t u16; i16_t i16; x8_t x8[2]; } x16_t;
DUMB_STATIC_ASSERT(sizeof(x16_t) == 2);

// ######################################### 24 bit types ##########################################

typedef union __attribute__((packed)) u24_t { u32_t u24:24; } u24_t;
DUMB_STATIC_ASSERT(sizeof(u24_t) == 3);

typedef union __attribute__((packed)) i24_t { i32_t i24:24; } i24_t;
DUMB_STATIC_ASSERT(sizeof(i24_t) == 3);

typedef union __attribute__((packed)) x24_t {
	u32_t u24:24;
	i32_t i24:24;
	x8_t x8[3];
	struct  __attribute__((packed)) { u16_t u16; u8_t u8a; };
	struct  __attribute__((packed)) { i16_t i16; u8_t u8b; };
} x24_t;
DUMB_STATIC_ASSERT(sizeof(x24_t) == 3);

// ########################################## x32 types ############################################

typedef union __attribute__((packed)) x32_t {
	int	iX;
	u32_t u32;
	i32_t i32;
	f32_t f32;
	x16_t x16[2];
	x8_t x8[4];
	struct __attribute__((packed)) { x24_t x24; x8_t X8val; };
	struct __attribute__((packed)) { u24_t u24; x8_t U8val; };
	struct __attribute__((packed)) { i24_t i24; x8_t I8val; };
} x32_t;
DUMB_STATIC_ASSERT(sizeof(x32_t) == 4);

// ########################################## x64 types ############################################

typedef union __attribute__((packed)) x64_t {
	u64_t u64; 
	i64_t i64; 
	f64_t f64; 
	x32_t x32[2]; 
	x16_t x16[4]; 
	x8_t x8[8];
} x64_t;
DUMB_STATIC_ASSERT(sizeof(x64_t) == 8);

// ########################################## Other types ##########################################

typedef struct __attribute__((packed)) Q8dot4_t {
	int	res1 : 4;					// LSB
	int fract : 4;
	int integ : 8;
} Q8dot4_t;

typedef struct __attribute__((packed)) Q18dot2_t {
	int	res1 : 4;					// LSB
	int fract : 2;
	int integ : 18;
} Q18dot2_t;

// ###################################### px (32/64) pointer types #################################

typedef union px_t {
	void * pv;
	void * * ppv;
	int * piX;
	x64_t * px64;					// pointers to x64
	u64_t *	pu64;
	i64_t *	pi64;
	double * pf64;
	x32_t * px32;					// pointers to x32
	u32_t *	pu32;
	i32_t *	pi32;
	float *	pf32;
	x24_t * px24;					// pointers to x24
	u24_t * pu24;
	i24_t * pi24;
	x16_t *	px16;					// pointer to x16
	u16_t *	pu16;
	i16_t *	pi16;
	x8_t * px8;						// pointers to x08
	u8_t * pu8;
	s8_t * pi8;
	char * pc8;
	char ** ppc8;
	const char *pcc8;
	seconds_t *	psec;
} px_t;
DUMB_STATIC_ASSERT(sizeof(px_t) == sizeof(void *));

// ################################## Structure pointer container ##################################

struct epw_t;
struct x32mma_t;
struct x32mmab_t;
struct x32stat_t;
struct pcnt_t;
struct tsz_t;
struct vt_enum_t;
union ow_rom_t;
struct mb_srv_t;
union vd_t;
struct v32_t;
struct v64_t;

typedef union ps_t {
	struct epw_t * psEPW;
	struct x32mma_t * pMMA;
	struct x32mmab_t * pMMAB;
	struct x32stat_t * pSTAT;
	struct pcnt_t * psPCNT;
	struct tsz_t * pTSZ;
	const struct vt_enum_t * psCX;
	union ow_rom_t * pOW_ROM;
	struct mb_srv_t * psMBSrv;
	union vd_t * psVD;
	struct v32_t * psV32;
	struct v64_t * psV64;
	px_t pX;
	void * pV;
} ps_t;
DUMB_STATIC_ASSERT(sizeof(ps_t) == sizeof(void *));

// #################################### All-In-One container #######################################

typedef	union z32_t { x32_t x32; px_t px; ps_t ps; } z32_t;
DUMB_STATIC_ASSERT(sizeof(z32_t) == 4);

typedef	union z64_t { x64_t x64; px_t px; ps_t ps; } z64_t;
DUMB_STATIC_ASSERT(sizeof(z64_t) == 8);

#ifdef __cplusplus
}
#endif
