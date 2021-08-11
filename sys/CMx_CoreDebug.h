/*
 * CMx_CoreDebug.c - subset of CMSIS definitions specifically to access SCB & debug functionality
 *	Date		Ver		Who		Description
 *	2015/06/01	1.00	AMM		Original version
 *	2016/07/14	1.10	AMM		Simplified and added extra structure & union bitmaps

 * NOTES:
 * 1. Requires __CORTEX_M to be defined to reflect the CMSIS definition for the applicable core
 *
 */

#include	<stdint.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __CORTEX_M
	#if		defined( cc3200 )
		#define		__CORTEX_M							0x04
	#elif	defined( HW_P_PHOTON )
		#define		__CORTEX_M							0x03
	#endif
#endif

#if (!(__CORTEX_M == 0x03) && !(__CORTEX_M == 0x04))
//	#warning "Invalid value for __CORTEX_M !!!"
	#undef		__CORTEX_M
	#define		__CORTEX_M		0x03
#endif

/* IO definitions (access restrictions to peripheral registers) */

#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions                 */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions                 */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions                */
#define     __IO    volatile             /*!< Defines 'read / write' permissions              */

/*
typedef	union {
	struct {
		uint32_t	 	: 10 ;					//
		uint32_t	 	: 10 ;					//
		uint32_t	 	: 10 ;					//
		uint32_t	 	: 10 ;					//
	} ;
	uint32_t	w ;
} _t ;
*/


// #################################################################################################

typedef union APSR_u {
  struct {
#if (__CORTEX_M != 0x04)
    uint32_t _reserved0:27;              /*!< bit:  0..26  Reserved                           */
#else
    uint32_t _reserved0:16;              /*!< bit:  0..15  Reserved                           */
    uint32_t GE:4;                       /*!< bit: 16..19  Greater than or Equal flags        */
    uint32_t _reserved1:7;               /*!< bit: 20..26  Reserved                           */
#endif
    uint32_t Q:1;                        /*!< bit:     27  Saturation condition flag          */
    uint32_t V:1;                        /*!< bit:     28  Overflow condition code flag       */
    uint32_t C:1;                        /*!< bit:     29  Carry condition code flag          */
    uint32_t Z:1;                        /*!< bit:     30  Zero condition code flag           */
    uint32_t N:1;                        /*!< bit:     31  Negative condition code flag       */
  } b;                                   /*!< Structure used for bit  access                  */
  uint32_t w;                            /*!< Type      used for word access                  */
} APSR_Type;

typedef union IPSR_u {
  struct {
    uint32_t ISR:9;                      /*!< bit:  0.. 8  Exception number                   */
    uint32_t _reserved0:23;              /*!< bit:  9..31  Reserved                           */
  } b;                                   /*!< Structure used for bit  access                  */
  uint32_t w;                            /*!< Type      used for word access                  */
} IPSR_Type;

typedef union xPSR_u {
  struct {
    uint32_t ISR:9;                      /*!< bit:  0.. 8  Exception number                   */
#if (__CORTEX_M != 0x04)
    uint32_t _reserved0:15;              /*!< bit:  9..23  Reserved                           */
#else
    uint32_t _reserved0:7;               /*!< bit:  9..15  Reserved                           */
    uint32_t GE:4;                       /*!< bit: 16..19  Greater than or Equal flags        */
    uint32_t _reserved1:4;               /*!< bit: 20..23  Reserved                           */
#endif
    uint32_t T:1;                        /*!< bit:     24  Thumb bit        (read 0)          */
    uint32_t IT:2;                       /*!< bit: 25..26  saved IT state   (read 0)          */
    uint32_t Q:1;                        /*!< bit:     27  Saturation condition flag          */
    uint32_t V:1;                        /*!< bit:     28  Overflow condition code flag       */
    uint32_t C:1;                        /*!< bit:     29  Carry condition code flag          */
    uint32_t Z:1;                        /*!< bit:     30  Zero condition code flag           */
    uint32_t N:1;                        /*!< bit:     31  Negative condition code flag       */
  } b;                                   /*!< Structure used for bit  access                  */
  uint32_t w;                            /*!< Type      used for word access                  */
} xPSR_Type;

typedef union CONTROL_s {
  struct {
    uint32_t nPRIV		: 1 ;            /*!< bit:      0  Execution privilege in Thread mode */
    uint32_t SPSEL		: 1 ;            /*!< bit:      1  Stack to be used                   */
    uint32_t FPCA		: 1 ;            /*!< bit:      2  FP extension active flag           */
    uint32_t _reserved0	: 29;            /*!< bit:  3..31  Reserved                           */
  } b;                                   /*!< Structure used for bit  access                  */
  uint32_t w;                            /*!< Type      used for word access                  */
} CONTROL_Type ;

// #################################################################################################

typedef	union TER_u {
	struct {
		uint32_t	bit00 : 1 ;
		uint32_t	bit01 : 1 ;
		uint32_t	bit02 : 1 ;
		uint32_t	bit03 : 1 ;
		uint32_t	bit04 : 1 ;
		uint32_t	bit05 : 1 ;
		uint32_t	bit06 : 1 ;
		uint32_t	bit07 : 1 ;
		uint32_t	bit08 : 1 ;
		uint32_t	bit09 : 1 ;
		uint32_t	bit10 : 1 ;
		uint32_t	bit11 : 1 ;
		uint32_t	bit12 : 1 ;
		uint32_t	bit13 : 1 ;
		uint32_t	bit14 : 1 ;
		uint32_t	bit15 : 1 ;
		uint32_t	bit16 : 1 ;
		uint32_t	bit17 : 1 ;
		uint32_t	bit18 : 1 ;
		uint32_t	bit19 : 1 ;
		uint32_t	bit20 : 1 ;
		uint32_t	bit21 : 1 ;
		uint32_t	bit22 : 1 ;
		uint32_t	bit23 : 1 ;
		uint32_t	bit24 : 1 ;
		uint32_t	bit25 : 1 ;
		uint32_t	bit26 : 1 ;
		uint32_t	bit27 : 1 ;
		uint32_t	bit28 : 1 ;
		uint32_t	bit29 : 1 ;
		uint32_t	bit30 : 1 ;
		uint32_t	bit31 : 1 ;
	} ;
	uint32_t	w ;
} TER_t ;

typedef	union TCR_u {
	struct {
		uint32_t	ITMENA		: 1 ;
		uint32_t	TSENA		: 1 ;
		uint32_t	SYNCENA		: 1 ;
		uint32_t	DWTENA		: 1 ;
		uint32_t	SWOENA		: 1 ;
		uint32_t	RESERVED0	: 3 ;
		uint32_t	TSPrescale	: 2 ;
		uint32_t	GTSFREQ 	: 2 ;
		uint32_t	RESERVED1	: 4 ;
		uint32_t	TraceBusID	: 7 ;
		uint32_t	BUSY 		: 1 ;
		uint32_t	RESERVED2	: 8 ;
	} ;
	uint32_t	w ;
} TCR_t ;

typedef struct ITM_s {
  __O  union {
    __O  uint8_t    u8;                  /*!< Offset: 0x000 ( /W)  ITM Stimulus Port 8-bit                   */
    __O  uint16_t   u16;                 /*!< Offset: 0x000 ( /W)  ITM Stimulus Port 16-bit                  */
    __O  uint32_t   u32;                 /*!< Offset: 0x000 ( /W)  ITM Stimulus Port 32-bit                  */
  }  PORT [32];                          /*!< Offset: 0x000 ( /W)  ITM Stimulus Port Registers               */
       uint32_t RESERVED0[864];
  __IO TER_t	TER;                     /*!< Offset: 0xE00 (R/W)  ITM Trace Enable Register                 */
       uint32_t RESERVED1[15];
  __IO uint32_t TPR;                     /*!< Offset: 0xE40 (R/W)  ITM Trace Privilege Register              */
       uint32_t RESERVED2[15];
  __IO TCR_t	TCR;                     /*!< Offset: 0xE80 (R/W)  ITM Trace Control Register                */
       uint32_t RESERVED3[29];
  __O  uint32_t IWR;                     /*!< Offset: 0xEF8 ( /W)  ITM Integration Write Register            */
  __I  uint32_t IRR;                     /*!< Offset: 0xEFC (R/ )  ITM Integration Read Register             */
  __IO uint32_t IMCR;                    /*!< Offset: 0xF00 (R/W)  ITM Integration Mode Control Register     */
       uint32_t RESERVED4[43];
  __O  uint32_t LAR;                     /*!< Offset: 0xFB0 ( /W)  ITM Lock Access Register                  */
  __I  uint32_t LSR;                     /*!< Offset: 0xFB4 (R/ )  ITM Lock Status Register                  */
       uint32_t RESERVED5[6];
  __I  uint32_t PID4;                    /*!< Offset: 0xFD0 (R/ )  ITM Peripheral Identification Register #4 */
  __I  uint32_t PID5;                    /*!< Offset: 0xFD4 (R/ )  ITM Peripheral Identification Register #5 */
  __I  uint32_t PID6;                    /*!< Offset: 0xFD8 (R/ )  ITM Peripheral Identification Register #6 */
  __I  uint32_t PID7;                    /*!< Offset: 0xFDC (R/ )  ITM Peripheral Identification Register #7 */
  __I  uint32_t PID0;                    /*!< Offset: 0xFE0 (R/ )  ITM Peripheral Identification Register #0 */
  __I  uint32_t PID1;                    /*!< Offset: 0xFE4 (R/ )  ITM Peripheral Identification Register #1 */
  __I  uint32_t PID2;                    /*!< Offset: 0xFE8 (R/ )  ITM Peripheral Identification Register #2 */
  __I  uint32_t PID3;                    /*!< Offset: 0xFEC (R/ )  ITM Peripheral Identification Register #3 */
  __I  uint32_t CID0;                    /*!< Offset: 0xFF0 (R/ )  ITM Component  Identification Register #0 */
  __I  uint32_t CID1;                    /*!< Offset: 0xFF4 (R/ )  ITM Component  Identification Register #1 */
  __I  uint32_t CID2;                    /*!< Offset: 0xFF8 (R/ )  ITM Component  Identification Register #2 */
  __I  uint32_t CID3;                    /*!< Offset: 0xFFC (R/ )  ITM Component  Identification Register #3 */
} ITM_Type ;

