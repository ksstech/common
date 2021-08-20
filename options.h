/*
 * options.h
 */

#pragma once

#include	<stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ############################################ macros #############################################

#define	ioB1GET(i)					maskGET1B(sNVSvars.ioBX.ioB1, (i-ioB1_0), uint64_t)
#define	ioB2GET(i)					maskGET2B(sNVSvars.ioBX.ioB2, (i-ioB2_0), uint64_t)
#define	ioB3GET(i)					maskGET3B(sNVSvars.ioBX.ioB3, (i-ioB3_0), uint64_t)
#define	ioB4GET(i)					maskGET4B(sNVSvars.ioBX.ioB4, (i-ioB4_0), uint64_t)

#define	ioB1SET(i,x)				maskSET1B(sNVSvars.ioBX.ioB1, (i-ioB1_0), x, uint64_t)
#define	ioB2SET(i,x)				maskSET2B(sNVSvars.ioBX.ioB2, (i-ioB2_0), x, uint64_t)
#define	ioB3SET(i,x)				maskSET3B(sNVSvars.ioBX.ioB3, (i-ioB3_0), x, uint64_t)
#define	ioB4SET(i,x)				maskSET4B(sNVSvars.ioBX.ioB4, (i-ioB4_0), x, uint64_t)

// ######################################### enumerations ##########################################

enum {								// enumerated option numbers used by ioBxSET
	ioB1_0 = 0,						// DS248x status & config changes tracking
	ioB1_1,							// I2C task auto remove Delay if 0.
	ioB1_2,							// halWL events track 0=DISABLE 1=ENABLE
	ioB1_3,							// halWL wifi storage 0=FLASH 1=RAM
	ioFlags,						// log flag changes 0=DISable 1=ENable
	// ...
	ioB1_63 = 63,

	ioB2_0	= 64,
	// ...
	ioB2_31 = 95,

	// 0=9600, 57600, 115200, 230400, 460800, 912600, 1024000, 7=2048000
	ioB3_0 = 96,
	ioU0Speed = ioB3_0,					// UARTx speed
	ioU1Speed,
	ioU2Speed,
	ioU0RXbuf,							// UARTx RX buffers
	ioU1RXbuf,
	ioU2RXbuf,
	ioU0TXbuf,							// UARTx TX buffers
	ioU1TXbuf,
	ioU2TXbuf,
	// ...
	ioB3_20 = 116, ioSLOGhi = ioB3_20,	// SLOG maximum level (0 -> 7)
	// 4-bit options
	ioB4_0=117,
	// ...
	ioB4_15=132,
	// various larger value options
	ioS_NWMO,							// set network mode 0->3
	ioS_WIFI,
	ioS_MQTT,
	ioS_PEEK,
} ;

// ########################################## structures ###########################################

