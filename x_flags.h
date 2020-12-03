/*
 * x_flags.h
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
 * BitBand macros for ARM M3/4 MCU's
 */
#define		BB_SRAM_BASE			0x22000000
#define		BB_SRAM(a,b) 			(BB_SRAM_BASE + ((a-SRAM_BASE) << 5) + (b << 2))
#define		BB_SRAM_SET(a,b)		(*(volatile uint32_t *) BB_SRAM(a,b) = 1)
#define		BB_SRAM_CLR(a,b)		(*(volatile uint32_t *) BB_SRAM(a,b) = 0)
#define		BB_SRAM_TST(a,b)		(*(volatile uint32_t *) BB_SRAM(a,b) == 1 ? true : false)
#define		BB_SRAM_TGL(a,b)		(*(volatile uint32_t *) BB_SRAM(a,b) = (BB_SRAM_TST(a,b)) ? 0 : 1)

#define		PERI_BASE				0x40000000
#define		BB_PERI_BASE			0x42000000
#define		BB_PERI(a,b) 			(BB_PERI_BASE + ((a-PERI_BASE) << 5) + (b << 2))
#define		BB_PERI_SET(a,b)		(*(volatile uint32_t *) BB_PERI(a,b) = 1)
#define		BB_PERI_CLR(a,b)		(*(volatile uint32_t *) BB_PERI(a,b) = 0)
#define		BB_PERI_TST(a,b)		(*(volatile uint32_t *) BB_PERI(a,b) == 1 ? true : false)
#define		BB_PERI_TGL(a,b)		(*(volatile uint32_t *) BB_PERI(a,b) = BB_PERI_TST(a,b) ? 0 : 1)

#define		FLAG_SYSTEM				((uint32_t) &FlagSystem)

#define		SYSTEM_SET(b)			BB_SRAM_SET(FLAG_SYSTEM,b)
#define		SYSTEM_CLR(b)			BB_SRAM_CLR(FLAG_SYSTEM,b)
#define		SYSTEM_TST(b)			BB_SRAM_TST(FLAG_SYSTEM,b)
#define		SYSTEM_TGL(b)			BB_SRAM_TGL(FLAG_SYSTEM,b)

#ifdef __cplusplus
}
#endif
