#pragma once

// Includes
#include <crust.h>

// Defines
#define CRUST_TEST_NULL_HANDLE 0
#define CRUST_TEST_DEFINE_HANDLE(TYPE) typedef u64 TYPE

#define CRUST_TEST_CHECK(x) do { if (!(x)) crustTestFail(__FILE__, (u32)__LINE__, #x); } while (0)
#define CRUST_TEST_REQUIRE(x) do { if (!(x)) { crustTestFail(__FILE__, (u32)__LINE__, #x); return 1; } } while (0)
#define CRUST_TEST_RUN(test_case) crustTestRun(#test_case, test_case)

#ifdef __cplusplus
extern "C" {
#endif

// Opaque handles
CRUST_TEST_DEFINE_HANDLE(CrustTest_Thread);

// Function pointers
typedef int (*PFN_crustTestCase)(void);
typedef void (*PFN_crustTestThreadFunction)(void *arg);

// Types
typedef struct CrustTest_Allocator_t
{
	Crust_Allocator parent;
	usize budget;
	usize blocks;
	usize bytes;
} CrustTest_Allocator;

// API
void crustTestFail(const char *file, u32 line, const char *expression);
void crustTestRun(const char *name, PFN_crustTestCase test_case);
int crustTestResult(void);

void crustTestRandomSeed(u32 seed);
u32 crustTestRandom(void);
f64 crustTestRandomF64(void);

Crust_Allocator crustTestAllocatorInit(CrustTest_Allocator *allocator, usize budget);

CrustTest_Thread crustTestThreadCreate(PFN_crustTestThreadFunction function, void *arg);
void crustTestThreadJoin(CrustTest_Thread thread);

#ifdef __cplusplus
}
#endif
