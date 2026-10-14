#pragma once

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

//
typedef struct Crust_SpinLock_t
{
	volatile u32 counter;
} Crust_SpinLock;

CRUST_APIENTRY Crust_SpinLock crustSpinLockInit(void);
CRUST_APIENTRY void crustSpinLockAcquire(Crust_SpinLock *lock);
CRUST_APIENTRY u8 crustSpinLockTryAcquire(Crust_SpinLock *lock);
CRUST_APIENTRY void crustSpinLockRelease(Crust_SpinLock *lock);

//
typedef struct Crust_RwLock_t
{
	volatile u32 counter;
} Crust_RwLock;

CRUST_APIENTRY Crust_RwLock crustRwLockInit(void);
CRUST_APIENTRY void crustRwLockAcquireRead(Crust_RwLock *lock);
CRUST_APIENTRY void crustRwLockAcquireWrite(Crust_RwLock *lock);
CRUST_APIENTRY u8 crustRwLockTryAcquireRead(Crust_RwLock *lock);
CRUST_APIENTRY u8 crustRwLockTryAcquireWrite(Crust_RwLock *lock);
CRUST_APIENTRY void crustRwLockReleaseRead(Crust_RwLock *lock);
CRUST_APIENTRY void crustRwLockReleaseWrite(Crust_RwLock *lock);

#ifdef __cplusplus
}
#endif
