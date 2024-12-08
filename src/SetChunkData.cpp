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
				BlockState BlockType = BlockData.GetBlock({x, y, z});
				if (BlockType != BlockType::Air)
				{
					Height = y;
					break;
				}
			}  // for y
			auto idx = x + cChunkDef::Width * z;
			HeightMap[static_cast<UInt64>(idx)] = Height;
		}  // for z
	}  // for x
}
