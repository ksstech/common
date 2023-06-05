/*
 * options_appl.h
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// ######################################### build macros ##########################################

#if (configPRODUCTION == 0)			// Settings specifically for DEVelopment code builds
	#define iosetDEFAULTS	\
	.B1_1	= 0,			\
	.B1_4	= 1,			\
	.B1_6	= 0,			\
	.B1_31	= 0,			\
	.B1_32	= 1,			\
	.B1_33	= 1,			\
	.B1_34	= 0,			\
	.B1_58	= 1,			\
	.B1_62	= 1,			\
	.B2_0	= hostDEV,		\
	.B2_1	= hostDEV,		\
	.B2_2	= hostDEV,		\
	.B2_3	= hostDEV,		\
	.B3_3	= 1,			\
	.B3_6	= 1,			\
	.B3_17	= 4,			\
	.B3_19	= CONFIG_LOG_DEFAULT_LEVEL + 1,	\
	.B3_20	= CONFIG_LOG_DEFAULT_LEVEL + 2,	\
	.B4_0	= 5,			\
	.B4_1	= 7,			\
	.B4_2	= 8,			\
	.B4_8	= 8,			\
	.B4_15	= 2,			\
	.B8_0	= 50,			\
	.B8_1	= 50
#else								// PRODuction code builds
	#define iosetDEFAULTS	\
	.B1_4	= 0,			\
	.B1_31	= 0,			\
	.B1_32	= 1,			\
	.B1_33	= 1,			\
	.B1_34	= 1,			\
	.B1_58	= 1,			\
	.B1_62	= 1,			\
	.B2_0	= hostPROD,		\
	.B2_1	= hostPROD,		\
	.B2_2	= hostPROD,		\
	.B2_3	= hostPROD,		\
	.B3_3	= 1,			\
	.B3_6	= 1,			\
	.B3_17	= 4,			\
	.B3_19	= CONFIG_LOG_DEFAULT_LEVEL + 0,	\
	.B3_20	= CONFIG_LOG_DEFAULT_LEVEL + 1,	\
	.B4_0	= 5,			\
	.B4_1	= 1,			\
	.B4_2	= 8,			\
	.B4_8	= 8,			\
	.B4_15	= 2,			\
	.B8_0	= 50,			\
	.B8_1	= 50
#endif

#if (halHAS_ADE7953 > 0)
	#define mesB2_9 "ade7953NVS\0"
#elif (halHAS_M90E26 > 0)
	#define mesB2_9 "m90e26NVS\0"
#else
	#define mesB2_9 "\0"
#endif

#define iosetHELP_MESSAGE \
/*1*/	"TZlocal\0"		"I2Cinit\0"		"I2Cdly\0"		"FOTA\0"		"Flags\0"		"Timeout\0"		"UpDown\0"		"\0"			\
		"\0"			"dbgSyntax\0"	"dbgHTTPreq\0"	"dbgJSONwr\0"	"dbgSense\0"	"dbgMode\0"		"dbgEPinfo\0"	"dbgREflow\0"	\
		"\0"			"\0"			"MQTTcon\0"		"MQTTsub\0"		"MQTTpub\0"		"\0"			"\0"			"Alerts\0"		\
		"Memory\0"		"\0"			"REtable\0"		"\0"			"\0"			"LittleFS\0"	"TNETtrack\0"	"HTTPtrack\0"	\
		"TNETstart\0"	"HTTPstart\0"	"TNETauth\0"	"\0"			"\0"			"\0"			"\0"			"\0"			\
		"dbgGDIO\0"		"dbgGAIO\0"		"\0"			"M90write\0"	"M90offset\0"	"\0"			"\0"			"\0"			\
		"LIS2HH12\0"	"\0"			"\0"			"dbgMODBUS\0"	"dbgDS1820\0"	"dbgDS1990\0"	"dbgOWscan\0"	"dbgTracker\0"	\
		"\0"			"WL ExtAnt\0"	"WLhidden\0"	"WLmode\0"		"WLevents\0"	"WLram\0"		"WLscan\0"		"WLsort\0"		\
/*2*/	"HostMQTT\0"	"HostFOTA\0"	"HostSLOG\0"	"HostCONF\0"	"QoS 0~2\0"		"Port 0~3\0"	"\0"			"\0"			\
		"APindex\0"		mesB2_9			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			\
		"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			\
		"dbgIdent\0"	"dbgActuate\0"	"dbgGUI\0"		"dbgGPI\0"		"\0"			"\0"			"\0"			"dbgDS248X\0"	\
/*3*/	"U0speed\0"		"U1speed\0"		"U2speed\0"		"U0RXbuf\0"		"U1RXbuf\0"		"U2RXbuf\0"		"U0TXbuf\0"		"U1TXbuf\0"		\
		"U2TXbuf\0"		"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			\
		"\0"			"WLretry\0"		"WL Auth\0"		"SlHostMax\0"	"SlShowMax\0"													\