#define ITM_BASE            				0xE0000000									/*!< ITM Base Address                   */
#define ITM                 				((ITM_Type * ) ITM_BASE)					/*!< ITM configuration struct           */

/* ITM Trace Privilege Register Definitions */
#define ITM_TPR_PRIVMASK_Pos                0                                             /*!< ITM TPR: PRIVMASK Position */
#define ITM_TPR_PRIVMASK_Msk               (0xFUL << ITM_TPR_PRIVMASK_Pos)                /*!< ITM TPR: PRIVMASK Mask */

/* ITM Integration Write Register Definitions */
#define ITM_IWR_ATVALIDM_Pos                0                                             /*!< ITM IWR: ATVALIDM Position */
#define ITM_IWR_ATVALIDM_Msk               (1UL << ITM_IWR_ATVALIDM_Pos)                  /*!< ITM IWR: ATVALIDM Mask */

/* ITM Integration Read Register Definitions */
#define ITM_IRR_ATREADYM_Pos                0                                             /*!< ITM IRR: ATREADYM Position */
#define ITM_IRR_ATREADYM_Msk               (1UL << ITM_IRR_ATREADYM_Pos)                  /*!< ITM IRR: ATREADYM Mask */

/* ITM Integration Mode Control Register Definitions */
#define ITM_IMCR_INTEGRATION_Pos            0                                             /*!< ITM IMCR: INTEGRATION Position */
#define ITM_IMCR_INTEGRATION_Msk           (1UL << ITM_IMCR_INTEGRATION_Pos)              /*!< ITM IMCR: INTEGRATION Mask */

/* ITM Lock Status Register Definitions */
#define ITM_LSR_ByteAcc_Pos                 2                                             /*!< ITM LSR: ByteAcc Position */
#define ITM_LSR_ByteAcc_Msk                (1UL << ITM_LSR_ByteAcc_Pos)                   /*!< ITM LSR: ByteAcc Mask */

#define ITM_LSR_Access_Pos                  1                                             /*!< ITM LSR: Access Position */
#define ITM_LSR_Access_Msk                 (1UL << ITM_LSR_Access_Pos)                    /*!< ITM LSR: Access Mask */

#define ITM_LSR_Present_Pos                 0                                             /*!< ITM LSR: Present Position */
#define ITM_LSR_Present_Msk                (1UL << ITM_LSR_Present_Pos)                   /*!< ITM LSR: Present Mask */

// #################################################################################################

typedef	union STIR_u {
	struct {
		uint32_t	INTID			: 9 ;					// Interrupt ID
		uint32_t	Reserved		: 23 ;
	} ;
	uint32_t	w ;
} STIR_t ;

typedef struct NVIC_s {
  __IO uint32_t ISER[8];                 /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register           */
       uint32_t RESERVED0[24];
  __IO uint32_t ICER[8];                 /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register         */
       uint32_t RSERVED1[24];
  __IO uint32_t ISPR[8];                 /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register          */
       uint32_t RESERVED2[24];
  __IO uint32_t ICPR[8];                 /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register        */
       uint32_t RESERVED3[24];
  __IO uint32_t IABR[8];                 /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register           */
       uint32_t RESERVED4[56];
  __IO uint8_t  IP[240];                 /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
       uint32_t RESERVED5[644];
  __O  STIR_t	STIR;                    /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register     */
}  NVIC_Type;

#define NVIC_BASE           				0xE000E100			             			/*!< NVIC Base Address                  */
#define NVIC                				((NVIC_Type * ) NVIC_BASE )					/*!< NVIC configuration struct          */

// #################################################################################################

typedef	union CPUID_u {
	struct {
		uint32_t	REVISION 	: 4 ;					//
		uint32_t	PARTNO		: 12 ;					//
		uint32_t	CONSTANT	: 4 ;					// Architecture
		uint32_t	VARIANT		: 4 ;					//
		uint32_t	IMPLEMENTER	: 8 ;					//
	} ;
	uint32_t	w ;
} CPUID_t ;

typedef	union ICSR_u {
	struct {
		uint32_t	VECTACTIVE 	: 10 ;					// current running ISR#
		uint32_t	RETTOBASE	: 1 ;					// 1=running exception handler
		uint32_t	VECTPENDING	: 10 ;					// pending ISR#
		uint32_t	ISRPENDING	: 1 ;					// 1=external interrupt pending
		uint32_t	ISRPREEMPT	: 1 ;					// 1=pending ISR will be active next step
		uint32_t	PENDSTCLR	: 1 ;					// set to 1 to clear SYSTICK
		uint32_t	PENDSTSET	: 1 ;
		uint32_t	PENDSVCLR	: 1 ;
		uint32_t	PENDSVSET	: 1 ;
		uint32_t	RESERVED1	: 2 ;
		uint32_t	NMIPENDSET	: 1 ;
	} ;
	uint32_t	w ;
} ICSR_t ;

typedef	union VTOR_u {
	struct {
		uint32_t	Reserved1		: 1 ;
		uint32_t	SLEEPONEXIT		: 1 ;					//
		uint32_t	SLEEPDEEP		: 1 ;					//
		uint32_t	Reserved2		: 1 ;
		uint32_t	SEVONPEND		: 1 ;					//
		uint32_t	Reserved3		: 2 ;
		uint32_t	TBLOFF			: 25 ;
	} ;
	uint32_t	w ;
} VTOR_t ;

typedef	union AIRCR_u {
	struct {
		uint32_t	VECTRESET		: 1 ;				// Reset processor logic excl DEBUG & EXTERNAL circuits
		uint32_t	VECTCLRACTIVE	: 1 ;				// Clear all active exception state info
		uint32_t	SYSRESETREQ		: 1 ;				// Request to generate a reset
		uint32_t	RESERVED0		: 5 ;
		uint32_t	PRIGROUP		: 3 ;				// Priority group
		uint32_t	RESERVED1		: 4 ;
		uint32_t	ENDIANNESS		: 1 ;				// 1 = Big, 0 = Little endian
		uint32_t	VECTKEY			: 16 ;				// Write 0x05FA to access, read as 0xFA05
	} ;
	uint32_t	w ;
} AIRCR_t ;

#define SCB_AIRCR_VECTKEY_Pos              16                                             /*!< SCB AIRCR: VECTKEY Position */
#define SCB_AIRCR_VECTKEY_Msk              (0xFFFFUL << SCB_AIRCR_VECTKEY_Pos)            /*!< SCB AIRCR: VECTKEY Mask */

#define SCB_AIRCR_VECTKEYSTAT_Pos          16                                             /*!< SCB AIRCR: VECTKEYSTAT Position */
#define SCB_AIRCR_VECTKEYSTAT_Msk          (0xFFFFUL << SCB_AIRCR_VECTKEYSTAT_Pos)        /*!< SCB AIRCR: VECTKEYSTAT Mask */

#define SCB_AIRCR_ENDIANESS_Pos            15                                             /*!< SCB AIRCR: ENDIANESS Position */
#define SCB_AIRCR_ENDIANESS_Msk            (1UL << SCB_AIRCR_ENDIANESS_Pos)               /*!< SCB AIRCR: ENDIANESS Mask */

#define SCB_AIRCR_PRIGROUP_Pos              8                                             /*!< SCB AIRCR: PRIGROUP Position */
#define SCB_AIRCR_PRIGROUP_Msk             (7UL << SCB_AIRCR_PRIGROUP_Pos)                /*!< SCB AIRCR: PRIGROUP Mask */

#define SCB_AIRCR_SYSRESETREQ_Pos           2                                             /*!< SCB AIRCR: SYSRESETREQ Position */
#define SCB_AIRCR_SYSRESETREQ_Msk          (1UL << SCB_AIRCR_SYSRESETREQ_Pos)             /*!< SCB AIRCR: SYSRESETREQ Mask */

#define SCB_AIRCR_VECTCLRACTIVE_Pos         1                                             /*!< SCB AIRCR: VECTCLRACTIVE Position */
#define SCB_AIRCR_VECTCLRACTIVE_Msk        (1UL << SCB_AIRCR_VECTCLRACTIVE_Pos)           /*!< SCB AIRCR: VECTCLRACTIVE Mask */

#define SCB_AIRCR_VECTRESET_Pos             0                                             /*!< SCB AIRCR: VECTRESET Position */
#define SCB_AIRCR_VECTRESET_Msk            (1UL /*<< SCB_AIRCR_VECTRESET_Pos*/)           /*!< SCB AIRCR: VECTRESET Mask */

typedef	union CCR_u {
	struct {
		uint32_t	NONBASETHRENA: 1 ;					// non base trhead enable
		uint32_t	USERSETMPEND: 1 ;					// User code Software Trigger Interrupt write enable
		uint32_t	RESERVED1	: 1 ;
		uint32_t	UNALIGN_TRP	: 1 ;					// Trap on unaligned access
		uint32_t	DIV_0_TRP	: 1 ;					// Trap on Divide by zero
		uint32_t	RESERVED2	: 3 ;
		uint32_t	BFHFNMIGN	: 1 ;					// Ignore Bus fault duringHard & NMI handlers
		uint32_t	STKALIGN	: 1 ;					// Force double word (8 byte) stack alignment
		uint32_t	RESERVED3 	: 22 ;
	} ;
	uint32_t	w ;
} CCR_t ;

