/*
 * options.h
 */

#pragma once

#include <stdint.h>
#include "definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

// ############################################ macros #############################################

#define	ioB1GET(i)					maskGET1B(sNVSvars.ioBX.ioB1, (i-ioB1_0), u64_t)
#define	ioB2GET(i)					maskGET2B(sNVSvars.ioBX.ioB2, (i-ioB2_0), u64_t)
#define	ioB3GET(i)					maskGET3B(sNVSvars.ioBX.ioB3, (i-ioB3_0), u64_t)
#define	ioB4GET(i)					maskGET4B(sNVSvars.ioBX.ioB4, (i-ioB4_0), u64_t)

#define	ioB1SET(i,x)				maskSET1B(sNVSvars.ioBX.ioB1, (i-ioB1_0), x, u64_t)
#define	ioB2SET(i,x)				maskSET2B(sNVSvars.ioBX.ioB2, (i-ioB2_0), x, u64_t)
#define	ioB3SET(i,x)				maskSET3B(sNVSvars.ioBX.ioB3, (i-ioB3_0), x, u64_t)
#define	ioB4SET(i,x)				maskSET4B(sNVSvars.ioBX.ioB4, (i-ioB4_0), x, u64_t)

#define iosetDEFAULT_DEV					\
	.B1_4	= 1,							\
	.B1_34	= 0,							\
	.B2_0	= hostDEV,						\
	.B2_1	= hostDEV,						\
	.B2_2	= hostDEV,						\
	.B2_3	= hostDEV,						\
	.B3_19	= CONFIG_LOG_DEFAULT_LEVEL + 1,	\
	.B3_20	= CONFIG_LOG_DEFAULT_LEVEL + 2,	\
	.B4_1	= 7,

#define iosetDEFAULT_PROD					\
	.B1_4	= 0,							\
	.B1_34	= 1,							\
	.B2_0	= hostPROD,						\
	.B2_1	= hostPROD,						\
	.B2_2	= hostPROD,						\
	.B2_3	= hostPROD,						\
	.B3_19	= CONFIG_LOG_DEFAULT_LEVEL + 0,	\
	.B3_20	= CONFIG_LOG_DEFAULT_LEVEL + 1,	\
	.B4_1	= 1,

#define iosetDEFAULT	\
	.B1_32	= 1,		\
	.B1_33	= 1,		\
	.B1_58	= 1,		\
	.B1_62	= 1,		\
	.B3_3	= 1,		\
	.B3_6	= 1,		\
	.B3_17	= 4,		\
	.B4_0	= 5,

// ######################################### enumerations ##########################################

