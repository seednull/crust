# ==================================================================================================
# Dependencies
# ==================================================================================================
set(CRUST_TESTS_THREADED OFF)

if (NOT EMSCRIPTEN AND NOT CMAKE_SYSTEM_NAME STREQUAL "WASI")
	set(CRUST_TESTS_THREADED ON)

	if (NOT WIN32)
		find_package(Threads REQUIRED)
	endif()
endif()

# ==================================================================================================
# Functions
# ==================================================================================================
function(crust_add_test name)
	set(target crust_test_${name})

	add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/${name}.c)
	target_link_libraries(${target} PRIVATE crust_test_framework)

	if (UNIX)
		target_link_libraries(${target} PRIVATE m)
	endif()

	if (MSVC)
		target_compile_options(${target} PRIVATE /W4)
	else()
		target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
	endif()

	add_test(NAME ${name} COMMAND ${target})
	set_tests_properties(${name} PROPERTIES TIMEOUT 120)
endfunction()

# ==================================================================================================
# Framework
# ==================================================================================================
add_subdirectory(framework)

# ==================================================================================================
# Tests
# ==================================================================================================
crust_add_test(arena)
crust_add_test(atomic)
crust_add_test(bip)
crust_add_test(hash)
crust_add_test(heap)
crust_add_test(lock)
crust_add_test(math)
crust_add_test(pool)
crust_add_test(ring)
crust_add_test(simd)
crust_add_test(stack)
crust_add_test(types)

# ==================================================================================================
# C++
# ==================================================================================================
include(CheckLanguage)
check_language(CXX)

if (CMAKE_CXX_COMPILER)
	add_subdirectory(cxx)
endif()

# ==================================================================================================
# Headers
# ==================================================================================================
file(GLOB CRUST_TEST_HEADERS RELATIVE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/crust/*.h)
list(APPEND CRUST_TEST_HEADERS crust.h)

set(CRUST_TEST_HEADER_SOURCES)

foreach(header ${CRUST_TEST_HEADERS})
	string(MAKE_C_IDENTIFIER ${header} name)
	set(source ${CMAKE_CURRENT_BINARY_DIR}/headers/${name}.c)

	file(WRITE ${source} "#include <${header}>\n")
	list(APPEND CRUST_TEST_HEADER_SOURCES ${source})
endforeach()

add_library(crust_test_headers OBJECT ${CRUST_TEST_HEADER_SOURCES})
target_link_libraries(crust_test_headers PRIVATE crust)

if (MSVC)
	target_compile_options(crust_test_headers PRIVATE /W4)
else()
	target_compile_options(crust_test_headers PRIVATE -Wall -Wextra -Wpedantic)
endif()
