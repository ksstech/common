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
	ioB1_0, ioSTDIO = ioB1_0,		// DS248x status & config changes tracking
	ioI2Cinit,
	ioI2Cdly,						// I2C task auto remove Delay if 0.
	ioFOTA,
	ioFlags,						// Log flag changes 0=DISable 1=ENable
	ioTimeout,
	ioStart,
	ioRstrt,						// shutdown/restart tracking
	ioParse,
	ioSyntax,
	ioSense,
	ioMode,
	ioEndPoint,
	ioIdent,
	ioDBmatch,
	ioDBerr,
	ioMQcon,
	ioMQsub,
	ioMQpub,
	ioOWscan,
	ioActuate,						// RulesEngine * TaskActuator
	ioAlert,
	ioMemory,
	ioTNET,
	ioHTTP,
	ioSENSOR,						// vTaskSensor()
	ioREtable,
	ioREsched,
	ioREident,
	// hardware devices
	ioDS18x20 = 32,
	ioDS1990x,
	ioDS248Xstat,
	ioM90write,
	ioM90offset,
	// Add more ...
	ioWLmode=59,
	ioWLevt,						// halWL events track 0=DISABLE 1=ENABLE
	ioWLram,						// halWL wifi storage 0=FLASH 1=RAM
	ioWLscan,
	ioB1_63=63, ioWLsort=ioB1_63,
	// ##################### START of 2-bit flags
	ioB2_0=64,
	ioDS248Xdbg = ioB2_0,
	// Add more ...
	ioB2_31=95,
	// ##################### START of 3-bit flags
	ioB3_0=96, ioU0Speed=ioB3_0,	// UARTx speed
	ioU1Speed,
	ioU2Speed,
	ioU0RXbuf,						// UARTx RX buffers
	ioU1RXbuf,
	ioU2RXbuf,
	ioU0TXbuf,						// UARTx TX buffers
	ioU1TXbuf,
	ioU2TXbuf,
	// Add more ...
	ioWLauth=115,
	ioB3_20=116, ioSLOGhi=ioB3_20,	// SLOG maximum level (0 -> 7)
	// ##################### START 4-bit flags
	ioB4_0=117,
	ioDS1990RdDly = ioB4_0,			// delay (sec) between successive read same tag
	// Add more ...
	ioB4_15=132,
	// ##################### START special flags
	ioS_NWMO,						// 133 {0->3} 			(network mode)
	ioS_WIFI,						// 134 {ssid} {pswd}
	ioS_MQTT,						// 135 {w.x.y.z[:???]}	(mqtt broker/proxy)
	ioS_PEEK,						// 136 {addr} {size}
	ioS_IOdef,						// 137 					(reset to defaults)
	ioS_LAST = ioS_IOdef,
} ;

// ########################################## structures ###########################################

