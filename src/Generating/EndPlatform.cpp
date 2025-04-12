#include "Globals.h"
#include "EndPlatform.h"
#include "BlockType.h"

void EndPlatform::Generate(cWorld * m_EndWorld)
{
	Vector3i platformPos(100, 48, 0);
	class cEndPlatformCallback : public cChunkCoordCallback
	{
		public:
			cEndPlatformCallback(cWorld * world, Vector3d platformPos) : m_World(world), m_PlatformPos(platformPos)
			{

			}





			virtual void Call(cChunkCoords coords, bool success) override
			{
				if (!success)
				{
					LOGD("Couldn't prepare chunk for end obsidian platform generation!");
					return;
				}

				// For some reason, the platform is split between two chunks, so we need to have code for both of them
				for (int x = -2; x <= 2; x++)
				{
					for (int z = (coords.m_ChunkZ == 0 ? 0 : -2); z < (coords.m_ChunkZ == 0 ? 3 : 0); z++)
					{
						Vector3d currentBlockPos = m_PlatformPos.addedXZ(x, z);
						m_World->FastSetBlock(currentBlockPos, E_BLOCK_OBSIDIAN, 0);
						for (int y = 1; y <= 3; y++)
						{
							Vector3d blockToReplacePos = currentBlockPos.addedY(y);
							BLOCKTYPE blockToReplaceType = m_World->GetBlock(blockToReplacePos);
							if (blockToReplaceType != E_BLOCK_AIR)
							{
								// According to the Minecraft Wiki, the blocks above the platform should drop as items
								if (blockToReplaceType == E_BLOCK_END_STONE)
								{
									// If the block is end stone, the platform will have spawned underground, so we don't want to drop the block as an item
									m_World->DigBlock(blockToReplacePos);
								}
								else
								{
									m_World->DropBlockAsPickups(currentBlockPos.addedY(y));
								}
							}
						}
					}
				}
			}
		private:
			cWorld * m_World;
			Vector3d m_PlatformPos;
	};

	// First, we need to load the two chunks because they're probably not loaded yet
	m_EndWorld->PrepareChunk(6, 0, std::make_unique<cEndPlatformCallback>(m_EndWorld, platformPos));
	m_EndWorld->PrepareChunk(6, 1, std::make_unique<cEndPlatformCallback>(m_EndWorld, platformPos));
}
