//
#if CRUST_COMPILER_MSVC
	#if CRUST_ARCH_X86 || CRUST_ARCH_X64
		#include "scalar_msvc_x86.inl"
	#elif CRUST_ARCH_ARM64
		#include "scalar_msvc_arm64.inl"
	#else
		#error "Unsupported MSVC architecture"
	#endif
#elif CRUST_COMPILER_GCC || CRUST_COMPILER_CLANG
	#include "scalar_gcc_clang.inl"
#else
	#error "Unsupported compiler"
#endif

//
static CRUST_INLINE f32 crustRsqrtF32(f32 v) { return 1.0f / crustSqrtF32(v); }
static CRUST_INLINE f64 crustRsqrtF64(f64 v) { return 1.0 / crustSqrtF64(v); }
