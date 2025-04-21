
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NoteEntity.h"
#include "../World.h"
#include "json/value.h"
#include "../Entities/Player.h"





cNoteEntity::cNoteEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World):
	Super(a_Block, a_Pos, a_World),
	m_Note(0)
{
	ASSERT(a_Block.Type() == BlockType::NoteBlock);
}





void cNoteEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cNoteEntity &>(a_Src);
	m_Note = src.m_Note;
}





bool cNoteEntity::UsedBy(cPlayer * a_Player)
{
	a_Player->GetStatistics().Custom[CustomStatistic::TuneNoteblock]++;
	IncrementNote();
	MakeSound();
	return true;
}





void cNoteEntity::MakeSound(void)
{
	char Instrument;
	AString SampleName;

	switch (m_World->GetBlock(m_Pos.addedY(-1)).Type())
	{
		case BlockType::AcaciaDoor:
		case BlockType::AcaciaFence:
		case BlockType::AcaciaFenceGate:
		case BlockType::AcaciaLog:
		case BlockType::AcaciaSign:
		case BlockType::AcaciaStairs:
		case BlockType::AcaciaTrapdoor:
		case BlockType::AcaciaWallSign:
		case BlockType::BirchDoor:
		case BlockType::BirchFence:
		case BlockType::BirchFenceGate:
		case BlockType::BirchLog:
		case BlockType::BirchSign:
		case BlockType::BirchStairs:
		case BlockType::BirchTrapdoor:
		case BlockType::BirchWallSign:
		case BlockType::BlackBanner:
		case BlockType::BlackWallBanner:
		case BlockType::BlueBanner:
		case BlockType::BlueWallBanner:
		case BlockType::Bookshelf:
		case BlockType::BrownBanner:
		case BlockType::BrownMushroomBlock:
		case BlockType::BrownWallBanner:
		case BlockType::Chest:
		case BlockType::CraftingTable:
		case BlockType::CrimsonSign:
		case BlockType::CrimsonTrapdoor:
		case BlockType::CrimsonWallSign:
		case BlockType::CyanBanner:
		case BlockType::CyanWallBanner:
		case BlockType::DarkOakDoor:
		case BlockType::DarkOakFence:
		case BlockType::DarkOakFenceGate:
		case BlockType::DarkOakLog:
		case BlockType::DarkOakSign:
		case BlockType::DarkOakStairs:
		case BlockType::DarkOakTrapdoor:
		case BlockType::DarkOakWallSign:
		case BlockType::DaylightDetector:
		case BlockType::GrayBanner:
		case BlockType::GrayWallBanner:
		case BlockType::GreenBanner:
		case BlockType::GreenWallBanner:
		case BlockType::IronTrapdoor:
		case BlockType::Jukebox:
		case BlockType::JungleDoor:
		case BlockType::JungleFence:
		case BlockType::JungleFenceGate:
		case BlockType::JungleLog:
		case BlockType::JungleSign:
		case BlockType::JungleStairs:
		case BlockType::JungleTrapdoor:
		case BlockType::JungleWallSign:
		case BlockType::LightBlueBanner:
		case BlockType::LightBlueWallBanner:
		case BlockType::LightGrayBanner:
		case BlockType::LightGrayWallBanner:
		case BlockType::LimeBanner:
		case BlockType::LimeWallBanner:
		case BlockType::MagentaBanner:
		case BlockType::MagentaWallBanner:
		case BlockType::NoteBlock:
		case BlockType::OakDoor:
		case BlockType::OakFence:
		case BlockType::OakFenceGate:
		case BlockType::OakLog:
		case BlockType::OakPlanks:
		case BlockType::OakPressurePlate:
		case BlockType::OakSign:
		case BlockType::OakSlab:
		case BlockType::OakStairs:
		case BlockType::OakTrapdoor:
		case BlockType::OakWallSign:
		case BlockType::OrangeBanner:
		case BlockType::OrangeWallBanner:
		case BlockType::PinkBanner:
		case BlockType::PinkWallBanner:
		case BlockType::PurpleBanner:
		case BlockType::PurpleWallBanner:
		case BlockType::RedBanner:
		case BlockType::RedMushroomBlock:
		case BlockType::RedWallBanner:
		case BlockType::SpruceDoor:
		case BlockType::SpruceFence:
		case BlockType::SpruceFenceGate:
		case BlockType::SpruceLog:
		case BlockType::SpruceSign:
		case BlockType::SpruceStairs:
		case BlockType::SpruceTrapdoor:
		case BlockType::SpruceWallSign:
		case BlockType::TrappedChest:
		case BlockType::WarpedFence:
		case BlockType::WarpedSign:
		case BlockType::WarpedTrapdoor:
		case BlockType::WarpedWallSign:
		case BlockType::WhiteBanner:
		case BlockType::WhiteWallBanner:
		case BlockType::YellowBanner:
		case BlockType::YellowWallBanner:

		{
			Instrument = E_INST_DOUBLE_BASS;
			SampleName = "block.note_block.bass";
			break;
		}

		case BlockType::Gravel:
		case BlockType::Sand:
		case BlockType::SoulSand:
		{
			Instrument = E_INST_SNARE_DRUM;
			SampleName = "block.note_block.snare";
			break;
		}
		case BlockType::Beacon:
		case BlockType::BlackStainedGlass:
		case BlockType::BlackStainedGlassPane:
		case BlockType::BlueStainedGlass:
		case BlockType::BlueStainedGlassPane:
		case BlockType::BrownStainedGlass:
		case BlockType::BrownStainedGlassPane:
		case BlockType::CyanStainedGlass:
		case BlockType::CyanStainedGlassPane:
		case BlockType::Glass:
		case BlockType::GlassPane:
		case BlockType::Glowstone:
		case BlockType::GrayStainedGlass:
		case BlockType::GrayStainedGlassPane:
		case BlockType::GreenStainedGlass:
		case BlockType::GreenStainedGlassPane:
		case BlockType::LightBlueStainedGlass:
		case BlockType::LightBlueStainedGlassPane:
		case BlockType::LightGrayStainedGlass:
		case BlockType::LightGrayStainedGlassPane:
		case BlockType::LimeStainedGlass:
		case BlockType::LimeStainedGlassPane:
		case BlockType::MagentaStainedGlass:
		case BlockType::MagentaStainedGlassPane:
		case BlockType::OrangeStainedGlass:
		case BlockType::OrangeStainedGlassPane:
		case BlockType::PinkStainedGlass:
		case BlockType::PinkStainedGlassPane:
		case BlockType::PurpleStainedGlass:
		case BlockType::PurpleStainedGlassPane:
		case BlockType::RedStainedGlass:
		case BlockType::RedStainedGlassPane:
		case BlockType::SeaLantern:
		case BlockType::WhiteStainedGlass:
		case BlockType::WhiteStainedGlassPane:
		case BlockType::YellowStainedGlass:
		case BlockType::YellowStainedGlassPane:
		{
			Instrument = E_INST_CLICKS;
			SampleName = "block.note_block.hat";
			break;
		}

		case BlockType::Bedrock:
		case BlockType::BlackShulkerBox:
		case BlockType::BlueShulkerBox:
		case BlockType::BrickStairs:
		case BlockType::Bricks:
		case BlockType::BrownShulkerBox:
		case BlockType::CoalBlock:
		case BlockType::CoalOre:
		case BlockType::Cobblestone:
		case BlockType::CobblestoneStairs:
		case BlockType::CobblestoneWall:
		case BlockType::CyanShulkerBox:
		case BlockType::DiamondOre:
		case BlockType::Dispenser:
		case BlockType::Dropper:
		case BlockType::EmeraldOre:
		case BlockType::EnchantingTable:
		case BlockType::EndPortalFrame:
		case BlockType::EndStone:
		case BlockType::EndStoneBrickSlab:
		case BlockType::EndStoneBrickStairs:
		case BlockType::EndStoneBricks:
		case BlockType::EnderChest:
		case BlockType::Furnace:
		case BlockType::GoldOre:
		case BlockType::GrayShulkerBox:
		case BlockType::GreenShulkerBox:
		case BlockType::IronOre:
		case BlockType::LapisOre:
		case BlockType::LightBlueShulkerBox:
		case BlockType::LightGrayShulkerBox:
		case BlockType::LimeShulkerBox:
		case BlockType::MagentaShulkerBox:
		case BlockType::MagmaBlock:
		case BlockType::MossyCobblestone:
		case BlockType::MossyCobblestoneSlab:
		case BlockType::MossyCobblestoneStairs:
		case BlockType::MossyCobblestoneWall:
		case BlockType::NetherBrickFence:
		case BlockType::NetherBrickStairs:
		case BlockType::NetherBricks:
		case BlockType::NetherQuartzOre:
		case BlockType::Netherrack:
		case BlockType::Observer:
		case BlockType::Obsidian:
		case BlockType::OrangeShulkerBox:
		case BlockType::PinkShulkerBox:
		case BlockType::Prismarine:
		case BlockType::PurpleShulkerBox:
		case BlockType::PurpurBlock:
		case BlockType::PurpurPillar:
		case BlockType::PurpurSlab:
		case BlockType::PurpurStairs:
		case BlockType::QuartzBlock:
		case BlockType::QuartzStairs:
		case BlockType::RedNetherBrickSlab:
		case BlockType::RedNetherBrickStairs:
		case BlockType::RedNetherBrickWall:
		case BlockType::RedNetherBricks:
		case BlockType::RedSandstone:
		case BlockType::RedSandstoneSlab:
		case BlockType::RedSandstoneStairs:
		case BlockType::RedShulkerBox:
		case BlockType::RedstoneOre:
		case BlockType::Sandstone:
		case BlockType::SandstoneStairs:
		case BlockType::Spawner:
		case BlockType::Stone:
		case BlockType::StoneBrickStairs:
		case BlockType::StoneBricks:
		case BlockType::StonePressurePlate:
		case BlockType::StoneSlab:
		case BlockType::WhiteShulkerBox:
		case BlockType::YellowShulkerBox:
		{
			Instrument = E_INST_BASS_DRUM;
			SampleName = "block.note_block.basedrum";
			break;
		}

		case BlockType::Clay:
		{
			Instrument = E_INST_FLUTE;
			SampleName = "block.note_block.flute";
			break;
		}

		case BlockType::GoldBlock:
		{
			Instrument = E_INST_BELL;
			SampleName = "block.note_block.bell";
			break;
		}

		case BlockType::BlackWool:
		case BlockType::BlueWool:
		case BlockType::BrownWool:
		case BlockType::CyanWool:
		case BlockType::GrayWool:
		case BlockType::GreenWool:
		case BlockType::LightBlueWool:
		case BlockType::LightGrayWool:
		case BlockType::LimeWool:
		case BlockType::MagentaWool:
		case BlockType::OrangeWool:
		case BlockType::PinkWool:
		case BlockType::PurpleWool:
		case BlockType::RedWool:
		case BlockType::WhiteWool:
		case BlockType::YellowWool:
		{
			Instrument = E_INST_GUITAR;
			SampleName = "block.note_block.guitar";
			break;
		}

		case BlockType::PackedIce:
		{
			Instrument = E_INST_CHIME;
			SampleName = "block.note_block.chime";
			break;
		}

		case BlockType::BoneBlock:
		{
			Instrument = E_INST_XYLOPHONE;
			SampleName = "block.note_block.xylophone";
			break;
		}

		default:
		{
			Instrument = E_INST_HARP_PIANO;
			SampleName = "block.note_block.harp";
			break;
		}
	}

	m_World->BroadcastBlockAction(m_Pos, static_cast<Byte>(Instrument), static_cast<Byte>(m_Note), BlockType::NoteBlock);

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