typedef	union SHCSR_u {
	struct {
		uint32_t	MEMFAULTACT	: 1 ;					//
		uint32_t	BUSFAULTACT	: 1 ;					//
		uint32_t	RESERVED1	: 1 ;
		uint32_t	USGFAULTACT	: 1 ;					//
		uint32_t	RESERVED2	: 3 ;
		uint32_t	SVCALLACT	: 1 ;					//
		uint32_t	MONITORACT	: 1 ;					//
		uint32_t	RESERVED3	: 1 ;
		uint32_t	PENDSVACT	: 1 ;					//
		uint32_t	SYSTICKACT	: 1 ;					//
		uint32_t	USGFAULTPENDED	: 1 ;				//
		uint32_t	MEMFAULTPENDED	: 1 ;				//
		uint32_t	BUSFAULTPENDED	: 1 ;				//
		uint32_t	SVCALLPENDED	: 1 ;				//
		uint32_t	MEMFAULTENA	: 1 ;					//
		uint32_t	BUSFAULTENA	: 1 ;					//
		uint32_t	USGFAULTENA	: 1 ;					//
		uint32_t	RESERVED4	: 13 ;
	} ;
	uint32_t	w ;
} SHCSR_t ;

typedef	union CFSR_u {
	struct {
		uint32_t	MMFSR		: 8 ;			// MemManageFault
		uint32_t	BFSR 		: 8 ;			// BusFault
		uint32_t	UFSR 		: 16 ;			// UsageFault
	} ;
	struct {
		uint32_t	IACCVIOL	: 1 ;			//
		uint32_t	DACCVIOL	: 1 ;			//
		uint32_t	RESERVED1	: 1 ;
		uint32_t	MUNSTKERR	: 1 ;			//
		uint32_t	MSTKERR		: 1 ;			//
		uint32_t	MLSPERR		: 1 ;			//
		uint32_t	RESERVED2	: 1 ;
		uint32_t	MMARVALID	: 1 ;			//
	// end of MMFSR
		uint32_t	IBUSERR		: 1 ;			//
		uint32_t	PRECISERR	: 1 ;			//
		uint32_t	IMPRECISERR	: 1 ;			//
		uint32_t	UNSTKERR	: 1 ;			//
		uint32_t	STKERR		: 1 ;			//
		uint32_t	LSPERR		: 1 ;			//
		uint32_t	RESERVED3	: 1 ;
		uint32_t	BFARVALID	: 1 ;			//
	// end of BFSR
		uint32_t	UNDEFINSTR	: 1 ;			//
		uint32_t	INVSTATE	: 1 ;			//
		uint32_t	INVPC		: 1 ;			//
		uint32_t	NOCP		: 1 ;			//
		uint32_t	RESERVED4	: 4 ;
		uint32_t	UNALIGNED	: 1 ;			//
		uint32_t	DIVBYZERO	: 1 ;			//
		uint32_t	RESERVED5	: 6 ;
	} ;
	uint32_t	w ;
} CFSR_t ;

typedef	union HFSR_u {
	struct {
		uint32_t	RESERVED1 	: 1 ;					//
		uint32_t	VECTBL  	: 1 ;					// ERROR CAUSED BY VECTOR TABLE FETCH
		uint32_t	RESERVED2 	: 28 ;					//
		uint32_t	FORCED  	: 1 ;					// caused by BUS, MEM or USAGE fault
		uint32_t	DEBUGEVT  	: 1 ;					// caused by DEBUG event
	} ;
	uint32_t	w ;
} HFSR_t ;

typedef	union DFSR_u {
	struct {
		uint32_t	HALTED		: 1 ;					//
		uint32_t	BKPT		: 1 ;					//
		uint32_t	DWTTRAP		: 1 ;					//
		uint32_t	VCATCH		: 1 ;					//
		uint32_t	EXTERNAL 	: 1 ;					//
		uint32_t	RESERVED	: 27 ;
	} ;
	uint32_t	w ;
} DFSR_t ;

typedef struct SCB_s {
  __I  CPUID_t	CPUID;                   /*!< Offset: 0x000 (R/ )  CPUID Base Register                                   */
  __IO ICSR_t	ICSR;                    /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register                  */
  __IO VTOR_t	VTOR;                    /*!< Offset: 0x008 (R/W)  Vector Table Offset Register                          */
  __IO AIRCR_t	AIRCR;                   /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register      */
  __IO uint32_t SCR;                     /*!< Offset: 0x010 (R/W)  System Control Register                               */
  __IO CCR_t	CCR;                     /*!< Offset: 0x014 (R/W)  Configuration Control Register                        */
  __IO uint8_t  SHP[12];                 /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
  __IO SHCSR_t	SHCSR;                   /*!< Offset: 0x024 (R/W)  System Handler Control and State Register             */
  __IO CFSR_t	CFSR;                    /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register                    */
  __IO HFSR_t	HFSR;                    /*!< Offset: 0x02C (R/W)  HardFault Status Register                             */
  __IO DFSR_t	DFSR;                    /*!< Offset: 0x030 (R/W)  Debug Fault Status Register                           */
  __IO uint32_t MMFAR;                   /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register                      */
  __IO uint32_t BFAR;                    /*!< Offset: 0x038 (R/W)  BusFault Address Register                             */
  __IO uint32_t AFSR;                    /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register                       */
  __I  uint32_t PFR[2];                  /*!< Offset: 0x040 (R/ )  Processor Feature Register                            */
  __I  uint32_t DFR;                     /*!< Offset: 0x048 (R/ )  Debug Feature Register                                */
  __I  uint32_t ADR;                     /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register                            */
  __I  uint32_t MMFR[4];                 /*!< Offset: 0x050 (R/ )  Memory Model Feature Register                         */
  __I  uint32_t ISAR[5];                 /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register                   */
       uint32_t RESERVED0[5];
  __IO uint32_t CPACR;                   /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register                   */
} SCB_Type;

#define SCB_BASE        				    (0xE000ED00)
#define SCB                 				((SCB_Type *) SCB_BASE)

// #################################################################################################

typedef	union ICTR_u {
	struct {
		uint32_t	INTLINESNUM		: 4 ;					//
		uint32_t	Reserved		: 28 ;
	} ;
	uint32_t	w ;
} ICTR_t ;

typedef	union ACTLR_u {
	struct {
		uint32_t	DISMCYCINT		: 1 ;					//
		uint32_t	DISDEFWBUF		: 1 ;					//
		uint32_t	DISFOLD			: 1 ;					//
		uint32_t	Reserved1		: 5 ;					//
		uint32_t	DISFPCA			: 1 ;					//
		uint32_t	DISOOFP			: 1 ;					//
		uint32_t	Reserved2		: 22 ;					//
	} ;
	uint32_t	w ;
} ACTLR_t ;

typedef struct SCnSCB_s {
       uint32_t RESERVED0[1];
  __I  ICTR_t	ICTR;                    /*!< Offset: 0x004 (R/ )  Interrupt Controller Type Register      */
  __IO ACTLR_t	ACTLR;                   /*!< Offset: 0x008 (R/W)  Auxiliary Control Register              */
} SCnSCB_Type ;

#define SCS_BASE            				(0xE000E000)                            /*!< System Control Space Base Address  */
#define SCnSCB              				((SCnSCB_Type    *)     SCS_BASE      )   /*!< System control Register not in SCB */

// #################################################################################################

typedef	union CTRL_u {
	struct {
		uint32_t	ENABLE			: 1 ;					//
		uint32_t	TICKINT			: 1 ;					//
		uint32_t	CLKSOURCE		: 1 ;					//
		uint32_t	Reserved1		: 13 ;
		uint32_t	COUNTFLAG		: 1 ;					//
		uint32_t	Reserved2		: 15 ;
	} ;
	uint32_t	w ;
} CTRL_t ;

typedef	union LOAD_u {
	struct {
		uint32_t	RELOAD			: 24 ;					//
		uint32_t	Reserved		: 8 ;
	} ;
	uint32_t	w ;
} LOAD_t ;

typedef	union VAL_u {
	struct {
		uint32_t	CURRENT			: 24 ;					//
		uint32_t	Reserved		: 8 ;
	} ;
	uint32_t	w ;
} VAL_t ;

typedef	union CALIB_u {
	struct {
		uint32_t	TENMS			: 24 ;					//
		uint32_t	Reserved		: 6 ;
		uint32_t	SKEW			: 1 ;					//
		uint32_t	NOREF			: 1 ;					//
	} ;
	uint32_t	w ;
} CALIB_t ;

typedef struct SysTick_s {
  __IO CTRL_t	CTRL;                    /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
  __IO LOAD_t	LOAD;                    /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register       */
  __IO VAL_t	VAL;                     /*!< Offset: 0x008 (R/W)  SysTick Current Value Register      */
  __I  CALIB_t	CALIB;                   /*!< Offset: 0x00C (R/ )  SysTick Calibration Register        */
} SysTick_Type ;

#define SysTick_BASE        				(0xE000E010)
#define SysTick             				((SysTick_Type * ) SysTick_BASE)

// #################################################################################################

