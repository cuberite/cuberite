#include "Globals.h"
#include "BlockBed.h"





void cBlockBedHandler::OnPlacedByPlayer(
	cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
)
{
	if (a_BlockMeta < 8)
	{
		Vector3i Direction = MetaDataToDirection(a_BlockMeta);
		a_ChunkInterface.SetBlock(a_WorldInterface,a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z, E_BLOCK_BED, a_BlockMeta | 0x8);
	}
}





void cBlockBedHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	Vector3i ThisPos( a_BlockX, a_BlockY, a_BlockZ );
	Vector3i Direction = MetaDataToDirection( OldMeta & 0x7 );
	if (OldMeta & 0x8)
	{
		// Was pillow
		if (a_ChunkInterface.GetBlock(ThisPos - Direction) == E_BLOCK_BED)
		{
			a_ChunkInterface.FastSetBlock(ThisPos - Direction, E_BLOCK_AIR, 0);
		}
	}
	else
	{
		// Was foot end
		if (a_ChunkInterface.GetBlock(ThisPos + Direction) == E_BLOCK_BED)
		{
			a_ChunkInterface.FastSetBlock(ThisPos + Direction, E_BLOCK_AIR, 0);
		}
	}
}





void cBlockBedHandler::OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	if (a_WorldInterface.GetDimension() != dimOverworld)
	{
		Vector3i Coords(a_BlockX, a_BlockY, a_BlockZ);
		a_WorldInterface.DoExplosionAt(5, a_BlockX, a_BlockY, a_BlockZ, true, esBed, &Coords);
	}
	else
	{
		if (a_WorldInterface.GetTimeOfDay() > 13000)
		{
			NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
			if(Meta & 0x4)
			{
				a_Player->SendMessageFailure("This bed is occupied.");
			}
			else
			{
				if (Meta & 0x8)
				{
					// Is pillow	
					a_WorldInterface.GetBroadcastManager().BroadcastUseBed(*a_Player, a_BlockX, a_BlockY, a_BlockZ);
				}
				else
				{
					// Is foot end
					Vector3i Direction = MetaDataToDirection( Meta & 0x7 );
					if (a_ChunkInterface.GetBlock(a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z) == E_BLOCK_BED) // Must always use pillow location for sleeping
					{
						a_WorldInterface.GetBroadcastManager().BroadcastUseBed(*a_Player, a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z);
					}
				}
				a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, (Meta | (1 << 2)));
			}
			
		} else {
			a_Player->SendMessageFailure("You can only sleep at night");
		}
	}
}






