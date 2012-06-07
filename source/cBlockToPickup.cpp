
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cBlockToPickup.h"
#include "Defines.h"
#include "BlockID.h"
#include "MersenneTwister.h"





static void AddRandomDrop(cItems & a_Drops, MTRand & r1, int a_OneInNChance, ENUM_ITEM_ID a_ItemID)
{
	if (r1.randInt(a_OneInNChance - 1) != 0)
	{
		return;
	}
	a_Drops.push_back(cItem(a_ItemID));
}





void cBlockToPickup::ToPickup(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, ENUM_ITEM_ID a_UsedItemID, cItems & a_Drops)
{
	MTRand r1;

	switch (a_BlockType)
	{
		// Blocks that always drop themselves as the only item, no matter what tool; copy damage from meta:
		case E_BLOCK_LOG:
		case E_BLOCK_PLANKS:
		case E_BLOCK_WOOL:
		{
			a_Drops.push_back(cItem((ENUM_ITEM_ID)a_BlockType, 1, a_BlockMeta));
			return;
		}
		
		
		// Blocks that always drop themselves as the only item, no matter what tool, set damage value zero:
		case E_BLOCK_DIRT:
		case E_BLOCK_SAPLING:
		case E_BLOCK_SAND:
		case E_BLOCK_TORCH:
		case E_BLOCK_YELLOW_FLOWER:
		case E_BLOCK_RED_ROSE:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_RED_MUSHROOM:
		case E_BLOCK_TNT:
		case E_BLOCK_CRAFTING_TABLE:
		case E_BLOCK_FURNACE:
		case E_BLOCK_CACTUS:
		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_POWERED_RAIL:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_RAIL:
		case E_BLOCK_LADDER:
		case E_BLOCK_LEVER:
		case E_BLOCK_WOODEN_PRESSURE_PLATE:
		case E_BLOCK_STONE_BUTTON:
		case E_BLOCK_JUKEBOX:
		case E_BLOCK_FENCE:
		case E_BLOCK_FENCE_GATE:
		case E_BLOCK_PUMPKIN:
		case E_BLOCK_NETHERRACK:
		case E_BLOCK_SOULSAND:
		case E_BLOCK_JACK_O_LANTERN:
		case E_BLOCK_TRAPDOOR:
		{
			a_Drops.push_back(cItem((ENUM_ITEM_ID)a_BlockType, 1, 0));
			return;
		}
		
		
		// Blocks that always drop a single item, no matter what tool:
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_WALLSIGN:              a_Drops.push_back(cItem(E_ITEM_SIGN, 1));              return;
		case E_BLOCK_REDSTONE_WIRE:         a_Drops.push_back(cItem(E_ITEM_REDSTONE_DUST, 1));     return;
		case E_BLOCK_GLOWSTONE:             a_Drops.push_back(cItem(E_ITEM_GLOWSTONE_DUST, 1));    return;
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON:  a_Drops.push_back(cItem(E_ITEM_REDSTONE_REPEATER, 1)); return;
		case E_BLOCK_COBWEB:                a_Drops.push_back(cItem(E_ITEM_STRING, 1));            return;
		case E_BLOCK_FARMLAND:
		case E_BLOCK_GRASS:                 a_Drops.push_back(cItem(E_ITEM_DIRT, 1));              return;
		case E_BLOCK_LIT_FURNACE:           a_Drops.push_back(cItem(E_ITEM_FURNACE, 1));           return;
		case E_BLOCK_SUGARCANE:             a_Drops.push_back(cItem(E_ITEM_SUGARCANE, 1));         return;
		case E_BLOCK_PUMPKIN_STEM:          a_Drops.push_back(cItem(E_ITEM_PUMPKIN_SEEDS, 1));     return;
		case E_BLOCK_MELON_STEM:            a_Drops.push_back(cItem(E_ITEM_MELON_SEEDS, 1));       return;
		
		
		// Doors seem to need their meta set to 1
		case E_BLOCK_WOODEN_DOOR: a_Drops.push_back(cItem(E_ITEM_WOODEN_DOOR, 1, 1)); return;
		case E_BLOCK_IRON_DOOR:   a_Drops.push_back(cItem(E_ITEM_IRON_DOOR,   1, 1)); return;
		

		////////////////////////
		// Ores:
		
		// Coal ore requires a pickaxe:
		case E_BLOCK_COAL_ORE:
		{
			if (ItemCategory::IsPickaxe(a_UsedItemID))
			{
				a_Drops.push_back(cItem(E_ITEM_COAL, 1));
			}
			return;
		}
		
		// Iron ore requires a stone or better pickaxe:
		case E_BLOCK_IRON_ORE:
		{
			if (
				(a_UsedItemID == E_ITEM_STONE_PICKAXE) ||
				(a_UsedItemID == E_ITEM_IRON_PICKAXE)  ||
				(a_UsedItemID == E_ITEM_DIAMOND_PICKAXE)
			)
			{
				a_Drops.push_back(cItem(E_ITEM_IRON_ORE, 1));
			}
			return;
		}
		
		// Gold and diamond ores require an iron or better pickaxe:
		case E_BLOCK_GOLD_ORE:
		case E_BLOCK_DIAMOND_ORE:
		{
			if (
				(a_UsedItemID == E_ITEM_IRON_PICKAXE) ||
				(a_UsedItemID == E_ITEM_DIAMOND_PICKAXE)
			)
			{
				a_Drops.push_back(cItem((ENUM_ITEM_ID)a_BlockType, 1));
			}
			return;
		}

		// Obsidian require a diamond pickaxe:
		case E_BLOCK_OBSIDIAN:
		{
			if (a_UsedItemID == E_ITEM_DIAMOND_PICKAXE)
			{
				a_Drops.push_back(cItem((ENUM_ITEM_ID)a_BlockType, 1));
			}
			return;
		}

		// Redstone requires an iron or better pickaxe:
		case E_BLOCK_REDSTONE_ORE_GLOWING:
		case E_BLOCK_REDSTONE_ORE:
		{
			if (
				(a_UsedItemID == E_ITEM_IRON_PICKAXE) ||
				(a_UsedItemID == E_ITEM_DIAMOND_PICKAXE)
			)
			{
				a_Drops.push_back(cItem(E_ITEM_REDSTONE_DUST, 4 + (short)r1.randInt(1)));
			}
			return;
		}
				
		// Lapis ore requires a stone or better pickaxe:
		case E_BLOCK_LAPIS_ORE:
		{
			if (
				(a_UsedItemID == E_ITEM_STONE_PICKAXE) ||
				(a_UsedItemID == E_ITEM_IRON_PICKAXE)  ||
				(a_UsedItemID == E_ITEM_GOLD_PICKAXE)  ||
				(a_UsedItemID == E_ITEM_DIAMOND_PICKAXE)
			)
			{
				a_Drops.push_back(cItem(E_ITEM_DYE, 4 + (short)r1.randInt(4), E_META_DYE_BLUE));
			}
			return;
		}
		

		////////////////////////
		// Resource blocks:
		
		// Iron and lapis blocks require a stone or better pickaxe:
		case E_BLOCK_IRON_BLOCK:
		case E_BLOCK_LAPIS_BLOCK:
		{
			if (
				(a_UsedItemID == E_ITEM_STONE_PICKAXE) ||
				(a_UsedItemID == E_ITEM_IRON_PICKAXE)  ||
				(a_UsedItemID == E_ITEM_DIAMOND_PICKAXE)
			)
			{
				a_Drops.push_back(cItem((ENUM_ITEM_ID)a_BlockType, 1));
			}
			return;
		}
		
		// Diamond and gold blocks require an iron or better pickaxe:
		case E_BLOCK_DIAMOND_BLOCK:
		{
			if (
				(a_UsedItemID == E_ITEM_IRON_PICKAXE) ||
				(a_UsedItemID == E_ITEM_DIAMOND_PICKAXE)
			)
			{
				a_Drops.push_back(cItem((ENUM_ITEM_ID)a_BlockType, 1));
			}
		}
		
		
		// These blocks require a pickaxe to drop themselves:
		case E_BLOCK_COBBLESTONE:
		case E_BLOCK_BRICK:
		case E_BLOCK_NETHER_BRICK:
		case E_BLOCK_MOSSY_COBBLESTONE:
		case E_BLOCK_STONE_SLAB:
		case E_BLOCK_COBBLESTONE_STAIRS:
		case E_BLOCK_STONE_BRICK_STAIRS:
		case E_BLOCK_NETHER_BRICK_STAIRS:
		case E_BLOCK_SANDSTONE_STAIRS:
		case E_BLOCK_SANDSTONE:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		{
			if (ItemCategory::IsPickaxe(a_UsedItemID))
			{
				a_Drops.push_back(cItem((ENUM_ITEM_ID)a_BlockType, 1));
			}
			return;
		}
		

		// Stone requires a pickaxe to drop cobblestone:
		case E_BLOCK_STONE:
		{
			if (ItemCategory::IsPickaxe(a_UsedItemID))
			{
				a_Drops.push_back(cItem(E_ITEM_COBBLESTONE, 1));
			}
			return;
		}


		// Snow requires a shovel to harvest:
		case E_BLOCK_SNOW:
		{
			if (ItemCategory::IsShovel(a_UsedItemID))
			{
				a_Drops.push_back(cItem(E_ITEM_SNOWBALL, 1));
			}
			return;
		}
		
		
		// Leaves require shears for harvesting and have a chance of dropping a sapling and a red apple:
		case E_BLOCK_LEAVES:
		{
			if (a_UsedItemID == E_ITEM_SHEARS)
			{
				a_Drops.push_back(cItem(E_ITEM_LEAVES, 1));
			}
			else
			{
				AddRandomDrop(a_Drops, r1, 5,   E_ITEM_SAPLING);
				AddRandomDrop(a_Drops, r1, 200, E_ITEM_APPLE);
			}
			return;
		}
		
		
		// Crops drop a wheat and possibly another seeds when ripe; always drop at least a single seed
		case E_BLOCK_CROPS:
		{
			if (a_BlockMeta == 7)
			{
				AddRandomDrop(a_Drops, r1, 3, E_ITEM_SEEDS);
				a_Drops.push_back(cItem(E_ITEM_WHEAT, 1));
			}
			a_Drops.push_back(cItem(E_ITEM_SEEDS, 1));
			return;
		}
		
		
		// Vines drop only with shears, otherwise they are destroyed
		case E_BLOCK_VINES:
		{
			if (a_UsedItemID == E_ITEM_SHEARS)
			{
				a_Drops.push_back(cItem(E_ITEM_VINES, 1));
			}
			return;
		}
		
		
		// Snow drops only when using a shovel
		case E_BLOCK_SNOW_BLOCK:
		{
			if (ItemCategory::IsShovel(a_UsedItemID))
			{
				a_Drops.push_back(cItem(E_ITEM_SNOWBALL,   4, 0)); return;
			}
			return;
		}


		// Random multi-drop blocks:
		case E_BLOCK_TALL_GRASS: a_Drops.push_back(cItem(E_ITEM_SEEDS,           (short)r1.randInt(3) / 2, 1)); return;
		case E_BLOCK_MELON:      a_Drops.push_back(cItem(E_ITEM_MELON_SLICE, 3 + (short)r1.randInt(2),     1)); return;
		

		// Fixed multi-drop blocks:
		case E_BLOCK_DOUBLE_STONE_SLAB:  a_Drops.push_back(cItem(E_ITEM_STONE_SLAB, 2, 0)); return;
		case E_BLOCK_DOUBLE_WOODEN_SLAB: a_Drops.push_back(cItem(E_ITEM_STEP,       2, 0)); return;
		
		default:
		{
			return;
		}
	}  // switch (a_BlockType)
}





