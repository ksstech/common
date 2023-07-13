/*
 * Copyright (c) 2020-2023 Andre M. Maree/KSS Technologies (Pty) Ltd.
 */

#include "hal_variables.h"			// required by options.h

#include "options.h"
#include "hal_network.h"
#include "hal_usart.h"
#include "hal_gpio.h"

#include "printfx.h"
#include "syslog.h"
#include "x_errors_events.h"

// ######################################## BUILD macros ###########################################

#define	debugFLAG					0xF000

#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

// ############################################ macros #############################################


// ######################################### enumerations ##########################################


// ########################################## structures ###########################################


// ###################################### private constants ########################################

const char ioBXmes[] = iosetHELP_MESSAGE;

const char ioSXmes[] = "Non-bit options:\r\n"
						"141=WLmode\t"
						"142=APdetail\t"
						"143=MQTTproxy\t"
						#if (configPRODUCTION == 0)
						"144=MEMpeek\t"
						"145=MEMpoke\t"
						#else
						"144=Unused\t"
						"145=Unused\t"
						#endif
						"146=Defaults\r\n\n";

ioset_t const ioDefaults = { iosetDEFAULTS };

// ###################################### private variables ########################################


// ####################################### public variables ########################################


// ####################################### public functions ########################################

static void xOptionsSetDefaults(void) { memcpy(&sNVSvars.ioBX, &ioDefaults, sizeof(ioset_t)); }

/**
 * @brief	Set value of 1, 2, 3 or 4 bit option values
 * @param	ON - option number
 * @param	OV - option value
 * @return	1 if value changed, 0 if value same or error code
 */
int xOptionsSetDirect(int ON, int OV) {
	int iRV;
	IF_EXIT_MX(ON >= ioBXlast, "Invalid option number", erINV_OPERATION);

	// determine the max option value, based on size, and check validity
	int EVL = (ON >= ioB8_0) ? 255 : (ON >= ioB4_0) ? 15 : (ON >= ioB3_0) ? 7 : (ON >= ioB2_0) ? 3 : 1;
	IF_EXIT_MX(OUTSIDE(0, OV, EVL), "Invalid option value", erINV_VALUE);

	// check for exceptions, allowed value not same as full range
	IF_EXIT_MX(ON == ioMQTT_QoS && OUTSIDE(0, OV, 2), "MQTT QoS 0->2", erINV_VALUE);

	// to avoid unnecessary flash writes, only write if value is different.
	iRV = 1;
	if (ON >= ioB8_0) {
		if (ioB8GET(ON) != OV) ioB8SET(ON, OV) else iRV = 0;
	} else if (ON >= ioB4_0) {
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

		} else if (ON == ioAPindex) {
			WLstate.Retry = 0;
			WLstate.Index = ioB2GET(ioAPindex);
			halWL_SetMode(WIFI_MODE_STA, 1);

		} else if (INRANGE(ioU0Speed, ON, ioU2Speed)) {
			halUART_SetSpeed(ON - ioU0Speed);			// UARTx speed change
		} else if (INRANGE(ioU0RXbuf, ON, ioU2TXbuf)) {
			halUART_CalcBuffersSizes();					// UARTx TX/RX buffer size change
		}

		#if	(cmakePLTFRM == HW_WIPY)
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

int	xOptionsSet(int	OptNum, int OptVal, int Persist) {
	int iRV = erSUCCESS;
	IF_P(debugTRACK && ioB1GET(dbgSyntax), "\rioset %d=%d (%d)\r\n", OptNum, OptVal, Persist);
	if (OptNum < ioBXlast) {
		iRV = xOptionsSetDirect(OptNum, OptVal);
		// If nothing changed, force persistence flag to false
		if (iRV == 0)
			Persist = 0;
	} else if (OptNum == ioS_IOdef) {						// reset ALL IOSet values to defaults
		xOptionsSetDefaults();
	}
	if ((iRV >= erSUCCESS) && Persist == 1)
		setSYSFLAGS(vfIOSET);
	return iRV;
}

int xOptionGetCurrent(int ON) {
	return (ON >= ioBXlast) ? erFAILURE :
			(ON >= ioB8_0) ? ioB8GET(ON) :
			(ON >= ioB4_0) ? ioB4GET(ON) :
			(ON >= ioB3_0) ? ioB3GET(ON) :
			(ON >= ioB2_0) ? ioB2GET(ON) : ioB1GET(ON);
}

int xOptionGetDefault(int ON) {
	return (ON >= ioBXlast) ? erFAILURE :
			(ON >= ioB8_0) ? maskGET8B(ioDefaults.ioB8, (ON - ioB8_0), uint64_t) :
			(ON >= ioB4_0) ? maskGET4B(ioDefaults.ioB4, (ON - ioB4_0), uint64_t) :
			(ON >= ioB3_0) ? maskGET3B(ioDefaults.ioB3, (ON - ioB3_0), uint64_t) :
			(ON >= ioB2_0) ? maskGET2B(ioDefaults.ioB2, (ON - ioB2_0), uint64_t) :
							maskGET1B(ioDefaults.ioB1, (ON - ioB1_0), uint64_t);
}

void vOptionsShow(void) {
	const char * pcMess = ioBXmes;
	int Cur, Def, Col, Len, Idx = 0;
	printfx_lock();
	for (int Num = ioB1_0; Num < ioBXlast; ++Num) {
		Cur = xOptionGetCurrent(Num);
		Def = xOptionGetDefault(Num);
		Col = (Cur == Def) ? 0 : colourBG_CYAN;
		if (Num == ioB1_0) {
			printfx_nolock("1-Bit options: 0x%0llX\r\n", sNVSvars.ioBX.ioB1);
			Idx = 0;
		} else if (Num == ioB2_0) {
			printfx_nolock("2-Bit options: 0x%0llX\r\n", sNVSvars.ioBX.ioB2);
			Idx = 0;
		} else if (Num == ioB3_0) {
			printfx_nolock("3-Bit options: 0x%0llX\r\n", sNVSvars.ioBX.ioB3);
			Idx = 0;
		} else if (Num == ioB4_0) {
			printfx_nolock("\r\n4-Bit options: 0x%0llX\r\n", sNVSvars.ioBX.ioB4);
			Idx = 0;
		} else if (Num == ioB8_0) {
			printfx_nolock("8-Bit options: 0x%0llX\r\n", sNVSvars.ioBX.ioB8);
			Idx = 0;
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
	printfx_nolock(ioSXmes);
	printfx_unlock();
}
