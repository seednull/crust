#include <crust/ring.h>
#include <crust/simd.h>
#include <crust.h>

int main(void)
{
	static u8 storage[64];
	Crust_RingBuffer ring = crustRingBufferAttach(storage, sizeof(storage));

	u32 value = 7;
	crustRingBufferWrite(&ring, &value, sizeof(value));

	u32 read = 0;
	crustRingBufferRead(&ring, &read, sizeof(read));

	Crust_Simd128 v = crustSimd128SplatU32(read);
	u32 sum = crustSimd128GetLaneU32(crustSimd128AddU32(v, v), 3);

	Crust_Allocator allocator = crustSystemAllocatorInit();
	u32 *values = crustAllocArray(allocator, u32, 16);
	int allocated = values != CRUST_NULL;
	crustFreeArray(allocator, values, u32, 16);

	return (sum == 14 && allocated && CRUST_VERSION_MAJOR == 1) ? 0 : 1;
}
