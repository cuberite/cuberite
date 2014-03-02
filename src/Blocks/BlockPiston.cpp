
#include "Globals.h"
#include "BlockPiston.h"
#include "../Item.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Piston.h"





#define AddPistonDir(x, y, z, dir, amount) \
	switch (dir) \
	{ \
		case 0: (y) -= (amount); break; \
		case 1: (y) += (amount); break; \
		case 2: (z) -= (amount); break; \
		case 3: (z) += (amount); break; \
		case 4: (x) -= (amount); break; \
		case 5: (x) += (amount); break; \
	}




cBlockPistonHandler::cBlockPistonHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockPistonHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	int newX = a_BlockX;
	int newY = a_BlockY;
	int newZ = a_BlockZ;
	AddPistonDir(newX, newY, newZ, OldMeta & ~(8), 1);

	if (a_ChunkInterface.GetBlock(newX, newY, newZ) == E_BLOCK_PISTON_EXTENSION)
	{
		a_ChunkInterface.SetBlock(a_WorldInterface, newX, newY, newZ, E_BLOCK_AIR, 0);
	}
}





bool cBlockPistonHandler::GetPlacementBlockTypeMeta(
	cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
)
{
	a_BlockType = m_BlockType;
	a_BlockMeta = cPiston::RotationPitchToMetaData(a_Player->GetYaw(), a_Player->GetPitch());
	return true;
}





NIBBLETYPE cBlockPistonHandler::MetaRotateCW(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag.
	NIBBLETYPE OtherMeta = a_Meta & 0x08;
	// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
	switch (a_Meta & 0x07)
	{
	case 0x02: return 0x05 + OtherMeta;  // North -> East
	case 0x05: return 0x03 + OtherMeta;  // East  -> South
	case 0x03: return 0x04 + OtherMeta;  // South -> West
	case 0x04: return 0x02 + OtherMeta;  // West  -> North
	}
	// Faces up/down
	return a_Meta;
}





NIBBLETYPE cBlockPistonHandler::MetaRotateCCW(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag.
	NIBBLETYPE OtherMeta = a_Meta & 0x08;
	// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
	switch (a_Meta & 0x07)
	{
	case 0x02: return 0x04 + OtherMeta;  // North -> West
	case 0x04: return 0x03 + OtherMeta;  // West  -> South
	case 0x03: return 0x05 + OtherMeta;  // South -> East
	case 0x05: return 0x02 + OtherMeta;  // East  -> North
	}
	// Faces up/down
	return a_Meta;
}





NIBBLETYPE cBlockPistonHandler::MetaMirrorXY(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag.
	NIBBLETYPE OtherMeta = a_Meta & 0x08;
	// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
	switch (a_Meta & 0x07)
	{
	case 0x02: return 0x03 + OtherMeta;  // North -> South
	case 0x03: return 0x02 + OtherMeta;  // South -> North
	}
	// Not Facing East or West; No change.
	return a_Meta;
}





NIBBLETYPE cBlockPistonHandler::MetaMirrorXZ(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag.
	NIBBLETYPE OtherMeta = a_Meta & 0x08;
	// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
	switch (a_Meta & 0x07)
	{
	case 0x00: return 0x01 + OtherMeta;  // Up   -> down
	case 0x01: return 0x00 + OtherMeta;  // Down -> Up
	}
	// Not Facing Up or Down; No change.
	return a_Meta;
}





NIBBLETYPE cBlockPistonHandler::MetaMirrorYZ(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag.
	NIBBLETYPE OtherMeta = a_Meta & 0x08;
	// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
	switch (a_Meta & 0x07)
	{
	case 0x04: return 0x05 + OtherMeta;  // West -> East
	case 0x05: return 0x04 + OtherMeta;  // East -> West
	}
	// Not Facing East or West; No change.
	return a_Meta;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBlockPistonHeadHandler:

cBlockPistonHeadHandler::cBlockPistonHeadHandler(void) :
	super(E_BLOCK_PISTON_EXTENSION)
{
}





void cBlockPistonHeadHandler::OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	int newX = a_BlockX;
	int newY = a_BlockY;
	int newZ = a_BlockZ;
	AddPistonDir(newX, newY, newZ, OldMeta & ~(8), -1);

	BLOCKTYPE Block = a_ChunkInterface.GetBlock(newX, newY, newZ);
	if ((Block == E_BLOCK_STICKY_PISTON) || (Block == E_BLOCK_PISTON))
	{
		a_ChunkInterface.DigBlock(a_WorldInterface, newX, newY, newZ);
		if (a_Player->IsGameModeCreative())
		{
			return; // No pickups if creative
		}

		cItems Pickups;
		Pickups.push_back(cItem(Block, 1));
		a_WorldInterface.SpawnItemPickups(Pickups, a_BlockX + 0.5, a_BlockY + 0.5, a_BlockZ + 0.5);
	}
}





NIBBLETYPE cBlockPistonHeadHandler::MetaRotateCW(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag.
	NIBBLETYPE OtherMeta = a_Meta & 0x08;
	// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
	switch (a_Meta & 0x07)
	{
	case 0x02: return 0x05 + OtherMeta;  // North -> East
	case 0x05: return 0x03 + OtherMeta;  // East  -> South
	case 0x03: return 0x04 + OtherMeta;  // South -> West
	case 0x04: return 0x02 + OtherMeta;  // West  -> North
	}
	// Faces up/down
	return a_Meta;
}





NIBBLETYPE cBlockPistonHeadHandler::MetaRotateCCW(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag.
	NIBBLETYPE OtherMeta = a_Meta & 0x08;
	// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
	switch (a_Meta & 0x07)
	{
	case 0x02: return 0x04 + OtherMeta;  // North -> West
	case 0x04: return 0x03 + OtherMeta;  // West  -> South
	case 0x03: return 0x05 + OtherMeta;  // South -> East
	case 0x05: return 0x02 + OtherMeta;  // East  -> North
	}
	// Faces up/down
	return a_Meta;
}





NIBBLETYPE cBlockPistonHeadHandler::MetaMirrorXY(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag.
	NIBBLETYPE OtherMeta = a_Meta & 0x08;
	// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
	switch (a_Meta & 0x07)
	{
	case 0x02: return 0x03 + OtherMeta;  // North -> South
	case 0x03: return 0x02 + OtherMeta;  // South -> North
	}
	// Not Facing East or West; No change.
	return a_Meta;
}





NIBBLETYPE cBlockPistonHeadHandler::MetaMirrorXZ(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag.
	NIBBLETYPE OtherMeta = a_Meta & 0x08;
	// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
	switch (a_Meta & 0x07)
	{
	case 0x00: return 0x01 + OtherMeta;  // Up   -> down
	case 0x01: return 0x00 + OtherMeta;  // Down -> Up
	}
	// Not Facing Up or Down; No change.
	return a_Meta;
}





NIBBLETYPE cBlockPistonHeadHandler::MetaMirrorYZ(NIBBLETYPE a_Meta)
{
	// Bit 0x08 (Bit #4) is a flag.
	NIBBLETYPE OtherMeta = a_Meta & 0x08;
	// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
	switch (a_Meta & 0x07)
	{
	case 0x04: return 0x05 + OtherMeta;  // West -> East
	case 0x05: return 0x04 + OtherMeta;  // East -> West
	}
	// Not Facing East or West; No change.
	return a_Meta;
}




