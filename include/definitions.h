// definitions.h

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ####################################### ASCII char set ##########################################

#define	CHR_NUL						0x00		// c-@
#define	CHR_SOH						0x01		// c-A		Edit-SelectAll
#define	CHR_STX						0x02		// c-B
#define	CHR_ETX						0x03		// c-C		Edit-Copy
#define	CHR_EOT						0x04		// c-D
#define	CHR_ENQ						0x05		// c-E
#define	CHR_ACK						0x06		// c-F
#define	CHR_BEL						0x07		// c-G
#define	CHR_BS						0x08		// c-H		\b
#define	CHR_TAB						0x09		// c-I		\t
#define	CHR_LF						0x0A		// c-J		\n
#define	CHR_VT						0x0B		// c-K
#define	CHR_FF						0x0C		// c-L		FormFeed
#define	CHR_CR						0x0D		// c-M		\r
#define	CHR_SO						0x0E		// c-N
#define	CHR_SI						0x0F		// c-O
#define	CHR_DLE						0x10		// c-P
#define	CHR_DC1						0x11		// c-Q
#define	CHR_DC2						0x12		// c-R
#define	CHR_DC3						0x13		// c-S		XOFF
#define	CHR_DC4						0x14		// c-T
#define	CHR_NAK						0x15		// c-U
#define	CHR_SYN						0x16		// c-V		Edit-Paste
#define	CHR_ETB						0x17		// c-W
#define	CHR_CAN						0x18		// c-X		Edit-Delete
#define	CHR_EM						0x19		// c-Y
#define	CHR_SUB						0x1A		// c-Z
#define	CHR_ESC						0x1B		// c-3		\e
#define	CHR_FS						0x1C		// c-4		c-FWD SLASH
#define	CHR_GS						0x1D		// c-5		c-]
#define	CHR_RS						0x1E		// c-6		c-"
#define	CHR_US						0x1F		// c-7		c--		c-BACK SLASH

#define	CHR_SPACE					0x20		// ' '
#define	CHR_EXCLAMATION				0x21		// !
#define	CHR_DOUBLE_QUOTE			0x22		// "
#define	CHR_HASH					0x23		// #
#define	CHR_DOLLAR					0x24		// $
#define	CHR_PERCENT					0x25		// %
#define	CHR_AMPERSAND				0x26		// &
#define	CHR_SINGLE_QUOTE			0x27		// '
#define	CHR_L_ROUND					0x28		// (
#define	CHR_R_ROUND					0x29		// )
#define	CHR_ASTERISK				0x2A		// *
#define	CHR_PLUS					0x2B		// +
#define	CHR_COMMA					0x2C		// ,
#define	CHR_MINUS					0x2D		// -
#define	CHR_FULLSTOP				0x2E		// .
#define	CHR_FWDSLASH				0x2F		// /
#define	CHR_0						0x30
#define	CHR_1 						0x31
#define	CHR_2	  					0x32
#define	CHR_3		  				0x33
#define	CHR_4			  			0x34
#define	CHR_5				  		0x35
#define	CHR_6					  	0x36
#define	CHR_7						0x37
#define	CHR_8			  			0x38
#define	CHR_9				  		0x39
#define	CHR_COLON					0x3A		// :
#define	CHR_SEMICOLON				0x3B		// ;
#define	CHR_L_ANGLED				0x3C		// <
#define	CHR_EQUALS					0x3D		// =
#define	CHR_R_ANGLED				0x3E		// >
#define	CHR_QUESTION				0x3F		// ?
#define	CHR_AT_SIGN					0x40		// @
/* Uppercase A-Z */
#define	CHR_A						0x41		// Cursor UP
#define	CHR_B						0x42		// Cursor DOWN
#define	CHR_C						0x43
#define	CHR_D						0x44
#define	CHR_E						0x45
#define	CHR_F						0x46
#define	CHR_G						0x47
#define	CHR_H						0x48
#define	CHR_I						0x49
#define	CHR_J						0x4A
#define	CHR_K						0x4B
#define	CHR_L						0x4C
#define	CHR_M						0x4D
#define	CHR_N						0x4E
#define	CHR_O						0x4F
#define	CHR_P						0x50
#define	CHR_Q						0x51
#define	CHR_R						0x52
#define	CHR_S						0x53
#define	CHR_T						0x54
#define	CHR_U						0x55
#define	CHR_V						0x56
#define	CHR_W						0x57
#define	CHR_X						0x58
#define	CHR_Y						0x59
#define	CHR_Z						0x5A
#define	CHR_L_SQUARE				0x5B		// '['
#define	CHR_BACKSLASH				0x5C		// '\'
#define	CHR_R_SQUARE				0x5D		// ']'
#define	CHR_CARET					0x5E		// '^'
#define	CHR_UNDERSCORE				0x5F		// '_'
#define	CHR_GRAVE					0x60		// '`'
/* Lowercase a-z */
#define	CHR_a						0x61
#define	CHR_b						0x62
#define	CHR_c						0x63
#define	CHR_d						0x64
#define	CHR_e						0x65
#define	CHR_f						0x66
#define	CHR_g						0x67
#define	CHR_h						0x68
#define	CHR_i						0x69
#define	CHR_j						0x6A
#define	CHR_k						0x6B
#define	CHR_l						0x6C
#define	CHR_m						0x6D
#define	CHR_n						0x6E
#define	CHR_o						0x6F
#define	CHR_p						0x70
#define	CHR_q						0x71
#define	CHR_r						0x72
#define	CHR_s						0x73
#define	CHR_t						0x74
#define	CHR_u						0x75		// c-1
#define	CHR_v						0x76		// c-3
#define	CHR_w						0x77
#define	CHR_x						0x78
#define	CHR_y						0x79
#define	CHR_z						0x7A
#define	CHR_L_CURLY					0x7B		// '{'
#define	CHR_VERT_BAR				0x7C		// '|'
#define	CHR_R_CURLY					0x7D		// '}'
#define	CHR_TILDE					0x7E		// '~'
#define	CHR_DEL						0x7F		// c-8 = non printing !!!!

