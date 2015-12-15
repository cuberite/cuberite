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
}
