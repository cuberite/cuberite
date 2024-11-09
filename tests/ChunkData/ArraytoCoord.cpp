
#include "Globals.h"
#include "../TestHelpers.h"
#include "ChunkData.h"



/** Performs the entire ArrayToCoords test. */
static void test()
{
	{
		// Test first segment (blocks)
		ChunkBlockData buffer;

		BLOCKTYPE SrcBlockBuffer[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, sizeof(SrcBlockBuffer));
		SrcBlockBuffer[7 + (4 * 16) + (5 * 16 * 16)] = 0xcd;

		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		memset(SrcNibbleBuffer, 0x00, sizeof(SrcNibbleBuffer));
		SrcNibbleBuffer[(6 + (1 * 16) + (2 * 16 * 16)) / 2] = 0xe;

		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);

		TEST_EQUAL(buffer.GetBlock({ 7, 5, 4 }), 0xcd);
		TEST_EQUAL(buffer.GetMeta({ 6, 2, 1 }), 0xe);
	}

	{
		// Test first segment (lights)
		ChunkLightData buffer;

		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		memset(SrcNibbleBuffer, 0x00, sizeof(SrcNibbleBuffer));
		SrcNibbleBuffer[(6 + (1 * 16) + (2 * 16 * 16)) / 2] = 0xe;

		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer);

		TEST_EQUAL(buffer.GetBlockLight({ 6, 2, 1 }), 0xe);
		TEST_EQUAL(buffer.GetSkyLight({ 6, 2, 1 }), 0xe);
	}

	{
		// test following segment (blocks)
		ChunkBlockData buffer;

		BLOCKTYPE SrcBlockBuffer[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, sizeof(SrcBlockBuffer));
		SrcBlockBuffer[7 + (4 * 16) + (24 * 16 * 16)] = 0xcd;

		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		memset(SrcNibbleBuffer, 0x00, sizeof(SrcNibbleBuffer));
		SrcNibbleBuffer[(6 + (1 * 16) + (24 * 16 * 16)) / 2] = 0xe;

		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);

		TEST_EQUAL(buffer.GetBlock({ 7, 24, 4 }), 0xcd);
		TEST_EQUAL(buffer.GetMeta({ 6, 24, 1 }), 0xe);
	}

	{
		// test following segment (lights)
		ChunkLightData buffer;

		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		memset(SrcNibbleBuffer, 0x00, sizeof(SrcNibbleBuffer));
		SrcNibbleBuffer[(6 + (1 * 16) + (24 * 16 * 16)) / 2] = 0xe;

		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer);

		TEST_EQUAL(buffer.GetBlockLight({ 6, 24, 1 }), 0xe);
		TEST_EQUAL(buffer.GetSkyLight({ 6, 24, 1 }), 0xe);
	}

	{
		// test zeros (blocks)
		ChunkBlockData buffer;

		BLOCKTYPE SrcBlockBuffer[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, sizeof(SrcBlockBuffer));

		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		memset(SrcNibbleBuffer, 0x00, sizeof(SrcNibbleBuffer));

		buffer.SetAll(SrcBlockBuffer, SrcNibbleBuffer);

		TEST_EQUAL(buffer.GetBlock({ 7, 24, 4 }), 0x00);
		TEST_EQUAL(buffer.GetMeta({ 6, 24, 1 }), 0x0);
	}

	{
		// test zeros (lights)
		ChunkLightData buffer;

		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		memset(SrcNibbleBuffer, 0x00, sizeof(SrcNibbleBuffer));

		NIBBLETYPE SrcNibbleBuffer2[16 * 16 * 256 / 2];
		memset(SrcNibbleBuffer2, 0xff, sizeof(SrcNibbleBuffer2));

		buffer.SetAll(SrcNibbleBuffer, SrcNibbleBuffer2);

		TEST_EQUAL(buffer.GetBlockLight({ 6, 24, 1 }), 0x0);
		TEST_EQUAL(buffer.GetSkyLight({ 6, 24, 1 }), 0xf);
	}
}





IMPLEMENT_TEST_MAIN("ChunkData ArrayToCoord",
	test();
)