/* Linux = LF, macOS = CR,  Win = CRLF */
#if (CONFIG_LIBC_STDOUT_LINE_ENDING_CRLF == 1)
	#define	strNL					"\r\n"
	#define	strNLx2					"\r\n\n"
#elif (CONFIG_LIBC_STDOUT_LINE_ENDING_CR == 1)
	#define	strNL					"\r"
	#define	strNLx2					"\r\r"
#else		// default (CONFIG_LIBC_STDOUT_LINE_ENDING_LF == 1)
	#define	strNL					"\n"
	#define	strNLx2					"\n\n"
#endif

#define strNUL  					(char *) ""
#define	strNULL						(char *) "null"
#define	strOOR						(char *) "pOOR"

#define	strINV_CVI					"Invalid cvI"

#define	strPARSE_s	  				"~[%s]"
#define	strPARSE_Ps					"~[%*s]"
#define strPARSE_d	  				"~[%d]"
#define strPARSE_lu 				"~[]"
#define strPARSE_I					"~[%-I]"
#define strPARSE_b					"~[%b]"	 // dynamic format

// ################ Time related constants, all based on normal (NON leap) years ###################

#define MICROS_IN_MILLISEC			1000

#define MICROS_IN_SECOND			1000000
#define MILLIS_IN_SECOND			1000

#define SECONDS_IN_MINUTE			60

#define SECONDS_IN_HOUR				3600
#define	MINUTES_IN_HOUR				60

#define MILLIS_IN_DAY				86400000
#define SECONDS_IN_DAY				86400
#define	MINUTES_IN_DAY				1440
#define	HOURS_IN_DAY				24

#define SECONDS_IN_WEEK				604800
#define	MINUTES_IN_WEEK				10080
#define	HOURS_IN_WEEK				168
#define	DAYS_IN_WEEK				7
#define	DAYS_IN_MONTH_MAX			31

#define	SECONDS_IN_MONTH28D			( SECONDS_IN_DAY * 28 )
#define	SECONDS_IN_MONTH29D			( SECONDS_IN_DAY * 29 )
#define	SECONDS_IN_MONTH30D			( SECONDS_IN_DAY * 30 )
#define	SECONDS_IN_MONTH31D			( SECONDS_IN_DAY * 31 )

#define	SECONDS_IN_YEAR365			( SECONDS_IN_DAY * DAYS_IN_YEAR )
#define	MINUTES_IN_YEAR				( MINUTES_IN_DAY * DAYS_IN_YEAR )
#define	HOURS_IN_YEAR				( HOURS_IN_DAY * DAYS_IN_YEAR )
#define	DAYS_IN_YEAR				365
#define	MONTHS_IN_YEAR				12

