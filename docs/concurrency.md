# Atomics and locks

| Header | Contents |
|---|---|
| `<crust/atomic.h>` | Memory orders, atomic operations, `crustCpuRelax` |
| `<crust/lock.h>` | `Crust_SpinLock`, `Crust_RwLock` |

## Memory orders

```c
typedef enum Crust_MemoryOrder_t
{
	CRUST_MEMORY_ORDER_RELAXED,
	CRUST_MEMORY_ORDER_CONSUME,
	CRUST_MEMORY_ORDER_ACQUIRE,
	CRUST_MEMORY_ORDER_RELEASE,
	CRUST_MEMORY_ORDER_ACQ_REL,
	CRUST_MEMORY_ORDER_SEQ_CST,
} Crust_MemoryOrder;
```

The orders mean the same as in C11 and C++11. Compilers implement `CONSUME` as `ACQUIRE`.

## Atomic operations

Available for `U32`, `U64` and `USize`. Every operation takes a `volatile` pointer and an explicit order:

| Function | Effect | Returns | Allowed orders |
|---|---|---|---|
| `crustAtomicLoadU32(p, order)` | Reads `*p` | The value | All except `RELEASE`, `ACQ_REL` |
| `crustAtomicStoreU32(p, v, order)` | Writes `v` | — | `RELAXED`, `RELEASE`, `SEQ_CST` |
| `crustAtomicSwapU32(p, v, order)` | Writes `v` | The **previous** value | All |
| `crustAtomicIncrementU32(p, order)` | Adds 1 | The **new** value | All |
| `crustAtomicDecrementU32(p, order)` | Subtracts 1 | The **new** value | All |
| `crustAtomicCompareAndSwapU32(p, desired, expected, success, failure)` | Writes `desired` if `*p == expected` | The value `*p` had before | see below |

Two things differ from C11:

- **`Increment` and `Decrement` return the new value**, unlike `atomic_fetch_add`, which returns the old one. A reference count reaches zero when `crustAtomicDecrementU32(&refs, ...) == 0`.
- **`CompareAndSwap` takes `desired` before `expected`** and returns the observed value instead of a flag. It succeeded if and only if the result equals `expected`. It is a strong CAS: it never fails spuriously.

The `failure` order of `CompareAndSwap` may not be `RELEASE` or `ACQ_REL` and may not be stronger than `success`. Load, store and CAS orders are checked with `CRUST_ASSERT`.

```c
static volatile u32 refs = 1;

crustAtomicIncrementU32(&refs, CRUST_MEMORY_ORDER_RELAXED);

if (crustAtomicDecrementU32(&refs, CRUST_MEMORY_ORDER_ACQ_REL) == 0)
	destroy();
```

A CAS loop:

```c
u32 old_value = crustAtomicLoadU32(&state, CRUST_MEMORY_ORDER_RELAXED);

for (;;)
{
	u32 new_value = old_value | FLAG;
	u32 seen = crustAtomicCompareAndSwapU32(&state, new_value, old_value, CRUST_MEMORY_ORDER_ACQ_REL, CRUST_MEMORY_ORDER_RELAXED);

	if (seen == old_value)
		break;

	old_value = seen;
}
```

### Nuances

- **`volatile` alone is not atomic.** Crust marks shared fields `volatile`, but a plain read or write of such a field has no ordering guarantees. Always go through `crustAtomic*`.
- **Alignment.** Atomic variables must be naturally aligned. On 32-bit x86 with GCC and Clang, a `u64` inside a struct is only 4-byte aligned; declare 64-bit atomics as `CRUST_ALIGNAS(8) volatile u64 counter;` there.
- **MSVC x86 and x64** ignore the order argument for read-modify-write operations: the `lock`-prefixed instructions are always full barriers. On 32-bit x86, 64-bit swap, increment and decrement are `cmpxchg8b` loops.
- **MSVC ARM64** maps orders to the `_nf`, `_acq` and `_rel` interlocked variants and to `ldar`/`stlr`.
- **GCC and Clang** use the `__atomic` builtins.
- **USize** operations are the 32-bit or 64-bit ones, depending on the pointer width.

## crustCpuRelax

`crustCpuRelax()` tells the CPU that the thread is spinning: `pause` on x86, `yield` on ARM, a compiler barrier elsewhere. Call it inside busy-wait loops.

## Crust_SpinLock

A test-and-test-and-set spin lock with exponential backoff.

```c
Crust_SpinLock lock = crustSpinLockInit();

crustSpinLockAcquire(&lock);
// critical section
crustSpinLockRelease(&lock);

if (crustSpinLockTryAcquire(&lock))
{
	// acquired without waiting
	crustSpinLockRelease(&lock);
}
```

- **Acquire/release semantics:** everything written inside the critical section is visible to the next owner.
- **Busy-waiting:** the lock never sleeps in the OS. Waiting threads spin with `crustCpuRelax`, doubling the number of pauses up to 1024. Use it for short critical sections.
- **Not fair and not reentrant:** acquiring twice from the same thread deadlocks.
- **Zero means unlocked:** `crustSpinLockInit()` returns a zeroed lock, so a zero-initialized `Crust_SpinLock` is a valid unlocked lock.

## Crust_RwLock

A reader-writer spin lock that prefers writers.

```c
Crust_RwLock lock = crustRwLockInit();

crustRwLockAcquireRead(&lock);
// many readers at once
crustRwLockReleaseRead(&lock);

crustRwLockAcquireWrite(&lock);
// one writer, no readers
crustRwLockReleaseWrite(&lock);
```

How writers win: a writer first sets a *pending* bit, which stops new readers from entering, then waits until the current readers leave.

| Function | Behavior |
|---|---|
| `crustRwLockAcquireRead` | Waits while a writer holds the lock or is waiting for it |
| `crustRwLockAcquireWrite` | Waits for other writers, announces itself, then waits for readers to drain |
| `crustRwLockTryAcquireRead` | Fails if a writer holds or waits for the lock |
| `crustRwLockTryAcquireWrite` | Succeeds only if the lock is completely free: no readers, no writer, no pending writer |
| `crustRwLockReleaseRead`, `crustRwLockReleaseWrite` | Mismatched releases are caught by assertions |

Nuances:

- **Read locks are not reentrant.** If a thread holding a read lock tries to take it again while a writer is pending, the writer waits for that thread and the thread waits for the writer: a deadlock.
- **Up to 2³⁰ − 1** readers can hold the lock at once.
- **Busy-waiting**, like the spin lock: keep critical sections short.
- **Zero means unlocked:** a zeroed `Crust_RwLock` is valid.

## Lock-free queues

For passing data between exactly one producer and one consumer without locks, see `Crust_SpscRingBuffer` and `Crust_SpscBipBuffer` in [Buffers](buffers.md#single-producer-single-consumer-variants).