typedef struct DWT_s {
  __IO uint32_t CTRL;                    /*!< Offset: 0x000 (R/W)  Control Register                          */
  __IO uint32_t CYCCNT;                  /*!< Offset: 0x004 (R/W)  Cycle Count Register                      */
  __IO uint32_t CPICNT;                  /*!< Offset: 0x008 (R/W)  CPI Count Register                        */
  __IO uint32_t EXCCNT;                  /*!< Offset: 0x00C (R/W)  Exception Overhead Count Register         */
  __IO uint32_t SLEEPCNT;                /*!< Offset: 0x010 (R/W)  Sleep Count Register                      */
  __IO uint32_t LSUCNT;                  /*!< Offset: 0x014 (R/W)  LSU Count Register                        */
  __IO uint32_t FOLDCNT;                 /*!< Offset: 0x018 (R/W)  Folded-instruction Count Register         */
  __I  uint32_t PCSR;                    /*!< Offset: 0x01C (R/ )  Program Counter Sample Register           */
  __IO uint32_t COMP0;                   /*!< Offset: 0x020 (R/W)  Comparator Register 0                     */
  __IO uint32_t MASK0;                   /*!< Offset: 0x024 (R/W)  Mask Register 0                           */
  __IO uint32_t FUNCTION0;               /*!< Offset: 0x028 (R/W)  Function Register 0                       */
       uint32_t RESERVED0[1];
  __IO uint32_t COMP1;                   /*!< Offset: 0x030 (R/W)  Comparator Register 1                     */
  __IO uint32_t MASK1;                   /*!< Offset: 0x034 (R/W)  Mask Register 1                           */
  __IO uint32_t FUNCTION1;               /*!< Offset: 0x038 (R/W)  Function Register 1                       */
       uint32_t RESERVED1[1];
  __IO uint32_t COMP2;                   /*!< Offset: 0x040 (R/W)  Comparator Register 2                     */
  __IO uint32_t MASK2;                   /*!< Offset: 0x044 (R/W)  Mask Register 2                           */
  __IO uint32_t FUNCTION2;               /*!< Offset: 0x048 (R/W)  Function Register 2                       */
       uint32_t RESERVED2[1];
  __IO uint32_t COMP3;                   /*!< Offset: 0x050 (R/W)  Comparator Register 3                     */
  __IO uint32_t MASK3;                   /*!< Offset: 0x054 (R/W)  Mask Register 3                           */
  __IO uint32_t FUNCTION3;               /*!< Offset: 0x058 (R/W)  Function Register 3                       */
} DWT_Type ;

#define DWT_BASE            				(0xE0001000)                            /*!< DWT Base Address                   */
#define DWT                 				((DWT_Type *) DWT_BASE)					/*!< DWT configuration struct           */

/* DWT Control Register Definitions */
#define DWT_CTRL_NUMCOMP_Pos               28                                          /*!< DWT CTRL: NUMCOMP Position */
#define DWT_CTRL_NUMCOMP_Msk               (0xFUL << DWT_CTRL_NUMCOMP_Pos)             /*!< DWT CTRL: NUMCOMP Mask */

#define DWT_CTRL_NOTRCPKT_Pos              27                                          /*!< DWT CTRL: NOTRCPKT Position */
#define DWT_CTRL_NOTRCPKT_Msk              (0x1UL << DWT_CTRL_NOTRCPKT_Pos)            /*!< DWT CTRL: NOTRCPKT Mask */

#define DWT_CTRL_NOEXTTRIG_Pos             26                                          /*!< DWT CTRL: NOEXTTRIG Position */
#define DWT_CTRL_NOEXTTRIG_Msk             (0x1UL << DWT_CTRL_NOEXTTRIG_Pos)           /*!< DWT CTRL: NOEXTTRIG Mask */

#define DWT_CTRL_NOCYCCNT_Pos              25                                          /*!< DWT CTRL: NOCYCCNT Position */
#define DWT_CTRL_NOCYCCNT_Msk              (0x1UL << DWT_CTRL_NOCYCCNT_Pos)            /*!< DWT CTRL: NOCYCCNT Mask */

#define DWT_CTRL_NOPRFCNT_Pos              24                                          /*!< DWT CTRL: NOPRFCNT Position */
#define DWT_CTRL_NOPRFCNT_Msk              (0x1UL << DWT_CTRL_NOPRFCNT_Pos)            /*!< DWT CTRL: NOPRFCNT Mask */

#define DWT_CTRL_CYCEVTENA_Pos             22                                          /*!< DWT CTRL: CYCEVTENA Position */
#define DWT_CTRL_CYCEVTENA_Msk             (0x1UL << DWT_CTRL_CYCEVTENA_Pos)           /*!< DWT CTRL: CYCEVTENA Mask */

#define DWT_CTRL_FOLDEVTENA_Pos            21                                          /*!< DWT CTRL: FOLDEVTENA Position */
#define DWT_CTRL_FOLDEVTENA_Msk            (0x1UL << DWT_CTRL_FOLDEVTENA_Pos)          /*!< DWT CTRL: FOLDEVTENA Mask */

#define DWT_CTRL_LSUEVTENA_Pos             20                                          /*!< DWT CTRL: LSUEVTENA Position */
#define DWT_CTRL_LSUEVTENA_Msk             (0x1UL << DWT_CTRL_LSUEVTENA_Pos)           /*!< DWT CTRL: LSUEVTENA Mask */

#define DWT_CTRL_SLEEPEVTENA_Pos           19                                          /*!< DWT CTRL: SLEEPEVTENA Position */
#define DWT_CTRL_SLEEPEVTENA_Msk           (0x1UL << DWT_CTRL_SLEEPEVTENA_Pos)         /*!< DWT CTRL: SLEEPEVTENA Mask */

#define DWT_CTRL_EXCEVTENA_Pos             18                                          /*!< DWT CTRL: EXCEVTENA Position */
#define DWT_CTRL_EXCEVTENA_Msk             (0x1UL << DWT_CTRL_EXCEVTENA_Pos)           /*!< DWT CTRL: EXCEVTENA Mask */

#define DWT_CTRL_CPIEVTENA_Pos             17                                          /*!< DWT CTRL: CPIEVTENA Position */
#define DWT_CTRL_CPIEVTENA_Msk             (0x1UL << DWT_CTRL_CPIEVTENA_Pos)           /*!< DWT CTRL: CPIEVTENA Mask */

#define DWT_CTRL_EXCTRCENA_Pos             16                                          /*!< DWT CTRL: EXCTRCENA Position */
#define DWT_CTRL_EXCTRCENA_Msk             (0x1UL << DWT_CTRL_EXCTRCENA_Pos)           /*!< DWT CTRL: EXCTRCENA Mask */

#define DWT_CTRL_PCSAMPLENA_Pos            12                                          /*!< DWT CTRL: PCSAMPLENA Position */
#define DWT_CTRL_PCSAMPLENA_Msk            (0x1UL << DWT_CTRL_PCSAMPLENA_Pos)          /*!< DWT CTRL: PCSAMPLENA Mask */

#define DWT_CTRL_SYNCTAP_Pos               10                                          /*!< DWT CTRL: SYNCTAP Position */
#define DWT_CTRL_SYNCTAP_Msk               (0x3UL << DWT_CTRL_SYNCTAP_Pos)             /*!< DWT CTRL: SYNCTAP Mask */

#define DWT_CTRL_CYCTAP_Pos                 9                                          /*!< DWT CTRL: CYCTAP Position */
#define DWT_CTRL_CYCTAP_Msk                (0x1UL << DWT_CTRL_CYCTAP_Pos)              /*!< DWT CTRL: CYCTAP Mask */

#define DWT_CTRL_POSTINIT_Pos               5                                          /*!< DWT CTRL: POSTINIT Position */
#define DWT_CTRL_POSTINIT_Msk              (0xFUL << DWT_CTRL_POSTINIT_Pos)            /*!< DWT CTRL: POSTINIT Mask */

#define DWT_CTRL_POSTPRESET_Pos             1                                          /*!< DWT CTRL: POSTPRESET Position */
#define DWT_CTRL_POSTPRESET_Msk            (0xFUL << DWT_CTRL_POSTPRESET_Pos)          /*!< DWT CTRL: POSTPRESET Mask */

#define DWT_CTRL_CYCCNTENA_Pos              0                                          /*!< DWT CTRL: CYCCNTENA Position */
#define DWT_CTRL_CYCCNTENA_Msk             (0x1UL << DWT_CTRL_CYCCNTENA_Pos)           /*!< DWT CTRL: CYCCNTENA Mask */

/* DWT CPI Count Register Definitions */
#define DWT_CPICNT_CPICNT_Pos               0                                          /*!< DWT CPICNT: CPICNT Position */
#define DWT_CPICNT_CPICNT_Msk              (0xFFUL << DWT_CPICNT_CPICNT_Pos)           /*!< DWT CPICNT: CPICNT Mask */

/* DWT Exception Overhead Count Register Definitions */
#define DWT_EXCCNT_EXCCNT_Pos               0                                          /*!< DWT EXCCNT: EXCCNT Position */
#define DWT_EXCCNT_EXCCNT_Msk              (0xFFUL << DWT_EXCCNT_EXCCNT_Pos)           /*!< DWT EXCCNT: EXCCNT Mask */

/* DWT Sleep Count Register Definitions */
#define DWT_SLEEPCNT_SLEEPCNT_Pos           0                                          /*!< DWT SLEEPCNT: SLEEPCNT Position */
#define DWT_SLEEPCNT_SLEEPCNT_Msk          (0xFFUL << DWT_SLEEPCNT_SLEEPCNT_Pos)       /*!< DWT SLEEPCNT: SLEEPCNT Mask */

/* DWT LSU Count Register Definitions */
#define DWT_LSUCNT_LSUCNT_Pos               0                                          /*!< DWT LSUCNT: LSUCNT Position */
#define DWT_LSUCNT_LSUCNT_Msk              (0xFFUL << DWT_LSUCNT_LSUCNT_Pos)           /*!< DWT LSUCNT: LSUCNT Mask */