#define	DAYS_IN_LEAPYEAR			366
#define	SECONDS_IN_LEAPYEAR			( SECONDS_IN_DAY * (DAYS_IN_LEAPYEAR ))

#define	SECONDS_IN_YEAR_AVG			( SECONDS_IN_YEAR365 + 20736)

// ########################## International Standards Organisation units ###########################

#define	KILO						1024UL
#define	MEGA						(KILO * KILO)
#define	GIGA						(KILO * KILO * KILO)
#define	TERA						(KILO * KILO * KILO * KILO)
#define	PETA						(KILO * KILO * KILO * KILO * KILO)

#define	THOUSAND					1000
#define	MILLION						1000000
#define	BILLION						1000000000
#define	TRILLION					1000000000000

// natural/physical constants
#define	CONSTANT_SQRT_2				1.4142136
#define	CONSTANT_PI					3.1428571

// number base constants
#define	BASE02						2
#define	BASE08						8
#define	BASE10						10
#define	BASE16						16

// true/false range tests
#define INRANGE(x,y,z)				(((y) >= (x)) && ((y) <= (z)) ? 1 : 0)
#define	BETWEEN(x,y,z)				(((y) >  (x)) && ((y) <  (z)) ? 1 : 0)
#define	OUTSIDE(x,y,z)				(((y) <  (x)) || ((y) >  (z)) ? 1 : 0)

#define	IR_T(x,y,z,c)				(((c)(x) <= (c)(y)) && ((c)(y) <= (c)(z)) ? 1 : 0)
#define	BT_T(x,y,z,c)				(((c)(x) <  (c)(y)) && ((c)(y) <  (c)(z)) ? 1 : 0)
#define	OS_T(x,y,z,c)				(((c)(x) >  (c)(y)) || ((c)(y) >  (c)(z)) ? 1 : 0)

// normalise a value to be in the range specified
#define	FIT2RANGE(x,y,z,c)			( y = ((c) x > (c) y) ? x : ((c) y > (c) z) ? z : y )
#define	TRIMMED(x,y,z,c)			( ((c) x > (c) y) ? x : ((c) y > (c) z) ? z : y )

/* Used to check build configuration where only 1 of 2 or 3 option may be selected
 * does not yet work correctly if a negative number is used in a definition */
#define	MORETHAN1of2(x,y)			((x==0 || x==1) && (y==0 || y==1) && ((x+y)==1)) ? 0 : 1
#define	MORETHAN1of3(x,y,z)			((x==0 || x==1) && (y==0 || y==1) && (z==0 || z==1) && ((x+y+z)==1)) ? 0 : 1
#define	MORETHAN1of4(w,x,y,z)		((w==0 || w==1) && (x==0 || x==1) && (y==0 || y==1) && (z==0 || z==1) && ((w+x+y+z)==1)) ? 0 : 1

#define	SUM_Xof2(x,y)				((x==0 || x==1) && (y==0 || y==1)) ? (x+y) : 0
#define	SUM_Xof3(x,y,z)				((x==0 || x==1) && (y==0 || y==1) && (z==0 || z==1)) ? (x+y+z) : 0
#define	SUM_Xof4(w,x,y,z)			((w==0 || w==1) && (x==0 || x==1) && (y==0 || y==1) && (z==0 || z==1)) ? (w+x+y+z) : 0

#define DEFINED_2OF2(x,y)			(((defined(x) + defined(y)) == 2) ? 1 : 0)
#define DEFINED_3OF3(x,y,z)			(((defined(x) + defined(y) + defined(z)) == 3) ? 1 : 0)
#define DEFINED_4OF4(w,x,y,z)		(((defined(w) + defined(x) + defined(y) + defined(z)) == 4) ? 1 : 0)

#define	POWER_OF_10(x)				((x) == 1 || (x) == 10 || (x) == 100 || (x) == 1000 ||	(x) == 10000 ||		\
									 (x) == 100000 || (x) == 1000000 || (x) == 10000000 || (x) == 100000000 ||	\
									 (x) == 1000000000 || (x) == 10000000000 || (x) == 100000000000 ? 1 : 0)

#define CNTR(T,V)					{ static u32_t V=0; if (T) ++V; else { PT(toSTR(V) "=%lu" strNL, V); V=0; } }

#define	SWAP(v1, v2, t)				{t vx = v1; v1 = v2; v2 = vx; }

