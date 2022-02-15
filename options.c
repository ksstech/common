/*
 * options.h
 */

#include	"hal_variables.h"
#include	"hal_network.h"
#include	"hal_usart.h"
#include	"hal_gpio.h"

#include	"printfx.h"
#include	"syslog.h"
#include	"x_errors_events.h"

#include	"string.h"

// ####################################### MACRO definitions #######################################

#define	debugFLAG					0xF000

#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

// ######################################### enumerations ##########################################


// ########################################## structures ###########################################


// ###################################### private constants ########################################

const char ioBXmes[] =
"STDIO Buf\0"	"I2Cinit\0"		"I2Cdly\0"		"FOTA\0"		"Flags\0"		"Timeout\0"		"UpDown\0"		"P_Token\0"
"P_Param\0"		"P_Syntax\0"	"P_JSON\0"		"Sense\0"		"Mode\0"		"EndPoint\0"	"DB Match\0"	"DB Error\0"
"MQTT Con\0"	"MQTT Sub\0"	"MQTT Pub\0"	"OW Scan\0"		"Actuate\0"		"Alerts\0"		"Memory\0"		"TNETstart\0"
"TNETtrack\0"	"HTTPstart\0"	"HTTPtrack\0"	"SensTrack\0"	"RuleTable\0"	"RuleSched\0"	"RuleIdent\0"	"LittleFS\0"
"DS18x20\0"		"DS1990x\0"		"DS24check\0"	"M90write\0"	"M90offset\0"	"LIS2HH12\0"	"\0"			"\0"
"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"WL ExtAnt\0"	"WL Hidden\0"	"WL Mode\0"		"WL Events\0"	"WL RAM\0"		"WL Scan\0"		"WL Sort\0"

"HostMQTT\0"	"HostFOTA\0"	"HostSLOG\0"	"HostCONF\0"	"\0"			"\0"			"\0"			"\0"
"APindex\0"		"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"DS248Xdbg\0"			"\0"

"U0speed\0"		"U1speed\0"		"U2speed\0"		"U0RXbuf\0"		"U1RXbuf\0"		"U2RXbuf\0"		"U0TXbuf\0"		"U1TXbuf\0"
"U2TXbuf\0"		"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"WLretry\0"		"WL Auth\0"		"SlHostMax\0"	"SlShowMax\0"

"DS1990Dly\0"	"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"GUIintval\0"	"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0";

const char ioSxMes[] = "Non-bit options:\n133=WL Mode\t134=AP detail\t135=MQTT Proxy\t136=Mem PEEK\t137=IOSet Def\n\n";

// ###################################### private variables ########################################

ioset_t const ioDefaults = {
#if (configPRODUCTION == 0)
	.B1_0	= 0,					// ioSTDIO
	.B1_4	= 1,					// ioFlags

	.B2_0	= hostDEV,
	.B2_1	= hostDEV,
	.B2_2	= hostDEV,
	.B2_3	= hostDEV,
	.B3_19	= CONFIG_LOG_DEFAULT_LEVEL + 1,				// Maximum level sent to host
	.B3_20	= CONFIG_LOG_DEFAULT_LEVEL + 2,
#else
	.B1_0	= 1,					// ioSTDIO
	.B1_4	= 0,					// ioFlags

	.B2_0	= hostPROD,
	.B2_1	= hostPROD,
	.B2_2	= hostPROD,
	.B2_3	= hostPROD,

	.B3_19	= CONFIG_LOG_DEFAULT_LEVEL + 0,				// Maximum level sent to host
	.B3_20	= CONFIG_LOG_DEFAULT_LEVEL + 1,
#endif
	// Common values for production & development
	.B1_23	= 1,					// TNETstart
	.B1_25	= 1,					// HTTPstart
	.B1_58	= 1,					// ioWLhidden 1=enabled
	.B1_62	= 1,					// ioWLscan 1=all channels

	.B3_3	= 1,					// ioU0RXbuf
	.B3_6	= 1,					// ioU0TXbuf
	.B3_17	= 4,					// ioWLretry

	.B4_0	= 5,					// successive read interval, avoid duplicates
	.B4_8	= 5,					// GUI delay between screens
};

// ####################################### public variables ########################################


// ####################################### public functions ########################################

void xOptionsSetDefaults(void) { memcpy(&sNVSvars.ioBX, &ioDefaults, sizeof(ioset_t)); }

/**
 * Set value of 1, 2, 3 or 4 bit option values
 * @param	ON - option number
 * @param	OV - option value
 * @return	1 if value changed, 0 if value same or error code
 */
