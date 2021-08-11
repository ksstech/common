/*
 * Generic TOOLSET support module
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if		defined( __CC_ARM )

	#define		__ASM            			__asm                                      /*!< asm keyword for ARM Compiler          */
	#define		__NOP						__nop
	#define		__IMPORT					IMPORT
	#define 	__INLINE         			__inline                                   /*!< inline keyword for ARM Compiler       */
	#define 	__STATIC_INLINE  			static __inline
	#define		DISABLE_INTERRUPTS()		__disable_fiq()
	#define		ENABLE_INTERRUPTS()			__enable_fiq()
	#define		DEBUG_BREAKPOINT()			__asm( "   bkpt #0 \n")
	#define 	NORETURN					__attribute__((noreturn))

	#if	defined( __TARGET_FPU_VFP )
		#if (__FPU_PRESENT == 1)
			#define 	__FPU_USED			1
		#else
			#warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
			#define 	__FPU_USED			0
		#endif
	#else
		#define 	__FPU_USED				0
	#endif

#elif 	defined( __GNUC__ )

	#define 	__ASM            			__asm                                      /*!< asm keyword for GNU Compiler          */
	#define 	__INLINE         			inline                                     /*!< inline keyword for GNU Compiler       */
	#define 	__STATIC_INLINE  			static inline
	#define		__NOP()						 __ASM volatile ("nop");
	#define		DISABLE_INTERRUPTS()
	#define		ENABLE_INTERRUPTS()
	#define		DEBUG_BREAKPOINT()			__asm( "   bkpt #0 \n")
	#define 	NORETURN					__attribute__((noreturn))

	#if defined( __VFP_FP__ ) && !defined( __SOFTFP__ )
		#if (__FPU_PRESENT == 1)
    		#define 	__FPU_USED       	1
		#else
			#warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
			#define 	__FPU_USED       	0
		#endif
	#else
		#define		__FPU_USED         		0
	#endif

#elif	defined( __TI_ARM__ )

	#define 	__ASM            			__asm                                      /*!< asm keyword for TI CCS Compiler       */
	#define 	__STATIC_INLINE  			static inline
	#define		DISABLE_INTERRUPTS()		_disable_interrupts()
	#define		ENABLE_INTERRUPTS()			_enable_interrupts()
	#define		DEBUG_BREAKPOINT()			__asm( "   bkpt #0 \n")
	#define		__NOP						__nop

	#if	defined	( __TI_VFP_SUPPORT__ )
		#if (__FPU_PRESENT == 1)
    		#define 	__FPU_USED       	1
		#else
    		#warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
    		#define 	__FPU_USED       	0
		#endif
	#else
		#define __FPU_USED         			0
	#endif

#elif	defined ( __ICCARM__ )

	#define 	__ASM            			__asm                                      /*!< asm keyword for IAR Compiler          */
	#define 	__INLINE         			inline                                     /*!< inline keyword for IAR Compiler. Only available in High optimization mode! */
	#define 	__STATIC_INLINE  			static inline

	#if defined( __ARMVFP__ )
		#if ( __FPU_PRESENT == 1 )
			#define		__FPU_USED       	1
		#else
			#warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
			#define 	__FPU_USED       	0
		#endif
	#else
		#define 	__FPU_USED         		0
	#endif

#elif 	defined ( __TASKING__ )

	#define 	__ASM            			__asm                                      /*!< asm keyword for TASKING Compiler      */
	#define 	__INLINE         			inline                                     /*!< inline keyword for TASKING Compiler   */
	#define 	__STATIC_INLINE  			static inline

	#if defined( __FPU_VFP__ )
		#if (__FPU_PRESENT == 1)
			#define 	__FPU_USED       	1
		#else
			#error "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
			#define 	__FPU_USED       	0
		#endif
	#else
		#define __FPU_USED         			0
	#endif

#else

	#error "Unsupported/no toolset defined !!!"

#endif

#ifdef __cplusplus
}
#endif