#define	IF_EXEC(T,C)				if (T) { do { C; } while (0); }
#define	IF_EXEC_0(T,F)				if (T) { do { F(); } while (0); }
#define	IF_EXEC_1(T,F,a)			if (T) { do { F(a); } while (0); }
#define	IF_EXEC_2(T,F,a,b)			if (T) { do { F(a,b); } while (0); }
#define	IF_EXEC_3(T,F,a,b,c)		if (T) { do { F(a,b,c); } while (0); }
#define	IF_EXEC_4(T,F,a,b,c,d)		if (T) { do { F(a,b,c,d); } while (0); }
#define	IF_EXEC_5(T,F,a,b,c,d,e)	if (T) { do { F(a,b,c,d,e); } while (0); }
#define	IF_EXEC_6(T,F,a,b,c,d,e,f)	if (T) { do { F(a,b,c,d,e,f); } while (0); }

/* ##################### macros to create variable width 8/16/32/64 bit masks ######################
 * a is the starting bit number 0 -> 7/15/31/63
 * b is the ending bit number 0 -> 7/15/31/63
 * a: 0->xFF	1->xFE		2->xFC		3->xF8		4->xF0		5->xE0		6->xC0		7->x80
 * b: 0->x01	1->x03		2->x07		4->x0F		4->x1F		5->x3F		6->x7F		7->xFF
 * m: 0,0->x01 	0,1->0x03	1,2->x06	2,3->x0C	3,4->x18	4,5->x30	5,6->x60	6,7->xC0  7,7->x80
 */
#define BIT_MASK8(a,b)				(u8_t) ((u8_t)(0xFF >> (7-(b))) & ~((u8_t)(0x01 << (a))-1))
#define BIT_MASK16(a,b)				((0xFFFF >> (15-(b))) & ~((0x0001 << (a))-1))
#define BIT_MASK32(a,b)				((0xFFFFFFFF >> (31-(b))) & ~((1UL << (a))-1))
#define BIT_MASK64(a,b)				((0xFFFFFFFFFFFFFFFF >> (63-(b))) & ~((1ULL << (a))-1))

/* ################################## variable bit field flags #####################################
 * Based on 32/64bit variables, support primarily 1/2/3/4/8 bit sized flags */
#define	mask1B						0x01
#define	mask2B						0x03
#define	mask3B						0x07
#define	mask4B						0x0F
#define	mask8B						0xFF

#if 0								/* ignores size/cast, works with u16/32/64 */
	#define	maskGET1B(f,i,s)		((f >> ((i) * 1)) & 0x001UL)
	#define	maskGET2B(f,i,s)		((f >> ((i) * 2)) & 0x003UL)
	#define	maskGET3B(f,i,s)		((f >> ((i) * 3)) & 0x007UL)
	#define	maskGET4B(f,i,s)		((f >> ((i) * 4)) & 0x00FUL)
	#define	maskGET8B(f,i,s)		((f >> ((i) * 8)) & 0x0FFUL)
#elif 1
	#define	maskGET1B(f,i,s)		((s) ((s)f >> ((s)(i) * 1)) & (s)mask1B)
	#define	maskGET2B(f,i,s)		((s) ((s)f >> ((s)(i) * 2)) & (s)mask2B)
	#define	maskGET3B(f,i,s)		((s) ((s)f >> ((s)(i) * 3)) & (s)mask3B)
	#define	maskGET4B(f,i,s)		((s) ((s)f >> ((s)(i) * 4)) & (s)mask4B)
	#define	maskGET8B(f,i,s)		((s) ((s)f >> ((s)(i) * 8)) & (s)mask8B)
#else
	#define	maskGET1B(f,i,s)		((s) ((f >> ( (i) * 1)) & (s) mask1B))
	#define	maskGET2B(f,i,s)		((s) ((f >> ( (i) * 2)) & (s) mask2B))
	#define	maskGET3B(f,i,s)		((s) ((f >> ( (i) * 3)) & (s) mask3B))
	#define	maskGET4B(f,i,s)		((s) ((f >> ( (i) * 4)) & (s) mask4B))
	#define	maskGET8B(f,i,s)		((s) ((f >> ( (i) * 8)) & (s) mask8B))
#endif

