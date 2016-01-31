
#include "Globals.h"
#include "WorldStorage/FastNBT.h"

int main() {
	{
		// -1 items
		char data[] = {10, 0, 0, 9, 0, 0, 0, -0x1, -0x1, -0x1, -0x1};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(!nbt.IsValid());
	}
	{
		// 1001 items
		char data[] = {10, 0, 0, 9, 0, 0, 0, 0, 0, 3, -0x68};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(!nbt.IsValid());
	}
	{
		// single invalid
		char data[] = {10, 0, 0, 9, 0, 0, -1, 0, 0, 0, 1, 0};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(!nbt.IsValid());
	}
	{
		// two valid
		char data[] = {10, 0, 0, 9, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(nbt.IsValid());
	}
}
