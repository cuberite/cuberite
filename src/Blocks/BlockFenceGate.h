
#pragma once

#include "BlockHandler.h"





class cBlockFenceGateHandler :
	public cBlockHandler
{
public:
	cBlockFenceGateHandler(BLOCKTYPE a_BlockType) :
		cBlockHandler(a_BlockType)
	{
	}


	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = PlayerYawToMetaData(a_Player->GetYaw());
		return true;
	}


	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		NIBBLETYPE OldMetaData = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		NIBBLETYPE NewMetaData = PlayerYawToMetaData(a_Player->GetYaw());
		OldMetaData ^= 4;  // Toggle the gate
		if ((OldMetaData & 1) == (NewMetaData & 1))
		{
			// Standing in front of the gate - apply new direction
			a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, (OldMetaData & 4) | (NewMetaData & 3));
		}
		else
		{
			// Standing aside - use last direction
			a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, OldMetaData);
		}
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}


	/// Converts the player's yaw to placed gate's blockmeta
	inline static NIBBLETYPE PlayerYawToMetaData(double a_Yaw)
	{
		ASSERT((a_Yaw >= -180) && (a_Yaw < 180));
		
		a_Yaw += 360 + 45;
		if (a_Yaw > 360)
		{
			a_Yaw -= 360;
		}
		if ((a_Yaw >= 0) && (a_Yaw < 90))
		{
			return 0x0;
		}
		else if ((a_Yaw >= 180) && (a_Yaw < 270))
		{
			return 0x2;
		}
		else if ((a_Yaw >= 90) && (a_Yaw < 180))
		{
			return 0x1;
		}
		else
		{
			return 0x3;
		}
	}


	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		// Bit 0x04 (Bit #3) is a flag; first two bits are for position. 0x0C = 1100
		NIBBLETYPE OtherMeta = a_Meta & 0x0C;
		// Rotates by returning appropate metavalue; values are determined by a Metadata table. 0x03 == 0011
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


	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		// Bit 0x04 (Bit #3) is a flag; first two bits are for position. 0x0C = 1100
		NIBBLETYPE OtherMeta = a_Meta & 0x0C;
		// Rotates by returning appropate metavalue; values are determined by a Metadata table. 0x03 == 0011
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


	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Bit 0x04 (Bit #3) is a flag; first two bits are for position. 0x0C = 1100
		NIBBLETYPE OtherMeta = a_Meta & 0x0C;
		// Mirrors by returning appropate metavalue; values are determined by a Metadata table. 0x03 == 0011
		switch (a_Meta & 0x03)
		{
		case 0x00: return 0x02 + OtherMeta; // South -> North
		case 0x02: return 0x00 + OtherMeta; // North -> South
		}
		// Not Facing North or South; No change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Bit 0x04 (Bit #3) is a flag; first two bits are for position. 0x0C = 1100
		NIBBLETYPE OtherMeta = a_Meta & 0x0C;
		// Mirrors by returning appropate metavalue; values are determined by a Metadata table. 0x03 == 0011
		switch (a_Meta & 0x03)
		{
		case 0x01: return 0x03 + OtherMeta; // West  -> East
		case 0x03: return 0x01 + OtherMeta; // East  -> West
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}
} ;




