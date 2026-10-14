#pragma once

#if CRUST_SIMD_SSE
	#if !defined(_INC_MALLOC) && !defined(_MM_MALLOC_H_INCLUDED) && !defined(__MM_MALLOC_H)
		#define _INC_MALLOC
		#define _MM_MALLOC_H_INCLUDED
		#define __MM_MALLOC_H
		#define CRUST_SKIPPED_MM_MALLOC
	#endif

	#include <nmmintrin.h>

	#if defined(CRUST_SKIPPED_MM_MALLOC)
		#undef _INC_MALLOC
		#undef _MM_MALLOC_H_INCLUDED
		#undef __MM_MALLOC_H
		#undef CRUST_SKIPPED_MM_MALLOC
	#endif
#elif CRUST_SIMD_NEON
	#include <arm_neon.h>
#elif CRUST_SIMD_WASM
	#include <wasm_simd128.h>
#endif
