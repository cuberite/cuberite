
#include "Globals.h"
#include "ChunkData.h"

int main(int argc, char** argv)
{
	LOGD("Test started");

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
	cChunkData buffer(Pool);
	return 0;
}
