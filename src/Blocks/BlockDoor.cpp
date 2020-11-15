
#include "Globals.h"
#include "BlockDoor.h"
#include "../EffectID.h"





void cBlockDoorHandler::OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos, BLOCKTYPE a_OldBlockType,
		NIBBLETYPE a_OldBlockMeta,
		const cEntity * a_Digger
) const
{
	UNUSED(a_Digger);
	// A part of the multiblock door was broken; the relevant half will drop any pickups as required.
	// All that is left to do is to delete the other half of the multiblock.

	if ((a_OldBlockMeta & 0x08) != 0)
	{
		const auto Lower = a_BlockPos.addedY(-1);
		if ((Lower.y >= 0) && IsDoorBlockType(a_ChunkInterface.GetBlock(Lower)))
		{
			// Was upper part of door, remove lower:
			a_ChunkInterface.SetBlock(Lower, E_BLOCK_AIR, 0);
		}
	}
	else
	{
		const auto Upper = a_BlockPos.addedY(1);
		if ((Upper.y < cChunkDef::Height) && IsDoorBlockType(a_ChunkInterface.GetBlock(Upper)))
		{
			// Was lower part, remove upper:
			a_ChunkInterface.SetBlock(Upper, E_BLOCK_AIR, 0);
		}
	}
}





bool cBlockDoorHandler::OnUse(
	cChunkInterface & a_ChunkInterface,
	cWorldInterface & a_WorldInterface,
	cPlayer & a_Player,
	const Vector3i a_BlockPos,
	eBlockFace a_BlockFace,
	const Vector3i a_CursorPos
) const
{
	UNUSED(a_WorldInterface);
	UNUSED(a_BlockFace);
	UNUSED(a_CursorPos);

	switch (a_ChunkInterface.GetBlock(a_BlockPos))
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
			ChangeDoor(a_ChunkInterface, a_BlockPos);
			a_Player.GetWorld()->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_WOODEN_DOOR_OPEN, a_BlockPos, 0, a_Player.GetClientHandle());
			break;
		}
		case E_BLOCK_IRON_DOOR:
		{
			// Prevent iron door from opening on player click (#2415):
			OnCancelRightClick(a_ChunkInterface, a_WorldInterface, a_Player, a_BlockPos, a_BlockFace);

			// Allow placement actions to instead take place:
			return false;
		}
	}

	return true;
}





void cBlockDoorHandler::OnCancelRightClick(
	cChunkInterface & a_ChunkInterface,
	cWorldInterface & a_WorldInterface,
	cPlayer & a_Player,
	const Vector3i a_BlockPos,
	eBlockFace a_BlockFace
) const
{
	UNUSED(a_ChunkInterface);
	UNUSED(a_BlockFace);

	a_WorldInterface.SendBlockTo(a_BlockPos, a_Player);
	NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockPos);

	if (Meta & 0x08)
	{
		// Current block is top of the door, send the bottom part:
		a_WorldInterface.SendBlockTo(a_BlockPos.addedY(-1), a_Player);
	}
	else
	{
		// Current block is bottom of the door, send the top part:
		a_WorldInterface.SendBlockTo(a_BlockPos.addedY(1), a_Player);
	}
}





cBoundingBox cBlockDoorHandler::GetPlacementCollisionBox(BLOCKTYPE a_XM, BLOCKTYPE a_XP, BLOCKTYPE a_YM, BLOCKTYPE a_YP, BLOCKTYPE a_ZM, BLOCKTYPE a_ZP) const
{
	// Doors can be placed inside the player
	return cBoundingBox(0, 0, 0, 0, 0, 0);
}





NIBBLETYPE cBlockDoorHandler::MetaRotateCCW(NIBBLETYPE a_Meta) const
{
	if (a_Meta & 0x08)
	{
		// The meta doesn't change for the top block
		return a_Meta;
	}
	else
	{
		// Rotate the bottom block
		return Super::MetaRotateCCW(a_Meta);
	}
}





NIBBLETYPE cBlockDoorHandler::MetaRotateCW(NIBBLETYPE a_Meta) const
{
	if (a_Meta & 0x08)
	{
		// The meta doesn't change for the top block
		return a_Meta;
	}
	else
	{
		// Rotate the bottom block
		return Super::MetaRotateCW(a_Meta);
	}
}





NIBBLETYPE cBlockDoorHandler::MetaMirrorXY(NIBBLETYPE a_Meta) const
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





NIBBLETYPE cBlockDoorHandler::MetaMirrorYZ(NIBBLETYPE a_Meta) const
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



