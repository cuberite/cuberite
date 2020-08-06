
#include "Globals.h"

#include "Simulator.h"
#include "../Chunk.h"
#include "../Cuboid.h"
#include "../World.h"





void cSimulator::WakeUp(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block)
{
	ASSERT(a_Chunk.IsValid());

	AddBlock(a_Chunk, a_Position, a_Block);
}





void cSimulator::WakeUp(cChunk & a_Chunk, Vector3i a_Position, Vector3i a_Offset, BLOCKTYPE a_Block)
{
	ASSERT(a_Chunk.IsValid());

	WakeUp(a_Chunk, a_Position, a_Block);
}





void cSimulator::WakeUp(const cCuboid & a_Area)
{
	cCuboid area(a_Area);
	area.Sort();
	area.Expand(1, 1, 1, 1, 1, 1);  // Expand the area to contain the neighbors, too.
	area.ClampY(0, cChunkDef::Height - 1);

	// Add all blocks, in a per-chunk manner:
	cChunkCoords ChunkStart = cChunkDef::BlockToChunk(area.p1);
	cChunkCoords ChunkEnd = cChunkDef::BlockToChunk(area.p2);
	for (int cz = ChunkStart.m_ChunkZ; cz <= ChunkEnd.m_ChunkZ; ++cz)
	{
		for (int cx = ChunkStart.m_ChunkX; cx <= ChunkEnd.m_ChunkX; ++cx)
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
								const auto Position = cChunkDef::AbsoluteToRelative({ x, y, z });
								AddBlock(a_CBChunk, Position, a_CBChunk.GetBlock(Position));
							}  // for x
						}  // for z
					}  // for y
					return true;
				}  // lambda
			);  // DoWithChunk
		}  // for cx
	}  // for cz
}
