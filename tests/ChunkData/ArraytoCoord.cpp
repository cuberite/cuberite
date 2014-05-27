
#include "Globals.h"
#include "ChunkData.h"



int main(int argc, char** argv)
{
	{
		// Test first segment
		cChunkData buffer;

		BLOCKTYPE SrcBlockBuffer[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		SrcBlockBuffer[7 + (4 * 16) + (5 * 16 * 16)] = 0xCD;
		buffer.SetBlocks(SrcBlockBuffer);
		testassert(buffer.GetBlock(7,5,4) == 0xCD);
	
		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 / 2);
		SrcNibbleBuffer[(6 + (1 * 16) + (2 * 16 * 16)) / 2] = 0xE;
		buffer.SetMeta(SrcNibbleBuffer);
		testassert(buffer.GetMeta(6, 2, 1) == 0xE);
	
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 / 2);
		SrcNibbleBuffer[(6 + (1 * 16) + (2 * 16 * 16)) / 2] = 0xE;
		buffer.SetBlockLight(SrcNibbleBuffer);
		testassert(buffer.GetBlockLight(6, 2, 1) == 0xE);
	
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 / 2);
		SrcNibbleBuffer[(6 + (1 * 16) + (2 * 16 * 16)) / 2] = 0xE;
		buffer.SetSkyLight(SrcNibbleBuffer);
		testassert(buffer.GetSkyLight(6, 2, 1) == 0xE);
	}
	
	{
		// test following segment
		cChunkData buffer;

		BLOCKTYPE SrcBlockBuffer[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		SrcBlockBuffer[7 + (4 * 16) + (24 * 16 * 16)] = 0xCD;
		buffer.SetBlocks(SrcBlockBuffer);
		testassert(buffer.GetBlock(7, 24, 4) == 0xCD);
	
		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 / 2);
		SrcNibbleBuffer[(6 + (1 * 16) + (24 * 16 * 16)) / 2] = 0xE;
		buffer.SetMeta(SrcNibbleBuffer);
		testassert(buffer.GetMeta(6, 24, 1) == 0xE);
	
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		SrcNibbleBuffer[(6+1*16+24*16*16)/2] = 0xE;
		buffer.SetBlockLight(SrcNibbleBuffer);
		testassert(buffer.GetBlockLight(6,24,1) == 0xE);
	
		memset(SrcNibbleBuffer, 0xFF, 16 * 16 * 256 / 2);
		SrcNibbleBuffer[(6 + (1 * 16) + (24 * 16 * 16))/2] = 0xE;
		buffer.SetSkyLight(SrcNibbleBuffer);
		testassert(buffer.GetSkyLight(6, 24, 1) == 0xE);
	}
	
	{
		// test zeros
		cChunkData buffer;

		BLOCKTYPE SrcBlockBuffer[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		buffer.SetBlocks(SrcBlockBuffer);
		testassert(buffer.GetBlock(7, 24, 4) == 0x00);
	
		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		buffer.SetMeta(SrcNibbleBuffer);
		testassert(buffer.GetMeta(6, 24, 1) == 0x0);
	
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		buffer.SetBlockLight(SrcNibbleBuffer);
		testassert(buffer.GetBlockLight(6,24,1) == 0x0);
	
		memset(SrcNibbleBuffer, 0xFF, 16 * 16 * 256 /2);
		buffer.SetSkyLight(SrcNibbleBuffer);
		testassert(buffer.GetSkyLight(6, 24, 1) == 0xF);
	}
}