#define	maskSET1B(f,i,x,s)			{s m=(s)mask1B<<((i)*1); f&=~m; f|=((((s)x)<<((i)*1))&m);}
#define	maskSET2B(f,i,x,s)			{s m=(s)mask2B<<((i)*2); f&=~m; f|=((((s)x)<<((i)*2))&m);}
#define	maskSET3B(f,i,x,s)			{s m=(s)mask3B<<((i)*3); f&=~m; f|=((((s)x)<<((i)*3))&m);}
#define	maskSET4B(f,i,x,s)			{s m=(s)mask4B<<((i)*4); f&=~m; f|=((((s)x)<<((i)*4))&m);}
#define	maskSET8B(f,i,x,s)			{s m=(s)mask8B<<((i)*8); f&=~m; f|=((((s)x)<<((i)*8))&m);}

// ###################################### General Use Macros #######################################

#define maskLOW8(x)					((x) & 0x000000FF)
#define maskLOW16(x)				((x) & 0x0000FFFF)
#define maskLOW24(x)				((x) & 0x00FFFFFF)

#define MSWofU64(U64)				((u32_t)(U64 >> 32))
#define LSWofU64(U64)				((u32_t)(U64 & BIT_MASK64(0,31)))
		
#define	VERSION(Va,Vb,Vc,Vd)		((Va << 24) | (Vb << 16) | (Vc << 8) | Vd)

// https://stackoverflow.com/questions/3553296/c-sizeof-single-struct-member
#define SO_MEM(T,M)					sizeof(((T *)0)->M)

// return the number of elements in an array
#define	NO_MEM(A)					((int) ((sizeof(A)) / (sizeof(A[0]))))

// return the number of elements in an array that is a member of a structure
#define NO_ELEM(S,M)				((int)(sizeof(((S *)0)->M) / sizeof(((S *)0)->M[0])))

#define ADR_TO_IDX(array, addr)		((addr - &array[0]) / sizeof(array[0]))

#define	UNUSED(x)					(void) (x) ;

#define	debugAPPL_PLACE				"I'm LOST!!\n"

#define CHECK_ONCE(mes, ...)		{ static bool b=0; if (!b) { PX(mes, ##__VA_ARGS__); b= 1; } }

// ######################################## delimiter strings ######################################

#define	sepSPACE					" "
#define	sepSPACE_SEMICOLON_CR_LF	" ;\r\n"

// ################################### missing standard definitions ################################

#ifndef NULL
	#define NULL ((void *) 0)
#endif

#define pvNULL						((void *) NULL)
#define	pvFAILURE					((void *) 0xFFFFFFFF)
#define	pcFAILURE					((char *) 0xFFFFFFFF)

#ifndef UINT8_MIN
	#define	UINT8_MIN				0x00
#endif
#ifndef UINT16_MIN
	#define	UINT16_MIN				0x0000
#endif
#ifndef UINT32_MIN
	#define	UINT32_MIN				0x00000000
#endif
#ifndef UINT64_MIN
	#define	UINT64_MIN				0x0000000000000000
#endif

#ifndef s8_t
	typedef	__int8_t s8_t;
#endif
#ifndef i8_t
	typedef	__int8_t i8_t;
#endif
#ifndef u8_t
	typedef __uint8_t u8_t;
#endif
#ifndef i16_t
	typedef	__int16_t i16_t;
#endif
#ifndef u16_t
	typedef __uint16_t u16_t;
#endif
#ifndef i32_t
	typedef	__int32_t i32_t;
#endif
#ifndef u32_t
	typedef __uint32_t u32_t;
#endif
#ifndef i64_t
	typedef	__int64_t i64_t;
#endif
#ifndef u64_t
	typedef __uint64_t u64_t;
#endif
#ifndef f32_t
	typedef float f32_t;
#endif
#ifndef f64_t
	typedef double f64_t;
#endif

typedef	u8_t * pu8_t;
typedef	s8_t * ps8_t;

typedef	char * pc_t;
typedef	const char * pcc_t;
typedef	const unsigned char * pcuc_t;

// ######################### common & standard network related definitions #########################

#define	configSSID_MAX_LENGTH		32
#define	lenMAC_ADDRESS				6

// ################################ Strings in Macros manipulation #################################

#ifndef toSTR
	#define _toSTR(x)				#x
	#define toSTR(x)				_toSTR(x)
	#define toSTR_PV(x)				(void*) toSTR(x)
	#define toSTR_PC(x)				(char*) toSTR(x)
#endif

#define STR_FMTB(x)					"%" toSTR(x) "s"

#define CONCAT2_(a, b)				a##b
#define CONCAT2(a, b)				CONCAT2_(a, b)

