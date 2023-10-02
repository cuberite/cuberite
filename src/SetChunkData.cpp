#include "Globals.h"
#include "SetChunkData.h"
#include "BlockType.h"
#include "Entities/Entity.h"





void SetChunkData::UpdateHeightMap()
{
	for (int x = 0; x < cChunkDef::Width; x++)
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			HEIGHTTYPE Height = 0;
			for (HEIGHTTYPE y = cChunkDef::Height - 1; y > 0; y--)
			{
				BLOCKTYPE BlockType = BlockData.GetBlock({x, y, z});
				if (BlockType != E_BLOCK_AIR)
				{
					Height = y;
					break;
				}
			}  // for y
			auto idx = static_cast<size_t>(x + cChunkDef::Width * z);
			HeightMap[idx] = Height;
		}  // for z
	}  // for x
}
