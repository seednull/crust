set(CRUST_TARGET crust)

# ==================================================================================================
# Options
# ==================================================================================================

# ==================================================================================================
# Variables
# ==================================================================================================
set(CRUST_DIR_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(CRUST_DIR_API ${CRUST_DIR_ROOT}/include)

# ==================================================================================================
# Dependencies
# ==================================================================================================

# ==================================================================================================
# Sources
# ==================================================================================================
set(CRUST_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/freestanding/arena.c
	${CMAKE_CURRENT_SOURCE_DIR}/freestanding/bip.c
	${CMAKE_CURRENT_SOURCE_DIR}/freestanding/hash.c
	${CMAKE_CURRENT_SOURCE_DIR}/freestanding/heap.c
	${CMAKE_CURRENT_SOURCE_DIR}/freestanding/lock.c
	${CMAKE_CURRENT_SOURCE_DIR}/freestanding/pool.c
	${CMAKE_CURRENT_SOURCE_DIR}/freestanding/ring.c
	${CMAKE_CURRENT_SOURCE_DIR}/freestanding/stack.c
	${CMAKE_CURRENT_SOURCE_DIR}/libc/math.c
	${CMAKE_CURRENT_SOURCE_DIR}/libc/memory.c
)

set(CRUST_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/libc/platform/memory_msvc.inl
	${CMAKE_CURRENT_SOURCE_DIR}/libc/platform/memory_posix.inl
)

set(CRUST_PUBLIC_HEADERS
	${CRUST_DIR_API}/crust.h
	${CRUST_DIR_API}/crust/allocator.h
	${CRUST_DIR_API}/crust/arena.h
	${CRUST_DIR_API}/crust/atomic.h
	${CRUST_DIR_API}/crust/bip.h
	${CRUST_DIR_API}/crust/core.h
	${CRUST_DIR_API}/crust/hash.h
	${CRUST_DIR_API}/crust/heap.h
	${CRUST_DIR_API}/crust/lock.h
	${CRUST_DIR_API}/crust/memory.h
	${CRUST_DIR_API}/crust/pool.h
	${CRUST_DIR_API}/crust/quat.h
	${CRUST_DIR_API}/crust/ring.h
	${CRUST_DIR_API}/crust/scalar.h
	${CRUST_DIR_API}/crust/simd.h
	${CRUST_DIR_API}/crust/stack.h
	${CRUST_DIR_API}/crust/transform.h
	${CRUST_DIR_API}/crust/vec.h
	${CRUST_DIR_API}/crust/platform/atomic_common.inl
	${CRUST_DIR_API}/crust/platform/atomic_gcc_clang.inl
	${CRUST_DIR_API}/crust/platform/atomic_msvc_arm64.inl
	${CRUST_DIR_API}/crust/platform/atomic_msvc_x86.inl
	${CRUST_DIR_API}/crust/platform/intrinsics_cpu.inl
	${CRUST_DIR_API}/crust/platform/intrinsics_simd.inl
	${CRUST_DIR_API}/crust/platform/scalar_common.inl
	${CRUST_DIR_API}/crust/platform/scalar_gcc_clang.inl
	${CRUST_DIR_API}/crust/platform/scalar_msvc_arm64.inl
	${CRUST_DIR_API}/crust/platform/scalar_msvc_x86.inl
	${CRUST_DIR_API}/crust/platform/simd_common.inl
	${CRUST_DIR_API}/crust/platform/simd_neon.inl
	${CRUST_DIR_API}/crust/platform/simd_scalar.inl
	${CRUST_DIR_API}/crust/platform/simd_sse.inl
	${CRUST_DIR_API}/crust/platform/simd_wasm.inl
)

# ==================================================================================================
# Target
# ==================================================================================================
add_library(${CRUST_TARGET} STATIC ${CRUST_SOURCES} ${CRUST_HEADERS} ${CRUST_PUBLIC_HEADERS})
add_library(crust::${CRUST_TARGET} ALIAS ${CRUST_TARGET})

# ==================================================================================================
# Properties
# ==================================================================================================
set_target_properties(${CRUST_TARGET} PROPERTIES DEBUG_POSTFIX d)
set_target_properties(${CRUST_TARGET} PROPERTIES PUBLIC_HEADER ${CRUST_DIR_API}/crust.h)
set_target_properties(${CRUST_TARGET} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if (WIN32)
	set_target_properties(${CRUST_TARGET} PROPERTIES PREFIX "")
endif()

# ==================================================================================================
# Includes
# ==================================================================================================
target_include_directories(${CRUST_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${CRUST_TARGET} PUBLIC "$<BUILD_INTERFACE:${CRUST_DIR_API}>")
target_include_directories(${CRUST_TARGET} PUBLIC "$<INSTALL_INTERFACE:include>")

# ==================================================================================================
# Preprocessor
# ==================================================================================================
if (CRUST_USE_SIMD)
	target_compile_definitions(${CRUST_TARGET} PUBLIC CRUST_USE_SIMD=1)
endif()

# ==================================================================================================
# Compiler
# ==================================================================================================
target_compile_features(${CRUST_TARGET} PUBLIC c_std_99)

if(MSVC)
	target_compile_options(${CRUST_TARGET} PRIVATE /W4 /external:W3)
	target_compile_definitions(${CRUST_TARGET} PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
	target_compile_options(${CRUST_TARGET} PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (CRUST_USE_SIMD)
	include(CheckCSourceCompiles)

	set(CMAKE_REQUIRED_FLAGS "-msse4.2 -mpopcnt")
	check_c_source_compiles("
		#if !defined(__SSE4_2__) || !defined(__POPCNT__)
			#error
		#endif
		int main(void) { return 0; }" CRUST_COMPILER_SUPPORTS_SSE42)

	set(CMAKE_REQUIRED_FLAGS "-msimd128")
	check_c_source_compiles("
		#if !defined(__wasm_simd128__)
			#error
		#endif
		int main(void) { return 0; }" CRUST_COMPILER_SUPPORTS_SIMD128)

	unset(CMAKE_REQUIRED_FLAGS)

	if (CRUST_COMPILER_SUPPORTS_SSE42)
		target_compile_options(${CRUST_TARGET} PUBLIC -msse4.2 -mpopcnt)
	endif()

	if (CRUST_COMPILER_SUPPORTS_SIMD128)
		target_compile_options(${CRUST_TARGET} PUBLIC -msimd128)
	endif()
endif()

# ==================================================================================================
# Linker
# ==================================================================================================

# ==================================================================================================
# Libraries
# ==================================================================================================
if (UNIX)
	target_link_libraries(${CRUST_TARGET} PRIVATE m)
endif()

# ==================================================================================================
# Installation
# ==================================================================================================
include(CMakePackageConfigHelpers)

install(
	TARGETS ${CRUST_TARGET}
	EXPORT crustTargets
	ARCHIVE DESTINATION lib
	PUBLIC_HEADER DESTINATION include
)

install(
	DIRECTORY ${CRUST_DIR_API}/crust
	DESTINATION include
	FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.inl"
)

install(
	EXPORT crustTargets
	FILE crustTargets.cmake
	NAMESPACE crust::
	DESTINATION lib/cmake/crust
)

configure_package_config_file(
	${PROJECT_SOURCE_DIR}/cmake/crustConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/crustConfig.cmake
	INSTALL_DESTINATION lib/cmake/crust
)

write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/crustConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/crustConfig.cmake
		${CMAKE_CURRENT_BINARY_DIR}/crustConfigVersion.cmake
	DESTINATION lib/cmake/crust
)
