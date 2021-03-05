
#include "Globals.h"
#include "../TestHelpers.h"
#include "ChunkData.h"





/** Helper that copies a data store into a contiguous flat array, filling in a default value for sections that aren't present. */
template <class StoreType, typename GetType, typename DefaultType, typename OutType>
static void CopyAll(const StoreType & Data, GetType Getter, DefaultType Default, OutType & Out)
{
	constexpr auto SectionCount = std::extent_v<OutType> / 16;

	for (size_t Y = 0; Y != 16; Y++)
	{
		const auto Section = (Data.*Getter)(Y);
		static_assert(SectionCount == std::tuple_size<std::remove_pointer_t<decltype(Section)>>::value, "Output array has wrong size");

		if (Section == nullptr)
		{
			std::fill_n(Out + Y * SectionCount, SectionCount, Default);
		}
		else
		{
			std::copy(Section->begin(), Section->end(), Out + Y * SectionCount);
		}
	}
}





/** Performs the entire Copies test. */
static void Test()
{
	LOGD("Test started");

	{
		ChunkBlockData buffer;

		buffer.SetBlock({ 3, 1, 4 }, 0xDE);
		buffer.SetMeta({ 3, 1, 4 }, 0xA);

		ChunkBlockData copy;
		copy.Assign(buffer);
		TEST_EQUAL(copy.GetBlock({ 3, 1, 4 }), 0xDE);
		TEST_EQUAL(copy.GetMeta({ 3, 1, 4 }), 0xA);

		BLOCKTYPE SrcBlockBuffer[16 * 16 * 256];
		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2]{};
		for (int i = 0; i < 16 * 16 * 256; i += 4)
		{
			SrcBlockBuffer[i + 0] = 0xde;
			SrcBlockBuffer[i + 1] = 0xad;
			SrcBlockBuffer[i + 2] = 0xbe;
			SrcBlockBuffer[i + 3] = 0xef;
		}

		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);
		BLOCKTYPE DstBlockBuffer[16 * 16 * 256];
		CopyAll(buffer, &ChunkBlockData::GetSection, ChunkBlockData::DefaultValue, DstBlockBuffer);
		TEST_EQUAL(memcmp(SrcBlockBuffer, DstBlockBuffer, (16 * 16 * 256) - 1), 0);

		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);
		CopyAll(buffer, &ChunkBlockData::GetSection, ChunkBlockData::DefaultValue, DstBlockBuffer);
		TEST_EQUAL(memcmp(SrcBlockBuffer, DstBlockBuffer, (16 * 16 * 256) - 1), 0);
	}

	{
		ChunkBlockData buffer;

		BLOCKTYPE SrcBlockBuffer[16 * 16 * 256]{};
		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		for (int i = 0; i < 16 * 16 * 256 / 2; i += 4)
		{
			SrcNibbleBuffer[i + 0] = 0xde;
			SrcNibbleBuffer[i + 1] = 0xad;
			SrcNibbleBuffer[i + 2] = 0xbe;
			SrcNibbleBuffer[i + 3] = 0xef;
		}

		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);
		NIBBLETYPE DstNibbleBuffer[16 * 16 * 256/ 2];
		CopyAll(buffer, &ChunkBlockData::GetMetaSection, ChunkBlockData::DefaultMetaValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1), 0);

		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);
		CopyAll(buffer, &ChunkBlockData::GetMetaSection, ChunkBlockData::DefaultMetaValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1), 0);
	}

	{
		ChunkLightData buffer;

		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		for (int i = 0; i < 16 * 16 * 256 / 2; i += 4)
		{
			SrcNibbleBuffer[i + 0] = 0xde;
			SrcNibbleBuffer[i + 1] = 0xad;
			SrcNibbleBuffer[i + 2] = 0xbe;
			SrcNibbleBuffer[i + 3] = 0xef;
		}

		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer);
		NIBBLETYPE DstNibbleBuffer[16 * 16 * 256 / 2];
		CopyAll(buffer, &ChunkLightData::GetBlockLightSection, ChunkLightData::DefaultBlockLightValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 /2) - 1), 0);

		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer);
		CopyAll(buffer, &ChunkLightData::GetBlockLightSection, ChunkLightData::DefaultBlockLightValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 /2) - 1), 0);
	}

	{
		ChunkLightData buffer;

		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		for (int i = 0; i < 16 * 16 * 256 / 2; i += 4)
		{
			SrcNibbleBuffer[i + 0] = 0xde;
			SrcNibbleBuffer[i + 1] = 0xad;
			SrcNibbleBuffer[i + 2] = 0xbe;
			SrcNibbleBuffer[i + 3] = 0xef;
		}

		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer);
		NIBBLETYPE DstNibbleBuffer[16 * 16 * 256/ 2];
		CopyAll(buffer, &ChunkLightData::GetSkyLightSection, ChunkLightData::DefaultSkyLightValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1), 0);

		memset(SrcNibbleBuffer, 0xFF, 16 * 16 * 256 / 2);
		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer);
		CopyAll(buffer, &ChunkLightData::GetSkyLightSection, ChunkLightData::DefaultSkyLightValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1), 0);
	}

	{
		ChunkBlockData buffer;

		BLOCKTYPE SrcBlockBuffer[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		BLOCKTYPE DstBlockBuffer[16 * 16 * 256];
		CopyAll(buffer, &ChunkBlockData::GetSection, ChunkBlockData::DefaultValue, DstBlockBuffer);
		TEST_EQUAL(memcmp(SrcBlockBuffer, DstBlockBuffer, (16 * 16 * 256) - 1), 0);

		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 / 2);
		NIBBLETYPE DstNibbleBuffer[16 * 16 * 256 / 2];
		CopyAll(buffer, &ChunkBlockData::GetMetaSection, ChunkBlockData::DefaultMetaValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1), 0);
	}

	{
		ChunkLightData buffer;

		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 / 2);
		NIBBLETYPE DstNibbleBuffer[16 * 16 * 256 / 2];
		CopyAll(buffer, &ChunkLightData::GetBlockLightSection, ChunkLightData::DefaultBlockLightValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1), 0);

		memset(SrcNibbleBuffer, 0xFF, 16 * 16 * 256 / 2);
		CopyAll(buffer, &ChunkLightData::GetSkyLightSection, ChunkLightData::DefaultSkyLightValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1), 0);
	}
}





IMPLEMENT_TEST_MAIN("ChunkData Copies",
	Test()
)