/*4*/	"dlyDS1990\0"	"CLIbuf\0"		"ioMBews\0"		"dbgDBase\0"	"\0"			"\0"			"\0"			"\0"			\
		"GUIintval\0"	"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"toConsole\0"	\
/*8*/	"dlyMBread\0"	"dlyMBpoll\0"	"\0"			"\0"			"\0"			"\0"			"\0"			"\0"

// ######################################### enumerations ##########################################

enum {								// enumerated option numbers used by ioBxSET
	ioB1_0 = 0,						// 1-bit options (64x 0->63)
	ioTZlocal = ioB1_0,
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
	dbgGDIO,
	dbgGAIO,
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
	ioWLhidden,						// scan show hidden APs
	ioWLmode,						// track AP <> STA activities
	ioWLevt,						// events track 0=DISABLE 1=ENABLE
	ioWLram,						// storage 0=FLASH 1=RAM
	ioWLscan,						// 0=Fast 1=All
	ioWLsort,						// 0=SIGnal, 1=SECurity

	ioB2_0 = 64,					// 2-bit options (64 -> 95)
	ioHostMQTT = ioB2_0,
	ioHostFOTA,
	ioHostSLOG,
	ioHostCONF,
	ioMQTT_QoS,
	ioMQTTport,
	ioB2_6,
	ioB2_7,
	ioAPindex,
#if (halHAS_ADE7953 > 0)
	ade7953NVS,
#elif (halHAS_M90E26 > 0)
	m90e26NVS,
#else
	ioB2_9,
#endif
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
	dbgIdent,						// debug/track iDENT activities
	dbgActuate,						// 0=Off, 1=RuleEngine, 2=TaskAct, 3=Both
	dbgGUI,							// 0=OFF, 1=  , 2=  , 3=ALL
	dbgGPI,							// 0=OFF, 1=ISR, 2=EVT, 3=BOTH
	ioB2_28,
	ioB2_29,
	ioB2_30,
	dbgDS248X,

	ioB3_0 = 96, 					// 3-bit options (21x 96->116)
	ioU0Speed = ioB3_0,				// UARTx speed
	ioU1Speed,
	ioU2Speed,
	ioU0RXbuf,						// UARTx Rx FIFO buffers
	ioU1RXbuf,
	ioU2RXbuf,
	ioU0TXbuf,						// UARTx Tx FIFO buffers
	ioU1TXbuf,
	ioU2TXbuf,
	ioB3_9,
	ioB3_10,
	ioB3_11,
	ioB3_12,
	ioB3_13,
	ioB3_14,
	ioB3_15,
	ioB3_16,
	ioWLretry,
	ioWLauth,						// OPEN->WEP->WPA_PSK->WPA2_PSK->WPA_WPA2_PSK->WPA2_ENTERPRISE etc...
	ioSLhost,						// Highest # (lowest PRI) sent to host (must <= ioSLOGhi)
	ioSLOGhi,						// Highest # (lowest PRI) evaluated

	ioB4_0 = 117,					// 4-bit options (16x 117->132)
	dlyDS1990 = ioB4_0,				// delay (sec) between successive read same tag
	ioCLIbuf, 						// 0=disable, 1+ = (n+1)*128 bytes in size
	ioMBews,
	dbgDBase,						// 1=Match, 2=Fields, 4=Records, 8=Database
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
	toConsole,

	ioB8_0 = 133, 					// 8-bit options (8x 133->140)
	dlyMBread = ioB8_0,
	dlyMBpoll,
	ioB8_2,
	ioB8_3,
	ioB8_4,
	ioB8_5,
	ioB8_6,
	ioB8_7,

	ioBXlast = 141, 				// Non NVS options....
	ioS_NWMO = ioBXlast,			// 141 {0->3} 			(network mode)
	ioS_WIFI,						// 142 {ssid} {pswd}
	ioS_MQTT,						// 143 {w.x.y.z[:???]}	(mqtt broker/proxy)
	ioS_PEEK,						// 144 {addr} {size}
	ioS_POKE,						// 145 {addr} u8 [u8 ... u8]
	ioS_IOdef,						// 146 					(reset to defaults)
	ioS_LAST = ioS_IOdef,
} ;

// ######################################### structures ############################################



// ###################################### Private variables ########################################


// ###################################### Public variables #########################################


// ######################################## Public APIs ############################################

#ifdef __cplusplus
}
#endif
