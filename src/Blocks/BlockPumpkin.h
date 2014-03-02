#pragma once

#include "BlockHandler.h"




class cBlockPumpkinHandler :
	public cBlockHandler
{
public:
	cBlockPumpkinHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	virtual void OnPlacedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override
	{
		// Check whether the pumpkin is a part of a golem or a snowman
		
		if (a_BlockY < 2)
		{
			// The pumpkin is too low for a golem / snowman
			return;
		}
		
		BLOCKTYPE BlockY1 = a_ChunkInterface.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ);
		BLOCKTYPE BlockY2 = a_ChunkInterface.GetBlock(a_BlockX, a_BlockY - 2, a_BlockZ);
		
		// Check for a snow golem:
		if ((BlockY1 == E_BLOCK_SNOW_BLOCK) && (BlockY2 == E_BLOCK_SNOW_BLOCK))
		{
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY,     a_BlockZ, E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 2, a_BlockZ, E_BLOCK_AIR, 0);
			a_WorldInterface.SpawnMob(a_BlockX + 0.5, a_BlockY - 2, a_BlockZ + 0.5, cMonster::mtSnowGolem);
			return;
		}
		
		// Check for an iron golem. First check only the body and legs, since those are the same for both orientations:
		if ((BlockY1 != E_BLOCK_IRON_BLOCK) || (BlockY2 != E_BLOCK_IRON_BLOCK))
		{
			// One of the blocks is not an iron, no chance of a golem here
			return;
		}
		
		// Now check both orientations for hands:
		if (
			(a_ChunkInterface.GetBlock(a_BlockX + 1, a_BlockY - 1, a_BlockZ) == E_BLOCK_IRON_BLOCK) &&
			(a_ChunkInterface.GetBlock(a_BlockX - 1, a_BlockY - 1, a_BlockZ) == E_BLOCK_IRON_BLOCK)
		)
		{
			// Remove the iron blocks:
			a_ChunkInterface.FastSetBlock(a_BlockX,     a_BlockY,     a_BlockZ, E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX,     a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX + 1, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX - 1, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX,     a_BlockY - 2, a_BlockZ, E_BLOCK_AIR, 0);
			
			// Spawn the golem:
			a_WorldInterface.SpawnMob(a_BlockX + 0.5, a_BlockY - 2, a_BlockZ + 0.5, cMonster::mtIronGolem);
		}
		else if (
			(a_ChunkInterface.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ + 1) == E_BLOCK_IRON_BLOCK) &&
			(a_ChunkInterface.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ - 1) == E_BLOCK_IRON_BLOCK)
		)
		{
			// Remove the iron blocks:
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY,     a_BlockZ,     E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ,     E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ + 1, E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ - 1, E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 2, a_BlockZ,     E_BLOCK_AIR, 0);
			
			// Spawn the golem:
			a_WorldInterface.SpawnMob(a_BlockX + 0.5, a_BlockY - 2, a_BlockZ + 0.5, cMonster::mtIronGolem); 
		}
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

	inline static NIBBLETYPE PlayerYawToMetaData(double a_Yaw)
	{
		ASSERT((a_Yaw >= -180) && (a_Yaw < 180));
		
		a_Yaw += 180 + 45;
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


	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
		switch (a_Meta)
		{
			case 0x00: return 0x01;  // South -> West
			case 0x01: return 0x02;  // West -> North
			case 0x02: return 0x03;  // North -> East
			case 0x03: return 0x00;  // East -> South
		}
		// No Face on Pumpkin, no change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
		switch (a_Meta)
		{
			case 0x00: return 0x03;  // South -> East
			case 0x03: return 0x02;  // East -> North
			case 0x02: return 0x01;  // North -> West
			case 0x01: return 0x00;  // West -> South
		}
		// No Face on pumpkin, no change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Mirrors over XY-plane (North/South).
		switch (a_Meta)
		{
			case 0x00: return 0x02;  // South -> North
			case 0x02: return 0x00;  // North -> South
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Mirrors over YZ-plane (East/West).
		switch (a_Meta)
		{
			case 0x01: return 0x03;  // West -> East
			case 0x03: return 0x01;  // East -> West
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}
} ;




