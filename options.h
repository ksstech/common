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

// Settings specifically for DEVelopment code builds
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

// Settings specifically for PRODuction code builds
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

// Settings common to both DEVelopment and PRODuction code builds
#define iosetDEFAULT	\
	.B1_6	= 1,		\
	.B1_32	= 1,		\
	.B1_33	= 1,		\
	.B1_58	= 1,		\
	.B1_62	= 1,		\
	.B3_3	= 1,		\
	.B3_6	= 1,		\
	.B3_17	= 4,		\
	.B4_0	= 5,		\
	.B4_2	= 8,		\
	.B4_8	= 8,		\
	.B4_15	= 2,		\
	.B8_0	= 50,		\
	.B8_1	= 50,

// ######################################### enumerations ##########################################

enum {								// enumerated option numbers used by ioBxSET
	ioTZlocal, ioB1_0 = ioTZlocal,
	ioI2Cinit,
	ioI2Cdly,						// Auto remove I2C task Delay if 0, compact operations
	ioFOTA,
	ioFlags,						// Log flag changes 0=DISable 1=ENable
	ioTimeout,
	ioUpDown,						// shutdown/restart tracking
	ioB1_7,
	ioB1_8,
	dbgSyntax,
	dbgHTTPreq,
	dbgJSONwr,
	dbgSense,
	dbgMode,
	dbgEPinfo,
	dbgREflow,
	ioB1_16,
	ioB1_17,
	ioMQcon,
	ioMQsub,
	ioMQpub,
	ioB1_21,
	ioB1_22,
	dbgAlert,
	ioMemory,
	ioB1_25,
	ioREtable,
	ioB1_27,
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
	ioB1_39,
	ioB1_40,
	ioB1_41,
	ioB1_42,
	ioM90write,
	ioM90offset,
	ioB1_45,
	ioB1_46,
	ioB1_47,
	dbgLIS2HH12,
	ioB1_49,
	ioB1_50,
	dbgMODBUS,
	dbgDS1820,
	dbgDS1990x,
	dbgOWscan,
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
	dbgActuate,						// 0=Off, 1=RuleEngine, 2=TaskAct, 3=Both
	dbgDBase,						// 0=Off, 1=Match, 2=Errors, 3=Both
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
	ioWLauth,						// OPEN->WEP->WPA_PSK->WPA2_PSK->WPA_WPA2_PSK->WPA2_ENTERPRISE etc...
	ioSLhost,						// Highest # (lowest PRI) sent to host (must <= ioSLOGhi)
	ioSLOGhi,						// Highest # (lowest PRI) evaluated
	// 4-bit options
	dlyDS1990,ioB4_0=dlyDS1990,	// delay (sec) between successive read same tag
	ioCLIbuf, ioB4_1=ioCLIbuf, 		// 0=disable, 1+ = (n+1)*128 bytes in size
	ioMBews,
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
	toConsole, ioB4_15=toConsole,
	dlyMBread, ioB8_0=dlyMBread,
	dlyMBpoll,
	ioB8_2,
	ioB8_3,
	ioB8_4,
	ioB8_5,
	ioB8_6,
	ioB8_7,
	ioBXlast, ioS_NWMO=ioBXlast,	// 141 {0->3} 			(network mode)
	ioS_WIFI,						// 142 {ssid} {pswd}
	ioS_MQTT,						// 143 {w.x.y.z[:???]}	(mqtt broker/proxy)
	ioS_PEEK,						// 144 {addr} {size}
	ioS_IOdef,						// 145 					(reset to defaults)
	ioS_LAST = ioS_IOdef,
} ;

// ########################################## structures ###########################################

#pragma GCC diagnostic ignored "-Wpacked-bitfield-compat"

typedef struct __attribute__((packed)) {	// 1/2/3/4/8 bit option variables
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

extern ioset_t const ioDefaults;

// ####################################### public functions ########################################

void xOptionsSetDefaults(void);
int xOptionsSetDirect(int ON, int OV);
int	xOptionsSet(int	ON, int OV, int PF);
void vOptionsShow(void);

#ifdef __cplusplus
}
#endif
