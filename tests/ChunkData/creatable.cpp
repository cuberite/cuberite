
#include "TestGlobals.h"
#include "ChunkData.h"

int main(int argc, char** argv)
{
	class cStarvationCallbacks
		: public cAllocationPool<cChunkData::sChunkSection,1600>::cStarvationCallbacks
	{
		virtual void OnStartingUsingBuffer() {}
		virtual void OnStopUsingBuffer() {}
		virtual void OnBufferEmpty() {}
	};
	cAllocationPool<cChunkData::sChunkSection,1600> Pool(std::auto_ptr<cAllocationPool<cChunkData::sChunkSection,1600>::cStarvationCallbacks>(new cStarvationCallbacks()));
	cChunkData buffer(Pool);
	return 0;
}
