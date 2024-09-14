// struct_union.h

#pragma	once

#include "x_time.h"				// time x_definitions

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ########################################### Macros ##############################################
// ######################################## enumerations ###########################################

typedef enum {
	epSCALE_NANO	= -3,
	epSCALE_MICRO	= -2,
	epSCALE_MILLI	= -1,
	epSCALE_NONE	= 0,
	epSCALE_KILO	= 1,
	epSCALE_MEGA	= 2,
	epSCALE_GIGA	= 3,
} ep_scale_t;

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

// ############################# common complex data types/ structures #############################

// See http://www.catb.org/esr/structure-packing/
// Also http://c0x.coding-guidelines.com/6.7.2.1.html

typedef union u64rt_t {	struct { u32_t LSW, MSW; }; u64_t U64val; } u64rt_t; // LSW then MSW sequence critical

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

typedef struct __attribute__((packed)) ioset_t {		// 1/2/3/4/8 bit option variables
	union {							// 1-bit option variables
		struct __attribute__((packed)) {
			u8_t B1_0	: 1 ;
			u8_t B1_1	: 1 ;
			u8_t B1_2	: 1 ;
			u8_t B1_3	: 1 ;
			u8_t B1_4	: 1 ;
			u8_t B1_5	: 1 ;
			u8_t B1_6	: 1 ;
			u8_t B1_7	: 1 ;
			u8_t B1_8	: 1 ;
			u8_t B1_9	: 1 ;
			u8_t B1_10	: 1 ;
			u8_t B1_11	: 1 ;
			u8_t B1_12	: 1 ;
			u8_t B1_13	: 1 ;
			u8_t B1_14	: 1 ;
			u8_t B1_15	: 1 ;
			u8_t B1_16	: 1 ;
			u8_t B1_17	: 1 ;
			u8_t B1_18	: 1 ;
			u8_t B1_19	: 1 ;
			u8_t B1_20	: 1 ;
			u8_t B1_21	: 1 ;
			u8_t B1_22	: 1 ;
			u8_t B1_23	: 1 ;
			u8_t B1_24	: 1 ;
			u8_t B1_25	: 1 ;
			u8_t B1_26	: 1 ;
			u8_t B1_27	: 1 ;
			u8_t B1_28	: 1 ;
			u8_t B1_29	: 1 ;
			u8_t B1_30	: 1 ;
			u8_t B1_31	: 1 ;
			u8_t B1_32	: 1 ;
			u8_t B1_33	: 1 ;
			u8_t B1_34	: 1 ;
			u8_t B1_35	: 1 ;
			u8_t B1_36	: 1 ;
			u8_t B1_37	: 1 ;
			u8_t B1_38	: 1 ;
			u8_t B1_39	: 1 ;
			u8_t B1_40	: 1 ;
			u8_t B1_41	: 1 ;
			u8_t B1_42	: 1 ;
			u8_t B1_43	: 1 ;
			u8_t B1_44	: 1 ;
			u8_t B1_45	: 1 ;
			u8_t B1_46	: 1 ;
			u8_t B1_47	: 1 ;
			u8_t B1_48	: 1 ;
			u8_t B1_49	: 1 ;
			u8_t B1_50	: 1 ;
			u8_t B1_51	: 1 ;
			u8_t B1_52	: 1 ;
			u8_t B1_53	: 1 ;
			u8_t B1_54	: 1 ;
			u8_t B1_55	: 1 ;
			u8_t B1_56	: 1 ;
			u8_t B1_57	: 1 ;
			u8_t B1_58	: 1 ;
			u8_t B1_59	: 1 ;
			u8_t B1_60	: 1 ;
			u8_t B1_61	: 1 ;
			u8_t B1_62	: 1 ;
			u8_t B1_63	: 1 ;
		};
		u64_t ioB1;
	};
	union {							// 2-bit option variables
		struct __attribute__((packed)) {
			u8_t B2_0	: 2 ;
			u8_t B2_1	: 2 ;
			u8_t B2_2	: 2 ;
			u8_t B2_3	: 2 ;
			u8_t B2_4	: 2 ;
			u8_t B2_5	: 2 ;
			u8_t B2_6	: 2 ;
			u8_t B2_7	: 2 ;
			u8_t B2_8	: 2 ;
			u8_t B2_9	: 2 ;
			u8_t B2_10	: 2 ;
			u8_t B2_11	: 2 ;
			u8_t B2_12	: 2 ;
			u8_t B2_13	: 2 ;
			u8_t B2_14	: 2 ;
			u8_t B2_15	: 2 ;
			u8_t B2_16	: 2 ;
			u8_t B2_17	: 2 ;
			u8_t B2_18	: 2 ;
			u8_t B2_19	: 2 ;
			u8_t B2_20	: 2 ;
			u8_t B2_21	: 2 ;
			u8_t B2_22	: 2 ;
			u8_t B2_23	: 2 ;
			u8_t B2_24	: 2 ;
			u8_t B2_25	: 2 ;
			u8_t B2_26	: 2 ;
			u8_t B2_27	: 2 ;
			u8_t B2_28	: 2 ;
			u8_t B2_29	: 2 ;
			u8_t B2_30	: 2 ;
			u8_t B2_31	: 2 ;
		};
		u64_t ioB2;
	};
	union {							// 3-bit option variables
		struct __attribute__((packed)) {
			u16_t B3_0	: 3 ;
			u16_t B3_1	: 3 ;
			u16_t B3_2	: 3 ;
			u16_t B3_3	: 3 ;
			u16_t B3_4	: 3 ;
			u16_t B3_5	: 3 ;
			u16_t B3_6	: 3 ;
			u16_t B3_7	: 3 ;
			u16_t B3_8	: 3 ;
			u16_t B3_9	: 3 ;
			u16_t B3_10	: 3 ;
			u16_t B3_11	: 3 ;
			u16_t B3_12	: 3 ;
			u16_t B3_13	: 3 ;
			u16_t B3_14	: 3 ;
			u16_t B3_15	: 3 ;
			u16_t B3_16	: 3 ;
			u16_t B3_17	: 3 ;
			u16_t B3_18	: 3 ;
			u16_t B3_19	: 3 ;
			u16_t B3_20	: 3 ;
			u16_t B3_xx	: 1 ;
		};
		u64_t ioB3;
	};
	union {							// 4-bit option variables
		struct __attribute__((packed)) {
			u8_t B4_0	: 4 ;
			u8_t B4_1	: 4 ;
			u8_t B4_2	: 4 ;
			u8_t B4_3	: 4 ;
			u8_t B4_4	: 4 ;
			u8_t B4_5	: 4 ;
			u8_t B4_6	: 4 ;
			u8_t B4_7	: 4 ;
			u8_t B4_8	: 4 ;
			u8_t B4_9	: 4 ;
			u8_t B4_10	: 4 ;
			u8_t B4_11	: 4 ;
			u8_t B4_12	: 4 ;
			u8_t B4_13	: 4 ;
			u8_t B4_14	: 4 ;
			u8_t B4_15	: 4 ;
		};
		u64_t ioB4;
	};
	union {							// 8-bit option variables (added at end)
		struct __attribute__((packed)) {
			u8_t B8_0;
			u8_t B8_1;
			u8_t B8_2;
			u8_t B8_3;
			u8_t B8_4;
			u8_t B8_5;
			u8_t B8_6;
			u8_t B8_7;
		};
		u64_t ioB8;
	};
} ioset_t;
DUMB_STATIC_ASSERT(sizeof(ioset_t) == 40);

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