typedef struct ioset_t {			// 1/2/3/4 bit option variables
	union {							// 4-bit option variables
		struct __attribute__((packed)) {
			int B4_0	: 4 ;
			int B4_1	: 4 ;
			int B4_2	: 4 ;
			int B4_3	: 4 ;
			int B4_4	: 4 ;
			int B4_5	: 4 ;
			int B4_6	: 4 ;
			int B4_7	: 4 ;
			int B4_8	: 4 ;
			int B4_9	: 4 ;
			int B4_10	: 4 ;
			int B4_11	: 4 ;
			int B4_12	: 4 ;
			int B4_13	: 4 ;
			int B4_14	: 4 ;
			int B4_15	: 4 ;
		};
		uint64_t ioB4 ;
	};
	union {							// 3-bit option variables
		struct __attribute__((packed)) {
			int B3_0	: 3 ;
			int B3_1	: 3 ;
			int B3_2	: 3 ;
			int B3_3	: 3 ;
			int B3_4	: 3 ;
			int B3_5	: 3 ;
			int B3_6	: 3 ;
			int B3_7	: 3 ;
			int B3_8	: 3 ;
			int B3_9	: 3 ;
			int B3_10	: 3 ;
			int B3_11	: 3 ;
			int B3_12	: 3 ;
			int B3_13	: 3 ;
			int B3_14	: 3 ;
			int B3_15	: 3 ;
			int B3_16	: 3 ;
			int B3_17	: 3 ;
			int B3_18	: 3 ;
			int B3_19	: 3 ;
			int B3_20	: 3 ;
			int B3_xx	: 1 ;
		};
		uint64_t ioB3 ;
	};
	union {							// 2-bit option variables
		struct __attribute__((packed)) {
			int B2_0	: 2 ;
			int B2_1	: 2 ;
			int B2_2	: 2 ;
			int B2_3	: 2 ;
			int B2_4	: 2 ;
			int B2_5	: 2 ;
			int B2_6	: 2 ;
			int B2_7	: 2 ;
			int B2_8	: 2 ;
			int B2_9	: 2 ;
			int B2_10	: 2 ;
			int B2_11	: 2 ;
			int B2_12	: 2 ;
			int B2_13	: 2 ;
			int B2_14	: 2 ;
			int B2_15	: 2 ;
			int B2_16	: 2 ;
			int B2_17	: 2 ;
			int B2_18	: 2 ;
			int B2_19	: 2 ;
			int B2_20	: 2 ;
			int B2_21	: 2 ;
			int B2_22	: 2 ;
			int B2_23	: 2 ;
			int B2_24	: 2 ;
			int B2_25	: 2 ;
			int B2_26	: 2 ;
			int B2_27	: 2 ;
			int B2_28	: 2 ;
			int B2_29	: 2 ;
			int B2_30	: 2 ;
			int B2_31	: 2 ;
		};
		uint64_t ioB2 ;
	};
	union {							// 1-bit option variables
		struct __attribute__((packed)) {
			int B1_0	: 1 ;
			int B1_1	: 1 ;
			int B1_2	: 1 ;
			int B1_3	: 1 ;
			int B1_4	: 1 ;
			int B1_5	: 1 ;
			int B1_6	: 1 ;
			int B1_7	: 1 ;
			int B1_8	: 1 ;
			int B1_9	: 1 ;
			int B1_10	: 1 ;
			int B1_11	: 1 ;
			int B1_12	: 1 ;
			int B1_13	: 1 ;
			int B1_14	: 1 ;
			int B1_15	: 1 ;
			int B1_16	: 1 ;
			int B1_17	: 1 ;
			int B1_18	: 1 ;
			int B1_19	: 1 ;
			int B1_20	: 1 ;
			int B1_21	: 1 ;
			int B1_22	: 1 ;
			int B1_23	: 1 ;
			int B1_24	: 1 ;
			int B1_25	: 1 ;
			int B1_26	: 1 ;
			int B1_27	: 1 ;
			int B1_28	: 1 ;
			int B1_29	: 1 ;
			int B1_30	: 1 ;
			int B1_31	: 1 ;
			int B1_32	: 1 ;
			int B1_33	: 1 ;
			int B1_34	: 1 ;
			int B1_35	: 1 ;
			int B1_36	: 1 ;
			int B1_37	: 1 ;
			int B1_38	: 1 ;
			int B1_39	: 1 ;
			int B1_40	: 1 ;
			int B1_41	: 1 ;
			int B1_42	: 1 ;
			int B1_43	: 1 ;
			int B1_44	: 1 ;
			int B1_45	: 1 ;
			int B1_46	: 1 ;
			int B1_47	: 1 ;
			int B1_48	: 1 ;
			int B1_49	: 1 ;
			int B1_50	: 1 ;
			int B1_51	: 1 ;
			int B1_52	: 1 ;
			int B1_53	: 1 ;
			int B1_54	: 1 ;
			int B1_55	: 1 ;
			int B1_56	: 1 ;
			int B1_57	: 1 ;
			int B1_58	: 1 ;
			int B1_59	: 1 ;
			int B1_60	: 1 ;
			int B1_61	: 1 ;
			int B1_62	: 1 ;
			int B1_63	: 1 ;
		};
		uint64_t ioB1 ;
	};
} ioset_t;

// ####################################### public variables ########################################


// ####################################### public functions ########################################

void xOptionsSetDefaults(void);
int xOptionsSetDirect(int ON, int OV);
int	xOptionsSet(int	ON, int OV, int PF);
void vOptionsShow(void);

#ifdef __cplusplus
}
#endif
