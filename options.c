/*
 * options.h
 */

#include	"hal_variables.h"
#include	"hal_network.h"

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

const char ioB1Mes[] =
	"0=STDIO Buf\t1=I2Cinit\t2=I2Cdly\t3=FOTA\t\t4=Flags \t5=Timeout\t6=Startup\t7=ShutDown\n"
	"\t8=ParaPar\t9=SyntPar\t10=Sense\t11=Mode\t\t12=EndPoint\t13=Ident\t14=DB Match\t15=DB Error\n"
	"\t16=MQTT Con\t17=MQTT Sub\t18=MQTT Pub\t19=OW Scan\t20=Actuate\t21=Alerts\t22=Memory\t23=TNETtrack\n"
	"\t24=HTTPtrack\t25=SENSORtrack\t26-RulesTable\t27=RulesSched\t28=RulesIdent\t29-LittleFS\n"
	"\t32=DS18x20\t33=DS1990x\t34=DS248Xstat\t35=M90write\t36=M90offset\n"
	"\t\t\t\t\t\t\t59=WL Mode\t60=WL Events\t61=WL RAM\t62=WL Scan\t63=WL Sort\n";

const char ioB2Mes[] = "64=DS248Xdbg\t65->95=Unused\n";

const char ioB3Mes[] =
	"\n\t96=U0 Speed\t97=U1 Speed\t98=U2 Speed\t99=U0 RXbuf\t100=U1 RXbuf\t101=U2 RXbuf\t102=U0 TXbuf\n"
	"\t103=U1 TXbuf\t104=U2 TXbuf\t115=WL Auth\t116=SLOG Max\n";

const char ioB4Mes[] = "117=DS199xRdDly\t118->132=Unused\n";

const char ioSxMes[] = "\t133=WL Mode\t134=AP detail\t135=MQTT Proxy\t136=Mem PEEK\t137=IOSet Def\n";

// ###################################### private variables ########################################

ioset_t const ioDefaults = {
	.B3_3	= 1,
	.B3_6	= 1,
	.B3_20	= CONFIG_LOG_DEFAULT_LEVEL + 2,
	.B4_0	= ds1990READ_INTVL,
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
		if (iRV == 0)
		{ PF = 0; LLTRACK("ON %d not changed", OV); }
	} else if (ON == ioS_NWMO) {
		iRV = INRANGE(WIFI_MODE_NULL, OV, WIFI_MODE_APSTA, int) ? halWL_SetMode(OV) : erFAILURE ;
	} else if (ON == ioS_IOdef) {						// reset ALL IOSet values to defaults
		xOptionsSetDefaults();
	}
	if ((iRV > erFAILURE) && (PF == 1))
		SystemFlag |= varFLAG_IOSET;
	return iRV ;
}

void vOptionsPrint(int Num, int v1, int v2) {
	printfx("%C%3d=%x%C ", (v1 == v2) ? 0 : xpfSGR(colourBG_CYAN,0,0,0), Num, v1, 0);
}

void vOptionsShow(void) {
	printfx("ioB1: 0x%llx\n\t", sNVSvars.ioBX.ioB1);
	for (int i = 0; i <= 63; ++i) {
		vOptionsPrint(i, ioB1GET(i), (ioDefaults.ioB1 >> i) & 1);
		if ((i % 16) == 15) printfx("\n\t");
	}
	printfx(ioB1Mes);
	printfx("ioB2: 0x%llx\n\t", sNVSvars.ioBX.ioB2);
	for (int i = 0; i <= 31; ++i) {
		vOptionsPrint(i+ioB2_0, ioB2GET(i+ioB2_0), (ioDefaults.ioB2 >> (i*2)) & 3);
		if ((i % 16) == 15) printfx("\n\t");
	}
	printfx(ioB2Mes);
	printfx("ioB3: 0x%llx\n\t", sNVSvars.ioBX.ioB3);
	for (int i = 0; i <= 20; ++i) {
		vOptionsPrint(i+ioB3_0, ioB3GET(i+ioB3_0), (ioDefaults.ioB3 >> (i*3)) & 7);
		if ((i % 16) == 15) printfx("\n\t");
	}
	printfx(ioB3Mes);
	printfx("ioB4: 0x%llx\n\t", sNVSvars.ioBX.ioB4);
	for (int i = 0; i <= 15; ++i) {
		vOptionsPrint(i+ioB4_0, ioB4GET(i+ioB4_0), (ioDefaults.ioB4 >> (i*4)) & 15);
		if ((i % 16) == 15) printfx("\n\t");
	}
	printfx(ioB4Mes);
	printfx(ioSxMes);
}

