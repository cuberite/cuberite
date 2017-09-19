
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NoteEntity.h"
#include "../World.h"
#include "json/value.h"





cNoteEntity::cNoteEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, a_World),
	m_Pitch(0)
{
	ASSERT(a_BlockType == E_BLOCK_NOTE_BLOCK);
}





void cNoteEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = reinterpret_cast<const cNoteEntity &>(a_Src);
	m_Pitch = src.m_Pitch;
}





bool cNoteEntity::UsedBy(cPlayer * a_Player)
{
	UNUSED(a_Player);
	IncrementPitch();
	MakeSound();
	return true;
}





void cNoteEntity::MakeSound(void)
{
	char instrument;
	AString sampleName;

	switch (m_World->GetBlock(m_PosX, m_PosY - 1, m_PosZ))
	{
		case E_BLOCK_ACACIA_DOOR:
		case E_BLOCK_ACACIA_FENCE:
		case E_BLOCK_ACACIA_FENCE_GATE:
		case E_BLOCK_ACACIA_WOOD_STAIRS:
		case E_BLOCK_BIRCH_DOOR:
		case E_BLOCK_BIRCH_FENCE:
		case E_BLOCK_BIRCH_FENCE_GATE:
		case E_BLOCK_BIRCH_WOOD_STAIRS:
		case E_BLOCK_BOOKCASE:
		case E_BLOCK_CHEST:
		case E_BLOCK_CRAFTING_TABLE:
		case E_BLOCK_DARK_OAK_DOOR:
		case E_BLOCK_DARK_OAK_FENCE:
		case E_BLOCK_DARK_OAK_FENCE_GATE:
		case E_BLOCK_DARK_OAK_WOOD_STAIRS:
		case E_BLOCK_DAYLIGHT_SENSOR:
		case E_BLOCK_DOUBLE_WOODEN_SLAB:
		case E_BLOCK_FENCE:
		case E_BLOCK_HUGE_BROWN_MUSHROOM:
		case E_BLOCK_HUGE_RED_MUSHROOM:
		case E_BLOCK_INVERTED_DAYLIGHT_SENSOR:
		case E_BLOCK_JUKEBOX:
		case E_BLOCK_JUNGLE_DOOR:
		case E_BLOCK_JUNGLE_FENCE:
		case E_BLOCK_JUNGLE_FENCE_GATE:
		case E_BLOCK_JUNGLE_WOOD_STAIRS:
		case E_BLOCK_LOG:
		case E_BLOCK_NEW_LOG:
		case E_BLOCK_NOTE_BLOCK:
		case E_BLOCK_OAK_DOOR:
		case E_BLOCK_OAK_FENCE_GATE:
		case E_BLOCK_OAK_WOOD_STAIRS:
		case E_BLOCK_PLANKS:
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_SPRUCE_DOOR:
		case E_BLOCK_SPRUCE_FENCE:
		case E_BLOCK_SPRUCE_FENCE_GATE:
		case E_BLOCK_SPRUCE_WOOD_STAIRS:
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_TRAPDOOR:
		case E_BLOCK_TRAPPED_CHEST:
		case E_BLOCK_WALL_BANNER:
		case E_BLOCK_WALLSIGN:
		case E_BLOCK_WOODEN_PRESSURE_PLATE:
		case E_BLOCK_WOODEN_SLAB:
		{
			instrument = E_INST_DOUBLE_BASS;
			sampleName = "block.note.bass";
			break;
		}

		case E_BLOCK_GRAVEL:
		case E_BLOCK_SAND:
		case E_BLOCK_SOULSAND:
		{
			instrument = E_INST_SNARE_DRUM;
			sampleName = "block.note.snare";
			break;
		}

		case E_BLOCK_BEACON:
		case E_BLOCK_GLASS:
		case E_BLOCK_GLASS_PANE:
		case E_BLOCK_GLOWSTONE:
		case E_BLOCK_SEA_LANTERN:
		case E_BLOCK_STAINED_GLASS:
		case E_BLOCK_STAINED_GLASS_PANE:
		{
			instrument = E_INST_CLICKS;
			sampleName = "block.note.hat";
			break;
		}

		case E_BLOCK_BEDROCK:
		case E_BLOCK_BLACK_SHULKER_BOX:
		case E_BLOCK_BLOCK_OF_COAL:
		case E_BLOCK_BLUE_SHULKER_BOX:
		case E_BLOCK_BRICK:
		case E_BLOCK_BRICK_STAIRS:
		case E_BLOCK_BROWN_SHULKER_BOX:
		case E_BLOCK_COAL_ORE:
		case E_BLOCK_COBBLESTONE:
		case E_BLOCK_COBBLESTONE_STAIRS:
		case E_BLOCK_COBBLESTONE_WALL:
		case E_BLOCK_CYAN_SHULKER_BOX:
		case E_BLOCK_DIAMOND_ORE:
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB:
		case E_BLOCK_DOUBLE_STONE_SLAB:
		case E_BLOCK_DROPPER:
		case E_BLOCK_EMERALD_ORE:
		case E_BLOCK_ENCHANTMENT_TABLE:
		case E_BLOCK_END_BRICKS:
		case E_BLOCK_END_PORTAL_FRAME:
		case E_BLOCK_END_STONE:
		case E_BLOCK_ENDER_CHEST:
		case E_BLOCK_FURNACE:
		case E_BLOCK_GOLD_ORE:
		case E_BLOCK_GRAY_SHULKER_BOX:
		case E_BLOCK_GREEN_SHULKER_BOX:
		case E_BLOCK_IRON_ORE:
		case E_BLOCK_LAPIS_ORE:
		case E_BLOCK_LIGHT_BLUE_SHULKER_BOX:
		case E_BLOCK_LIGHT_GRAY_SHULKER_BOX:
		case E_BLOCK_LIME_SHULKER_BOX:
		case E_BLOCK_LIT_FURNACE:
		case E_BLOCK_MAGENTA_SHULKER_BOX:
		case E_BLOCK_MAGMA:
		case E_BLOCK_MOB_SPAWNER:
		case E_BLOCK_MOSSY_COBBLESTONE:
		case E_BLOCK_NETHER_BRICK:
		case E_BLOCK_NETHER_BRICK_FENCE:
		case E_BLOCK_NETHER_BRICK_STAIRS:
		case E_BLOCK_NETHER_QUARTZ_ORE:
		case E_BLOCK_NETHERRACK:
		case E_BLOCK_OBSERVER:
		case E_BLOCK_OBSIDIAN:
		case E_BLOCK_ORANGE_SHULKER_BOX:
		case E_BLOCK_PINK_SHULKER_BOX:
		case E_BLOCK_PRISMARINE_BLOCK:
		case E_BLOCK_PURPLE_SHULKER_BOX:
		case E_BLOCK_PURPUR_BLOCK:
		case E_BLOCK_PURPUR_DOUBLE_SLAB:
		case E_BLOCK_PURPUR_PILLAR:
		case E_BLOCK_PURPUR_SLAB:
		case E_BLOCK_PURPUR_STAIRS:
		case E_BLOCK_QUARTZ_BLOCK:
		case E_BLOCK_QUARTZ_STAIRS:
		case E_BLOCK_RED_NETHER_BRICK:
		case E_BLOCK_RED_SANDSTONE:
		case E_BLOCK_RED_SANDSTONE_SLAB:
		case E_BLOCK_RED_SANDSTONE_STAIRS:
		case E_BLOCK_RED_SHULKER_BOX:
		case E_BLOCK_REDSTONE_ORE:
		case E_BLOCK_REDSTONE_ORE_GLOWING:
		case E_BLOCK_SANDSTONE:
		case E_BLOCK_SANDSTONE_STAIRS:
		case E_BLOCK_STONE:
		case E_BLOCK_STONE_BRICK_STAIRS:
		case E_BLOCK_STONE_BRICKS:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_STONE_SLAB:
		case E_BLOCK_WHITE_SHULKER_BOX:
		case E_BLOCK_YELLOW_SHULKER_BOX:
		{
			instrument = E_INST_BASS_DRUM;
			sampleName = "block.note.basedrum";
			break;
		}

		case E_BLOCK_CLAY:
		{
			instrument = E_INST_FLUTE;
			sampleName = "block.note.flute";
			break;
		}

		case E_BLOCK_GOLD_BLOCK:
		{
			instrument = E_INST_BELL;
			sampleName = "block.note.bell";
			break;
		}

		case E_BLOCK_WOOL:
		{
			instrument = E_INST_GUITAR;
			sampleName = "block.note.guitar";
			break;
		}

		case E_BLOCK_PACKED_ICE:
		{
			instrument = E_INST_CHIME;
			sampleName = "block.note.chime";
			break;
		}

		case E_BLOCK_BONE_BLOCK:
		{
			instrument = E_INST_XYLOPHONE;
			sampleName = "block.note.xylophone";
			break;
		}

		default:
		{
			instrument = E_INST_HARP_PIANO;
			sampleName = "block.note.harp";
			break;
		}
	}

	m_World->BroadcastBlockAction({m_PosX, m_PosY, m_PosZ}, static_cast<Byte>(instrument), static_cast<Byte>(m_Pitch), E_BLOCK_NOTE_BLOCK);

	// TODO: instead of calculating the power function over and over, make a precalculated table - there's only 24 pitches after all
	float calcPitch = static_cast<float>(pow(2.0f, static_cast<float>(m_Pitch - 12.0f) / 12.0f));
	m_World->BroadcastSoundEffect(
		sampleName,
		Vector3d(m_PosX, m_PosY, m_PosZ),
		3.0f,
		calcPitch
	);
}





char cNoteEntity::GetPitch(void)
{
	return m_Pitch;
}





void cNoteEntity::SetPitch(char a_Pitch)
{
	m_Pitch = a_Pitch % 25;
}





void cNoteEntity::IncrementPitch(void)
{
	SetPitch(m_Pitch + 1);
}




