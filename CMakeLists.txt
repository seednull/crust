cmake_minimum_required(VERSION 3.10)
project(crust VERSION 1.0.0)

# ==================================================================================================
# Global includes
# ==================================================================================================
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ==================================================================================================
# Global options
# ==================================================================================================
option(CRUST_BUILD_SAMPLES "Build samples" TRUE)

# ==================================================================================================
# Global Variables
# ==================================================================================================
enable_language(C)

set(CRUST_DIR_API ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CRUST_DIR_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (WIN32)
	set(CRUST_PLATFORM windows)
	set(CRUST_ABI x64)
endif()

if (APPLE)
	set(CRUST_PLATFORM macos)
	set(CRUST_ABI x64) # FIXME: Is it always x64?
	enable_language(OBJC)
	enable_language(OBJCXX)
endif()

if (EMSCRIPTEN)
	set(CRUST_PLATFORM emscripten)
	set(CRUST_ABI wasm)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

message("Platform: ${CRUST_PLATFORM}")
message("ABI: ${CRUST_ABI}")

# ==================================================================================================
# General compile flags
# ==================================================================================================
if (WIN32)
	set(CMAKE_C_FLAGS "/W4 /external:W3 /wd4996 /D_CRT_SECURE_NO_WARNINGS")
	set(CMAKE_CXX_FLAGS "/W4 /external:W3 /wd4996 /D_CRT_SECURE_NO_WARNINGS /EHsc /GR-")
endif()

if (EMSCRIPTEN)
	set(CMAKE_C_FLAGS_DEBUG "-g")
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
endif()

# ==================================================================================================
# General includes
# ==================================================================================================

# ==================================================================================================
# General libraries
# ==================================================================================================

# ==================================================================================================
# Sub-projects
# ==================================================================================================
# add_subdirectory(src)

if (CRUST_BUILD_SAMPLES)
	add_subdirectory(samples/01_math)
endif()
