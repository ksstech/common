/*
 * options.c
 * Copyright (c) 2020-2022 Andre M. Maree/KSS Technologies (Pty) Ltd.
 */

#include "hal_variables.h"
#include "hal_network.h"
#include "hal_usart.h"
#include "hal_gpio.h"

#include "printfx.h"
#include "syslog.h"
#include "x_errors_events.h"

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
"TZlocal\0"		"I2Cinit\0"		"I2Cdly\0"		"FOTA\0"		"Flags\0"		"Timeout\0"		"UpDown\0"		"P_Token\0"
"P_Param\0"		"P_Syntax\0"	"P_JSON\0"		"W_JSON\0"		"Sense\0"		"Mode\0"		"EndPoint\0"	"\0"
"DB Match\0"	"DB Error\0"	"MQTT Con\0"	"MQTT Sub\0"	"MQTT Pub\0"	"\0"			"Actuate\0"		"Alerts\0"
"Memory\0"		"SensTrack\0"	"RuleTable\0"	"RuleSched\0"	"\0"			"LittleFS\0"	"TNETtrack\0"	"HTTPtrack\0"
"TNETstart\0"	"HTTPstart\0"	"TNETauth\0"	"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"\0"			"\0"			"M90write\0"	"M90offset\0"	"\0"			"\0"			"\0"
"LIS2HH12\0"	"\0"			"\0"			"dbgMODBUS\0"	"dbgDS1820\0"	"dbgDS1990\0"	"dbgOWscan\0"	"dbgTracker\0"
"\0"			"WL ExtAnt\0"	"WLhidden\0"	"WLmode\0"		"WLevents\0"	"WLram\0"		"WLscan\0"		"WLsort\0"

"HostMQTT\0"	"HostFOTA\0"	"HostSLOG\0"	"HostCONF\0"	"QoS 0-2\0"		"\0"			"\0"			"\0"
"APindex\0"		"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"dbgIdent\0"	"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"dbgDS248X\0"

"U0speed\0"		"U1speed\0"		"U2speed\0"		"U0RXbuf\0"		"U1RXbuf\0"		"U2RXbuf\0"		"U0TXbuf\0"		"U1TXbuf\0"
"U2TXbuf\0"		"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"WLretry\0"		"WL Auth\0"		"SlHostMax\0"	"SlShowMax\0"	// only 21x 3bit options

"DS1990Dly\0"	"CLIbuf\0"		"ioMBews\0"		"\0"			"\0"			"\0"			"\0"			"\0"
"GUIintval\0"	"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"toConsole\0";

const char ioSxMes[] = "Non-bit options:\r\n133=WL Mode\t134=AP detail\t135=MQTT Proxy\t136=Mem PEEK\t137=IOSet Def\r\n\n";

// ###################################### private variables ########################################


// ####################################### public variables ########################################

ioset_t const ioDefaults = {
	#if (configPRODUCTION == 0)
	iosetDEFAULT_DEV
	iosetDEFAULT
	#else
	iosetDEFAULT_PROD
	iosetDEFAULT
	#endif
};

// ####################################### public functions ########################################

void xOptionsSetDefaults(void) { memcpy(&sNVSvars.ioBX, &ioDefaults, sizeof(ioset_t)); }

/**
 * Set value of 1, 2, 3 or 4 bit option values
 * @param	ON - option number
 * @param	OV - option value
 * @return	1 if value changed, 0 if value same or error code
 */
int xOptionsSetDirect(int ON, int OV) {
	int iRV;
	IF_EXIT_MX(ON > ioB4_15, "Invalid option number", erINV_OPERATION);

	int EVL = (ON >= ioB4_0) ? 15 : (ON >= ioB3_0) ? 7 : (ON >= ioB2_0) ? 3 : 1;
	IF_EXIT_MX(OUTSIDE(0, OV, EVL), "Invalid option value", erINV_VALUE);

	IF_EXIT_MX(ON == ioMQTT_QoS && OUTSIDE(0, OV, 2), "MQTT QoS 0->2", erINV_VALUE);

	// to avoid unnecessary flash writes, only write if value is different.
	iRV = 1;
	if (ON >= ioB4_0) {
		if (ioB4GET(ON) != OV) ioB4SET(ON, OV) else iRV = 0;
	} else if (ON >= ioB3_0) {
		if (ioB3GET(ON) != OV) ioB3SET(ON, OV) else iRV = 0;
	} else if (ON >= ioB2_0) {
		if (ioB2GET(ON) != OV) ioB2SET(ON, OV) else iRV = 0;
	} else {
		if (ioB1GET(ON) != OV) ioB1SET(ON, OV) else iRV = 0;
	}
	if (iRV) {					// Something changed, do exception processing
		if (ON == ioHostMQTT) {
			setSYSFLAGS(sfOPT_MQTT);					// MQTT host changed
		} else if (INRANGE(ioU0Speed, ON, ioU2Speed)) {
			halUART_SetSpeed(ON - ioU0Speed);			// UARTx speed change
		} else if (INRANGE(ioU0RXbuf, ON, ioU2TXbuf)) {
			halUART_CalcBuffersSizes();					// UARTx TX/RX buffer size change
		}
		#if	(halVARIANT == HW_WIPY)
		else if (ON == ioWLantenna) {					// Ext Antenna en/disable
			ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_21, ioB1GET(ioWLantenna)));
		}
		#endif
		#if	(configCONSOLE_TELNET == 1)
		else if (ON == ioTNETstart) {					// TNET task start/stop
			#include "x_telnet_server.h"
			vTnetStartStop();
		}
		#endif
		#if	(configCONSOLE_HTTP == 1)
		else if (ON == ioHTTPstart) {					// HTTP task start/stop
			#include "x_http_server.h"
			vHttpStartStop();
		}
		#endif
	} // end (iRV)
exit:
	return iRV;
}

int	xOptionsSet(int	ON, int OV, int PF) {
	int iRV = erSUCCESS ;
	IF_P(debugTRACK, "IOSET %d=%d (%d)\r\n", ON, OV, PF);
	if (ON <= ioB4_15) {
		iRV = xOptionsSetDirect(ON, OV);
		// If nothing changed, force persistence flag to false
		if (iRV == 0) {
			PF = 0;
		}
	} else if (ON == ioS_NWMO) {
		iRV = INRANGE(WIFI_MODE_NULL, OV, WIFI_MODE_APSTA) ? halWL_SetMode(OV) : erFAILURE ;
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
			printfx_nolock("1-Bit options: 0x%llX\r\n", sNVSvars.ioBX.ioB1);
			Idx = 0 ;
		} else if (Num == ioB2_0) {
			printfx_nolock("2-Bit options: 0x%llX\r\n", sNVSvars.ioBX.ioB2);
			Idx = 0 ;
		} else if (Num == ioB3_0) {
			printfx_nolock("3-Bit options: 0x%llX\r\n", sNVSvars.ioBX.ioB3);
			Idx = 0 ;
		} else if (Num == ioB4_0) {
			printfx_nolock("\r\n4-Bit options: 0x%llX\r\n", sNVSvars.ioBX.ioB4);
			Idx = 0 ;
		}
		printfx_nolock("%3d=%C%x%C/", Num, Col, Cur, 0);
		Len = printfx_nolock("%s", pcMess);
		pcMess += Len + 1;
		if (Idx == 7) {
			printfx_nolock(strCRLF);
			Idx = 0;
		} else {
			printfx_nolock("%.*s", 12-Len, "            ");
			++Idx;
		}
	}
	printfx_nolock(ioSxMes);
	printfx_unlock();
}