enum {								// enumerated option numbers used by ioBxSET
	ioTZlocal, ioB1_0 = ioTZlocal,
	ioI2Cinit,
	ioI2Cdly,						// I2C task auto remove Delay if 0.
	ioFOTA,
	ioFlags,						// Log flag changes 0=DISable 1=ENable
	ioTimeout,
	ioUpDown,						// shutdown/restart tracking
	ioToken,
	ioParse,
	ioSyntax,
	ioJSONpar,
	ioW_JSON,
	ioSense,
	ioMode,
	ioEndPoint,
	ioB1_15,
	ioDBmatch,
	ioDBerr,
	ioMQcon,
	ioMQsub,
	ioMQpub,
	ioOWscan,
	ioB1_22,
	ioAlert,
	ioMemory,
	ioSENSOR,						// vTaskSensor()
	ioREtable,
	ioREsched,
	ioB1_28,
	ioLFS,
	ioTNETtrack,
	ioHTTPtrack,
	ioTNETstart,					// 1 = Start
	ioHTTPstart,					// 1 = Start
	ioTNETauth,						// 1 = authenticate
	ioB1_35,
	ioB1_36,
	ioB1_37,
	ioB1_38,
	ioMBstart,						// 1 = start ModBus
	ioDS18x20,						// hardware devices
	ioDS1990x,
	ioDS248Xcheck,
	ioM90write,
	ioM90offset,
	ioLIS2HH12,
	ioB1_46,
	ioB1_47,
	ioB1_48,
	ioB1_49,
	ioB1_50,
	ioB1_51,
	ioB1_52,
	ioB1_53,
	ioB1_54,
	dbgTRACKER,						// dbgRTOSsema
	ioB1_56,
	ioWLantenna,					// external antenna 1=enable
	ioWLhidden,
	ioWLmode,
	ioWLevt,						// events track 0=DISABLE 1=ENABLE
	ioWLram,						// storage 0=FLASH 1=RAM
	ioWLscan,						// 0=Fast 1=All
	ioWLsort,						// 0=SIGnal, 1=SECurity
	// 2-bit options
	ioHostMQTT, ioB2_0=ioHostMQTT,
	ioHostFOTA,
	ioHostSLOG,
	ioHostCONF,
	ioMQTT_QoS,
	ioB2_5,
	ioB2_6,
	ioB2_7,
	ioAPindex,
	ioB2_9,
	ioB2_10,
	ioB2_11,
	ioB2_12,
	ioB2_13,
	ioB2_14,
	ioB2_15,
	ioB2_16,
	ioB2_17,
	ioB2_18,
	ioB2_19,
	ioB2_20,
	ioB2_21,
	ioB2_22,
	ioB2_23,
	dbgIdent,
	dbgActuate,						// RulesEngine * TaskActuator
	ioB2_26,
	ioB2_27,
	ioB2_28,
	ioB2_29,
	ioB2_30,
	dbgDS248X,
	//3-bit options
	ioU0Speed, ioB3_0=ioU0Speed,	// UARTx speed
	ioU1Speed,
	ioU2Speed,
	ioU0RXbuf,						// UARTx RX buffers
	ioU1RXbuf,
	ioU2RXbuf,
	ioU0TXbuf,						// UARTx TX buffers
	ioU1TXbuf,
	ioU2TXbuf,
	B3_9,
	B3_10,
	B3_11,
	B3_12,
	B3_13,
	B3_14,
	B3_15,
	B3_16,
	ioWLretry,
	ioWLauth,
	ioSLhost,						// Highest PRI sent to host (must <= ioSLOGhi)
	ioSLOGhi,						// Highest PRI evaluated
	// 4-bit options
	ioDS1990Dly,ioB4_0=ioDS1990Dly,	// delay (sec) between successive read same tag
	ioCLIbuf, ioB4_1=ioCLIbuf, 		// 0=disable, 1+ = (n+1)*128 bytes in size
	ioB4_2,
	ioB4_3,
	ioB4_4,
	ioB4_5,
	ioB4_6,
	ioB4_7,
	ioGUIintval,
	ioB4_9,
	ioB4_10,
	ioB4_11,
	ioB4_12,
	ioB4_13,
	ioB4_14,
	ioB4_15,
	ioS_NWMO,						// 133 {0->3} 			(network mode)
	ioS_WIFI,						// 134 {ssid} {pswd}
	ioS_MQTT,						// 135 {w.x.y.z[:???]}	(mqtt broker/proxy)
	ioS_PEEK,						// 136 {addr} {size}
	ioS_IOdef,						// 137 					(reset to defaults)
	ioS_LAST = ioS_IOdef,
} ;

// ########################################## structures ###########################################

#pragma GCC diagnostic ignored "-Wpacked-bitfield-compat"

typedef struct __attribute__((packed)) {	// 1/2/3/4 bit option variables
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
		u64_t ioB4 ;
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
		u64_t ioB3 ;
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
		u64_t ioB2 ;
	};
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
} ioset_t;
DUMB_STATIC_ASSERT(sizeof(ioset_t) == 32);

// ####################################### public variables ########################################

extern ioset_t const ioDefaults;

// ####################################### public functions ########################################

void xOptionsSetDefaults(void);
int xOptionsSetDirect(int ON, int OV);
int	xOptionsSet(int	ON, int OV, int PF);
void vOptionsShow(void);

#ifdef __cplusplus
}
#endif
