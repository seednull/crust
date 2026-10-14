#pragma once

#if CRUST_COMPILER_MSVC
	#include <intrin0.h>

	#if CRUST_ARCH_X86 || CRUST_ARCH_X64
		#if !defined(_INC_MALLOC)
			#define _INC_MALLOC
			#define CRUST_SKIPPED_INC_MALLOC
		#endif

		#include <nmmintrin.h>

		#if defined(CRUST_SKIPPED_INC_MALLOC)
			#undef _INC_MALLOC
			#undef CRUST_SKIPPED_INC_MALLOC
		#endif
	#elif CRUST_ARCH_ARM64
		#include <arm_neon.h>

		#ifdef __cplusplus
		extern "C" {
		#endif

		unsigned int _CountOneBits(unsigned long);

		long _InterlockedIncrement_nf(long volatile *);
		long _InterlockedIncrement_acq(long volatile *);
		long _InterlockedIncrement_rel(long volatile *);
		long _InterlockedDecrement_nf(long volatile *);
		long _InterlockedDecrement_acq(long volatile *);
		long _InterlockedDecrement_rel(long volatile *);

		__int64 _InterlockedIncrement64_nf(__int64 volatile *);
		__int64 _InterlockedIncrement64_acq(__int64 volatile *);
		__int64 _InterlockedIncrement64_rel(__int64 volatile *);
		__int64 _InterlockedDecrement64_nf(__int64 volatile *);
		__int64 _InterlockedDecrement64_acq(__int64 volatile *);
		__int64 _InterlockedDecrement64_rel(__int64 volatile *);

		#ifdef __cplusplus
		}
		#endif
	#endif
#endif
