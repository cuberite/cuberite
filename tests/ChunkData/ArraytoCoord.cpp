
#include "Globals.h"
#include "../TestHelpers.h"
#include "ChunkData.h"



/** Performs the entire ArrayToCoords test. */
static void test()
{
	{
		// Test first segment (blocks)
		ChunkBlockData buffer;

		cChunkDef::BlockTypes SrcBlockBuffer;
		SrcBlockBuffer.fill(0x00);
		SrcBlockBuffer[7 + (4 * 16) + (5 * 16 * 16)] = 0xcd;

		cChunkDef::BlockNibbles SrcNibbleBuffer;
		SrcNibbleBuffer.fill(0x00);
		SrcNibbleBuffer[(6 + (1 * 16) + (2 * 16 * 16)) / 2] = 0xe;

		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);

		TEST_EQUAL(buffer.GetBlock({ 7, 5, 4 }), 0xcd);
		TEST_EQUAL(buffer.GetMeta({ 6, 2, 1 }), 0xe);
	}

	{
		// Test first segment (lights)
		ChunkLightData buffer;

		cChunkDef::BlockNibbles SrcNibbleBuffer;
		SrcNibbleBuffer.fill(0x00);
		SrcNibbleBuffer[(6 + (1 * 16) + (2 * 16 * 16)) / 2] = 0xe;

		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer);

		TEST_EQUAL(buffer.GetBlockLight({ 6, 2, 1 }), 0xe);
		TEST_EQUAL(buffer.GetSkyLight({ 6, 2, 1 }), 0xe);
	}

	{
		// test following segment (blocks)
		ChunkBlockData buffer;

		cChunkDef::BlockTypes SrcBlockBuffer;
		SrcBlockBuffer.fill(0x00);
		SrcBlockBuffer[7 + (4 * 16) + (24 * 16 * 16)] = 0xcd;

		cChunkDef::BlockNibbles SrcNibbleBuffer;
		SrcNibbleBuffer.fill(0x00);
		SrcNibbleBuffer[(6 + (1 * 16) + (24 * 16 * 16)) / 2] = 0xe;

		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);

		TEST_EQUAL(buffer.GetBlock({ 7, 24, 4 }), 0xcd);
		TEST_EQUAL(buffer.GetMeta({ 6, 24, 1 }), 0xe);
	}

	{
		// test following segment (lights)
		ChunkLightData buffer;

		cChunkDef::BlockNibbles SrcNibbleBuffer;
		SrcNibbleBuffer.fill(0x00);
		SrcNibbleBuffer[(6 + (1 * 16) + (24 * 16 * 16)) / 2] = 0xe;

		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer);

		TEST_EQUAL(buffer.GetBlockLight({ 6, 24, 1 }), 0xe);
		TEST_EQUAL(buffer.GetSkyLight({ 6, 24, 1 }), 0xe);
	}

	{
		// test zeros (blocks)
		ChunkBlockData buffer;

		cChunkDef::BlockTypes SrcBlockBuffer;
		SrcBlockBuffer.fill(0x00);

		cChunkDef::BlockNibbles SrcNibbleBuffer;
		SrcNibbleBuffer.fill(0x00);

		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);

		TEST_EQUAL(buffer.GetBlock({ 7, 24, 4 }), 0x00);
		TEST_EQUAL(buffer.GetMeta({ 6, 24, 1 }), 0x0);
	}

	{
		// test zeros (lights)
		ChunkLightData buffer;

		cChunkDef::BlockNibbles SrcNibbleBuffer;
		SrcNibbleBuffer.fill(0x00);

		cChunkDef::BlockNibbles SrcNibbleBuffer2;
		SrcNibbleBuffer2.fill(0xFF);

		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer2);

		TEST_EQUAL(buffer.GetBlockLight({ 6, 24, 1 }), 0x0);
		TEST_EQUAL(buffer.GetSkyLight({ 6, 24, 1 }), 0xf);
	}
}





IMPLEMENT_TEST_MAIN("ChunkData ArrayToCoord",
	test();
)
