
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NoteEntity.h"
#include "../World.h"
#include "json/value.h"
#include "../Entities/Player.h"





cNoteEntity::cNoteEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_Pos, a_World),
	m_Note(0)
{
	ASSERT(a_BlockType == E_BLOCK_NOTE_BLOCK);
}





void cNoteEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cNoteEntity &>(a_Src);
	m_Note = src.m_Note;
}





bool cNoteEntity::UsedBy(cPlayer * a_Player)
{
	a_Player->GetStatManager().AddValue(Statistic::TuneNoteblock);
	IncrementNote();
	MakeSound();
	return true;
}





void cNoteEntity::MakeSound(void)
{
	char Instrument;
	AString SampleName;

	switch (m_World->GetBlock(m_Pos.addedY(-1)))
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
			Instrument = E_INST_DOUBLE_BASS;
			SampleName = "block.note.bass";
			break;
		}

		case E_BLOCK_GRAVEL:
		case E_BLOCK_SAND:
		case E_BLOCK_SOULSAND:
		{
			Instrument = E_INST_SNARE_DRUM;
			SampleName = "block.note.snare";
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
			Instrument = E_INST_CLICKS;
			SampleName = "block.note.hat";
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
			Instrument = E_INST_BASS_DRUM;
			SampleName = "block.note.basedrum";
			break;
		}

		case E_BLOCK_CLAY:
		{
			Instrument = E_INST_FLUTE;
			SampleName = "block.note.flute";
			break;
		}

		case E_BLOCK_GOLD_BLOCK:
		{
			Instrument = E_INST_BELL;
			SampleName = "block.note.bell";
			break;
		}

		case E_BLOCK_WOOL:
		{
			Instrument = E_INST_GUITAR;
			SampleName = "block.note.guitar";
			break;
		}

		case E_BLOCK_PACKED_ICE:
		{
			Instrument = E_INST_CHIME;
			SampleName = "block.note.chime";
			break;
		}

		case E_BLOCK_BONE_BLOCK:
		{
			Instrument = E_INST_XYLOPHONE;
			SampleName = "block.note.xylophone";
			break;
		}

		default:
		{
			Instrument = E_INST_HARP_PIANO;
			SampleName = "block.note.harp";
			break;
		}
	}

	m_World->BroadcastBlockAction(m_Pos, static_cast<Byte>(Instrument), static_cast<Byte>(m_Note), E_BLOCK_NOTE_BLOCK);

	m_World->BroadcastSoundEffect(
		SampleName,
		m_Pos,
		3.0f,
		PitchFromNote(m_Note)
	);
}





unsigned char cNoteEntity::GetNote(void)
{
	return m_Note;
}





void cNoteEntity::SetNote(unsigned char a_Note)
{
	m_Note = a_Note % 25;
}





void cNoteEntity::IncrementNote(void)
{
	SetNote(m_Note + 1);
}





float cNoteEntity::PitchFromNote(unsigned char a_Pitch)
{
	// This replaces the calculation of:
	// float calcPitch = static_cast<float>(pow(2.0f, static_cast<float>(m_Note - 12.0f) / 12.0f));
	// So 2 ^ ((m_Note - 12) / 12)
	switch (a_Pitch)
	{
		case 0: return 0.5f;
		case 1: return 0.5297315471796477f;
		case 2: return 0.5612310241546865f;
		case 3: return 0.5946035575013605f;
		case 4: return 0.6299605249474366f;
		case 5: return 0.6674199270850172f;
		case 6: return 0.7071067811865476f;
		case 7: return 0.7491535384383408f;
		case 8: return 0.7937005259840998f;
		case 9: return 0.8408964152537145f;
		case 10: return 0.8908987181403393f;
		case 11: return 0.9438743126816935f;
		case 12: return 1.0f;
		case 13: return 1.0594630943592953f;
		case 14: return 1.122462048309373f;
		case 15: return 1.189207115002721f;
		case 16: return 1.2599210498948732f;
		case 17: return 1.3348398541700344f;
		case 18: return 1.4142135623730951f;
		case 19: return 1.4983070768766815f;
		case 20: return 1.5874010519681994f;
		case 21: return 1.681792830507429f;
		case 22: return 1.7817974362806785f;
		case 23: return 1.887748625363387f;
		case 24: return 2.0f;
	}

	UNREACHABLE("Converted unknown pitch value");
}
