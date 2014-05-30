
// CopyBlocks.cpp

// Implements the test for cChunkData::CopyBlockTypes() range copying





#include "Globals.h"
#include "ChunkData.h"





int main(int argc, char ** argv)
{
	// Set up a cChunkData with known contents - all blocks 0x01, all metas 0x02:
	cChunkData Data;
	cChunkDef::BlockTypes   BlockTypes;
	cChunkDef::BlockNibbles BlockMetas;
	memset(BlockTypes, 0x01, sizeof(BlockTypes));
	memset(BlockMetas, 0x02, sizeof(BlockMetas));
	Data.SetBlockTypes(BlockTypes);
	Data.SetMetas(BlockMetas);

	// Try to read varying amounts of blocktypes from the cChunkData.
	// Verify that the exact amount of memory is copied, by copying to a larger buffer and checking its boundaries
	BLOCKTYPE TestBuffer[5 * cChunkDef::NumBlocks];
	size_t WritePosIdx = 2 * cChunkDef::NumBlocks;
	BLOCKTYPE * WritePosition = &TestBuffer[WritePosIdx];
	memset(TestBuffer, 0x03, sizeof(TestBuffer));
	for (size_t idx = 0; idx < 5000; idx++)
	{
		for (size_t len = 1; len < 1000; len += 15)
		{
			printf("Testing copying %u blocks from index %u\n", (unsigned)len, (unsigned)idx);

			Data.CopyBlockTypes(WritePosition, idx, len);

			// Verify the data copied:
			for (size_t i = 0; i < len; i++)
			{
				assert_test(WritePosition[i] == 0x01);
			}
			// Verify the space before the copied data hasn't been changed:
			for (size_t i = 0; i < WritePosIdx + idx; i++)
			{
				assert_test(TestBuffer[i] == 0x03);
			}
			// Verify the space after the copied data hasn't been changed:
			for (size_t i = WritePosIdx + idx + len; i < ARRAYCOUNT(TestBuffer); i++)
			{
				assert_test(TestBuffer[i] == 0x03);
			}
		}
	}  // for idx
	return 0;
}





