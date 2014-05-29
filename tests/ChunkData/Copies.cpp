
#include "Globals.h"
#include "ChunkData.h"



int main(int argc, char** argv)
{
	{
		cChunkData buffer;
	
		buffer.SetBlock(3, 1, 4, 0xDE);
		buffer.SetMeta(3, 1, 4, 0xA);
	
		cChunkData copy = buffer.Copy();
		testassert(copy.GetBlock(3, 1, 4) == 0xDE);
		testassert(copy.GetMeta(3, 1, 4) == 0xA);
	
		BLOCKTYPE SrcBlockBuffer[16 * 16 * 256];
		for (int i = 0; i < 16 * 16 * 256; i += 4)
		{
			SrcBlockBuffer[i+0] = 0xDE;
			SrcBlockBuffer[i+1] = 0xAD;
			SrcBlockBuffer[i+2] = 0xBE;
			SrcBlockBuffer[i+3] = 0xEF;
		}
	
		buffer.SetBlockTypes(SrcBlockBuffer);
		BLOCKTYPE DstBlockBuffer[16 * 16 * 256];
		buffer.CopyBlockTypes(DstBlockBuffer);
		testassert(memcmp(SrcBlockBuffer, DstBlockBuffer, (16 * 16 * 256) - 1) == 0);
		
		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		buffer.SetBlockTypes(SrcBlockBuffer);
		buffer.CopyBlockTypes(DstBlockBuffer);
		testassert(memcmp(SrcBlockBuffer, DstBlockBuffer, (16 * 16 * 256) - 1) == 0);
	
	}
	{
		cChunkData buffer;
	
		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		for (int i = 0; i < 16 * 16 * 256 / 2; i += 4)
		{
			SrcNibbleBuffer[i+0] = 0xEF;
			SrcNibbleBuffer[i+1] = 0xDE;
			SrcNibbleBuffer[i+2] = 0xAD;
			SrcNibbleBuffer[i+3] = 0xBE;
		}
	
		buffer.SetMetas(SrcNibbleBuffer);
		NIBBLETYPE DstNibbleBuffer[16 * 16 * 256/ 2];
		buffer.CopyMetas(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1) == 0);
		
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		buffer.SetMetas(SrcNibbleBuffer);
		buffer.CopyMetas(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1) == 0);

	
	}
	{
		cChunkData buffer;
		
		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		for (int i = 0; i < 16 * 16 * 256 / 2; i += 4)
		{
			SrcNibbleBuffer[i+0] = 0xDE;
			SrcNibbleBuffer[i+1] = 0xAD;
			SrcNibbleBuffer[i+2] = 0xBE;
			SrcNibbleBuffer[i+3] = 0xEF;
		}
	
		buffer.SetBlockLight(SrcNibbleBuffer);
		NIBBLETYPE DstNibbleBuffer[16 * 16 * 256 / 2];
		buffer.CopyBlockLight(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 /2) - 1) == 0);
		
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		buffer.SetBlockLight(SrcNibbleBuffer);
		buffer.CopyBlockLight(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 /2) - 1) == 0);
	
		
	}
	{
		cChunkData buffer;
		
		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		for (int i = 0; i < 16 * 16 * 256 / 2; i += 4)
		{
			SrcNibbleBuffer[i+0] = 0xAD;
			SrcNibbleBuffer[i+1] = 0xBE;
			SrcNibbleBuffer[i+2] = 0xEF;
			SrcNibbleBuffer[i+3] = 0xDE;
		}
	
		buffer.SetSkyLight(SrcNibbleBuffer);
		NIBBLETYPE DstNibbleBuffer[16 * 16 * 256/ 2];
		buffer.CopySkyLight(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1) == 0);
	

		memset(SrcNibbleBuffer, 0xFF, 16 * 16 * 256 / 2);
		buffer.SetSkyLight(SrcNibbleBuffer);
		buffer.CopySkyLight(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1) == 0);
		
	}
	{
		cChunkData buffer;
		
		BLOCKTYPE SrcBlockBuffer[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		BLOCKTYPE DstBlockBuffer[16 * 16 * 256];
		buffer.CopyBlockTypes(DstBlockBuffer);
		testassert(memcmp(SrcBlockBuffer, DstBlockBuffer, (16 * 16 * 256) - 1) == 0);
	
		NIBBLETYPE SrcNibbleBuffer[16 * 16 * 256 / 2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 / 2);
		NIBBLETYPE DstNibbleBuffer[16 * 16 * 256 / 2];
		buffer.CopyMetas(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1) == 0);
	
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 / 2);
		buffer.CopyBlockLight(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1) == 0);
	
		memset(SrcNibbleBuffer, 0xFF, 16 * 16 * 256 / 2);
		buffer.CopySkyLight(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 / 2) - 1) == 0);
	}
}
