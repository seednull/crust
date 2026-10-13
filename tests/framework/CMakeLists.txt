set(CRUST_TEST_FRAMEWORK_TARGET crust_test_framework)

# ==================================================================================================
# Sources
# ==================================================================================================
set(CRUST_TEST_FRAMEWORK_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/crust_test.c
)

set(CRUST_TEST_FRAMEWORK_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/crust_test.h
)

# ==================================================================================================
# Target
# ==================================================================================================
add_library(${CRUST_TEST_FRAMEWORK_TARGET} STATIC ${CRUST_TEST_FRAMEWORK_SOURCES} ${CRUST_TEST_FRAMEWORK_HEADERS})

# ==================================================================================================
# Includes
# ==================================================================================================
target_include_directories(${CRUST_TEST_FRAMEWORK_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${CRUST_TEST_FRAMEWORK_TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# ==================================================================================================
# Preprocessor
# ==================================================================================================
if (CRUST_TESTS_THREADED)
	target_compile_definitions(${CRUST_TEST_FRAMEWORK_TARGET} PUBLIC CRUST_TEST_THREADS)
endif()

# ==================================================================================================
# Compiler
# ==================================================================================================
if (MSVC)
	target_compile_options(${CRUST_TEST_FRAMEWORK_TARGET} PRIVATE /W4)
else()
	target_compile_options(${CRUST_TEST_FRAMEWORK_TARGET} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ==================================================================================================
# Libraries
# ==================================================================================================
target_link_libraries(${CRUST_TEST_FRAMEWORK_TARGET} PUBLIC crust)

if (CRUST_TESTS_THREADED AND NOT WIN32)
	target_link_libraries(${CRUST_TEST_FRAMEWORK_TARGET} PUBLIC Threads::Threads)
endif()
