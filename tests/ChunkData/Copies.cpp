
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

		ChunkBlockData copy;
		copy.Assign(buffer);
		TEST_EQUAL(copy.GetBlock({ 3, 1, 4 }), 0xDE);

		BlockState SrcBlockBuffer[16 * 16 * 256];
		for (int i = 0; i < 16 * 16 * 256; i += 4)
		{
			SrcBlockBuffer[i + 0] = BlockState(10);
			SrcBlockBuffer[i + 1] = BlockState(100);
			SrcBlockBuffer[i + 2] = BlockState(1000);
			SrcBlockBuffer[i + 3] = BlockState(10000);
		}

		buffer.SetAll(SrcBlockBuffer);
		BlockState DstBlockBuffer[16 * 16 * 256];
		CopyAll(buffer, &ChunkBlockData::GetSection, ChunkBlockData::DefaultValue, DstBlockBuffer);
		TEST_EQUAL(memcmp(SrcBlockBuffer, DstBlockBuffer, (16 * 16 * 256) - 1), 0);

		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		buffer.SetAll(SrcBlockBuffer);
		CopyAll(buffer, &ChunkBlockData::GetSection, ChunkBlockData::DefaultValue, DstBlockBuffer);
		TEST_EQUAL(memcmp(SrcBlockBuffer, DstBlockBuffer, (16 * 16 * 256) - 1), 0);
	}

	{
		ChunkLightData buffer;

		LIGHTTYPE SrcLightBuffer[16 * 16 * 256 / 2];
		for (int i = 0; i < 16 * 16 * 256 / 2; i += 4)
		{
			SrcLightBuffer[i + 0] = 0xde;
			SrcLightBuffer[i + 1] = 0xad;
			SrcLightBuffer[i + 2] = 0xbe;
			SrcLightBuffer[i + 3] = 0xef;
		}

		buffer.SetAll(SrcLightBuffer, SrcLightBuffer);
		LIGHTTYPE DstLightBuffer[16 * 16 * 256 / 2];
		CopyAll(buffer, &ChunkLightData::GetBlockLightSection, ChunkLightData::DefaultBlockLightValue, DstLightBuffer);
		TEST_EQUAL(memcmp(SrcLightBuffer, DstLightBuffer, (16 * 16 * 256 /2) - 1), 0);

		memset(SrcLightBuffer, 0x00, 16 * 16 * 256 /2);
		buffer.SetAll(SrcLightBuffer, SrcLightBuffer);
		CopyAll(buffer, &ChunkLightData::GetBlockLightSection, ChunkLightData::DefaultBlockLightValue, DstLightBuffer);
		TEST_EQUAL(memcmp(SrcLightBuffer, DstLightBuffer, (16 * 16 * 256 /2) - 1), 0);
	}

	{
		ChunkLightData buffer;

		LIGHTTYPE SrcLightBuffer[16 * 16 * 256 / 2];
		for (int i = 0; i < 16 * 16 * 256 / 2; i += 4)
		{
			SrcLightBuffer[i + 0] = 0xde;
			SrcLightBuffer[i + 1] = 0xad;
			SrcLightBuffer[i + 2] = 0xbe;
			SrcLightBuffer[i + 3] = 0xef;
		}

		buffer.SetAll(SrcLightBuffer, SrcLightBuffer);
		LIGHTTYPE DstLightBuffer[16 * 16 * 256/ 2];
		CopyAll(buffer, &ChunkLightData::GetSkyLightSection, ChunkLightData::DefaultSkyLightValue, DstLightBuffer);
		TEST_EQUAL(memcmp(SrcLightBuffer, DstLightBuffer, (16 * 16 * 256 / 2) - 1), 0);

		memset(SrcLightBuffer, 0xFF, 16 * 16 * 256 / 2);
		buffer.SetAll(SrcLightBuffer, SrcLightBuffer);
		CopyAll(buffer, &ChunkLightData::GetSkyLightSection, ChunkLightData::DefaultSkyLightValue, DstLightBuffer);
		TEST_EQUAL(memcmp(SrcLightBuffer, DstLightBuffer, (16 * 16 * 256 / 2) - 1), 0);
	}

	{
		ChunkBlockData buffer;

		BlockState SrcBlockBuffer[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		BlockState DstBlockBuffer[16 * 16 * 256];
		CopyAll(buffer, &ChunkBlockData::GetSection, ChunkBlockData::DefaultValue, DstBlockBuffer);
		TEST_EQUAL(memcmp(SrcBlockBuffer, DstBlockBuffer, (16 * 16 * 256) - 1), 0);
	}

	{
		ChunkLightData buffer;

		LIGHTTYPE SrcLightBuffer[16 * 16 * 256 / 2];
		memset(SrcLightBuffer, 0x00, 16 * 16 * 256 / 2);
		LIGHTTYPE DstLightBuffer[16 * 16 * 256 / 2];
		CopyAll(buffer, &ChunkLightData::GetBlockLightSection, ChunkLightData::DefaultBlockLightValue, DstLightBuffer);
		TEST_EQUAL(memcmp(SrcLightBuffer, DstLightBuffer, (16 * 16 * 256 / 2) - 1), 0);

		memset(SrcLightBuffer, 0xFF, 16 * 16 * 256 / 2);
		CopyAll(buffer, &ChunkLightData::GetSkyLightSection, ChunkLightData::DefaultSkyLightValue, DstLightBuffer);
		TEST_EQUAL(memcmp(SrcLightBuffer, DstLightBuffer, (16 * 16 * 256 / 2) - 1), 0);
	}
}





IMPLEMENT_TEST_MAIN("ChunkData Copies",
	Test()
)
