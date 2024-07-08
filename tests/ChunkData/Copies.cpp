
#include "Globals.h"
#include "../TestHelpers.h"
#include "ChunkData.h"





/** Helper that copies a data store into a contiguous flat array, filling in a default value for sections that aren't present. */
template <class StoreType, typename GetType, typename DefaultType, typename OutType>
static void CopyAll(const StoreType & Data, GetType Getter, DefaultType Default, OutType & Out)
{
	constexpr auto SectionBlockCount = std::tuple_size_v<OutType> / cChunkDef::NumSections;

	static_assert(std::is_same_v<OutType, std::array<typename OutType::value_type, std::tuple_size<OutType>::value>>, "arg must be an std::array");

	for (size_t Y = 0; Y < cChunkDef::NumSections; Y++)
	{
		const auto & Section = (Data.*Getter)(Y);
		using ElementType = typename std::remove_reference_t<decltype(Section)>::element_type;
		static_assert(SectionBlockCount == std::tuple_size_v<ElementType>, "Output array has wrong size");

		if (Section == nullptr)
		{
			std::fill_n(std::begin(Out) + Y * SectionBlockCount, SectionBlockCount, Default);
		}
		else
		{
			std::copy(Section->begin(), Section->end(), std::begin(Out) + Y * SectionBlockCount);
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

		cChunkDef::BlockTypes SrcBlockBuffer;
		cChunkDef::BlockNibbles SrcNibbleBuffer;
		for (int i = 0; i < cChunkDef::NumBlocks; i += 4)
		{
			SrcBlockBuffer[i + 0] = 0xde;
			SrcBlockBuffer[i + 1] = 0xad;
			SrcBlockBuffer[i + 2] = 0xbe;
			SrcBlockBuffer[i + 3] = 0xef;
		}

		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);
		cChunkDef::BlockTypes DstBlockBuffer;
		CopyAll(buffer, &ChunkBlockData::GetSection, ChunkBlockData::DefaultValue, DstBlockBuffer);
		TEST_TRUE(std::equal(SrcBlockBuffer.begin(), SrcBlockBuffer.end(), DstBlockBuffer.begin()));

		SrcBlockBuffer.fill(0x00);
		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);
		CopyAll(buffer, &ChunkBlockData::GetSection, ChunkBlockData::DefaultValue, DstBlockBuffer);
		TEST_TRUE(std::equal(SrcBlockBuffer.begin(), SrcBlockBuffer.end(), DstBlockBuffer.begin()));
	}

	{
		ChunkBlockData buffer;

		cChunkDef::BlockTypes SrcBlockBuffer;
		cChunkDef::BlockNibbles SrcNibbleBuffer;
		for (int i = 0; i < cChunkDef::NumBlocks / 2; i += 4)
		{
			SrcNibbleBuffer[i + 0] = 0xde;
			SrcNibbleBuffer[i + 1] = 0xad;
			SrcNibbleBuffer[i + 2] = 0xbe;
			SrcNibbleBuffer[i + 3] = 0xef;
		}

		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);
		cChunkDef::BlockNibbles DstNibbleBuffer;
		CopyAll(buffer, &ChunkBlockData::GetMetaSection, ChunkBlockData::DefaultMetaValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer.data(), DstNibbleBuffer.data(), cChunkDef::NumBlocks / 2 - 1), 0);

		SrcNibbleBuffer.fill(0x00);
		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);
		CopyAll(buffer, &ChunkBlockData::GetMetaSection, ChunkBlockData::DefaultMetaValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer.data(), DstNibbleBuffer.data(), cChunkDef::NumBlocks / 2 - 1), 0);
	}

	{
		ChunkLightData buffer;

		cChunkDef::BlockNibbles SrcNibbleBuffer;
		for (int i = 0; i < cChunkDef::NumBlocks / 2; i += 4)
		{
			SrcNibbleBuffer[i + 0] = 0xde;
			SrcNibbleBuffer[i + 1] = 0xad;
			SrcNibbleBuffer[i + 2] = 0xbe;
			SrcNibbleBuffer[i + 3] = 0xef;
		}

		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer);
		cChunkDef::BlockNibbles DstNibbleBuffer;
		CopyAll(buffer, &ChunkLightData::GetBlockLightSection, ChunkLightData::DefaultBlockLightValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer.data(), DstNibbleBuffer.data(), cChunkDef::NumBlocks / 2 - 1), 0);

		SrcNibbleBuffer.fill(0x00);
		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer);
		CopyAll(buffer, &ChunkLightData::GetBlockLightSection, ChunkLightData::DefaultBlockLightValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer.data(), DstNibbleBuffer.data(), cChunkDef::NumBlocks / 2 - 1), 0);
	}

	{
		ChunkLightData buffer;

		cChunkDef::BlockNibbles SrcNibbleBuffer;
		for (int i = 0; i < cChunkDef::NumBlocks / 2; i += 4)
		{
			SrcNibbleBuffer[i + 0] = 0xde;
			SrcNibbleBuffer[i + 1] = 0xad;
			SrcNibbleBuffer[i + 2] = 0xbe;
			SrcNibbleBuffer[i + 3] = 0xef;
		}

		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer);
		cChunkDef::BlockNibbles DstNibbleBuffer;
		CopyAll(buffer, &ChunkLightData::GetSkyLightSection, ChunkLightData::DefaultSkyLightValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer.data(), DstNibbleBuffer.data(), cChunkDef::NumBlocks / 2 - 1), 0);

		SrcNibbleBuffer.fill(0xFF);
		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer);
		CopyAll(buffer, &ChunkLightData::GetSkyLightSection, ChunkLightData::DefaultSkyLightValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer.data(), DstNibbleBuffer.data(), cChunkDef::NumBlocks / 2 - 1), 0);
	}

	{
		ChunkBlockData buffer;

		cChunkDef::BlockTypes SrcBlockBuffer;
		SrcBlockBuffer.fill(0x00);
		cChunkDef::BlockTypes DstBlockBuffer;
		CopyAll(buffer, &ChunkBlockData::GetSection, ChunkBlockData::DefaultValue, DstBlockBuffer);
		TEST_EQUAL(memcmp(SrcBlockBuffer.data(), DstBlockBuffer.data(), cChunkDef::NumBlocks - 1), 0);

		cChunkDef::BlockNibbles SrcNibbleBuffer;
		SrcNibbleBuffer.fill(0x00);
		cChunkDef::BlockNibbles DstNibbleBuffer;
		CopyAll(buffer, &ChunkBlockData::GetMetaSection, ChunkBlockData::DefaultMetaValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer.data(), DstNibbleBuffer.data(), cChunkDef::NumBlocks / 2 - 1), 0);
	}

	{
		ChunkLightData buffer;

		cChunkDef::BlockNibbles SrcNibbleBuffer;
		SrcNibbleBuffer.fill(0x00);
		cChunkDef::BlockNibbles DstNibbleBuffer;
		CopyAll(buffer, &ChunkLightData::GetBlockLightSection, ChunkLightData::DefaultBlockLightValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer.data(), DstNibbleBuffer.data(), cChunkDef::NumBlocks / 2 - 1), 0);

		SrcNibbleBuffer.fill(0xFF);
		CopyAll(buffer, &ChunkLightData::GetSkyLightSection, ChunkLightData::DefaultSkyLightValue, DstNibbleBuffer);
		TEST_EQUAL(memcmp(SrcNibbleBuffer.data(), DstNibbleBuffer.data(), cChunkDef::NumBlocks / 2 - 1), 0);
	}
}





IMPLEMENT_TEST_MAIN("ChunkData Copies",
	Test()
)
