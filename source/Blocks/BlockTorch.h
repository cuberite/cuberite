
#pragma once

#include "BlockHandler.h"
#include "../Torch.h"
#include "../World.h"





class cBlockTorchHandler :
	public cBlockHandler
{
public:
	cBlockTorchHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	
	
	virtual void PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		if (!TorchCanBePlacedAt(a_World, a_X, a_Y, a_Z, a_Dir))
		{
			a_Dir = FindSuitableDirection(a_World, a_X, a_Y, a_Z);
			
			if (a_Dir == BLOCK_FACE_BOTTOM)
			{
				return;
			}
		}

		a_World->SetBlock(a_X, a_Y, a_Z, m_BlockID, cTorch::DirectionToMetaData(a_Dir));
		OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
	}
	
	
	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}


	static bool CanBePlacedOn(BLOCKTYPE a_BlockType, char a_Direction)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_STONE:
			case E_BLOCK_GRASS:
			case E_BLOCK_DIRT:
			case E_BLOCK_COBBLESTONE:
			case E_BLOCK_PLANKS:
			case E_BLOCK_BEDROCK:
			case E_BLOCK_SAND:
			case E_BLOCK_GRAVEL:
			case E_BLOCK_GOLD_ORE:
			case E_BLOCK_IRON_ORE:
			case E_BLOCK_COAL_ORE:
			case E_BLOCK_LOG:
			case E_BLOCK_SPONGE:
			case E_BLOCK_LAPIS_ORE:
			case E_BLOCK_LAPIS_BLOCK:
			case E_BLOCK_SANDSTONE:
			case E_BLOCK_WOOL:
			case E_BLOCK_GOLD_BLOCK:
			case E_BLOCK_IRON_BLOCK:
			case E_BLOCK_DOUBLE_STONE_SLAB:
			case E_BLOCK_BRICK:
			case E_BLOCK_BOOKCASE:
			case E_BLOCK_MOSSY_COBBLESTONE:
			case E_BLOCK_OBSIDIAN:
			case E_BLOCK_MOB_SPAWNER:
			case E_BLOCK_DIAMOND_ORE:
			case E_BLOCK_DIAMOND_BLOCK:
			case E_BLOCK_CRAFTING_TABLE:
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_REDSTONE_ORE_GLOWING:
			case E_BLOCK_SNOW_BLOCK:
			case E_BLOCK_CLAY:
			case E_BLOCK_JUKEBOX:
			case E_BLOCK_PUMPKIN:
			case E_BLOCK_NETHERRACK:
			case E_BLOCK_SOULSAND:
			case E_BLOCK_JACK_O_LANTERN:
			case E_BLOCK_LOCKED_CHEST:
			case E_BLOCK_STONE_BRICKS:
			case E_BLOCK_MELON:
			case E_BLOCK_MYCELIUM:
			case E_BLOCK_NETHER_BRICK:
			case E_BLOCK_END_STONE:
			case E_BLOCK_REDSTONE_LAMP_OFF:
			case E_BLOCK_REDSTONE_LAMP_ON:
			case E_BLOCK_DOUBLE_WOODEN_SLAB:
			case E_BLOCK_EMERALD_ORE:
			case E_BLOCK_ENDER_CHEST:
			case E_BLOCK_EMERALD_BLOCK:
			{
				return true;
			}
			
			case E_BLOCK_GLASS:
			case E_BLOCK_FENCE:
			case E_BLOCK_NETHER_BRICK_FENCE:
			{
				return (a_Direction == 0x1);  // allow only direction "standing on floor"
			}
			
			default:
			{
				return false;
			}
		}
	}
	
	
	static bool TorchCanBePlacedAt(cWorld * a_World, int a_X, int a_Y, int a_Z, char a_Dir)
	{
		// TODO: If placing a torch from below, check all 4 XZ neighbors, place it on that neighbor instead
		// How to propagate that change up?
		// Simon: The easiest way is to calculate the position two times, shouldn´t cost much cpu power :)

		if (a_Dir == BLOCK_FACE_BOTTOM)
		{
			return false;
		}

		AddDirection( a_X, a_Y, a_Z, a_Dir, true );

		return CanBePlacedOn(a_World->GetBlock( a_X, a_Y, a_Z ), a_Dir);
	}
	
	
	/// Finds a suitable Direction for the Torch. Returns BLOCK_FACE_BOTTOM on failure
	static char FindSuitableDirection(cWorld * a_World, int a_X, int a_Y, int a_Z)
	{
		for (int i = 1; i <= 5; i++)
		{
			if (TorchCanBePlacedAt(a_World, a_X, a_Y, a_Z, i))
			{
				return i;
			}
		}
		return BLOCK_FACE_BOTTOM;
	}


	virtual bool CanBePlacedAt(cWorld * a_World, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		if(TorchCanBePlacedAt(a_World, a_X, a_Y, a_Z, a_Dir))
			return true;

		return FindSuitableDirection(a_World, a_X, a_Y, a_Z) != BLOCK_FACE_BOTTOM;
	}


	virtual bool CanBeAt(cWorld * a_World, int a_X, int a_Y, int a_Z) override
	{
		char Dir = cTorch::MetaDataToDirection(a_World->GetBlockMeta( a_X, a_Y, a_Z));
		return TorchCanBePlacedAt(a_World, a_X, a_Y, a_Z, Dir);
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Always drop meta = 0
		a_Pickups.push_back(cItem(m_BlockID, 1, 0));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




