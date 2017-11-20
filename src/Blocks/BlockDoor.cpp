
#include "Globals.h"
#include "BlockDoor.h"
#include "../EffectID.h"





cBlockDoorHandler::cBlockDoorHandler(BLOCKTYPE a_BlockType)
	: super(a_BlockType)
{
}





void cBlockDoorHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta({a_BlockX, a_BlockY, a_BlockZ});

	if (OldMeta & 8)
	{
		// Was upper part of door
		if (IsDoorBlockType(a_ChunkInterface.GetBlock({a_BlockX, a_BlockY - 1, a_BlockZ})))
		{
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
		}
	}
	else
	{
		// Was lower part
		if (IsDoorBlockType(a_ChunkInterface.GetBlock({a_BlockX, a_BlockY + 1, a_BlockZ})))
		{
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY + 1, a_BlockZ, E_BLOCK_AIR, 0);
		}
	}
}





bool cBlockDoorHandler::OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	UNUSED(a_WorldInterface);
	UNUSED(a_BlockFace);
	UNUSED(a_CursorX);
	UNUSED(a_CursorY);
	UNUSED(a_CursorZ);

	switch (a_ChunkInterface.GetBlock({a_BlockX, a_BlockY, a_BlockZ}))
	{
		default:
		{
			ASSERT(!"Unhandled door block type");
		}
		case E_BLOCK_ACACIA_DOOR:
		case E_BLOCK_BIRCH_DOOR:
		case E_BLOCK_DARK_OAK_DOOR:
		case E_BLOCK_JUNGLE_DOOR:
		case E_BLOCK_SPRUCE_DOOR:
		case E_BLOCK_OAK_DOOR:
		{
			ChangeDoor(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);
			a_Player.GetWorld()->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_WOODEN_DOOR_OPEN, a_BlockX, a_BlockY, a_BlockZ, 0, a_Player.GetClientHandle());
			break;
		}
		// Prevent iron door from opening on player click
		case E_BLOCK_IRON_DOOR:
		{
			OnCancelRightClick(a_ChunkInterface, a_WorldInterface, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			break;
		}
	}

	return true;
}





void cBlockDoorHandler::OnCancelRightClick(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace)
{
	UNUSED(a_ChunkInterface);

	a_WorldInterface.SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, a_Player);
	NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta({a_BlockX, a_BlockY, a_BlockZ});

	if (Meta & 0x8)
	{
		// Current block is top of the door
		a_WorldInterface.SendBlockTo(a_BlockX, a_BlockY - 1, a_BlockZ, a_Player);
	}
	else
	{
		// Current block is bottom of the door
		a_WorldInterface.SendBlockTo(a_BlockX, a_BlockY + 1, a_BlockZ, a_Player);
	}
}





cBoundingBox cBlockDoorHandler::GetPlacementCollisionBox(BLOCKTYPE a_XM, BLOCKTYPE a_XP, BLOCKTYPE a_YM, BLOCKTYPE a_YP, BLOCKTYPE a_ZM, BLOCKTYPE a_ZP)
{
	// Doors can be placed inside the player
	return cBoundingBox(0, 0, 0, 0, 0, 0);
}





NIBBLETYPE cBlockDoorHandler::MetaRotateCCW(NIBBLETYPE a_Meta)
{
	if (a_Meta & 0x08)
	{
		// The meta doesn't change for the top block
		return a_Meta;
	}
	else
	{
		// Rotate the bottom block
		return super::MetaRotateCCW(a_Meta);
	}
}





NIBBLETYPE cBlockDoorHandler::MetaRotateCW(NIBBLETYPE a_Meta)
{
	if (a_Meta & 0x08)
	{
		// The meta doesn't change for the top block
		return a_Meta;
	}
	else
	{
		// Rotate the bottom block
		return super::MetaRotateCW(a_Meta);
	}
}



NIBBLETYPE cBlockDoorHandler::MetaMirrorXY(NIBBLETYPE a_Meta)
{
	/*
	Top bit (0x08) contains door block position (Top / Bottom). Only Bottom blocks contain position data
	Return a_Meta if panel is a top panel (0x08 bit is set to 1)
	*/

	// Note: Currently, you can not properly mirror the hinges on a double door.  The orientation of the door is stored
	// in only the bottom tile while the hinge position is in the top tile.  This function only operates on one tile at a time,
	// so the function can only see either the hinge position or orientation, but not both, at any given time.  The class itself
	// needs extra datamembers.
	if (a_Meta & 0x08)
	{
		return a_Meta;
	}

	// Holds open / closed meta data. 0x0C == 1100.
	NIBBLETYPE OtherMeta = a_Meta & 0x0C;

	// Mirrors according to a table.  0x03 == 0011.
	switch (a_Meta & 0x03)
	{
		case 0x03: return 0x01 + OtherMeta;  // South -> North
		case 0x01: return 0x03 + OtherMeta;  // North -> South
	}

	// Not Facing North or South; No change.
	return a_Meta;
}



NIBBLETYPE cBlockDoorHandler::MetaMirrorYZ(NIBBLETYPE a_Meta)
{
	// Top bit (0x08) contains door panel type (Top / Bottom panel)  Only Bottom panels contain position data
	// Return a_Meta if panel is a top panel (0x08 bit is set to 1)

	// Note: Currently, you can not properly mirror the hinges on a double door.  The orientation of the door is stored
	// in only the bottom tile while the hinge position is in the top tile.  This function only operates on one tile at a time,
	// so the function can only see either the hinge position or orientation, but not both, at any given time.The class itself
	// needs extra datamembers.

	if (a_Meta & 0x08)
	{
		return a_Meta;
	}

	// Holds open / closed meta data. 0x0C == 1100.
	NIBBLETYPE OtherMeta = a_Meta & 0x0C;

	// Mirrors according to a table.  0x03 == 0011.
	switch (a_Meta & 0x03)
	{
		case 0x00: return 0x02 + OtherMeta;  // West  -> East
		case 0x02: return 0x00 + OtherMeta;  // East  -> West
	}

	// Not Facing North or South; No change.
	return a_Meta;
}



