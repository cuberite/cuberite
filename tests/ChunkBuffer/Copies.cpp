
#include "TestGlobals.h"
#include "ChunkBuffer.h"



int main(int argc, char** argv)
{
	{
		cChunkBuffer buffer;
	
		buffer.SetBlock(3,1,4,0xDE);
		buffer.SetMeta(3,1,4,0xA);
	
		cChunkBuffer copy = buffer.Copy();
		testassert(copy.GetBlock(3,1,4) == 0xDE);
		testassert(copy.GetMeta(3,1,4) == 0xA);
	
		BLOCKTYPE * SrcBlockBuffer = new BLOCKTYPE[16 * 16 * 256];
		for (int i = 0; i < 16 * 16 * 256; i += 4)
		{
			SrcBlockBuffer[i+0] = 0xDE;
			SrcBlockBuffer[i+1] = 0xAD;
			SrcBlockBuffer[i+2] = 0xBE;
			SrcBlockBuffer[i+3] = 0xEF;
		}
	
		buffer.SetBlocks(SrcBlockBuffer);
		BLOCKTYPE * DstBlockBuffer = new BLOCKTYPE[16 * 16 * 256];
		buffer.CopyBlocks(DstBlockBuffer);
		testassert(memcmp(SrcBlockBuffer, DstBlockBuffer, (16 * 16 * 256) -1) == 0);
		delete SrcBlockBuffer;
		delete DstBlockBuffer;
		SrcBlockBuffer = NULL;
		DstBlockBuffer = NULL;
	
		NIBBLETYPE * SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		for (int i = 0; i < 16 * 16 * 256 / 2; i += 4)
		{
			SrcNibbleBuffer[i+0] = 0xEF;
			SrcNibbleBuffer[i+1] = 0xDE;
			SrcNibbleBuffer[i+2] = 0xAD;
			SrcNibbleBuffer[i+3] = 0xBE;
		}
	
		buffer.SetMeta(SrcNibbleBuffer);
		NIBBLETYPE * DstNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/ 2];
		buffer.CopyMeta(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 /2) -1) == 0);
		delete SrcNibbleBuffer;
		delete DstNibbleBuffer;
		SrcNibbleBuffer = NULL;
		DstNibbleBuffer = NULL;
	
		SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		for (int i = 0; i < 16 * 16 * 256 / 2; i += 4)
		{
			SrcNibbleBuffer[i+0] = 0xDE;
			SrcNibbleBuffer[i+1] = 0xAD;
			SrcNibbleBuffer[i+2] = 0xBE;
			SrcNibbleBuffer[i+3] = 0xEF;
		}
	
		buffer.SetLight(SrcNibbleBuffer);
		DstNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/ 2];
		buffer.CopyLight(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 /2) -1) == 0);
		delete SrcNibbleBuffer;
		delete DstNibbleBuffer;
		SrcNibbleBuffer = NULL;
		DstNibbleBuffer = NULL;
	
		SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		for (int i = 0; i < 16 * 16 * 256 / 2; i += 4)
		{
			SrcNibbleBuffer[i+0] = 0xAD;
			SrcNibbleBuffer[i+1] = 0xBE;
			SrcNibbleBuffer[i+2] = 0xEF;
			SrcNibbleBuffer[i+3] = 0xDE;
		}
	
		buffer.SetSkyLight(SrcNibbleBuffer);
		DstNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/ 2];
		buffer.CopySkyLight(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 /2) -1) == 0);
		delete SrcNibbleBuffer;
		delete DstNibbleBuffer;
		SrcNibbleBuffer = NULL;
		DstNibbleBuffer = NULL;
	
		SrcBlockBuffer = new BLOCKTYPE[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		buffer.SetBlocks(SrcBlockBuffer);
		DstBlockBuffer = new BLOCKTYPE[16 * 16 * 256];
		buffer.CopyBlocks(DstBlockBuffer);
		testassert(memcmp(SrcBlockBuffer, DstBlockBuffer, (16 * 16 * 256) -1) == 0);
		delete SrcBlockBuffer;
		delete DstBlockBuffer;
		SrcBlockBuffer = NULL;
		DstBlockBuffer = NULL;
	
		SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		buffer.SetMeta(SrcNibbleBuffer);
		DstNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/ 2];
		buffer.CopyMeta(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 /2) -1) == 0);
		delete SrcNibbleBuffer;
		delete DstNibbleBuffer;
		SrcNibbleBuffer = NULL;
		DstNibbleBuffer = NULL;
	
		SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		buffer.SetLight(SrcNibbleBuffer);
		DstNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/ 2];
		buffer.CopyLight(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 /2) -1) == 0);
		delete SrcNibbleBuffer;
		delete DstNibbleBuffer;
		SrcNibbleBuffer = NULL;
		DstNibbleBuffer = NULL;
	
		SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0xFF, 16 * 16 * 256 /2);
		buffer.SetSkyLight(SrcNibbleBuffer);
		DstNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/ 2];
		buffer.CopySkyLight(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 /2) -1) == 0);
		delete SrcNibbleBuffer;
		delete DstNibbleBuffer;
		SrcNibbleBuffer = NULL;
		DstNibbleBuffer = NULL;
	}
	{
		cChunkBuffer buffer;
		
		BLOCKTYPE * SrcBlockBuffer = new BLOCKTYPE[16 * 16 * 256];
		memset(SrcBlockBuffer, 0x00, 16 * 16 * 256);
		BLOCKTYPE * DstBlockBuffer = new BLOCKTYPE[16 * 16 * 256];
		buffer.CopyBlocks(DstBlockBuffer);
		testassert(memcmp(SrcBlockBuffer, DstBlockBuffer, (16 * 16 * 256) -1) == 0);
		delete SrcBlockBuffer;
		delete DstBlockBuffer;
		SrcBlockBuffer = NULL;
		DstBlockBuffer = NULL;
	
		NIBBLETYPE * SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		NIBBLETYPE * DstNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/ 2];
		buffer.CopyMeta(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 /2) -1) == 0);
		delete SrcNibbleBuffer;
		delete DstNibbleBuffer;
		SrcNibbleBuffer = NULL;
		DstNibbleBuffer = NULL;
	
		SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0x00, 16 * 16 * 256 /2);
		DstNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/ 2];
		buffer.CopyLight(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 /2) -1) == 0);
		delete SrcNibbleBuffer;
		delete DstNibbleBuffer;
		SrcNibbleBuffer = NULL;
		DstNibbleBuffer = NULL;
	
		SrcNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/2];
		memset(SrcNibbleBuffer, 0xFF, 16 * 16 * 256 /2);
		DstNibbleBuffer = new NIBBLETYPE[16 * 16 * 256/ 2];
		buffer.CopySkyLight(DstNibbleBuffer);
		testassert(memcmp(SrcNibbleBuffer, DstNibbleBuffer, (16 * 16 * 256 /2) -1) == 0);
		delete SrcNibbleBuffer;
		delete DstNibbleBuffer;
		SrcNibbleBuffer = NULL;
		DstNibbleBuffer = NULL;
	}
}
