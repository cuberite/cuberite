
#include "TestGlobals.h"
#include "ChunkBuffer.h"



int main(int argc, char** argv)
{
	cChunkBuffer buffer;
	buffer.SetBlock(0,0,0, 0xAB);
	assert(buffer.GetBlock(0,0,0) == 0xAB);
	buffer.SetMeta(0,16,0, 0xC);
	assert(buffer.GetMeta(0,16,0) == 0xC);
	return 0;
}
