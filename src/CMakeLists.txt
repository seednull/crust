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
	${CRUST_DIR_API}/platform/cpu_gcc_clang.inl
	${CRUST_DIR_API}/platform/cpu_msvc_arm64.inl
	${CRUST_DIR_API}/platform/cpu_msvc_x86.inl
)

# ==================================================================================================
# Target
# ==================================================================================================
add_library(${CRUST_TARGET} ${CRUST_SOURCES} ${CRUST_HEADERS} ${CRUST_PUBLIC_HEADERS})
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
target_compile_definitions(${CRUST_TARGET} PRIVATE CRUST_IMPLEMENTATION)

if (BUILD_SHARED_LIBS)
	target_compile_definitions(${CRUST_TARGET} PUBLIC CRUST_SHARED_LIBRARY)
endif()

# ==================================================================================================
# Compiler
# ==================================================================================================
if(MSVC)
	target_compile_options(${CRUST_TARGET} PRIVATE /W4 /external:W3)
	target_compile_definitions(${CRUST_TARGET} PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
	target_compile_options(${CRUST_TARGET} PRIVATE -Wall -Wextra -Wpedantic)
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
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	PUBLIC_HEADER DESTINATION include
)

install(
	DIRECTORY ${CRUST_DIR_API}/platform
	DESTINATION include
	FILES_MATCHING PATTERN "*.inl"
)

install(
	EXPORT crustTargets
	FILE crustTargets.cmake
	NAMESPACE crust::
	DESTINATION lib/cmake/crust
)

# Генерируем crustConfig.cmake из шаблона
configure_package_config_file(
	${PROJECT_SOURCE_DIR}/cmake/crustConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/crustConfig.cmake
	INSTALL_DESTINATION lib/cmake/crust
)

# Генерируем crustConfigVersion.cmake для проверки версий
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
