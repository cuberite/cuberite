
#include "Globals.h"
#include "ChunkData.h"

int main(int argc, char** argv)
{
	LOGD("Test started");

	class cMockAllocationPool
		: public cAllocationPool<cChunkData::sChunkSection>
	{
		virtual cChunkData::sChunkSection * Allocate() override
		{
			return new cChunkData::sChunkSection();
		}

		virtual void Free(cChunkData::sChunkSection * a_Ptr) override
		{
			delete a_Ptr;
		}

		virtual bool DoIsEqual(const cAllocationPool<cChunkData::sChunkSection> &) const noexcept override
		{
			return false;
		}
	} Pool;
	cChunkData buffer(Pool);
	return 0;
}
