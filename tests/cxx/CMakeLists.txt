enable_language(CXX)

set(CRUST_TEST_CXX_TARGET crust_test_cxx)

# ==================================================================================================
# Target
# ==================================================================================================
add_executable(${CRUST_TEST_CXX_TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
set_target_properties(${CRUST_TEST_CXX_TARGET} PROPERTIES CXX_STANDARD 11 CXX_EXTENSIONS OFF)

# ==================================================================================================
# Compiler
# ==================================================================================================
if (MSVC)
	target_compile_options(${CRUST_TEST_CXX_TARGET} PRIVATE /W4)
else()
	target_compile_options(${CRUST_TEST_CXX_TARGET} PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(${CRUST_TEST_CXX_TARGET} PRIVATE -ffp-contract=off)
endif()

# ==================================================================================================
# Libraries
# ==================================================================================================
target_link_libraries(${CRUST_TEST_CXX_TARGET} PRIVATE crust_test_framework)

if (UNIX)
	target_link_libraries(${CRUST_TEST_CXX_TARGET} PRIVATE m)
endif()

# ==================================================================================================
# Tests
# ==================================================================================================
add_test(NAME cxx COMMAND ${CRUST_TEST_CXX_TARGET})
set_tests_properties(cxx PROPERTIES TIMEOUT 120)