typedef struct ioset_t {			// 1/2/3/4 bit option variables
	union {							// 4-bit option variables
		struct __attribute__((packed)) {
			uint32_t B4_0	: 4 ;
			uint32_t B4_1	: 4 ;
			uint32_t B4_2	: 4 ;
			uint32_t B4_3	: 4 ;
			uint32_t B4_4	: 4 ;
			uint32_t B4_5	: 4 ;
			uint32_t B4_6	: 4 ;
			uint32_t B4_7	: 4 ;
			uint32_t B4_8	: 4 ;
			uint32_t B4_9	: 4 ;
			uint32_t B4_10	: 4 ;
			uint32_t B4_11	: 4 ;
			uint32_t B4_12	: 4 ;
			uint32_t B4_13	: 4 ;
			uint32_t B4_14	: 4 ;
			uint32_t B4_15	: 4 ;
		};
		uint64_t ioB4 ;
	};
	union {							// 3-bit option variables
		struct __attribute__((packed)) {
			uint32_t B3_0	: 3 ;
			uint32_t B3_1	: 3 ;
			uint32_t B3_2	: 3 ;
			uint32_t B3_3	: 3 ;
			uint32_t B3_4	: 3 ;
			uint32_t B3_5	: 3 ;
			uint32_t B3_6	: 3 ;
			uint32_t B3_7	: 3 ;
			uint32_t B3_8	: 3 ;
			uint32_t B3_9	: 3 ;
			uint32_t B3_10	: 3 ;
			uint32_t B3_11	: 3 ;
			uint32_t B3_12	: 3 ;
			uint32_t B3_13	: 3 ;
			uint32_t B3_14	: 3 ;
			uint32_t B3_15	: 3 ;
			uint32_t B3_16	: 3 ;
			uint32_t B3_17	: 3 ;
			uint32_t B3_18	: 3 ;
			uint32_t B3_19	: 3 ;
			uint32_t B3_20	: 3 ;
			uint32_t B3_xx	: 1 ;
		};
		uint64_t ioB3 ;
	};
	union {							// 2-bit option variables
		struct __attribute__((packed)) {
			uint32_t B2_0	: 2 ;
			uint32_t B2_1	: 2 ;
			uint32_t B2_2	: 2 ;
			uint32_t B2_3	: 2 ;
			uint32_t B2_4	: 2 ;
			uint32_t B2_5	: 2 ;
			uint32_t B2_6	: 2 ;
			uint32_t B2_7	: 2 ;
			uint32_t B2_8	: 2 ;
			uint32_t B2_9	: 2 ;
			uint32_t B2_10	: 2 ;
			uint32_t B2_11	: 2 ;
			uint32_t B2_12	: 2 ;
			uint32_t B2_13	: 2 ;
			uint32_t B2_14	: 2 ;
			uint32_t B2_15	: 2 ;
			uint32_t B2_16	: 2 ;
			uint32_t B2_17	: 2 ;
			uint32_t B2_18	: 2 ;
			uint32_t B2_19	: 2 ;
			uint32_t B2_20	: 2 ;
			uint32_t B2_21	: 2 ;
			uint32_t B2_22	: 2 ;
			uint32_t B2_23	: 2 ;
			uint32_t B2_24	: 2 ;
			uint32_t B2_25	: 2 ;
			uint32_t B2_26	: 2 ;
			uint32_t B2_27	: 2 ;
			uint32_t B2_28	: 2 ;
			uint32_t B2_29	: 2 ;
			uint32_t B2_30	: 2 ;
			uint32_t B2_31	: 2 ;
		};
		uint64_t ioB2 ;
	};
	union {							// 1-bit option variables
		struct __attribute__((packed)) {
			uint32_t B1_0	: 1 ;
			uint32_t B1_1	: 1 ;
			uint32_t B1_2	: 1 ;
			uint32_t B1_3	: 1 ;
			uint32_t B1_4	: 1 ;
			uint32_t B1_5	: 1 ;
			uint32_t B1_6	: 1 ;
			uint32_t B1_7	: 1 ;
			uint32_t B1_8	: 1 ;
			uint32_t B1_9	: 1 ;
			uint32_t B1_10	: 1 ;
			uint32_t B1_11	: 1 ;
			uint32_t B1_12	: 1 ;
			uint32_t B1_13	: 1 ;
			uint32_t B1_14	: 1 ;
			uint32_t B1_15	: 1 ;
			uint32_t B1_16	: 1 ;
			uint32_t B1_17	: 1 ;
			uint32_t B1_18	: 1 ;
			uint32_t B1_19	: 1 ;
			uint32_t B1_20	: 1 ;
			uint32_t B1_21	: 1 ;
			uint32_t B1_22	: 1 ;
			uint32_t B1_23	: 1 ;
			uint32_t B1_24	: 1 ;
			uint32_t B1_25	: 1 ;
			uint32_t B1_26	: 1 ;
			uint32_t B1_27	: 1 ;
			uint32_t B1_28	: 1 ;
			uint32_t B1_29	: 1 ;
			uint32_t B1_30	: 1 ;
			uint32_t B1_31	: 1 ;
			uint32_t B1_32	: 1 ;
			uint32_t B1_33	: 1 ;
			uint32_t B1_34	: 1 ;
			uint32_t B1_35	: 1 ;
			uint32_t B1_36	: 1 ;
			uint32_t B1_37	: 1 ;
			uint32_t B1_38	: 1 ;
			uint32_t B1_39	: 1 ;
			uint32_t B1_40	: 1 ;
			uint32_t B1_41	: 1 ;
			uint32_t B1_42	: 1 ;
			uint32_t B1_43	: 1 ;
			uint32_t B1_44	: 1 ;
			uint32_t B1_45	: 1 ;
			uint32_t B1_46	: 1 ;
			uint32_t B1_47	: 1 ;
			uint32_t B1_48	: 1 ;
			uint32_t B1_49	: 1 ;
			uint32_t B1_50	: 1 ;
			uint32_t B1_51	: 1 ;
			uint32_t B1_52	: 1 ;
			uint32_t B1_53	: 1 ;
			uint32_t B1_54	: 1 ;
			uint32_t B1_55	: 1 ;
			uint32_t B1_56	: 1 ;
			uint32_t B1_57	: 1 ;
			uint32_t B1_58	: 1 ;
			uint32_t B1_59	: 1 ;
			uint32_t B1_60	: 1 ;
			uint32_t B1_61	: 1 ;
			uint32_t B1_62	: 1 ;
			uint32_t B1_63	: 1 ;
		};
		uint64_t ioB1 ;
	};
} ioset_t;


// ####################################### public variables ########################################


// ####################################### public functions ########################################

int xOptionsSetDirect(int EI, int EV, int Flag);
int	xOptionsSet(uint8_t	EI, int EV, int Flag);
void vOptionsShow(void);

#ifdef __cplusplus
}
#endif
