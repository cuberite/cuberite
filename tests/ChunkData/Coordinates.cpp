
#include "Globals.h"
#include "ChunkData.h"



int main(int argc, char** argv)
{
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
	{
		cChunkData buffer(Pool);

		// Empty chunks
		buffer.SetBlock(0, 0, 0, 0xAB);
		testassert(buffer.GetBlock(0, 0, 0) == 0xAB);
		buffer.SetMeta(0, 16, 0, 0xC);
		testassert(buffer.GetMeta(0, 16, 0) == 0xC);

		// loaded but not written segments
		testassert(buffer.GetBlock(1, 0, 0) == 0x0);
		testassert(buffer.GetMeta(1, 16, 0) == 0x0);

		// Notloaded segments
		testassert(buffer.GetBlock(0, 32, 0) == 0x0);
		testassert(buffer.GetMeta(0, 48, 0) == 0x0);

		// Out of Range
		CheckAsserts(
			buffer.SetBlock(-1, 0, 0, 0);
		);
		CheckAsserts(
			buffer.SetBlock(0, -1, 0, 0);
		);
		CheckAsserts(
			buffer.SetBlock(0, 0, -1, 0);
		);
		CheckAsserts(
			buffer.SetBlock(256, 0, 0, 0);
		);
		CheckAsserts(
			buffer.SetBlock(0, 256, 0, 0);
		);
		CheckAsserts(
			buffer.SetBlock(0, 0, 256, 0);
		);

		// Out of Range
		CheckAsserts(
			buffer.GetBlock(-1, 0, 0);
		);
		CheckAsserts(
			buffer.GetBlock(0, -1, 0);
		);
		CheckAsserts(
			buffer.GetBlock(0, 0, -1);
		);
		CheckAsserts(
			buffer.GetBlock(256, 0, 0);
		);
		CheckAsserts(
			buffer.GetBlock(0, 256, 0);
		);
		CheckAsserts(
			buffer.GetBlock(0, 0, 256);
		);

		// Out of Range
		CheckAsserts(
			buffer.SetMeta(-1, 0, 0, 0);
		);
		CheckAsserts(
			buffer.SetMeta(0, -1, 0, 0);
		);
		CheckAsserts(
			buffer.SetMeta(0, 0, -1, 0);
		);
		CheckAsserts(
			buffer.SetMeta(256, 0, 0, 0);
		);
		CheckAsserts(
			buffer.SetMeta(0, 256, 0, 0);
		);
		CheckAsserts(
			buffer.SetMeta(0, 0, 256, 0);
		);

		// Out of Range
		CheckAsserts(
			buffer.GetMeta(-1, 0, 0);
		);
		CheckAsserts(
			buffer.GetMeta(0, -1, 0);
		);
		CheckAsserts(
			buffer.GetMeta(0, 0, -1);
		);
		CheckAsserts(
			buffer.GetMeta(256, 0, 0);
		);
		CheckAsserts(
			buffer.GetMeta(0, 256, 0);
		);
		CheckAsserts(
			buffer.GetMeta(0, 0, 256);
		);
	}
	
	{
		cChunkData buffer(Pool);
		
		// Zero's
		buffer.SetBlock(0, 0, 0, 0x0);
		buffer.SetBlock(0, 0, 1, 0xab);
		testassert(buffer.GetBlock(0, 0, 0) == 0x0);
		testassert(buffer.GetBlock(0, 0, 1) == 0xab);

		buffer.SetMeta(0, 16, 0, 0x0);
		buffer.SetMeta(0, 16, 1, 0xc);
		testassert(buffer.GetMeta(0, 16, 0) == 0x0);
		testassert(buffer.GetMeta(0, 16, 1) == 0xc);
	}
	
	
	{
		// Operator =
		cChunkData buffer(Pool);
		buffer.SetBlock(0, 0, 0, 0x42);
		cChunkData copy(Pool);
		#if __cplusplus < 201103L
		copy = buffer;
		#else
		copy = std::move(buffer);
		#endif
		testassert(copy.GetBlock(0, 0, 0) == 0x42);
		#if __cplusplus < 201103L
		copy = copy;
		#else
		copy = std::move(copy);
		#endif
		testassert(copy.GetBlock(0, 0, 0) == 0x42);
	}
	
	return 0;
}