/* DWT Folded-instruction Count Register Definitions */
#define DWT_FOLDCNT_FOLDCNT_Pos             0                                          /*!< DWT FOLDCNT: FOLDCNT Position */
#define DWT_FOLDCNT_FOLDCNT_Msk            (0xFFUL << DWT_FOLDCNT_FOLDCNT_Pos)         /*!< DWT FOLDCNT: FOLDCNT Mask */

/* DWT Comparator Mask Register Definitions */
#define DWT_MASK_MASK_Pos                   0                                          /*!< DWT MASK: MASK Position */
#define DWT_MASK_MASK_Msk                  (0x1FUL << DWT_MASK_MASK_Pos)               /*!< DWT MASK: MASK Mask */

/* DWT Comparator Function Register Definitions */
#define DWT_FUNCTION_MATCHED_Pos           24                                          /*!< DWT FUNCTION: MATCHED Position */
#define DWT_FUNCTION_MATCHED_Msk           (0x1UL << DWT_FUNCTION_MATCHED_Pos)         /*!< DWT FUNCTION: MATCHED Mask */

#define DWT_FUNCTION_DATAVADDR1_Pos        16                                          /*!< DWT FUNCTION: DATAVADDR1 Position */
#define DWT_FUNCTION_DATAVADDR1_Msk        (0xFUL << DWT_FUNCTION_DATAVADDR1_Pos)      /*!< DWT FUNCTION: DATAVADDR1 Mask */

#define DWT_FUNCTION_DATAVADDR0_Pos        12                                          /*!< DWT FUNCTION: DATAVADDR0 Position */
#define DWT_FUNCTION_DATAVADDR0_Msk        (0xFUL << DWT_FUNCTION_DATAVADDR0_Pos)      /*!< DWT FUNCTION: DATAVADDR0 Mask */

#define DWT_FUNCTION_DATAVSIZE_Pos         10                                          /*!< DWT FUNCTION: DATAVSIZE Position */
#define DWT_FUNCTION_DATAVSIZE_Msk         (0x3UL << DWT_FUNCTION_DATAVSIZE_Pos)       /*!< DWT FUNCTION: DATAVSIZE Mask */

#define DWT_FUNCTION_LNK1ENA_Pos            9                                          /*!< DWT FUNCTION: LNK1ENA Position */
#define DWT_FUNCTION_LNK1ENA_Msk           (0x1UL << DWT_FUNCTION_LNK1ENA_Pos)         /*!< DWT FUNCTION: LNK1ENA Mask */

#define DWT_FUNCTION_DATAVMATCH_Pos         8                                          /*!< DWT FUNCTION: DATAVMATCH Position */
#define DWT_FUNCTION_DATAVMATCH_Msk        (0x1UL << DWT_FUNCTION_DATAVMATCH_Pos)      /*!< DWT FUNCTION: DATAVMATCH Mask */

#define DWT_FUNCTION_CYCMATCH_Pos           7                                          /*!< DWT FUNCTION: CYCMATCH Position */
#define DWT_FUNCTION_CYCMATCH_Msk          (0x1UL << DWT_FUNCTION_CYCMATCH_Pos)        /*!< DWT FUNCTION: CYCMATCH Mask */

#define DWT_FUNCTION_EMITRANGE_Pos          5                                          /*!< DWT FUNCTION: EMITRANGE Position */
#define DWT_FUNCTION_EMITRANGE_Msk         (0x1UL << DWT_FUNCTION_EMITRANGE_Pos)       /*!< DWT FUNCTION: EMITRANGE Mask */

#define DWT_FUNCTION_FUNCTION_Pos           0                                          /*!< DWT FUNCTION: FUNCTION Position */
#define DWT_FUNCTION_FUNCTION_Msk          (0xFUL << DWT_FUNCTION_FUNCTION_Pos)        /*!< DWT FUNCTION: FUNCTION Mask */

// #################################################################################################

typedef struct TPI_s {
  __IO uint32_t SSPSR;                   /*!< Offset: 0x000 (R/ )  Supported Parallel Port Size Register     */
  __IO uint32_t CSPSR;                   /*!< Offset: 0x004 (R/W)  Current Parallel Port Size Register */
       uint32_t RESERVED0[2];
  __IO uint32_t ACPR;                    /*!< Offset: 0x010 (R/W)  Asynchronous Clock Prescaler Register */
       uint32_t RESERVED1[55];
  __IO uint32_t SPPR;                    /*!< Offset: 0x0F0 (R/W)  Selected Pin Protocol Register */
       uint32_t RESERVED2[131];
  __I  uint32_t FFSR;                    /*!< Offset: 0x300 (R/ )  Formatter and Flush Status Register */
  __IO uint32_t FFCR;                    /*!< Offset: 0x304 (R/W)  Formatter and Flush Control Register */
  __I  uint32_t FSCR;                    /*!< Offset: 0x308 (R/ )  Formatter Synchronization Counter Register */
       uint32_t RESERVED3[759];
  __I  uint32_t TRIGGER;                 /*!< Offset: 0xEE8 (R/ )  TRIGGER */
  __I  uint32_t FIFO0;                   /*!< Offset: 0xEEC (R/ )  Integration ETM Data */
  __I  uint32_t ITATBCTR2;               /*!< Offset: 0xEF0 (R/ )  ITATBCTR2 */
       uint32_t RESERVED4[1];
  __I  uint32_t ITATBCTR0;               /*!< Offset: 0xEF8 (R/ )  ITATBCTR0 */
  __I  uint32_t FIFO1;                   /*!< Offset: 0xEFC (R/ )  Integration ITM Data */
  __IO uint32_t ITCTRL;                  /*!< Offset: 0xF00 (R/W)  Integration Mode Control */
       uint32_t RESERVED5[39];
  __IO uint32_t CLAIMSET;                /*!< Offset: 0xFA0 (R/W)  Claim tag set */
  __IO uint32_t CLAIMCLR;                /*!< Offset: 0xFA4 (R/W)  Claim tag clear */
       uint32_t RESERVED7[8];
  __I  uint32_t DEVID;                   /*!< Offset: 0xFC8 (R/ )  TPIU_DEVID */
  __I  uint32_t DEVTYPE;                 /*!< Offset: 0xFCC (R/ )  TPIU_DEVTYPE */
} TPI_Type;

#define TPI_BASE            				(0xE0040000)								/*!< TPI Base Address                   */
#define TPI                 				((TPI_Type *) TPI_BASE)   					/*!< TPI configuration struct           */

/* TPI Asynchronous Clock Prescaler Register Definitions */
#define TPI_ACPR_PRESCALER_Pos              0                                          /*!< TPI ACPR: PRESCALER Position */
#define TPI_ACPR_PRESCALER_Msk             (0x1FFFUL << TPI_ACPR_PRESCALER_Pos)        /*!< TPI ACPR: PRESCALER Mask */

/* TPI Selected Pin Protocol Register Definitions */
#define TPI_SPPR_TXMODE_Pos                 0                                          /*!< TPI SPPR: TXMODE Position */
#define TPI_SPPR_TXMODE_Msk                (0x3UL << TPI_SPPR_TXMODE_Pos)              /*!< TPI SPPR: TXMODE Mask */

/* TPI Formatter and Flush Status Register Definitions */
#define TPI_FFSR_FtNonStop_Pos              3                                          /*!< TPI FFSR: FtNonStop Position */
#define TPI_FFSR_FtNonStop_Msk             (0x1UL << TPI_FFSR_FtNonStop_Pos)           /*!< TPI FFSR: FtNonStop Mask */

#define TPI_FFSR_TCPresent_Pos              2                                          /*!< TPI FFSR: TCPresent Position */
#define TPI_FFSR_TCPresent_Msk             (0x1UL << TPI_FFSR_TCPresent_Pos)           /*!< TPI FFSR: TCPresent Mask */

#define TPI_FFSR_FtStopped_Pos              1                                          /*!< TPI FFSR: FtStopped Position */
#define TPI_FFSR_FtStopped_Msk             (0x1UL << TPI_FFSR_FtStopped_Pos)           /*!< TPI FFSR: FtStopped Mask */

#define TPI_FFSR_FlInProg_Pos               0                                          /*!< TPI FFSR: FlInProg Position */
#define TPI_FFSR_FlInProg_Msk              (0x1UL << TPI_FFSR_FlInProg_Pos)            /*!< TPI FFSR: FlInProg Mask */

/* TPI Formatter and Flush Control Register Definitions */
#define TPI_FFCR_TrigIn_Pos                 8                                          /*!< TPI FFCR: TrigIn Position */
#define TPI_FFCR_TrigIn_Msk                (0x1UL << TPI_FFCR_TrigIn_Pos)              /*!< TPI FFCR: TrigIn Mask */

#define TPI_FFCR_EnFCont_Pos                1                                          /*!< TPI FFCR: EnFCont Position */
#define TPI_FFCR_EnFCont_Msk               (0x1UL << TPI_FFCR_EnFCont_Pos)             /*!< TPI FFCR: EnFCont Mask */

/* TPI TRIGGER Register Definitions */
#define TPI_TRIGGER_TRIGGER_Pos             0                                          /*!< TPI TRIGGER: TRIGGER Position */
#define TPI_TRIGGER_TRIGGER_Msk            (0x1UL << TPI_TRIGGER_TRIGGER_Pos)          /*!< TPI TRIGGER: TRIGGER Mask */

