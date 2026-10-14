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
	#endif
#endif
