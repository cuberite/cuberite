
#pragma once

#include "World.h"
#include "Chunk.h"
#include "Component.h"
#include "RedstoneSimulator.h"
#include "Vector3.h"

namespace Redstone
{
	class Component;

	class ComponentFactory
	{

	public:
		ComponentFactory(cWorld & a_World, cRedstoneSimulatorChunkData * a_data);

		ComponentPtr GetComponent(Vector3i location);
		void PlaySound(std::string name, Vector3i location, float pitch);

		inline static bool IsSolidBlock(BLOCKTYPE blockType)
		{
			switch (blockType)
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
				case E_BLOCK_FARMLAND:
				case E_BLOCK_FURNACE:
				case E_BLOCK_LIT_FURNACE:
				case E_BLOCK_REDSTONE_ORE:
				case E_BLOCK_REDSTONE_ORE_GLOWING:
				case E_BLOCK_SNOW_BLOCK:
				case E_BLOCK_CLAY:
				case E_BLOCK_PUMPKIN:
				case E_BLOCK_NETHERRACK:
				case E_BLOCK_SOULSAND:
				case E_BLOCK_JACK_O_LANTERN:
				case E_BLOCK_STONE_BRICKS:
				case E_BLOCK_HUGE_BROWN_MUSHROOM:
				case E_BLOCK_HUGE_RED_MUSHROOM:
				case E_BLOCK_MELON:
				case E_BLOCK_MYCELIUM:
				case E_BLOCK_NETHER_BRICK:
				case E_BLOCK_END_PORTAL_FRAME:
				case E_BLOCK_END_STONE:
				case E_BLOCK_DOUBLE_WOODEN_SLAB:
				case E_BLOCK_EMERALD_ORE:
				case E_BLOCK_EMERALD_BLOCK:
				case E_BLOCK_COMMAND_BLOCK:
				case E_BLOCK_NETHER_QUARTZ_ORE:
				case E_BLOCK_QUARTZ_BLOCK:
				case E_BLOCK_STAINED_CLAY:
				case E_BLOCK_NEW_LOG:
				case E_BLOCK_PRISMARINE_BLOCK:
				case E_BLOCK_HARDENED_CLAY:
				case E_BLOCK_BLOCK_OF_COAL:
				case E_BLOCK_PACKED_ICE:
				case E_BLOCK_RED_SANDSTONE:
				case E_BLOCK_DOUBLE_NEW_STONE_SLAB:
					return true;
				default: return false;
			}
		}

	private:
		cWorld & m_World;
		cRedstoneSimulatorChunkData * m_Data;
		ComponentPtr CreateComponent(RedstoneType type, Vector3i location, BLOCKTYPE blockType, NIBBLETYPE meta);
		RedstoneType GetType(Vector3i location, BLOCKTYPE & blockType, NIBBLETYPE & meta);

		
	};

}