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
	"\t16=MQTT Con\t17=MQTT Sub\t18=MQTT Pub\t19=OW Scan\t20=Actuate\t21=Alerts\t22=Memory\n"
	"\t32=DS18x20\t33=DS1990x\t34=DS248\t35=M90Ex6\n"
	"\t\t\t\t\t\t\t59=WL Mode\t60=WL Events\t61=WL RAM\t62=WL Scan\t63=WL Sort\n";

const char ioB2Mes[] = "64->95=Unused\n";

const char ioB3Mes[] =
	"\n\t96=U0 Speed\t97=U1 Speed\t98=U2 Speed\t99=U0 RXbuf\t100=U1 RXbuf\t101=U2 RXbuf\t102=U0 TXbuf\n"
	"\t103=U1 TXbuf\t104=U2 TXbuf\t115=WL Auth\t116=SLOG Max\n";

const char ioB4Mes[] = "117=DS199xRdDly\n"
		"118->132=Unused\n";

const char ioSxMes[] = "\t133=WL Mode\t134=AP detail\t135=MQTT Proxy\t136=Mem PEEK\t137=IOSet Def\n";

// ###################################### private variables ########################################

ioset_t const ioDefaults = {
	.B3_3 = 1,
	.B3_6 = 1,
	.B3_20 = CONFIG_LOG_MAXIMUM_LEVEL + 2,
	.B4_0 = ds1990READ_INTVL,
};

// ####################################### public variables ########################################


// ####################################### public functions ########################################

void xOptionsSetDefaults(void) { memcpy(&sNVSvars.ioBX, &ioDefaults, sizeof(ioset_t)); }

int xOptionsSetDirect(int EI, int EV, int Flag) {
	if (EI > ioB4_15)
		{ SET_ERRINFO("Invalid IOSet value"); return erSCRIPT_INV_OPERATION; }

	int EVL = (EI >= ioB4_0) ? 15 : (EI >= ioB3_0) ? 7 : (EI >= ioB2_0) ? 3 : 1 ;
	if (OUTSIDE(0, EV, EVL, int) || OUTSIDE(0, Flag, 1, int))
		{ ErrLine = __LINE__; return erSCRIPT_INV_VALUE; }

	if (EI >= ioB4_0) { ioB4SET(EI, EV); }
	else if (EI >= ioB3_0) { ioB3SET(EI, EV); }
	else if (EI >= ioB2_0) { ioB2SET(EI, EV); }
	else { ioB1SET(EI, EV); }
	return erSUCCESS;
}

int	xOptionsSet(int	EI, int EV, int Persist) {
	int iRV = erSUCCESS ;
	if (EI <= ioB4_15) {
		iRV = xOptionsSetDirect(EI, EV, Persist) ;
	} else if (EI == ioS_NWMO) {
		iRV = INRANGE(WIFI_MODE_NULL, EV, WIFI_MODE_APSTA, int) ? halWL_SetMode(EV) : erFAILURE ;
	} else if (EI == ioS_IOdef) {						// reset ALL IOSet values to defaults
		xOptionsSetDefaults();
	}
	if (iRV > erFAILURE) {
		if (Persist) SystemFlag |= varFLAG_IOSET;
		IF_PRINT(debugTRACK, "IOSET %d=%d (%d)\n", EI, EV, Persist);
	}
	return iRV ;
}

void vOptionsPrint(int Num, int v1, int v2) {
	if (v1 != v2)
		printfx("%C%3d=%x%C ", xpfSGR(colourBG_CYAN,0,0,0), Num, v1, 0);
	else
		printfx("%3d=%d ", Num, v1);
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
		vOptionsPrint(i+ioB3_0, ioB2GET(i+ioB3_0), (ioDefaults.ioB3 >> (i*3)) & 7);
		if ((i % 16) == 15) printfx("\n\t");
	}
	printfx(ioB3Mes);
	printfx("ioB4: 0x%llx\n\t", sNVSvars.ioBX.ioB4);
	for (int i = 0; i <= 15; ++i) {
		vOptionsPrint(i+ioB4_0, ioB2GET(i+ioB4_0), (ioDefaults.ioB4 >> (i*4)) & 15);
		if ((i % 16) == 15) printfx("\n\t");
	}
	printfx(ioB4Mes);
	printfx(ioSxMes);
}

