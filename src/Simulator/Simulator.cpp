
#include "Globals.h"

#include "../World.h"
#include "../BlockID.h"
#include "../Defines.h"
#include "../Chunk.h"
#include "../Cuboid.h"

#ifdef __clang__
	#pragma clang diagnostic ignored "-Wweak-template-vtables"
#endif  // __clang__



#include "Simulator.h"





void cSimulator::WakeUp(const Vector3i & a_Block, cChunk * a_Chunk)
{
	AddBlock(a_Block.x, a_Block.y, a_Block.z, a_Chunk);
	AddBlock(a_Block.x - 1, a_Block.y, a_Block.z, a_Chunk->GetNeighborChunk(a_Block.x - 1, a_Block.z));
	AddBlock(a_Block.x + 1, a_Block.y, a_Block.z, a_Chunk->GetNeighborChunk(a_Block.x + 1, a_Block.z));
	AddBlock(a_Block.x, a_Block.y, a_Block.z - 1, a_Chunk->GetNeighborChunk(a_Block.x, a_Block.z - 1));
	AddBlock(a_Block.x, a_Block.y, a_Block.z + 1, a_Chunk->GetNeighborChunk(a_Block.x, a_Block.z + 1));
	if (a_Block.y > 0)
	{
		AddBlock(a_Block.x, a_Block.y - 1, a_Block.z, a_Chunk);
	}
	if (a_Block.y < cChunkDef::Height - 1)
	{
		AddBlock(a_Block.x, a_Block.y + 1, a_Block.z, a_Chunk);
	}
}





void cSimulator::WakeUpArea(const cCuboid & a_Area)
{
	cCuboid area(a_Area);
	area.Sort();
	area.Expand(1, 1, 1, 1, 1, 1);  // Expand the area to contain the neighbors, too.
	area.ClampY(0, cChunkDef::Height - 1);

	// Add all blocks, in a per-chunk manner:
	int chunkStartX, chunkStartZ, chunkEndX, chunkEndZ;
	cChunkDef::BlockToChunk(area.p1.x, area.p1.z, chunkStartX, chunkStartZ);
	cChunkDef::BlockToChunk(area.p2.x, area.p2.z, chunkEndX,   chunkEndZ);
	for (int cz = chunkStartZ; cz <= chunkEndZ; ++cz)
	{
		for (int cx = chunkStartX; cx <= chunkEndX; ++cx)
		{
			m_World.DoWithChunk(cx, cz, [this, &area](cChunk & a_CBChunk) -> bool
				{
					if (!a_CBChunk.IsValid())
					{
						LOGWARNING("%s: Trying to wake up inside a non-valid chunk [%d, %d]. Ignoring.",
							__FUNCTION__, a_CBChunk.GetPosX(), a_CBChunk.GetPosZ()
						);
						return true;
					}
					int startX = std::max(area.p1.x, a_CBChunk.GetPosX() * cChunkDef::Width);
					int startZ = std::max(area.p1.z, a_CBChunk.GetPosZ() * cChunkDef::Width);
					int endX = std::min(area.p2.x, a_CBChunk.GetPosX() * cChunkDef::Width + cChunkDef::Width - 1);
					int endZ = std::min(area.p2.z, a_CBChunk.GetPosZ() * cChunkDef::Width + cChunkDef::Width - 1);
					for (int y = area.p1.y; y <= area.p2.y; ++y)
					{
						for (int z = startZ; z <= endZ; ++z)
						{
							for (int x = startX; x <= endX; ++x)
							{
								AddBlock(x, y, z, &a_CBChunk);
							}  // for x
						}  // for z
					}  // for y
					return true;
				}  // lambda
			);  // DoWithChunk
		}  // for cx
	}  // for cz
}




