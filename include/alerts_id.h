// alerts_id.h - Copyright (c) 2016-26 Andre M. Maree / KSS Technologies (Pty) Ltd.

#pragma once

// Alert Type/Level namespace. Lives here, not in endpoints, because the actuators engine and any
// project embedding it need these WITHOUT the endpoint/rules stack. Values go on the wire (GPB +
// TB JSON) and are dispatched by RANGE - never renumber a block without checking both encoders.

#ifdef __cplusplus
extern "C" {
#endif

// ######################################### Enumerations #########################################

enum {													// IRMACOS Alert Levels
	alertLEVEL_CRITICAL,
	alertLEVEL_ERROR,
	alertLEVEL_WARNING,
	alertLEVEL_NOTICE,
	alertLEVEL_INFO,
	alertLEVEL_NUM
};

enum {													// IRMACOS Alert types
	alertTYPE_UNDEFINED,
	alertTYPE_L01_GRANTED = 1,							// TAG ID
	alertTYPE_L02_GRANTED,								// tNB (Ident 2 only)
	alertTYPE_L03_GRANTED,								// tNA
	alertTYPE_L04_GRANTED,								// UCP
	alertTYPE_L05_GRANTED,								// MAC
	alertTYPE_L06_GRANTED,								// CHan
	alertTYPE_L07_GRANTED,
	alertTYPE_L08_GRANTED,
	alertTYPE_L09_GRANTED,
	alertTYPE_L10_GRANTED,
	alertTYPE_L11_GRANTED,
	alertTYPE_L12_GRANTED,
	alertTYPE_L13_GRANTED,
	alertTYPE_L14_GRANTED,
	alertTYPE_L15_GRANTED,
	alertTYPE_L16_GRANTED,								// L16=Master (temporary ?)

	alertTYPE_L01_DENIED = 41,							// ID invalid
	alertTYPE_L02_DENIED,								// time ([tNB/]tNA) invalid
	alertTYPE_L04_DENIED,								// UCP or MAC invalid
	alertTYPE_L08_DENIED,								// CHan invalid

	alertTYPE_RES_DISPENSED = 61,
	alertTYPE_RES_WITHHELD,
// End of IDENTity related events
	alertTYPE_ACT_SENSE = 71,							// SENSE command as actuation
	alertTYPE_ACT_MODE,									// MODE command as actuation
// Actuator ONLY alerts
	alertTYPE_ACT_LOADED	= 81,
	alertTYPE_ACT_RELOAD,
	alertTYPE_ACT_UPDATE,
	alertTYPE_ACT_ADJUST,
	alertTYPE_ACT_SEQ,
	alertTYPE_ACT_STAGE,
	alertTYPE_ACT_DONE,
	alertTYPE_ACT_START,
	alertTYPE_ACT_STOPPED,
	alertTYPE_ACT_BLOCKED,

	alertTYPE_ERR_SYNTAX = 250,
	alertTYPE_ERR_PARAM,
};

#ifdef __cplusplus
}
#endif
