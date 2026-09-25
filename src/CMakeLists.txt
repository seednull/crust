cmake_minimum_required(VERSION 3.10)
set(TARGET crust)

# ==================================================================================================
# Options
# ==================================================================================================

# ==================================================================================================
# Variables
# ==================================================================================================

if (CRUST_BUILD_STATIC)
	set(CRUST_TARGET_TYPE STATIC)
	set(CRUST_PLATFORM_DEFINES CRUST_IMPLEMENTATION)
else()
	set(CRUST_TARGET_TYPE SHARED)
	set(CRUST_PLATFORM_DEFINES CRUST_SHARED_LIBRARY CRUST_IMPLEMENTATION)
endif()

# ==================================================================================================
# Dependencies
# ==================================================================================================

# ==================================================================================================
# Sources
# ==================================================================================================
file(GLOB SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/libc/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/freestanding/*.c
)

file(GLOB HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/libc/platform/*.inl
)

file (GLOB PUBLIC_HEADERS
	${CRUST_DIR_API}/crust.h
	${CRUST_DIR_API}/platform/*.inl
)

file (GLOB PUBLIC_HEADER
	${CRUST_DIR_API}/crust.h
)

# ==================================================================================================
# Target
# ==================================================================================================
add_library(${TARGET} ${CRUST_TARGET_TYPE} ${SOURCES} ${HEADERS} ${PUBLIC_HEADERS})

set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX d)
set_target_properties(${TARGET} PROPERTIES PUBLIC_HEADER ${PUBLIC_HEADER})

if (WIN32)
	set_target_properties(${TARGET} PROPERTIES PREFIX "")
	set_target_properties(${TARGET} PROPERTIES IMPORT_PREFIX "")
	set_target_properties(${TARGET} PROPERTIES IMPORT_SUFFIX ".lib")
else()
	set_target_properties(${TARGET} PROPERTIES IMPORT_SUFFIX ".a")
endif()

# ==================================================================================================
# Includes
# ==================================================================================================
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CRUST_DIR_API})

# ==================================================================================================
# Preprocessor
# ==================================================================================================
target_compile_definitions(${TARGET} PRIVATE ${CRUST_PLATFORM_DEFINES})

# ==================================================================================================
# Linker
# ==================================================================================================

# ==================================================================================================
# Libraries
# ==================================================================================================

# ==================================================================================================
# Installation
# ==================================================================================================
install(
	TARGETS ${TARGET}
	EXPORT ${TARGET}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include
	PUBLIC_HEADER DESTINATION include
)