/* TPI Integration ETM Data Register Definitions (FIFO0) */
#define TPI_FIFO0_ITM_ATVALID_Pos          29                                          /*!< TPI FIFO0: ITM_ATVALID Position */
#define TPI_FIFO0_ITM_ATVALID_Msk          (0x3UL << TPI_FIFO0_ITM_ATVALID_Pos)        /*!< TPI FIFO0: ITM_ATVALID Mask */

#define TPI_FIFO0_ITM_bytecount_Pos        27                                          /*!< TPI FIFO0: ITM_bytecount Position */
#define TPI_FIFO0_ITM_bytecount_Msk        (0x3UL << TPI_FIFO0_ITM_bytecount_Pos)      /*!< TPI FIFO0: ITM_bytecount Mask */

#define TPI_FIFO0_ETM_ATVALID_Pos          26                                          /*!< TPI FIFO0: ETM_ATVALID Position */
#define TPI_FIFO0_ETM_ATVALID_Msk          (0x3UL << TPI_FIFO0_ETM_ATVALID_Pos)        /*!< TPI FIFO0: ETM_ATVALID Mask */

#define TPI_FIFO0_ETM_bytecount_Pos        24                                          /*!< TPI FIFO0: ETM_bytecount Position */
#define TPI_FIFO0_ETM_bytecount_Msk        (0x3UL << TPI_FIFO0_ETM_bytecount_Pos)      /*!< TPI FIFO0: ETM_bytecount Mask */

#define TPI_FIFO0_ETM2_Pos                 16                                          /*!< TPI FIFO0: ETM2 Position */
#define TPI_FIFO0_ETM2_Msk                 (0xFFUL << TPI_FIFO0_ETM2_Pos)              /*!< TPI FIFO0: ETM2 Mask */

#define TPI_FIFO0_ETM1_Pos                  8                                          /*!< TPI FIFO0: ETM1 Position */
#define TPI_FIFO0_ETM1_Msk                 (0xFFUL << TPI_FIFO0_ETM1_Pos)              /*!< TPI FIFO0: ETM1 Mask */

#define TPI_FIFO0_ETM0_Pos                  0                                          /*!< TPI FIFO0: ETM0 Position */
#define TPI_FIFO0_ETM0_Msk                 (0xFFUL << TPI_FIFO0_ETM0_Pos)              /*!< TPI FIFO0: ETM0 Mask */

/* TPI ITATBCTR2 Register Definitions */
#define TPI_ITATBCTR2_ATREADY_Pos           0                                          /*!< TPI ITATBCTR2: ATREADY Position */
#define TPI_ITATBCTR2_ATREADY_Msk          (0x1UL << TPI_ITATBCTR2_ATREADY_Pos)        /*!< TPI ITATBCTR2: ATREADY Mask */

/* TPI Integration ITM Data Register Definitions (FIFO1) */
#define TPI_FIFO1_ITM_ATVALID_Pos          29                                          /*!< TPI FIFO1: ITM_ATVALID Position */
#define TPI_FIFO1_ITM_ATVALID_Msk          (0x3UL << TPI_FIFO1_ITM_ATVALID_Pos)        /*!< TPI FIFO1: ITM_ATVALID Mask */

#define TPI_FIFO1_ITM_bytecount_Pos        27                                          /*!< TPI FIFO1: ITM_bytecount Position */
#define TPI_FIFO1_ITM_bytecount_Msk        (0x3UL << TPI_FIFO1_ITM_bytecount_Pos)      /*!< TPI FIFO1: ITM_bytecount Mask */

#define TPI_FIFO1_ETM_ATVALID_Pos          26                                          /*!< TPI FIFO1: ETM_ATVALID Position */
#define TPI_FIFO1_ETM_ATVALID_Msk          (0x3UL << TPI_FIFO1_ETM_ATVALID_Pos)        /*!< TPI FIFO1: ETM_ATVALID Mask */

#define TPI_FIFO1_ETM_bytecount_Pos        24                                          /*!< TPI FIFO1: ETM_bytecount Position */
#define TPI_FIFO1_ETM_bytecount_Msk        (0x3UL << TPI_FIFO1_ETM_bytecount_Pos)      /*!< TPI FIFO1: ETM_bytecount Mask */

#define TPI_FIFO1_ITM2_Pos                 16                                          /*!< TPI FIFO1: ITM2 Position */
#define TPI_FIFO1_ITM2_Msk                 (0xFFUL << TPI_FIFO1_ITM2_Pos)              /*!< TPI FIFO1: ITM2 Mask */

#define TPI_FIFO1_ITM1_Pos                  8                                          /*!< TPI FIFO1: ITM1 Position */
#define TPI_FIFO1_ITM1_Msk                 (0xFFUL << TPI_FIFO1_ITM1_Pos)              /*!< TPI FIFO1: ITM1 Mask */

#define TPI_FIFO1_ITM0_Pos                  0                                          /*!< TPI FIFO1: ITM0 Position */
#define TPI_FIFO1_ITM0_Msk                 (0xFFUL << TPI_FIFO1_ITM0_Pos)              /*!< TPI FIFO1: ITM0 Mask */

/* TPI ITATBCTR0 Register Definitions */
#define TPI_ITATBCTR0_ATREADY_Pos           0                                          /*!< TPI ITATBCTR0: ATREADY Position */
#define TPI_ITATBCTR0_ATREADY_Msk          (0x1UL << TPI_ITATBCTR0_ATREADY_Pos)        /*!< TPI ITATBCTR0: ATREADY Mask */

/* TPI Integration Mode Control Register Definitions */
#define TPI_ITCTRL_Mode_Pos                 0                                          /*!< TPI ITCTRL: Mode Position */
#define TPI_ITCTRL_Mode_Msk                (0x1UL << TPI_ITCTRL_Mode_Pos)              /*!< TPI ITCTRL: Mode Mask */

/* TPI DEVID Register Definitions */
#define TPI_DEVID_NRZVALID_Pos             11                                          /*!< TPI DEVID: NRZVALID Position */
#define TPI_DEVID_NRZVALID_Msk             (0x1UL << TPI_DEVID_NRZVALID_Pos)           /*!< TPI DEVID: NRZVALID Mask */

#define TPI_DEVID_MANCVALID_Pos            10                                          /*!< TPI DEVID: MANCVALID Position */
#define TPI_DEVID_MANCVALID_Msk            (0x1UL << TPI_DEVID_MANCVALID_Pos)          /*!< TPI DEVID: MANCVALID Mask */

#define TPI_DEVID_PTINVALID_Pos             9                                          /*!< TPI DEVID: PTINVALID Position */
#define TPI_DEVID_PTINVALID_Msk            (0x1UL << TPI_DEVID_PTINVALID_Pos)          /*!< TPI DEVID: PTINVALID Mask */

#define TPI_DEVID_MinBufSz_Pos              6                                          /*!< TPI DEVID: MinBufSz Position */
#define TPI_DEVID_MinBufSz_Msk             (0x7UL << TPI_DEVID_MinBufSz_Pos)           /*!< TPI DEVID: MinBufSz Mask */

#define TPI_DEVID_AsynClkIn_Pos             5                                          /*!< TPI DEVID: AsynClkIn Position */
#define TPI_DEVID_AsynClkIn_Msk            (0x1UL << TPI_DEVID_AsynClkIn_Pos)          /*!< TPI DEVID: AsynClkIn Mask */

#define TPI_DEVID_NrTraceInput_Pos          0                                          /*!< TPI DEVID: NrTraceInput Position */
#define TPI_DEVID_NrTraceInput_Msk         (0x1FUL << TPI_DEVID_NrTraceInput_Pos)      /*!< TPI DEVID: NrTraceInput Mask */

/* TPI DEVTYPE Register Definitions */
#define TPI_DEVTYPE_SubType_Pos             0                                          /*!< TPI DEVTYPE: SubType Position */
#define TPI_DEVTYPE_SubType_Msk            (0xFUL << TPI_DEVTYPE_SubType_Pos)          /*!< TPI DEVTYPE: SubType Mask */

#define TPI_DEVTYPE_MajorType_Pos           4                                          /*!< TPI DEVTYPE: MajorType Position */
#define TPI_DEVTYPE_MajorType_Msk          (0xFUL << TPI_DEVTYPE_MajorType_Pos)        /*!< TPI DEVTYPE: MajorType Mask */

// #################################################################################################

typedef	union DHCSR_u {
	struct {
		uint32_t	C_DEBUGEN		: 1 ;
		uint32_t	C_HALT			: 1 ;
		uint32_t	C_STEP			: 1 ;
		uint32_t	C_MASKINTS		: 1 ;			// Mask interrupts when stepping or in halted debug
		uint32_t	RESERVED4		: 1 ;
		uint32_t	C_SNAPSTALL		: 1 ;			// core is stalled
		uint32_t	RESERVED3		: 10 ;
		uint32_t	DBGKEY 			: 16 ;			// Debug key, must be 0xA0DF to enable write to register
	} b_w ;
	struct {
		uint32_t	C_DEBUGEN		: 1 ;
		uint32_t	C_HALT			: 1 ;
		uint32_t	C_STEP			: 1 ;
		uint32_t	C_MASKINTS		: 1 ;			// Mask interrupts when stepping or in halted debug
		uint32_t	RESERVED4		: 1 ;
		uint32_t	C_SNAPSTALL		: 1 ;			// core is stalled
		uint32_t	RESERVED3		: 10 ;
		uint32_t	S_REGRDY		: 1 ;			// Reg R/W on Debug Core Register Selector register is available
		uint32_t	S_HALT			: 1 ;			// reads as 1 when in debug state
		uint32_t	S_SLEEP			: 1 ;			// Indicates sleeping WFI, WFE or SLEEP-ON-EXIT
		uint32_t	S_LOCKUP		: 1 ;			// Reads a 1 if core is running (not halted) and a lockup is present
		uint32_t	RESERVED2		: 4 ;			// RAZ
		uint32_t	S_RETIRE_ST		: 1 ;			// Stats, instruction completed since last read
		uint32_t	S_RESET_ST		: 1 ;			// Status, Core has been / is being reset
		uint32_t	RESERVED1		: 6 ;			// RAZ
	} b_r ;
	uint32_t	w ;									// all as a single word
} DHCSR_t ;

