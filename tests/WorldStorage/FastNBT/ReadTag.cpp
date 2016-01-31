

#include "Globals.h"
#include "WorldStorage/FastNBT.h"

int main() {
	{
		// TAG_BYTE 
		char data[] = {10, 0, 0, 1, 0, 0, 0, 0};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(nbt.IsValid());
	}
	{
		// TAG_SHORT 
		char data[] = {10, 0, 0, 2, 0, 0, 0, 0, 0};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(nbt.IsValid());
	}
	{
		// TAG_INT 
		char data[] = {10, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(nbt.IsValid());
	}
	{
		// TAG_LONG 
		char data[] = {10, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(nbt.IsValid());
	}
	{
		// TAG_FLOAT 
		char data[] = {10, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(nbt.IsValid());
	}
	{
		// TAG_DOUBLE 
		char data[] = {10, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(nbt.IsValid());
	}
	{
		// TAG_ByteArray negative length
		char data[] = {10, 0, 0, 7, 0, 0, -1, -1, -1, -1, 0};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(!nbt.IsValid());
	}
	{
		// TAG_Compound
		char data[] = {10, 0, 0, 10, 0, 0, 0, 0, 0};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(nbt.IsValid());
	}
	{
		// TAG_IntArray negative length
		char data[] = {10, 0, 0, 11, 0, 0, -1, -1, -1, -1, 0};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(!nbt.IsValid());
	}
	{
		// TAG_IntArray zero length
		char data[] = {10, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0};
		cParsedNBT nbt{data, sizeof(data)};

		testassert(nbt.IsValid());
	}
}
