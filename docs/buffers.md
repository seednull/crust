# Buffers

Byte buffers over memory you provide. None of them allocates: `*Attach` wraps your memory and `*Reset` empties the buffer.

| Type | What it is | Threads |
|---|---|---|
| `Crust_RingBuffer` | Byte FIFO; copies data in and out | One |
| `Crust_BipBuffer` | Byte FIFO that hands out contiguous regions to write and read in place | One |
| `Crust_Stack` | Byte LIFO | One |
| `Crust_SpscRingBuffer` | Lock-free ring buffer | One producer and one consumer |
| `Crust_SpscBipBuffer` | Lock-free bip buffer | One producer and one consumer |

All of them store raw bytes. They don't remember message boundaries: if you push values of different sizes, record the sizes yourself, for example with a small header in front of each message.

## Crust_RingBuffer

```c
static u8 storage[1024];
Crust_RingBuffer ring = crustRingBufferAttach(storage, sizeof(storage));

u32 value = 42;
if (ring.capacity - ring.size >= sizeof(value))
	crustRingBufferWrite(&ring, &value, sizeof(value));

if (ring.size >= sizeof(value))
	crustRingBufferRead(&ring, &value, sizeof(value));
```

| Function | Behavior |
|---|---|
| `crustRingBufferAttach(memory, capacity)` | Any capacity |
| `crustRingBufferWrite(ring, data, size)` | Copies exactly `size` bytes in, wrapping around the end of the storage |
| `crustRingBufferRead(ring, data, size)` | Copies exactly `size` bytes out |
| `crustRingBufferReset(ring)` | Discards everything |

`ring.size` is the number of bytes stored. Reads and writes are all-or-nothing: **writing more than the free space or reading more than `ring.size` is a contract violation**, caught by assertions in debug builds. Check before calling.

## Crust_Stack

```c
static u8 storage[256];
Crust_Stack stack = crustStackAttach(storage, sizeof(storage));

Crust_Vec3 v = { 1.0f, 2.0f, 3.0f };
crustStackPush(&stack, &v, sizeof(v));

Crust_Vec3 top;
crustStackPeek(&stack, &top, sizeof(top)); // reads without removing
crustStackPop(&stack, &top, sizeof(top));
```

`stack.head` is the number of bytes stored. Push, peek and pop copy bytes with `crustMemcpy`, so the stored data needs no alignment. The stack doesn't record sizes: pop exactly what was pushed, in reverse order. Overflow and underflow are asserted.

## Crust_BipBuffer

A bip buffer is a FIFO that never splits data across the end of its storage. Instead of copying, you *stage* a region, work on it in place and *commit* how much you used. It suits variable-sized messages and APIs that need contiguous memory, such as building a packet in place or handing a pointer to an I/O call.

```c
static u8 storage[4096];
Crust_BipBuffer bip = crustBipBufferAttach(storage, sizeof(storage));

// writer: reserve up to 256 bytes, use 100 of them
u8 *dst = (u8 *)crustBipBufferStageWrite(&bip, 256);
if (dst != CRUST_NULL)
{
	usize written = encode(dst, 256); // returns 100
	crustBipBufferCommitWrite(&bip, written);
}

// reader: look at the next 100 bytes in place
const u8 *src = (const u8 *)crustBipBufferStageRead(&bip, 100);
if (src != CRUST_NULL)
{
	decode(src, 100);
	crustBipBufferCommitRead(&bip, 100);
}
```

| Function | Behavior |
|---|---|
| `crustBipBufferStageWrite(bip, size)` | Returns `size` contiguous writable bytes, or `CRUST_NULL` if no such region is free |
| `crustBipBufferCommitWrite(bip, size)` | Publishes the first `size` bytes of the staged region, `0 < size ≤ staged` |
| `crustBipBufferStageRead(bip, size)` | Returns at least `size` contiguous readable bytes, or `CRUST_NULL` |
| `crustBipBufferCommitRead(bip, size)` | Consumes `size` bytes, `0 < size ≤ staged` |

