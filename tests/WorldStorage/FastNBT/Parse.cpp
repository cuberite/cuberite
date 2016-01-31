#include "Globals.h"
#include "WorldStorage/FastNBT.h"

int main() {
	{
		cParsedNBT nbt{nullptr, 0};

		testassert(!nbt.IsValid());
	}

	{
		cParsedNBT nbt{nullptr, 2};

		testassert(!nbt.IsValid());
	}
	{
		cParsedNBT nbt{"", 3};

		testassert(!nbt.IsValid());
	}
	{
		cParsedNBT nbt{nullptr, 3};

		testassert(!nbt.IsValid());
	}

	{
		char data[] = {10, 0, 0, 0x7F, -0x7F};
		cParsedNBT nbt{data, 5};

		testassert(!nbt.IsValid());
	}
	{
		char data[] = {2, 0, 0, 0x7F, -0x7F};
		cParsedNBT nbt{data, 5};

		testassert(!nbt.IsValid());
	}
}
