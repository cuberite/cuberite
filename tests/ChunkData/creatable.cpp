
#include "Globals.h"
#include "ChunkData.h"
#include "MemoryCounter.h"

int main(int argc, char** argv)
{
	cMemoryCounter Dummy;
	class cMockAllocationPool
		: public cAllocationPool<cChunkData::sChunkSection>
	{
		virtual cChunkData::sChunkSection * Allocate()
		{
			return new cChunkData::sChunkSection();
		}

		virtual void Free(cChunkData::sChunkSection * a_Ptr)
		{
			delete a_Ptr;
		}
	} Pool;
	cChunkData buffer(Pool, Dummy);
	return 0;
}
