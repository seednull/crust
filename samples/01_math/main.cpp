#include <crust.h>
#include <cassert>
#include <iostream>

void testVec2()
{
	Crust_Vec2 v1 {1.0f, 2.0f};
	Crust_Vec2 v2 {3.0f, 4.0f};

	Crust_Vec2 v3 = crustVec2Addv(v1, v2);
	assert(v3.x == 1.0f + 3.0f);
	assert(v3.y == 2.0f + 4.0f);
}

int main()
{
	testVec2();

	return 0;
}
