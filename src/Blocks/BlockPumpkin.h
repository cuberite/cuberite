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

	virtual void OnPlacedByPlayer(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override
	{
		if (a_BlockY > 1)  //Make sure server won't check for inexistent blocks (below y=0).
		{
			int BlockY1 = a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ);
			int BlockY2 = a_World->GetBlock(a_BlockX, a_BlockY - 2, a_BlockZ);//We don't need to check this 2 blocks more than 1 time.
			if 
				(
				(BlockY1 == E_BLOCK_SNOW_BLOCK) &&  //If the first two blocks below the pumpkin are snow blocks, spawn a snow golem.
				(BlockY2 == E_BLOCK_SNOW_BLOCK)
				)
			{
				a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
				a_World->FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
				a_World->FastSetBlock(a_BlockX, a_BlockY - 2, a_BlockZ, E_BLOCK_AIR, 0);
				a_World->SpawnMob(a_BlockX + 0.5, a_BlockY - 2, a_BlockZ + 0.5, cMonster::mtSnowGolem);
			}
			else if 
				(
				(BlockY1 == E_BLOCK_IRON_BLOCK) &&  //If the first two blocks below the pumpkin are iron blocks, spawn an iron golem.
				(BlockY2 == E_BLOCK_IRON_BLOCK)
				)
			{
				if		
					(
					(a_World->GetBlock(a_BlockX + 1, a_BlockY - 1, a_BlockZ) == E_BLOCK_IRON_BLOCK) &&  //Check the first possible locations for arms.
					(a_World->GetBlock(a_BlockX - 1, a_BlockY - 1, a_BlockZ) == E_BLOCK_IRON_BLOCK)
					)
				{
					a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
					a_World->FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
					a_World->FastSetBlock(a_BlockX + 1, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
					a_World->FastSetBlock(a_BlockX - 1, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
					a_World->FastSetBlock(a_BlockX, a_BlockY - 2, a_BlockZ, E_BLOCK_AIR, 0); //Set all blocks used for creation to air.
					a_World->SpawnMob(a_BlockX + 0.5, a_BlockY - 2, a_BlockZ + 0.5, cMonster::mtIronGolem); //Spawn an iron golem.
				}
				else if
					(
					(a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ + 1) == E_BLOCK_IRON_BLOCK) &&  //Check the other possible locations.
					(a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ - 1) == E_BLOCK_IRON_BLOCK) 
					)
				{
					a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
					a_World->FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
					a_World->FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ + 1, E_BLOCK_AIR, 0);
					a_World->FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ - 1, E_BLOCK_AIR, 0);
					a_World->FastSetBlock(a_BlockX, a_BlockY - 2, a_BlockZ, E_BLOCK_AIR, 0);
					a_World->SpawnMob(a_BlockX + 0.5, a_BlockY - 2, a_BlockZ + 0.5, cMonster::mtIronGolem); 
				}
			}
		}
	}

	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = PlayerYawToMetaData(a_Player->GetRotation());
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

} ;