typedef	union DCRSR_u {
	struct {		// Check all !!
		uint32_t	REGSEL			: 5 ;
		uint32_t	Reserved1 		: 11 ;
		uint32_t	REGWnR			: 1 ;
		uint32_t	Reserved2 		: 15 ;
	} ;
	uint32_t	w ;
} DCRSR_t ;

typedef	union DEMCR_u {
	struct {
		uint32_t	VC_CORERESET	: 1 ;
		uint32_t	RESERVED1		: 3 ;
		uint32_t	VC_MMERR		: 1 ;
		uint32_t	VC_NOCPERR		: 1 ;
		uint32_t	VC_CHKERR		: 1 ;
		uint32_t	VC_STATERR		: 1 ;
		uint32_t	VC_BUSERR		: 1 ;
		uint32_t	VC_INTERR		: 1 ;
		uint32_t	VC_HARDERR		: 1 ;
		uint32_t	RESERVED2		: 5 ;
		uint32_t	MON_EN			: 1 ;
		uint32_t	MON_PEND		: 1 ;
		uint32_t	MON_STEP		: 1 ;
		uint32_t	MON_REQ			: 1 ;
		uint32_t	RESERVED3		: 4 ;
		uint32_t	TRCENA			: 1 ;
		uint32_t	RESERVED4		: 7 ;
	} ;
	uint32_t	w ;									// all as a single word
} DEMCR_t ;

typedef struct CoreDebug_s {
	__IO	DHCSR_t		DHCSR ;					/*!< Offset: 0x000 (R/W)  Debug Halting Control and Status Register    */
	__O 	DCRSR_t 	DCRSR ;                 /*!< Offset: 0x004 ( /W)  Debug Core Register Selector Register        */
	__IO	uint32_t	DCRDR ;                 /*!< Offset: 0x008 (R/W)  Debug Core Register Data Register            */
	__IO	DEMCR_t		DEMCR ;                 /*!< Offset: 0x00C (R/W)  Debug Exception and Monitor Control Register */
} CoreDebug_Type ;

#define CoreDebug_BASE      				(0xE000EDF0)                            /*!< Core Debug Base Address            */
#define CoreDebug           				((CoreDebug_Type *) CoreDebug_BASE)		/*!< Core Debug configuration struct    */

// #################################################################################################

#if (__MPU_PRESENT == 1)
typedef struct MPU_s {
  __I  uint32_t TYPE;                    /*!< Offset: 0x000 (R/ )  MPU Type Register                              */
  __IO uint32_t CTRL;                    /*!< Offset: 0x004 (R/W)  MPU Control Register                           */
  __IO uint32_t RNR;                     /*!< Offset: 0x008 (R/W)  MPU Region RNRber Register                     */
  __IO uint32_t RBAR;                    /*!< Offset: 0x00C (R/W)  MPU Region Base Address Register               */
  __IO uint32_t RASR;                    /*!< Offset: 0x010 (R/W)  MPU Region Attribute and Size Register         */
  __IO uint32_t RBAR_A1;                 /*!< Offset: 0x014 (R/W)  MPU Alias 1 Region Base Address Register       */
  __IO uint32_t RASR_A1;                 /*!< Offset: 0x018 (R/W)  MPU Alias 1 Region Attribute and Size Register */
  __IO uint32_t RBAR_A2;                 /*!< Offset: 0x01C (R/W)  MPU Alias 2 Region Base Address Register       */
  __IO uint32_t RASR_A2;                 /*!< Offset: 0x020 (R/W)  MPU Alias 2 Region Attribute and Size Register */
  __IO uint32_t RBAR_A3;                 /*!< Offset: 0x024 (R/W)  MPU Alias 3 Region Base Address Register       */
  __IO uint32_t RASR_A3;                 /*!< Offset: 0x028 (R/W)  MPU Alias 3 Region Attribute and Size Register */
} MPU_Type;

/* MPU Type Register */
#define MPU_TYPE_IREGION_Pos               16                                             /*!< MPU TYPE: IREGION Position */
#define MPU_TYPE_IREGION_Msk               (0xFFUL << MPU_TYPE_IREGION_Pos)               /*!< MPU TYPE: IREGION Mask */

#define MPU_TYPE_DREGION_Pos                8                                             /*!< MPU TYPE: DREGION Position */
#define MPU_TYPE_DREGION_Msk               (0xFFUL << MPU_TYPE_DREGION_Pos)               /*!< MPU TYPE: DREGION Mask */

#define MPU_TYPE_SEPARATE_Pos               0                                             /*!< MPU TYPE: SEPARATE Position */
#define MPU_TYPE_SEPARATE_Msk              (1UL << MPU_TYPE_SEPARATE_Pos)                 /*!< MPU TYPE: SEPARATE Mask */

/* MPU Control Register */
#define MPU_CTRL_PRIVDEFENA_Pos             2                                             /*!< MPU CTRL: PRIVDEFENA Position */
#define MPU_CTRL_PRIVDEFENA_Msk            (1UL << MPU_CTRL_PRIVDEFENA_Pos)               /*!< MPU CTRL: PRIVDEFENA Mask */

#define MPU_CTRL_HFNMIENA_Pos               1                                             /*!< MPU CTRL: HFNMIENA Position */
#define MPU_CTRL_HFNMIENA_Msk              (1UL << MPU_CTRL_HFNMIENA_Pos)                 /*!< MPU CTRL: HFNMIENA Mask */

#define MPU_CTRL_ENABLE_Pos                 0                                             /*!< MPU CTRL: ENABLE Position */
#define MPU_CTRL_ENABLE_Msk                (1UL << MPU_CTRL_ENABLE_Pos)                   /*!< MPU CTRL: ENABLE Mask */

/* MPU Region Number Register */
#define MPU_RNR_REGION_Pos                  0                                             /*!< MPU RNR: REGION Position */
#define MPU_RNR_REGION_Msk                 (0xFFUL << MPU_RNR_REGION_Pos)                 /*!< MPU RNR: REGION Mask */

/* MPU Region Base Address Register */
#define MPU_RBAR_ADDR_Pos                   5                                             /*!< MPU RBAR: ADDR Position */
#define MPU_RBAR_ADDR_Msk                  (0x7FFFFFFUL << MPU_RBAR_ADDR_Pos)             /*!< MPU RBAR: ADDR Mask */

#define MPU_RBAR_VALID_Pos                  4                                             /*!< MPU RBAR: VALID Position */
#define MPU_RBAR_VALID_Msk                 (1UL << MPU_RBAR_VALID_Pos)                    /*!< MPU RBAR: VALID Mask */

#define MPU_RBAR_REGION_Pos                 0                                             /*!< MPU RBAR: REGION Position */
#define MPU_RBAR_REGION_Msk                (0xFUL << MPU_RBAR_REGION_Pos)                 /*!< MPU RBAR: REGION Mask */

/* MPU Region Attribute and Size Register */
#define MPU_RASR_ATTRS_Pos                 16                                             /*!< MPU RASR: MPU Region Attribute field Position */
#define MPU_RASR_ATTRS_Msk                 (0xFFFFUL << MPU_RASR_ATTRS_Pos)               /*!< MPU RASR: MPU Region Attribute field Mask */

#define MPU_RASR_XN_Pos                    28                                             /*!< MPU RASR: ATTRS.XN Position */
#define MPU_RASR_XN_Msk                    (1UL << MPU_RASR_XN_Pos)                       /*!< MPU RASR: ATTRS.XN Mask */

#define MPU_RASR_AP_Pos                    24                                             /*!< MPU RASR: ATTRS.AP Position */
#define MPU_RASR_AP_Msk                    (0x7UL << MPU_RASR_AP_Pos)                     /*!< MPU RASR: ATTRS.AP Mask */

#define MPU_RASR_TEX_Pos                   19                                             /*!< MPU RASR: ATTRS.TEX Position */
#define MPU_RASR_TEX_Msk                   (0x7UL << MPU_RASR_TEX_Pos)                    /*!< MPU RASR: ATTRS.TEX Mask */

#define MPU_RASR_S_Pos                     18                                             /*!< MPU RASR: ATTRS.S Position */
#define MPU_RASR_S_Msk                     (1UL << MPU_RASR_S_Pos)                        /*!< MPU RASR: ATTRS.S Mask */

#define MPU_RASR_C_Pos                     17                                             /*!< MPU RASR: ATTRS.C Position */
#define MPU_RASR_C_Msk                     (1UL << MPU_RASR_C_Pos)                        /*!< MPU RASR: ATTRS.C Mask */

#define MPU_RASR_B_Pos                     16                                             /*!< MPU RASR: ATTRS.B Position */
#define MPU_RASR_B_Msk                     (1UL << MPU_RASR_B_Pos)                        /*!< MPU RASR: ATTRS.B Mask */

#define MPU_RASR_SRD_Pos                    8                                             /*!< MPU RASR: Sub-Region Disable Position */
#define MPU_RASR_SRD_Msk                   (0xFFUL << MPU_RASR_SRD_Pos)                   /*!< MPU RASR: Sub-Region Disable Mask */

#define MPU_RASR_SIZE_Pos                   1                                             /*!< MPU RASR: Region Size Field Position */
#define MPU_RASR_SIZE_Msk                  (0x1FUL << MPU_RASR_SIZE_Pos)                  /*!< MPU RASR: Region Size Field Mask */

