/*
 * options.h
 */

#include	"hal_variables.h"
#include	"hal_network.h"
#include	"hal_usart.h"

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
"STDIO Buf\0"	"I2Cinit\0"		"I2Cdly\0"		"FOTA\0"		"Flags\0"		"Timeout\0"		"Startup\0"		"ShutDown\0"
"ParaPar\0"		"SyntPar\0"		"JSONpar\0"		"Sense\0"		"Mode\0"		"EndPoint\0"	"DB Match\0"	"DB Error\0"
"MQTT Con\0"	"MQTT Sub\0"	"MQTT Pub\0"	"OW Scan\0"		"Actuate\0"		"Alerts\0"		"Memory\0"		"\0"
"TNETtrack\0"	"HTTPtrack\0"	"HTTPclnt\0"	"SensTrack\0"	"RuleTable\0"	"RuleSched\0"	"RuleIdent\0"	"LittleFS\0"
"DS18x20\0"		"DS1990x\0"		"DS24check\0"	"M90write\0"	"M90offset\0"	"\0"			"\0"			"\0"
"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"\0"			"WL Hidden\0"	"WL Mode\0"		"WL Events\0"	"WL RAM\0"		"WL Scan\0"		"WL Sort\0"

"DS248Xdbg\0"	"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"

"U0speed\0"		"U1speed\0"		"U2speed\0"		"U0RXbuf\0"		"U1RXbuf\0"		"U2RXbuf\0"		"U0TXbuf\0"		"U1TXbuf\0"
"U2TXbuf\0"		"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"WLretry\0"		"WL Auth\0"		"SlHostMax\0"	"SlShowMax\0"

"DS1990Dly\0"	"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"
"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0"			"\0";

const char ioSxMes[] = "Non-bit options:\n133=WL Mode\t134=AP detail\t135=MQTT Proxy\t136=Mem PEEK\t137=IOSet Def\n\n";

// ###################################### private variables ########################################

#if (configPRODUCTION == 0)
ioset_t const ioDefaults = {
	.B1_0	= 0,					// ioSTDIO
	.B1_4	= 1,					// ioFlags
	.B1_58	= 1,					// ioWLhidden 1=enabled
	.B1_62	= 1,					// ioWLscan 1=all channels

	.B3_3	= 1,					// ioU0RXbuf
	.B3_6	= 1,					// ioU0TXbuf
	.B3_17	= 4,					// ioWLretry
	.B3_19	= 4,					// Maximum level sent to host
	.B3_20	= CONFIG_LOG_DEFAULT_LEVEL + 3,

	.B4_0	= 5,					// successive read interval, avoid duplicates
};
#else
ioset_t const ioDefaults = {
	.B1_0	= 1,					// ioSTDIO
	.B1_4	= 0,					// ioFlags
	.B1_58	= 1,					// ioWLhidden 1=enabled
	.B1_62	= 1,					// ioWLscan 1=all channels

	.B3_3	= 1,					// ioU0RXbuf
	.B3_6	= 1,					// ioU0TXbuf
	.B3_17	= 4,					// ioWLretry
	.B3_19	= 4,					// Maximum level sent to host
	.B3_20	= CONFIG_LOG_DEFAULT_LEVEL + 2,

	.B4_0	= 5,					// successive read interval, avoid duplicates
};
#endif

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
	if (ON > ioB4_15)
		ERR_EXIT("Invalid option number", erSCRIPT_INV_OPERATION);

	int EVL = (ON >= ioB4_0) ? 15 : (ON >= ioB3_0) ? 7 : (ON >= ioB2_0) ? 3 : 1 ;
	if (OUTSIDE(0, OV, EVL, int))
		ERR_EXIT("Invalid option value", erSCRIPT_INV_VALUE);

	// to avoid unnecessary flash writes, only write if value is different.
	if (ON >= ioB4_0) {
		if (ioB4GET(ON) != OV) ioB4SET(ON, OV) else iRV = 0;
	} else if (ON >= ioB3_0) {
		if (ioB3GET(ON) != OV) ioB3SET(ON, OV) else iRV = 0;
	} else if (ON >= ioB2_0) {
		if (ioB2GET(ON) != OV) ioB2SET(ON, OV) else iRV = 0;
	} else {
		if (ioB1GET(ON) != OV) ioB1SET(ON, OV) else iRV = 0;
		if (ioU0Speed <= ON && ON <= ioU2Speed) {
			halUART_SetSpeed(ON - ioU0Speed);
		} else if (ioU0TXbuf <= ON && ON <= ioU2TXbuf) {
			halUART_CalcBuffersSizes();
		}
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
		SystemFlag |= varFLAG_IOSET;
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
	for (int Num = ioB1_0; Num <= ioB4_15; ++Num) {
		Cur = xOptionGetCurrent(Num);
		Def = xOptionGetDefault(Num);
		Col = (Cur == Def) ? 0 : colourBG_CYAN;
		if (Num == ioB1_0) {
			printfx("1-Bit options: 0x%llX\n", sNVSvars.ioBX.ioB1);
			Idx = 0 ;
		} else if (Num == ioB2_0) {
			printfx("2-Bit options: 0x%llX\n", sNVSvars.ioBX.ioB2);
			Idx = 0 ;
		} else if (Num == ioB3_0) {
			printfx("3-Bit options: 0x%llX\n", sNVSvars.ioBX.ioB3);
			Idx = 0 ;
		} else if (Num == ioB4_0) {
			printfx("\n4-Bit options: 0x%llX\n", sNVSvars.ioBX.ioB4);
			Idx = 0 ;
		}
		printfx("%3d=%C%x%C/", Num, Col, Cur, 0);
		Len = printfx("%s", pcMess);
		pcMess += Len + 1;
		if (Idx == 7) {
			printfx("\n");
			Idx = 0;
		} else {
			printfx("%.*s", 12-Len, "            ");
			++Idx;
		}
	}
	printfx(ioSxMes);
}
