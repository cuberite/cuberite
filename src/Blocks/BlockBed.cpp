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
			if (Meta & 0x4)
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





NIBBLETYPE cBlockBedHandler::MetaRotateCCW(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag; 0 is foot of bed, 1 is head of the bed; first two bits are for position. 0x03 = 11
	NIBBLETYPE OtherMeta = a_Meta & (UCHAR_MAX - 0x03);
	// Rotates by returning appropate metavalue; values are determined by a Metadata table.
	switch (a_Meta & 0x03)
	{
		case 0x00: return 0x03 + OtherMeta; // South -> East
		case 0x03: return 0x02 + OtherMeta; // East  -> North
		case 0x02: return 0x01 + OtherMeta; // North -> West
		case 0x01: return 0x00 + OtherMeta; // West  -> South
	}
	// Not reachable, but to avoid a compiler warning:
	return a_Meta;
}





NIBBLETYPE cBlockBedHandler::MetaRotateCW(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag; 0 is foot of bed, 1 is head of the bed; first two bits are for position. 0x03 = 11
	NIBBLETYPE OtherMeta = a_Meta & (UCHAR_MAX - 0x03);
	// Rotates by returning appropate metavalue; values are determined by a Metadata table.
	switch (a_Meta & 0x03)
	{
		case 0x00: return 0x01 + OtherMeta; // South -> West
		case 0x01: return 0x02 + OtherMeta; // West  -> North
		case 0x02: return 0x03 + OtherMeta; // North -> East
		case 0x03: return 0x00 + OtherMeta; // East  -> South
	}
	// Not reachable, but to avoid a compiler warning:
	return a_Meta;
}





NIBBLETYPE cBlockBedHandler::MetaMirrorXY(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag; 0 is foot of bed, 1 is head of the bed; first two bits are for position. 0x03 = 11
	NIBBLETYPE OtherMeta = a_Meta & (UCHAR_MAX - 0x03);
	// Flips by returning appropate metavalue; values are determined by a Metadata table.
	switch (a_Meta & 0x03)
	{
	case 0x00: return 0x02 + OtherMeta; // South -> North
	case 0x02: return 0x00 + OtherMeta; // North -> South
	}
	// Not Facing North or South; No change.
	return a_Meta;
}





NIBBLETYPE cBlockBedHandler::MetaMirrorYZ(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag; 0 is foot of bed, 1 is head of the bed; first two bits are for position. 0x03 = 11
	NIBBLETYPE OtherMeta = a_Meta & (UCHAR_MAX - 0x03);
	// Rotates by returning appropate metavalue; values are determined by a Metadata table.
	switch (a_Meta & 0x03)
	{
		case 0x01: return 0x03 + OtherMeta; // West  -> East
		case 0x03: return 0x01 + OtherMeta; // East  -> West
	}
	// Not Facing East or West; No change.
	return a_Meta;
}
