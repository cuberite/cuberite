#include "Globals.h"
#include "BlockBed.h"





void cBlockBedHandler::PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir)
{
	if (a_Dir != 1) // Can only be placed on the floor
	{
		return;
	}

	NIBBLETYPE Meta = RotationToMetaData( a_Player->GetRotation() );
	Vector3i Direction = MetaDataToDirection( Meta );

	if (a_World->GetBlock(a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z) != E_BLOCK_AIR)
	{
		return;
	}

	a_World->SetBlock(a_BlockX,               a_BlockY, a_BlockZ,               E_BLOCK_BED, Meta);
	a_World->SetBlock(a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z, E_BLOCK_BED, Meta | 0x8);

	OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
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





void cBlockBedHandler::OnUse(cWorld *a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
	if (Meta & 0x8)
	{
		// Is pillow	
		a_World->BroadcastUseBed( *a_Player, a_BlockX, a_BlockY, a_BlockZ );
	}
	else
	{
		// Is foot end
		Vector3i Direction = MetaDataToDirection( Meta & 0x7 );
		if (a_World->GetBlock(a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z) == E_BLOCK_BED) // Must always use pillow location for sleeping
		{
			a_World->BroadcastUseBed(*a_Player, a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z );
		}

	}
}