#define MPU_RASR_ENABLE_Pos                 0                                             /*!< MPU RASR: Region enable bit Position */
#define MPU_RASR_ENABLE_Msk                (1UL << MPU_RASR_ENABLE_Pos)                   /*!< MPU RASR: Region enable bit Disable Mask */

#define MPU_BASE          (SCS_BASE +  0x0D90UL)                    /*!< Memory Protection Unit             */
#define MPU               ((MPU_Type       *)     MPU_BASE      )   /*!< Memory Protection Unit             */
#endif

// #################################################################################################

#if (__FPU_PRESENT == 1)
typedef struct FPU_s {
       uint32_t RESERVED0[1];
  __IO uint32_t FPCCR;                   /*!< Offset: 0x004 (R/W)  Floating-Point Context Control Register               */
  __IO uint32_t FPCAR;                   /*!< Offset: 0x008 (R/W)  Floating-Point Context Address Register               */
  __IO uint32_t FPDSCR;                  /*!< Offset: 0x00C (R/W)  Floating-Point Default Status Control Register        */
  __I  uint32_t MVFR0;                   /*!< Offset: 0x010 (R/ )  Media and FP Feature Register 0                       */
  __I  uint32_t MVFR1;                   /*!< Offset: 0x014 (R/ )  Media and FP Feature Register 1                       */
} FPU_Type;

/* Floating-Point Context Control Register */
#define FPU_FPCCR_ASPEN_Pos                31                                             /*!< FPCCR: ASPEN bit Position */
#define FPU_FPCCR_ASPEN_Msk                (1UL << FPU_FPCCR_ASPEN_Pos)                   /*!< FPCCR: ASPEN bit Mask */

#define FPU_FPCCR_LSPEN_Pos                30                                             /*!< FPCCR: LSPEN Position */
#define FPU_FPCCR_LSPEN_Msk                (1UL << FPU_FPCCR_LSPEN_Pos)                   /*!< FPCCR: LSPEN bit Mask */

#define FPU_FPCCR_MONRDY_Pos                8                                             /*!< FPCCR: MONRDY Position */
#define FPU_FPCCR_MONRDY_Msk               (1UL << FPU_FPCCR_MONRDY_Pos)                  /*!< FPCCR: MONRDY bit Mask */

#define FPU_FPCCR_BFRDY_Pos                 6                                             /*!< FPCCR: BFRDY Position */
#define FPU_FPCCR_BFRDY_Msk                (1UL << FPU_FPCCR_BFRDY_Pos)                   /*!< FPCCR: BFRDY bit Mask */

#define FPU_FPCCR_MMRDY_Pos                 5                                             /*!< FPCCR: MMRDY Position */
#define FPU_FPCCR_MMRDY_Msk                (1UL << FPU_FPCCR_MMRDY_Pos)                   /*!< FPCCR: MMRDY bit Mask */

#define FPU_FPCCR_HFRDY_Pos                 4                                             /*!< FPCCR: HFRDY Position */
#define FPU_FPCCR_HFRDY_Msk                (1UL << FPU_FPCCR_HFRDY_Pos)                   /*!< FPCCR: HFRDY bit Mask */

#define FPU_FPCCR_THREAD_Pos                3                                             /*!< FPCCR: processor mode bit Position */
#define FPU_FPCCR_THREAD_Msk               (1UL << FPU_FPCCR_THREAD_Pos)                  /*!< FPCCR: processor mode active bit Mask */

#define FPU_FPCCR_USER_Pos                  1                                             /*!< FPCCR: privilege level bit Position */
#define FPU_FPCCR_USER_Msk                 (1UL << FPU_FPCCR_USER_Pos)                    /*!< FPCCR: privilege level bit Mask */

#define FPU_FPCCR_LSPACT_Pos                0                                             /*!< FPCCR: Lazy state preservation active bit Position */
#define FPU_FPCCR_LSPACT_Msk               (1UL << FPU_FPCCR_LSPACT_Pos)                  /*!< FPCCR: Lazy state preservation active bit Mask */

/* Floating-Point Context Address Register */
#define FPU_FPCAR_ADDRESS_Pos               3                                             /*!< FPCAR: ADDRESS bit Position */
#define FPU_FPCAR_ADDRESS_Msk              (0x1FFFFFFFUL << FPU_FPCAR_ADDRESS_Pos)        /*!< FPCAR: ADDRESS bit Mask */

/* Floating-Point Default Status Control Register */
#define FPU_FPDSCR_AHP_Pos                 26                                             /*!< FPDSCR: AHP bit Position */
#define FPU_FPDSCR_AHP_Msk                 (1UL << FPU_FPDSCR_AHP_Pos)                    /*!< FPDSCR: AHP bit Mask */

#define FPU_FPDSCR_DN_Pos                  25                                             /*!< FPDSCR: DN bit Position */
#define FPU_FPDSCR_DN_Msk                  (1UL << FPU_FPDSCR_DN_Pos)                     /*!< FPDSCR: DN bit Mask */

#define FPU_FPDSCR_FZ_Pos                  24                                             /*!< FPDSCR: FZ bit Position */
#define FPU_FPDSCR_FZ_Msk                  (1UL << FPU_FPDSCR_FZ_Pos)                     /*!< FPDSCR: FZ bit Mask */

#define FPU_FPDSCR_RMode_Pos               22                                             /*!< FPDSCR: RMode bit Position */
#define FPU_FPDSCR_RMode_Msk               (3UL << FPU_FPDSCR_RMode_Pos)                  /*!< FPDSCR: RMode bit Mask */

/* Media and FP Feature Register 0 */
#define FPU_MVFR0_FP_rounding_modes_Pos    28                                             /*!< MVFR0: FP rounding modes bits Position */
#define FPU_MVFR0_FP_rounding_modes_Msk    (0xFUL << FPU_MVFR0_FP_rounding_modes_Pos)     /*!< MVFR0: FP rounding modes bits Mask */

#define FPU_MVFR0_Short_vectors_Pos        24                                             /*!< MVFR0: Short vectors bits Position */
#define FPU_MVFR0_Short_vectors_Msk        (0xFUL << FPU_MVFR0_Short_vectors_Pos)         /*!< MVFR0: Short vectors bits Mask */

#define FPU_MVFR0_Square_root_Pos          20                                             /*!< MVFR0: Square root bits Position */
#define FPU_MVFR0_Square_root_Msk          (0xFUL << FPU_MVFR0_Square_root_Pos)           /*!< MVFR0: Square root bits Mask */

#define FPU_MVFR0_Divide_Pos               16                                             /*!< MVFR0: Divide bits Position */
#define FPU_MVFR0_Divide_Msk               (0xFUL << FPU_MVFR0_Divide_Pos)                /*!< MVFR0: Divide bits Mask */

#define FPU_MVFR0_FP_excep_trapping_Pos    12                                             /*!< MVFR0: FP exception trapping bits Position */
#define FPU_MVFR0_FP_excep_trapping_Msk    (0xFUL << FPU_MVFR0_FP_excep_trapping_Pos)     /*!< MVFR0: FP exception trapping bits Mask */

#define FPU_MVFR0_Double_precision_Pos      8                                             /*!< MVFR0: Double-precision bits Position */
#define FPU_MVFR0_Double_precision_Msk     (0xFUL << FPU_MVFR0_Double_precision_Pos)      /*!< MVFR0: Double-precision bits Mask */

#define FPU_MVFR0_Single_precision_Pos      4                                             /*!< MVFR0: Single-precision bits Position */
#define FPU_MVFR0_Single_precision_Msk     (0xFUL << FPU_MVFR0_Single_precision_Pos)      /*!< MVFR0: Single-precision bits Mask */

#define FPU_MVFR0_A_SIMD_registers_Pos      0                                             /*!< MVFR0: A_SIMD registers bits Position */
#define FPU_MVFR0_A_SIMD_registers_Msk     (0xFUL << FPU_MVFR0_A_SIMD_registers_Pos)      /*!< MVFR0: A_SIMD registers bits Mask */

/* Media and FP Feature Register 1 */
#define FPU_MVFR1_FP_fused_MAC_Pos         28                                             /*!< MVFR1: FP fused MAC bits Position */
#define FPU_MVFR1_FP_fused_MAC_Msk         (0xFUL << FPU_MVFR1_FP_fused_MAC_Pos)          /*!< MVFR1: FP fused MAC bits Mask */

#define FPU_MVFR1_FP_HPFP_Pos              24                                             /*!< MVFR1: FP HPFP bits Position */
#define FPU_MVFR1_FP_HPFP_Msk              (0xFUL << FPU_MVFR1_FP_HPFP_Pos)               /*!< MVFR1: FP HPFP bits Mask */

#define FPU_MVFR1_D_NaN_mode_Pos            4                                             /*!< MVFR1: D_NaN mode bits Position */
#define FPU_MVFR1_D_NaN_mode_Msk           (0xFUL << FPU_MVFR1_D_NaN_mode_Pos)            /*!< MVFR1: D_NaN mode bits Mask */

#define FPU_MVFR1_FtZ_mode_Pos              0                                             /*!< MVFR1: FtZ mode bits Position */
#define FPU_MVFR1_FtZ_mode_Msk             (0xFUL << FPU_MVFR1_FtZ_mode_Pos)              /*!< MVFR1: FtZ mode bits Mask */

#define FPU_BASE          (SCS_BASE +  0x0F30UL)                    /*!< Floating Point Unit                */
#define FPU               ((FPU_Type       *)     FPU_BASE      )   /*!< Floating Point Unit                */
#endif

#ifdef __cplusplus
}
#endif