Rules:

- **One staged write and one staged read at a time.** Every successful `Stage` must be followed by a `Commit` of at least one byte before the next `Stage` of the same kind. Staging twice is asserted, and there is no way to cancel a staged region.
- **Commit at most what you staged.** To read a message whose size is in its header, stage and commit the header first, then stage the payload, see the [example below](#crust_spscbipbuffer).
- **Reads never span the end of the storage.** When a write doesn't fit at the end, the buffer starts a new region at the beginning, and the gap left at the end is skipped. So read in the same units you wrote, for example one message at a time. A read that would need to combine the tail of one region with the start of the next returns `CRUST_NULL`.
- **Sizes are greater than zero** for every stage and commit (asserted).
- **Large writes wait for the reader.** A write of up to `capacity` bytes succeeds once the reader has consumed the data in its way; until then `StageWrite` returns `CRUST_NULL`. Keep messages well below the capacity so that the writer and the reader can work at the same time.

## Single-producer single-consumer variants

`Crust_SpscRingBuffer` and `Crust_SpscBipBuffer` are lock-free versions for exactly **one producer thread and one consumer thread**. The producer only writes, the consumer only reads, and neither ever blocks.

### Crust_SpscRingBuffer

```c
static u8 storage[4096]; // capacity must be a power of two
static Crust_SpscRingBuffer queue;

queue = crustSpscRingBufferAttach(storage, sizeof(storage));

// producer thread
usize written = crustSpscRingBufferWrite(&queue, data, size);

// consumer thread
usize read = crustSpscRingBufferRead(&queue, buffer, sizeof(buffer));
```

Unlike `Crust_RingBuffer`, reads and writes are **partial**: they transfer as many bytes as fit or are available and return the count, possibly `0`. A message can therefore arrive in pieces; loop until you have all of it, or use the bip buffer when messages must stay whole.

- **The capacity must be a power of two** (asserted).
- **Memory ordering:** a write publishes its bytes with release semantics and a read observes them with acquire semantics, so everything the producer wrote before `Write` is visible to the consumer after `Read` returns those bytes.
- **`Reset`** is only safe while neither thread is using the buffer.

### Crust_SpscBipBuffer

The same stage/commit API as `Crust_BipBuffer`. The producer calls `StageWrite`/`CommitWrite`, the consumer calls `StageRead`/`CommitRead`. Here every message starts with a small header that holds the payload size:

```c
typedef struct Header_t
{
	usize size;
} Header;
```

```c
// producer
Header *message = (Header *)crustSpscBipBufferStageWrite(&bip, sizeof(Header) + payload_size);
if (message != CRUST_NULL)
{
	message->size = payload_size;
	fill(message + 1, payload_size);
	crustSpscBipBufferCommitWrite(&bip, sizeof(Header) + payload_size);
}

// consumer
const Header *header = (const Header *)crustSpscBipBufferStageRead(&bip, sizeof(Header));
if (header != CRUST_NULL)
{
	usize size = header->size;
	crustSpscBipBufferCommitRead(&bip, sizeof(Header));

	const void *payload = crustSpscBipBufferStageRead(&bip, size);
	consume(payload, size);
	crustSpscBipBufferCommitRead(&bip, size);
}
```

A message written with one `CommitWrite` is published at once and stays contiguous, so once the header is visible, the payload right after it is too. The consumer copies `size` out of the header before committing it, because after the commit the producer may reuse that memory. Payloads must not be empty, since staging zero bytes is not allowed.

Make message sizes keep the alignment you need: the buffer places data right after the previous message, so a `Header` with 8-byte members needs message sizes that are multiples of 8, and storage that is itself aligned.

### Layout

The producer's and the consumer's indices are placed on separate 128-byte cache lines to avoid false sharing. Both structs are therefore large and **128-byte aligned**. Static and stack variables get that alignment from the compiler. When allocating them dynamically, use `crustAlloc`, which passes `CRUST_ALIGNOF(T)` to the allocator; plain `malloc` doesn't guarantee 128 bytes.
