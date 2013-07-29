#include "Globals.h"
#include "BlockBed.h"





void cBlockBedHandler::OnPlacedByPlayer(
	cWorld * a_World, cPlayer * a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace,
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
)
{
	if (a_BlockMeta < 8)
	{
		Vector3i Direction = MetaDataToDirection(a_BlockMeta);
		a_World->SetBlock(a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z, E_BLOCK_BED, a_BlockMeta | 0x8);
	}
}





void cBlockBedHandler::OnDestroyed(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE OldMeta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	Vector3i ThisPos( a_BlockX, a_BlockY, a_BlockZ );
	Vector3i Direction = MetaDataToDirection( OldMeta & 0x7 );
	if (OldMeta & 0x8)
	{
		// Was pillow
		if (a_World->GetBlock(ThisPos - Direction) == E_BLOCK_BED)
		{
			a_World->FastSetBlock(ThisPos - Direction, E_BLOCK_AIR, 0);
		}
	}
	else
	{
		// Was foot end
		if (a_World->GetBlock(ThisPos + Direction) == E_BLOCK_BED)
		{
			a_World->FastSetBlock(ThisPos + Direction, E_BLOCK_AIR, 0);
		}
	}
}





void cBlockBedHandler::OnUse(cWorld *a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	if (a_World->GetDimension() != 0)
	{
		a_World->DoExplosiontAt(5, a_BlockX, a_BlockY, a_BlockZ);
	} else {
		if (a_World->GetTimeOfDay() > 13000)
		{
			NIBBLETYPE Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
			if (Meta & 0x8)
			{
				// Is pillow	
				a_World->BroadcastUseBed(*a_Player, a_BlockX, a_BlockY, a_BlockZ);
			}
			else
			{
				// Is foot end
				Vector3i Direction = MetaDataToDirection( Meta & 0x7 );
				if (a_World->GetBlock(a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z) == E_BLOCK_BED) // Must always use pillow location for sleeping
				{
					a_World->BroadcastUseBed(*a_Player, a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z);
				}
			}
		} else {
			a_Player->SendMessage("You can only sleep at night");
		}
	}
}