#define CONCAT3_(a, b, c)			a##b##c
#define CONCAT3(a, b, c)			CONCAT3_(a, b, c)

#define CONCAT4_(a, b, c, d)		a##b##c##d
#define CONCAT4(a, b, c, d)			CONCAT4_(a, b, c, d)

// #################################################################################################

#define	BITS_IN_BYTE				8
#define	MAINS_FREQ_HZ				50
#define	MAINS_SUPPLY_V				230.0

/*
https://stackoverflow.com/questions/32082004/embedding-json-as-a-string-in-c-code-using-preprocessor
https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/preprocessor/macros/__VA_ARGS__/count-arguments
https://riptutorial.com/cplusplus/example/3527/macros

#define LSC( { "key" : "value", "num1" : 12345 } ) ;
*/

#define LSC(...) 					#__VA_ARGS__

#define sizeofMACRO(test)			(( !!(test) )*2-1)
#define DUMB_STATIC_ASSERT(test)	typedef char assertion_on_mystruct[sizeofMACRO(test)]

#define _HEADER_EXISTS(file)		defined(__has_include) && __has_include(file)
#define HEADER_EXISTS(file)			_HEADER_EXISTS(file)

#define _DEFINED_VALUE(sym,val)		defined (sym) && ( sym == val )
#define DEFINED_VALUE(sym,val)		_DEFINED_VALUE(sym,val)

// ########################################### Bit masks ##########################################

enum {								// 32bit bit masks
	B00MASK = 0x00000001,
	B01MASK = 0x00000002,
	B02MASK = 0x00000004,
	B03MASK = 0x00000008,
	B04MASK = 0x00000010,
	B05MASK = 0x00000020,
	B06MASK = 0x00000040,
	B07MASK = 0x00000080,
	B08MASK = 0x00000100,
	B09MASK = 0x00000200,
	B10MASK = 0x00000400,
	B11MASK = 0x00000800,
	B12MASK = 0x00001000,
	B13MASK = 0x00002000,
	B14MASK = 0x00004000,
	B15MASK = 0x00008000,
	B16MASK = 0x00010000,
	B17MASK = 0x00020000,
	B18MASK = 0x00040000,
	B19MASK = 0x00080000,
	B20MASK = 0x00100000,
	B21MASK = 0x00200000,
	B22MASK = 0x00400000,
	B23MASK = 0x00800000,
	B24MASK = 0x01000000,
	B25MASK = 0x02000000,
	B26MASK = 0x04000000,
	B27MASK = 0x08000000,
	B28MASK = 0x10000000,
	B29MASK = 0x20000000,
	B30MASK = 0x40000000,
	B31MASK = 0x80000000,
} ;

enum {								// 64bit bit masks
	BIT00MASK = (1ULL << 0),
	BIT01MASK = (1ULL << 1),
	BIT02MASK = (1ULL << 2),
	BIT03MASK = (1ULL << 3),
	BIT04MASK = (1ULL << 4),
	BIT05MASK = (1ULL << 5),
	BIT06MASK = (1ULL << 6),
	BIT07MASK = (1ULL << 7),
  	BIT08MASK = (1ULL << 8),
	BIT09MASK = (1ULL << 9),
	BIT10MASK = (1ULL << 10),
	BIT11MASK = (1ULL << 11),
	BIT12MASK = (1ULL << 12),
	BIT13MASK = (1ULL << 13),
	BIT14MASK = (1ULL << 14),
	BIT15MASK = (1ULL << 15),
	BIT16MASK = (1ULL << 16),
	BIT17MASK = (1ULL << 17),
  	BIT18MASK = (1ULL << 18),
	BIT19MASK = (1ULL << 19),
	BIT20MASK = (1ULL << 20),
	BIT21MASK = (1ULL << 21),
	BIT22MASK = (1ULL << 22),
	BIT23MASK = (1ULL << 23),
	BIT24MASK = (1ULL << 24),
	BIT25MASK = (1ULL << 25),
	BIT26MASK = (1ULL << 26),
	BIT27MASK = (1ULL << 27),
  	BIT28MASK = (1ULL << 28),
	BIT29MASK = (1ULL << 29),
	BIT30MASK = (1ULL << 30),
	BIT31MASK = (1ULL << 31),
	BIT32MASK = (1ULL << 32),
	BIT33MASK = (1ULL << 33),
	BIT34MASK = (1ULL << 34),
	BIT35MASK = (1ULL << 35),
	BIT36MASK = (1ULL << 36),
	BIT37MASK = (1ULL << 37),
  	BIT38MASK = (1ULL << 38),
	BIT39MASK = (1ULL << 39),
	BIT40MASK = (1ULL << 40),
	BIT41MASK = (1ULL << 41),
	BIT42MASK = (1ULL << 42),
	BIT43MASK = (1ULL << 43),
	BIT44MASK = (1ULL << 44),
	BIT45MASK = (1ULL << 45),
	BIT46MASK = (1ULL << 46),
	BIT47MASK = (1ULL << 47),
  	BIT48MASK = (1ULL << 48),
	BIT49MASK = (1ULL << 49),
	BIT50MASK = (1ULL << 50),
	BIT51MASK = (1ULL << 51),
	BIT52MASK = (1ULL << 52),
	BIT53MASK = (1ULL << 53),
	BIT54MASK = (1ULL << 54),
	BIT55MASK = (1ULL << 55),
	BIT56MASK = (1ULL << 56),
	BIT57MASK = (1ULL << 57),
  	BIT58MASK = (1ULL << 58),
	BIT59MASK = (1ULL << 59),
	BIT60MASK = (1ULL << 60),
	BIT61MASK = (1ULL << 61),
	BIT62MASK = (1ULL << 62),
	BIT63MASK = (1ULL << 63),
} ;

