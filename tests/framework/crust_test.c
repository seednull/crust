#include "crust_test.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(CRUST_TEST_THREADS)
	#if CRUST_TOOLCHAIN_MSVC || CRUST_TOOLCHAIN_MINGW
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
	#else
		#include <pthread.h>
	#endif
#endif

//
static u32 crust_test_failures = 0;
static u32 crust_test_random_state = 2463534242u;

//
#if defined(CRUST_TEST_THREADS)
typedef struct CrustTest_ThreadState_t
{
	PFN_crustTestThreadFunction function;
	void *arg;

#if CRUST_TOOLCHAIN_MSVC || CRUST_TOOLCHAIN_MINGW
	HANDLE handle;
#else
	pthread_t handle;
#endif
} CrustTest_ThreadState;

#if CRUST_TOOLCHAIN_MSVC || CRUST_TOOLCHAIN_MINGW
static DWORD WINAPI crustTestThreadEntry(LPVOID param)
{
	CrustTest_ThreadState *state = (CrustTest_ThreadState *)param;
	state->function(state->arg);

	return 0;
}
#else
static void *crustTestThreadEntry(void *param)
{
	CrustTest_ThreadState *state = (CrustTest_ThreadState *)param;
	state->function(state->arg);

	return NULL;
}
#endif
#endif

//
void crustTestFail(const char *file, u32 line, const char *expression)
{
	printf("%s(%u): failed: %s\n", file, line, expression);
	crust_test_failures++;
}

void crustTestRun(const char *name, PFN_crustTestCase test_case)
{
	u32 failures = crust_test_failures;
	int result = test_case();

	if (result != 0 || crust_test_failures != failures)
		printf("test case failed: %s\n", name);

	if (result != 0 && crust_test_failures == failures)
		crust_test_failures++;
}

int crustTestResult(void)
{
	return crust_test_failures != 0;
}

//
void crustTestRandomSeed(u32 seed)
{
	crust_test_random_state = seed;
}

u32 crustTestRandom(void)
{
	crust_test_random_state ^= crust_test_random_state << 13;
	crust_test_random_state ^= crust_test_random_state >> 17;
	crust_test_random_state ^= crust_test_random_state << 5;

	return crust_test_random_state;
}

f64 crustTestRandomF64(void)
{
	return (f64)crustTestRandom() / 4294967296.0;
}

//
static void *crustTestAllocatorAlloc(void *context, usize size, usize alignment)
{
	CrustTest_Allocator *allocator = (CrustTest_Allocator *)context;

	if (allocator->budget == 0)
		return CRUST_NULL;

	void *ptr = crustAllocatorAlloc(allocator->parent, size, alignment);
	if (ptr == CRUST_NULL)
		return CRUST_NULL;

	allocator->budget--;
	allocator->blocks++;
	allocator->bytes += size;

	return ptr;
}

static void *crustTestAllocatorRealloc(void *context, void *ptr, usize old_size, usize new_size, usize alignment)
{
	CrustTest_Allocator *allocator = (CrustTest_Allocator *)context;

	if (allocator->budget == 0)
		return CRUST_NULL;

	void *new_ptr = crustAllocatorRealloc(allocator->parent, ptr, old_size, new_size, alignment);
	if (new_ptr == CRUST_NULL)
		return CRUST_NULL;

	allocator->budget--;
	allocator->bytes = allocator->bytes - old_size + new_size;

	return new_ptr;
}

static void crustTestAllocatorFree(void *context, void *ptr, usize size, usize alignment)
{
	CrustTest_Allocator *allocator = (CrustTest_Allocator *)context;

	crustAllocatorFree(allocator->parent, ptr, size, alignment);

	allocator->blocks--;
	allocator->bytes -= size;
}

static const Crust_AllocatorVtbl crust_test_allocator_vtbl =
{
	crustTestAllocatorAlloc,
	crustTestAllocatorRealloc,
	crustTestAllocatorFree,
};

Crust_Allocator crustTestAllocatorInit(CrustTest_Allocator *allocator, usize budget)
{
	CRUST_ASSERT(allocator != CRUST_NULL);

	allocator->parent = crustSystemAllocatorInit();
	allocator->budget = budget;
	allocator->blocks = 0;
	allocator->bytes = 0;

	Crust_Allocator result;
	result.context = allocator;
	result.vtbl = &crust_test_allocator_vtbl;

	return result;
}

//
#if defined(CRUST_TEST_THREADS)
CrustTest_Thread crustTestThreadCreate(PFN_crustTestThreadFunction function, void *arg)
{
	CRUST_ASSERT(function != CRUST_NULL);

	CrustTest_ThreadState *state = (CrustTest_ThreadState *)malloc(sizeof(CrustTest_ThreadState));
	CRUST_ASSERT(state != CRUST_NULL);

	state->function = function;
	state->arg = arg;

#if CRUST_TOOLCHAIN_MSVC || CRUST_TOOLCHAIN_MINGW
	state->handle = CreateThread(NULL, 0, crustTestThreadEntry, state, 0, NULL);
	CRUST_ASSERT(state->handle != NULL);
#else
	int result = pthread_create(&state->handle, NULL, crustTestThreadEntry, state);
	CRUST_ASSERT(result == 0);
	CRUST_UNUSED(result);
#endif

	return (CrustTest_Thread)(usize)state;
}

void crustTestThreadJoin(CrustTest_Thread thread)
{
	CRUST_ASSERT(thread != CRUST_TEST_NULL_HANDLE);

	CrustTest_ThreadState *state = (CrustTest_ThreadState *)(usize)thread;

#if CRUST_TOOLCHAIN_MSVC || CRUST_TOOLCHAIN_MINGW
	WaitForSingleObject(state->handle, INFINITE);
	CloseHandle(state->handle);
#else
	pthread_join(state->handle, NULL);
#endif

	free(state);
}
#endif
