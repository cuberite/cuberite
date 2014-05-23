
#include "TestGlobals.h"
#include "ChunkData.h"



int main(int argc, char** argv)
{
	{
		// Test first segment
		cChunkData buffer;

		BLOCKTYPE*  SrcBlockBuffer = new BLOCKTYPE[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		SrcBlockBuffer[7+4*16+5*16*16] = 0xCD;
		buffer.SetBlocks(SrcBlockBuffer);
		testassert(buffer.GetBlock(7,5,4) == 0xCD);
		delete SrcBlockBuffer;
		SrcBlockBuffer = NULL;
	
		NIBBLETYPE * SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		SrcNibbleBuffer[(6+1*16+2*16*16)/2] = 0xE;
		buffer.SetMeta(SrcNibbleBuffer);
		testassert(buffer.GetMeta(6,2,1) == 0xE);
		delete SrcNibbleBuffer;
		SrcNibbleBuffer = NULL;
	
		SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		SrcNibbleBuffer[(6+1*16+2*16*16)/2] = 0xE;
		buffer.SetLight(SrcNibbleBuffer);
		testassert(buffer.GetBlockLight(6,2,1) == 0xE);
		delete SrcNibbleBuffer;
		SrcNibbleBuffer = NULL;
	
		SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		SrcNibbleBuffer[(6+1*16+2*16*16)/2] = 0xE;
		buffer.SetSkyLight(SrcNibbleBuffer);
		testassert(buffer.GetSkyLight(6,2,1) == 0xE);
		delete SrcNibbleBuffer;
		SrcNibbleBuffer = NULL;
	}
	
	{
		// test following segment
		cChunkData buffer;

		BLOCKTYPE*  SrcBlockBuffer = new BLOCKTYPE[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		SrcBlockBuffer[7+4*16+24*16*16] = 0xCD;
		buffer.SetBlocks(SrcBlockBuffer);
		testassert(buffer.GetBlock(7,24,4) == 0xCD);
		delete SrcBlockBuffer;
		SrcBlockBuffer = NULL;
	
		NIBBLETYPE * SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		SrcNibbleBuffer[(6+1*16+24*16*16)/2] = 0xE;
		buffer.SetMeta(SrcNibbleBuffer);
		testassert(buffer.GetMeta(6,24,1) == 0xE);
		delete SrcNibbleBuffer;
		SrcNibbleBuffer = NULL;
	
		SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		SrcNibbleBuffer[(6+1*16+24*16*16)/2] = 0xE;
		buffer.SetLight(SrcNibbleBuffer);
		testassert(buffer.GetBlockLight(6,24,1) == 0xE);
		delete SrcNibbleBuffer;
		SrcNibbleBuffer = NULL;
	
		SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		SrcNibbleBuffer[(6+1*16+24*16*16)/2] = 0xE;
		buffer.SetSkyLight(SrcNibbleBuffer);
		testassert(buffer.GetSkyLight(6,24,1) == 0xE);
		delete SrcNibbleBuffer;
		SrcNibbleBuffer = NULL;
	}
	
	{
		// test zeros
		cChunkData buffer;

		BLOCKTYPE*  SrcBlockBuffer = new BLOCKTYPE[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		buffer.SetBlocks(SrcBlockBuffer);
		testassert(buffer.GetBlock(7,24,4) == 0x00);
		delete SrcBlockBuffer;
		SrcBlockBuffer = NULL;
	
		NIBBLETYPE * SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		buffer.SetMeta(SrcNibbleBuffer);
		testassert(buffer.GetMeta(6,24,1) == 0x0);
		delete SrcNibbleBuffer;
		SrcNibbleBuffer = NULL;
	
		SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		buffer.SetLight(SrcNibbleBuffer);
		testassert(buffer.GetBlockLight(6,24,1) == 0x0);
		delete SrcNibbleBuffer;
		SrcNibbleBuffer = NULL;
	
		SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0xFF, 16 * 16 * 256 /2);
		buffer.SetSkyLight(SrcNibbleBuffer);
		testassert(buffer.GetSkyLight(6,24,1) == 0xF);
		delete SrcNibbleBuffer;
		SrcNibbleBuffer = NULL;
	}
}

