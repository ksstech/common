/*

 * options.h
 */

#pragma once

#include "definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

// ############################################ macros #############################################

#define	ioB1GET(i)					maskGET1B(sNVSvars.ioBX.ioB1, (i-ioB1_0), u64_t)
#define	ioB2GET(i)					maskGET2B(sNVSvars.ioBX.ioB2, (i-ioB2_0), u64_t)
#define	ioB3GET(i)					maskGET3B(sNVSvars.ioBX.ioB3, (i-ioB3_0), u64_t)
#define	ioB4GET(i)					maskGET4B(sNVSvars.ioBX.ioB4, (i-ioB4_0), u64_t)
#define	ioB8GET(i)					maskGET8B(sNVSvars.ioBX.ioB8, (i-ioB8_0), u64_t)

#define	ioB1SET(i,x)				maskSET1B(sNVSvars.ioBX.ioB1, (i-ioB1_0), x, u64_t)
#define	ioB2SET(i,x)				maskSET2B(sNVSvars.ioBX.ioB2, (i-ioB2_0), x, u64_t)
#define	ioB3SET(i,x)				maskSET3B(sNVSvars.ioBX.ioB3, (i-ioB3_0), x, u64_t)
#define	ioB4SET(i,x)				maskSET4B(sNVSvars.ioBX.ioB4, (i-ioB4_0), x, u64_t)
#define	ioB8SET(i,x)				maskSET8B(sNVSvars.ioBX.ioB8, (i-ioB8_0), x, u64_t)

// ######################################### enumerations ##########################################


// ########################################## structures ###########################################

#pragma GCC diagnostic ignored "-Wpacked-bitfield-compat"

typedef struct __attribute__((packed)) {	// 1/2/3/4/8 bit option variables
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
			u8_t B8_0:8;
			u8_t B8_1:8;
			u8_t B8_2:8;
			u8_t B8_3:8;
			u8_t B8_4:8;
			u8_t B8_5:8;
			u8_t B8_6:8;
			u8_t B8_7:8;
		};
		u64_t ioB8;
	};
} ioset_t;
DUMB_STATIC_ASSERT(sizeof(ioset_t) == 40);

// ####################################### public variables ########################################


// ####################################### public functions ########################################

void xOptionsSetDefaults(void);
int xOptionsSetDirect(int ON, int OV);
int	xOptionsSet(int	ON, int OV, int PF);
void vOptionsShow(void);

#ifdef __cplusplus
}
#endif
