
#pragma once

#include "BlockEntity.h"





class cBlockEnderchestHandler :
	public cBlockEntityHandler
{
public:
	cBlockEnderchestHandler(BLOCKTYPE a_BlockType)
		: cBlockEntityHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		//todo: Drop Ender Chest if using silk touch pickaxe
		a_Pickups.push_back(cItem(E_BLOCK_OBSIDIAN, 8, 0));
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
		) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = RotationToMetaData(a_Player->GetYaw());
		return true;
	}

	virtual const char * GetStepSound(void) override
	{
		return "step.stone";
	}
	
	static NIBBLETYPE RotationToMetaData(double a_Rotation)
	{
		a_Rotation += 90 + 45;  // So its not aligned with axis

		if (a_Rotation > 360.f)
		{
			a_Rotation -= 360.f;
		}
		if ((a_Rotation >= 0.f) && (a_Rotation < 90.f))
		{
			return 0x4;
		}
		else if ((a_Rotation >= 180) && (a_Rotation < 270))
		{
			return 0x5;
		}
		else if ((a_Rotation >= 90) && (a_Rotation < 180))
		{
			return 0x2;
		}
		else
		{
			return 0x3;
		}
	}


	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
		switch (a_Meta)
		{
		case 0x02: return 0x05;  // North -> East
		case 0x05: return 0x03;  // East  -> South
		case 0x03: return 0x04;  // South -> West
		case 0x04: return 0x02;  // West  -> North
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
		switch (a_Meta)
		{
		case 0x02: return 0x04;  // North -> West
		case 0x04: return 0x03;  // West  -> South
		case 0x03: return 0x05;  // South -> East
		case 0x05: return 0x02;  // East  -> North
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
		switch (a_Meta)
		{
		case 0x02: return 0x03;  // North -> South
		case 0x03: return 0x02;  // South -> North
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Mirrors over YZ-plane (East/West).
		switch (a_Meta)
		{
		case 0x04: return 0x05;  // West -> East
		case 0x05: return 0x04;  // East -> West
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}
} ;