enum {								// ANSI standard terminal colors
	attrRESET = 0,
	attrBRIGHT = 1,
	attrDIM = 2,		attrNORM_INTEN = 22,	
	atrITALICS = 3,
	attrULINE_ON = 4,	attrULINE_OFF = 24,
	attrSLOW_BLINK = 5,
	attrRAPID_BLINK = 6,
	attrREV_ON = 7,		attrREV_OFF = 27,
	attrHIDDEN = 8,		attrREVEAL = 28,
	attrSTRIKE_ON = 9,  attrSTRIKE_OFF = 29,
// foreground colors
	colourFOREGND = 30,
	colourFG_BLACK = colourFOREGND,
	colourFG_RED,
	colourFG_GREEN,
	colourFG_YELLOW,
	colourFG_BLUE,
	colourFG_MAGENTA,
	colourFG_CYAN,
	colourFG_WHITE,										// 30 + 7 = 37
// background colors
	colourBACKGND = 40,
	colourBG_BLACK = colourBACKGND,
	colourBG_RED,
	colourBG_GREEN,
	colourBG_YELLOW,
	colourBG_BLUE,
	colourBG_MAGENTA,
	colourBG_CYAN,
	colourBG_WHITE,										// 40 + 7 = 47
};

// ######################################### Play area #############################################

#ifdef __cplusplus
}
#endif

#if 0

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

// Get the name of a type
#define typename(x) _Generic((x),                     							  \
                                                                                  \
        _Bool: "_Bool",                  unsigned char: "unsigned char",          \
         char: "char",                     signed char: "signed char",            \
    short int: "short int",         unsigned short int: "unsigned short int",     \
          int: "int",                     unsigned int: "unsigned int",           \
     long int: "long int",           unsigned long int: "unsigned long int",      \
long long int: "long long int", unsigned long long int: "unsigned long long int", \
        float: "float",                         double: "double",                 \
  long double: "long double",                   char *: "pointer to char",        \
       void *: "pointer to void",                int *: "pointer to int",         \
      default: "other")

#define fmt "%20s is '%s'" strNL

int main() {
  size_t s; ptrdiff_t p; intmax_t i; int ai[3] = {0}; return printf( fmt fmt fmt fmt fmt fmt fmt fmt,

     "size_t", typename(s),               "ptrdiff_t", typename(p),     
   "intmax_t", typename(i),      "character constant", typename('0'),
 "0x7FFFFFFF", typename(0x7FFFFFFF),     "0xFFFFFFFF", typename(0xFFFFFFFF),
"0x7FFFFFFFU", typename(0x7FFFFFFFU),  "array of int", typename(ai));
}

#define _CHECK_VAL(OPT)			\
	#ifndef val##OPT			\
		#define val##OPT "\0"	\
	#endif

#define _CHECK_MES(OPT)			\
	#ifndef mes##OPT			\
		#define mes##OPT "\0"	\
	#endif

#define	CREATE_UNDEF(OPT)	\
	_CHECK_VAL(OPT)			\
	_CHECK_VAL(OPT)