int xOptionsSetDirect(int ON, int OV) {
	int iRV = 1;
	if (ON > ioB4_15) {
		ERR_EXIT("Invalid option number", erSCRIPT_INV_OPERATION);
	}
	int EVL = (ON >= ioB4_0) ? 15 : (ON >= ioB3_0) ? 7 : (ON >= ioB2_0) ? 3 : 1 ;
	if (OUTSIDE(0, OV, EVL, int)) {
		ERR_EXIT("Invalid option value", erSCRIPT_INV_VALUE);
	}
	// to avoid unnecessary flash writes, only write if value is different.
	if (ON >= ioB4_0) {
		if (ioB4GET(ON) != OV)
			ioB4SET(ON, OV)
		else
			iRV = 0;
	} else if (ON >= ioB3_0) {
		if (ioB3GET(ON) != OV)
			ioB3SET(ON, OV)
		else
			iRV = 0;
	} else if (ON >= ioB2_0) {
		if (ioB2GET(ON) != OV)
			ioB2SET(ON, OV)
		else
			iRV = 0;
	} else {
		if (ioB1GET(ON) != OV)
			ioB1SET(ON, OV)
		else
			iRV = 0;
		if (iRV) {					// Something changed, do exception processing
			if (ioU0Speed <= ON && ON <= ioU2Speed) {		// UARTx speed change
				halUART_SetSpeed(ON - ioU0Speed);
			} else if (ioU0RXbuf <= ON && ON <= ioU2TXbuf) {// UARTx TX/RX buffer size change
				halUART_CalcBuffersSizes();
			}
			#if	(HW_VARIANT == HW_WIPY)
			else if (ON == ioWLantenna) {					// Ext Antenna en/disable
				ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_21, ioB1GET(ioWLantenna)));
			}
			#endif
			#if	(configCONSOLE_TELNET == 1)
			else if (ON == ioTNETstart) {					// TNET task start/stop
				#include "x_telnet_server.h"
				vTaskTnetStatus();
			}
			#endif
			#if	(configCONSOLE_HTTP == 1)
			else if (ON == ioHTTPstart) {					// HTTP task start/stop
				#include "x_http_server.h"
				vTaskHttpStatus();
			}
			#endif
		} // end (iRV)
	}
exit:
	return iRV;
}

int	xOptionsSet(int	ON, int OV, int PF) {
	int iRV = erSUCCESS ;
	IF_PRINT(debugTRACK, "IOSET %d=%d (%d)\n", ON, OV, PF);
	if (ON <= ioB4_15) {
		iRV = xOptionsSetDirect(ON, OV);
		// If nothing changed, force persistence flag to false
		if (iRV == 0) {
			PF = 0;
			printf("ON=%d not changed", OV);
		}
	} else if (ON == ioS_NWMO) {
		iRV = INRANGE(WIFI_MODE_NULL, OV, WIFI_MODE_APSTA, int) ? halWL_SetMode(OV) : erFAILURE ;
	} else if (ON == ioS_IOdef) {						// reset ALL IOSet values to defaults
		xOptionsSetDefaults();
	}
	if ((iRV > erFAILURE) && (PF == 1))
		setSYSFLAGS(vfIOSET);
	return iRV ;
}

int xOptionGetCurrent(int ON) {
	return (ON >= ioS_NWMO) ? erFAILURE :
			(ON >= ioB4_0) ? ioB4GET(ON) :
			(ON >= ioB3_0) ? ioB3GET(ON) :
			(ON >= ioB2_0) ? ioB2GET(ON) :
							ioB1GET(ON);
}

int xOptionGetDefault(int ON) {
	return (ON >= ioS_NWMO) ? erFAILURE :
			(ON >= ioB4_0) ? maskGET4B(ioDefaults.ioB4, (ON - ioB4_0), uint64_t) :
			(ON >= ioB3_0) ? maskGET3B(ioDefaults.ioB3, (ON - ioB3_0), uint64_t) :
			(ON >= ioB2_0) ? maskGET2B(ioDefaults.ioB2, (ON - ioB2_0), uint64_t) :
							maskGET1B(ioDefaults.ioB1, (ON - ioB1_0), uint64_t);
}

void vOptionsShow(void) {
	const char * pcMess = ioBXmes;
	int Cur, Def, Col, Len, Idx = 0;
	printfx_lock();
	for (int Num = ioB1_0; Num <= ioB4_15; ++Num) {
		Cur = xOptionGetCurrent(Num);
		Def = xOptionGetDefault(Num);
		Col = (Cur == Def) ? 0 : colourBG_CYAN;
		if (Num == ioB1_0) {
			printfx_nolock("1-Bit options: 0x%llX\n", sNVSvars.ioBX.ioB1);
			Idx = 0 ;
		} else if (Num == ioB2_0) {
			printfx_nolock("2-Bit options: 0x%llX\n", sNVSvars.ioBX.ioB2);
			Idx = 0 ;
		} else if (Num == ioB3_0) {
			printfx_nolock("3-Bit options: 0x%llX\n", sNVSvars.ioBX.ioB3);
			Idx = 0 ;
		} else if (Num == ioB4_0) {
			printfx_nolock("\n4-Bit options: 0x%llX\n", sNVSvars.ioBX.ioB4);
			Idx = 0 ;
		}
		printfx_nolock("%3d=%C%x%C/", Num, Col, Cur, 0);
		Len = printfx_nolock("%s", pcMess);
		pcMess += Len + 1;
		if (Idx == 7) {
			printfx_nolock("\n");
			Idx = 0;
		} else {
			printfx_nolock("%.*s", 12-Len, "            ");
			++Idx;
		}
	}
	printfx_nolock(ioSxMes);
	printfx_unlock();
}
