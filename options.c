/*
 * options.h
 */

#include	"hal_variables.h"
#include	"printfx.h"
#include	"syslog.h"
#include	"x_errors_events.h"

#include	"hal_network.h"

#include	"string.h"

// ####################################### MACRO definitions #######################################

#define	debugFLAG					0xF000

#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

// ######################################### enumerations ##########################################


// ########################################## structures ###########################################


// ####################################### public variables ########################################


// ####################################### public functions ########################################

int xOptionsSetDirect(int EI, int EV, int Flag) {
	if (EI > ioB4_15) {
		SET_ERRINFO("Invalid IOSet value");
		return erSCRIPT_INV_OPERATION;
	}
	int EVL = (EI >= ioB4_0) ? 15 : (EI >= ioB3_0) ? 7 : (EI >= ioB2_0) ? 3 : 1 ;
	if (OUTSIDE(0, EV, EVL, int) || OUTSIDE(0, Flag, 1, int))
		return erSCRIPT_INV_VALUE;
	if (EI >= ioB4_0) {
		ioB4SET(EI, EV);
	} else if (EI >= ioB3_0) {
		ioB3SET(EI, EV);
	} else if (EI >= ioB2_0) {
		ioB2SET(EI, EV);
	} else {
		ioB1SET(EI, EV);
	}
	IF_PRINT(debugTRACK, "IOSET %d=%d (%d)\n", EI, EV, Flag);
	if (Flag) BlobsFlag |= varFLAG_IOSET;
	return erSUCCESS;
}

int	xOptionsSet(uint8_t	EI, int EV, int Flag) {
	if (EI <= ioB4_15) {
		return xOptionsSetDirect(EI, EV, Flag) ;
	} else if (EI == ioS_NWMO) {
		return INRANGE(WIFI_MODE_NULL, EV, WIFI_MODE_APSTA, int) ? halWL_SetMode(EV) : erFAILURE ;

	} else if (EI == ioS_WIFI) {
		return erSUCCESS ;

	} else if (EI == ioS_MQTT) {
		return erSUCCESS ;

	} else if (EI == ioS_PEEK) {
		return erSUCCESS ;
	}
	return erFAILURE ;
}

void vOptionsShow(void) {
	printfx("ioB1:\n");
	for (int i = ioB1_0; i <= ioB1_63; ++i) {
		printfx("%3d=%d ", i, ioB1GET(i));
		if ((i % 16) == 15) printfx("\n");
	}
	printfx("ioB2:\n");
	for (int i = ioB2_0; i <= ioB2_31; ++i) {
		printfx("%3d=%d ", i, ioB2GET(i));
		if ((i % 16) == 15) printfx("\n");
	}
	printfx("ioB3:\n");
	for (int i = ioB3_0; i <= ioB3_20; ++i) {
		printfx("%3d=%d ", i, ioB3GET(i));
		if ((i % 16) == 15) printfx("\n");
	}
	printfx("\nioB4:\n");
	for (int i = ioB4_0; i <= ioB4_15; ++i) {
		printfx("%3d=%d ", i, ioB4GET(i));
		if (((i - ioB4_0) % 16) == 15) printfx("\n");
	}
	printfx("\n");
}

