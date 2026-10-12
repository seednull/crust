# ==================================================================================================
# Dependencies
# ==================================================================================================
set(CRUST_TESTS_THREADED OFF)

if (NOT EMSCRIPTEN)
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
	target_link_libraries(${target} PRIVATE crust crust_test_framework)

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
crust_add_test(math)
crust_add_test(pool)
crust_add_test(ring)
crust_add_test(stack)

if (CRUST_TESTS_THREADED)
	crust_add_test(lock)
endif()