#define _CHECK_VAL(OPT)					\
	#ifndef CONCAT2(val,OPT)			\
		#define CONCAT2(val,OPT) "\0"	\
	#endif

#define _CHECK_MES(OPT)					\
	#ifndef CONCAT2(mes,OPT)			\
		#define CONCAT2(mes,OPT) "\0"	\
	#endif

#define	CREATE_UNDEF(OPT)	\
	_CHECK_VAL(OPT)			\
	_CHECK_VAL(OPT)

	General purpose static assert.

	Works in/out -side of scope:
		STATIC_ASSERT(sizeof(long)==8);
		int main()
		{
			STATIC_ASSERT(sizeof(int)==4);
		}

#define STATIC_ASSERT(X)			STATIC_ASSERT2(X,__LINE__)

/*
	These macros are required by STATIC_ASSERT to make token pasting work.
	Not really useful by themselves.
*/
#define STATIC_ASSERT2(X,L)		 STATIC_ASSERT3(X,L)
#define STATIC_ASSERT3(X,L)		 STATIC_ASSERT_MSG(X,at_line_##L)

/*
	Static assertion with special error message.
	Note: It depends on compiler whether message is visible or not!

	STATIC_ASSERT_MSG(sizeof(long)==8, long_is_not_eight_bytes);
*/
#define STATIC_ASSERT_MSG(COND,MSG) \
	typedef char static_assertion_##MSG[(!!(COND))*2-1]

/* ####################### to create tables of enums with matched text strings #####################
 *
 * Most works well for non-sparse enum lists, those without big unused gaps in sequential values
 *
 *	https://stackoverflow.com/questions/126277/making-something-both-a-c-identifier-and-a-string
 *	http://www.drdobbs.com/the-new-c-x-macros/184401387
 *	https://stackoverflow.com/questions/147267/easy-way-to-use-variables-of-enum-types-as-string-in-c
 */

#define ENUM_CASE(name,assign) case name: return #name;

// expansion macro for string to enum conversion
#define ENUM_STRCMP(name,assign) if (!strcmp(str,#name)) return name;

/// declare the access function and define enum values
#define DECLARE_ENUM(EnumType,ENUM_DEF) \
  enum EnumType { \
	ENUM_DEF(ENUM_VALUE) \
  }; \
  const char *GetString(EnumType dummy); \
  EnumType Get##EnumType##Value(const char *string); \

/// define the access function names
#define DEFINE_ENUM(EnumType,ENUM_DEF) \
  const char *GetString(EnumType value) \
  { \
	switch(value) \
	{ \
	  ENUM_DEF(ENUM_CASE) \
	  default: return ""; /* handle input error */ \
	} \
  } \
  EnumType Get##EnumType##Value(const char *str) \
  { \
	ENUM_DEF(ENUM_STRCMP) \
	return (EnumType)0; /* handle input error */ \
  } \




/* Then in the C module declare as follows

#include "enumFactory.h"
#define SOME_ENUM(XX) \
XX(FirstValue,) \
XX(SecondValue,) \
XX(SomeOtherValue,=50) \
XX(OneMoreValue,=100) \

 DECLARE_ENUM(SomeEnum,SOME_ENUM)

or in C++ as follows:
#include "someEnum.h"
DEFINE_ENUM(SomeEnum,SOME_ENUM)

************************ Another alternative mechanism:
#define MACROSTR(k) #k
typedef enum { kZero, kOne, kTwo, kThree } kConst;
static char *kConstStr[] = { MACROSTR(kZero), MACROSTR(kOne), MACROSTR(kTwo), MACROSTR(kThree) };
static void kConstPrinter(kConst k) { printf("%s", kConstStr[k]); }
*******************************************************************/


// http://www.pixelbeat.org/programming/gcc/static_assert.html
#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)

/* These can't be used after statements in c89. */
	#ifdef __COUNTER__
		#define STATIC_ASSERT(e,m) \
		;enum { ASSERT_CONCAT(static_assert_, __COUNTER__) = 1/(!!(e)) }
	#else
		/* This can't be used twice on the same line so ensure if using in headers
		 * that the headers are not included twice (by wrapping in #ifndef...#endif)
		 * Note it doesn't cause an issue when used on same line of separate modules
		 * compiled with gcc -combine -fwhole-program.  */
		#define STATIC_ASSERT(e,m) \
		;enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }
	#endif
#endif
