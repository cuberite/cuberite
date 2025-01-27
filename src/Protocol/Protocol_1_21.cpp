#include "Globals.h"
#include "Protocol_1_21.h"
#include "Packetizer.h"
#include "Root.h"
#include "Server.h"
#include "Entities/Player.h"
#include <ClientHandle.h>
#include "../AllTags/BlockTags.h"
#include "BlockEntities/BlockEntity.h"
#include "WorldStorage/FastNBT.h"
#include "Palettes/Palette_1_21.h"
#include "Palettes/Palette_1_21_2.h"
#include "../Entities/Entity.h"


////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_21:

cProtocol::Version cProtocol_1_21::GetProtocolVersion() const
{
	return Version::v1_21;
}





UInt32 cProtocol_1_21::GetPacketID(ePacketType a_PacketType) const
{
	return Super::GetPacketID(a_PacketType);
	/* switch (m_State)
	{
		case cProtocol::Status:
		case cProtocol::Login: return Super::GetPacketID(a_PacketType);
		case cProtocol::Game:
			switch (a_PacketType)
			{
				default: return Super::GetPacketID(a_PacketType);
				// CustomReportDetailsS2CPacket 0x0F
				// ServerLinksS2CPacket 0x10
			}
			break;
		case cProtocol::Configuration:
			switch (a_PacketType)
			{
				default: return Super::GetPacketID(a_PacketType);
				// CustomReportDetailsS2CPacket 0x7A
				// ServerLinksS2CPacket 0x7B
			}
			break;
		// default: ASSERT(false, "Invalid m_State: %d", m_State); break;
	}
	UNREACHABLE(""); */
}





void cProtocol_1_21::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	ASSERT(m_State == 3);  // In game mode?

	Byte Action;

	auto type = a_BlockEntity.GetBlockType();
	if (BlockTags::Banners(type))
	{
		Action = 19;
	}
	else if (BlockTags::Beds(type))
	{
		Action = 24;
	}
	else if (BlockTags::AllHangingSigns(type))
	{
		Action = 8;
	}
	else if (BlockTags::StandingSigns(type))
	{
		Action = 7;
	}
	else if (BlockTags::FlowerPots(type))
	{
		return;  // temp fix
	}
	else 	// TODO: skulls
	{
		switch (a_BlockEntity.GetBlockType())
		{
			// case BlockType::Banner:                Action = 19; break;
			case BlockType::Barrel:                Action = 26; break;
			case BlockType::Beacon:                Action = 14; break;
			// case BlockType::Bed:                   Action = 24; break;
			case BlockType::Beehive:               Action = 33; break;
			case BlockType::Bell:                  Action = 30; break;
			case BlockType::BlastFurnace:          Action = 28; break;
			case BlockType::BrewingStand:          Action = 11; break;
			// case BlockType::BrushableBlock:        Action = 39; break;
			case BlockType::CalibratedSculkSensor: Action = 35; break;
			case BlockType::Campfire:              Action = 32; break;
			case BlockType::Chest:                 Action = 1; break;
			case BlockType::ChiseledBookshelf:     Action = 38; break;
			case BlockType::RepeatingCommandBlock:
			case BlockType::ChainCommandBlock:
			case BlockType::CommandBlock:          Action = 22; break;
			case BlockType::Comparator:            Action = 18; break;
			case BlockType::Conduit:               Action = 25; break;
			case BlockType::Crafter:               Action = 41; break;
			case BlockType::DaylightDetector:      Action = 16; break;
			case BlockType::DecoratedPot:          Action = 40; break;
			case BlockType::Dispenser:             Action = 5; break;
			case BlockType::Dropper:               Action = 6; break;
			case BlockType::EnchantingTable:       Action = 12; break;
			case BlockType::EndGateway:            Action = 21; break;
			case BlockType::EndPortal:             Action = 13; break;
			case BlockType::EnderChest:            Action = 3; break;
			case BlockType::Furnace:               Action = 0; break;
			// case BlockType::HangingSign:           Action = 8; break;
			case BlockType::Hopper:                Action = 17; break;
			case BlockType::Jigsaw:                Action = 31; break;
			case BlockType::Jukebox:               Action = 4; break;
			case BlockType::Lectern:               Action = 29; break;
			case BlockType::Spawner:               Action = 9; break;
			case BlockType::Piston:                Action = 10; break;
			case BlockType::SculkCatalyst:         Action = 36; break;
			case BlockType::SculkSensor:           Action = 34; break;
			case BlockType::SculkShrieker:         Action = 37; break;
			case BlockType::ShulkerBox:            Action = 23; break;
			// case BlockType::Sign:                  Action = 7; break;
			// case BlockType::Skull:                 Action = 15; break;
			case BlockType::Smoker:                Action = 27; break;
			case BlockType::StructureBlock:        Action = 20; break;
			case BlockType::TrappedChest:          Action = 2; break;
			case BlockType::TrialSpawner:          Action = 42; break;
			case BlockType::Vault:                 Action = 43; break;

			default: UNREACHABLE("Unknown block entity");  // Block entities change between versions
		}
	}


	cPacketizer Pkt(*this, pktUpdateBlockEntity);
	Pkt.WriteXZYPosition64(a_BlockEntity.GetPosX(), a_BlockEntity.GetPosY(), a_BlockEntity.GetPosZ());
	Pkt.WriteVarInt32(Action);

	cFastNBTWriter Writer(true);
	WriteBlockEntity(Writer, a_BlockEntity);
	Writer.Finish();
	Pkt.WriteBuf(Writer.GetResult());
}





void cProtocol_1_21::SendDynamicRegistries()
{
	{
		{
			cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
			Pkt.WriteString("minecraft:dimension_type");
			Pkt.WriteVarInt32(1);
			Pkt.WriteString("minecraft:overworld");
			Pkt.WriteBool(true);
			cFastNBTWriter Writer(true);
			Writer.AddByte("piglin_safe", 1);
			Writer.AddByte("natural", 1);
			Writer.AddFloat("ambient_light", 1.0);
			Writer.AddInt("monster_spawn_block_light_limit", 0);
			Writer.AddString("infiniburn", "#infiniburn_overworld");
			Writer.AddByte("respawn_anchor_works", 1);
			Writer.AddByte("has_skylight", 1);
			Writer.AddByte("bed_works", 1);
			Writer.AddString("effects", "minecraft:overworld");
			Writer.AddByte("has_raids", 1);
			Writer.AddInt("logical_height", 256);
			Writer.AddDouble("coordinate_scale", 1.0);
			Writer.AddByte("ultrawarm", 0);
			Writer.AddByte("has_ceiling", 0);
			Writer.AddInt("min_y", 0);
			Writer.AddInt("height", 256);
			Writer.AddInt("monster_spawn_light_level", 0);
			Writer.AddInt("monster_spawn_block_light_limit", 0);
			Writer.Finish();
			Pkt.WriteBuf(Writer.GetResult());
		}
		{
			cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
			Pkt.WriteString("minecraft:worldgen/biome");
			Pkt.WriteVarInt32(64);
			Pkt.WriteString("plains"); Pkt.WriteBool(false);  // temporarily here
			Pkt.WriteString("badlands"); Pkt.WriteBool(false);
			Pkt.WriteString("bamboo_jungle"); Pkt.WriteBool(false);
			Pkt.WriteString("basalt_deltas"); Pkt.WriteBool(false);
			Pkt.WriteString("beach"); Pkt.WriteBool(false);
			Pkt.WriteString("birch_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("cherry_grove"); Pkt.WriteBool(false);
			Pkt.WriteString("cold_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("crimson_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("dark_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("deep_cold_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("deep_dark"); Pkt.WriteBool(false);
			Pkt.WriteString("deep_frozen_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("deep_lukewarm_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("deep_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("desert"); Pkt.WriteBool(false);
			Pkt.WriteString("dripstone_caves"); Pkt.WriteBool(false);
			Pkt.WriteString("end_barrens"); Pkt.WriteBool(false);
			Pkt.WriteString("end_highlands"); Pkt.WriteBool(false);
			Pkt.WriteString("end_midlands"); Pkt.WriteBool(false);
			Pkt.WriteString("eroded_badlands"); Pkt.WriteBool(false);
			Pkt.WriteString("flower_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("forest"); Pkt.WriteBool(false);
			Pkt.WriteString("frozen_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("frozen_peaks"); Pkt.WriteBool(false);
			Pkt.WriteString("frozen_river"); Pkt.WriteBool(false);
			Pkt.WriteString("grove"); Pkt.WriteBool(false);
			Pkt.WriteString("ice_spikes"); Pkt.WriteBool(false);
			Pkt.WriteString("jagged_peaks"); Pkt.WriteBool(false);
			Pkt.WriteString("jungle"); Pkt.WriteBool(false);
			Pkt.WriteString("lukewarm_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("lush_caves"); Pkt.WriteBool(false);
			Pkt.WriteString("mangrove_swamp"); Pkt.WriteBool(false);
			Pkt.WriteString("meadow"); Pkt.WriteBool(false);
			Pkt.WriteString("mushroom_fields"); Pkt.WriteBool(false);
			Pkt.WriteString("nether_wastes"); Pkt.WriteBool(false);
			Pkt.WriteString("ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("old_growth_birch_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("old_growth_pine_taiga"); Pkt.WriteBool(false);
			Pkt.WriteString("old_growth_spruce_taiga"); Pkt.WriteBool(false);
			// Pkt.WriteString("plains"); Pkt.WriteBool(false);
			Pkt.WriteString("river"); Pkt.WriteBool(false);
			Pkt.WriteString("savanna"); Pkt.WriteBool(false);
			Pkt.WriteString("savanna_plateau"); Pkt.WriteBool(false);
			Pkt.WriteString("small_end_islands"); Pkt.WriteBool(false);
			Pkt.WriteString("snowy_beach"); Pkt.WriteBool(false);
			Pkt.WriteString("snowy_plains"); Pkt.WriteBool(false);
			Pkt.WriteString("snowy_slopes"); Pkt.WriteBool(false);
			Pkt.WriteString("snowy_taiga"); Pkt.WriteBool(false);
			Pkt.WriteString("soul_sand_valley"); Pkt.WriteBool(false);
			Pkt.WriteString("sparse_jungle"); Pkt.WriteBool(false);
			Pkt.WriteString("stony_peaks"); Pkt.WriteBool(false);
			Pkt.WriteString("stony_shore"); Pkt.WriteBool(false);
			Pkt.WriteString("sunflower_plains"); Pkt.WriteBool(false);
			Pkt.WriteString("swamp"); Pkt.WriteBool(false);
			Pkt.WriteString("taiga"); Pkt.WriteBool(false);
			Pkt.WriteString("the_end"); Pkt.WriteBool(false);
			Pkt.WriteString("the_void"); Pkt.WriteBool(false);
			Pkt.WriteString("warm_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("warped_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("windswept_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("windswept_gravelly_hills"); Pkt.WriteBool(false);
			Pkt.WriteString("windswept_hills"); Pkt.WriteBool(false);
			Pkt.WriteString("windswept_savanna"); Pkt.WriteBool(false);
			Pkt.WriteString("wooded_badlands"); Pkt.WriteBool(false);
		}
		{
			cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
			Pkt.WriteString("minecraft:wolf_variant");
			Pkt.WriteVarInt32(9);
			Pkt.WriteString("ashen"); Pkt.WriteBool(false);
			Pkt.WriteString("black"); Pkt.WriteBool(false);
			Pkt.WriteString("chestnut"); Pkt.WriteBool(false);
			Pkt.WriteString("pale"); Pkt.WriteBool(false);
			Pkt.WriteString("rusty"); Pkt.WriteBool(false);
			Pkt.WriteString("snowy"); Pkt.WriteBool(false);
			Pkt.WriteString("spotted"); Pkt.WriteBool(false);
			Pkt.WriteString("striped"); Pkt.WriteBool(false);
			Pkt.WriteString("woods"); Pkt.WriteBool(false);
		}
		{
			cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
			Pkt.WriteString("minecraft:painting_variant");
			Pkt.WriteVarInt32(50);
			Pkt.WriteString("alban"); Pkt.WriteBool(false);
			Pkt.WriteString("aztec"); Pkt.WriteBool(false);
			Pkt.WriteString("aztec2"); Pkt.WriteBool(false);
			Pkt.WriteString("backyard"); Pkt.WriteBool(false);
			Pkt.WriteString("baroque"); Pkt.WriteBool(false);
			Pkt.WriteString("bomb"); Pkt.WriteBool(false);
			Pkt.WriteString("bouquet"); Pkt.WriteBool(false);
			Pkt.WriteString("burning_skull"); Pkt.WriteBool(false);
			Pkt.WriteString("bust"); Pkt.WriteBool(false);
			Pkt.WriteString("cavebird"); Pkt.WriteBool(false);
			Pkt.WriteString("changing"); Pkt.WriteBool(false);
			Pkt.WriteString("cotan"); Pkt.WriteBool(false);
			Pkt.WriteString("courbet"); Pkt.WriteBool(false);
			Pkt.WriteString("creebet"); Pkt.WriteBool(false);
			Pkt.WriteString("donkey_kong"); Pkt.WriteBool(false);
			Pkt.WriteString("earth"); Pkt.WriteBool(false);
			Pkt.WriteString("endboss"); Pkt.WriteBool(false);
			Pkt.WriteString("fern"); Pkt.WriteBool(false);
			Pkt.WriteString("fighters"); Pkt.WriteBool(false);
			Pkt.WriteString("finding"); Pkt.WriteBool(false);
			Pkt.WriteString("fire"); Pkt.WriteBool(false);
			Pkt.WriteString("graham"); Pkt.WriteBool(false);
			Pkt.WriteString("humble"); Pkt.WriteBool(false);
			Pkt.WriteString("kebab"); Pkt.WriteBool(false);
			Pkt.WriteString("lowmist"); Pkt.WriteBool(false);
			Pkt.WriteString("match"); Pkt.WriteBool(false);
			Pkt.WriteString("meditative"); Pkt.WriteBool(false);
			Pkt.WriteString("orb"); Pkt.WriteBool(false);
			Pkt.WriteString("owlemons"); Pkt.WriteBool(false);
			Pkt.WriteString("passage"); Pkt.WriteBool(false);
			Pkt.WriteString("pigscene"); Pkt.WriteBool(false);
			Pkt.WriteString("plant"); Pkt.WriteBool(false);
			Pkt.WriteString("pointer"); Pkt.WriteBool(false);
			Pkt.WriteString("pond"); Pkt.WriteBool(false);
			Pkt.WriteString("pool"); Pkt.WriteBool(false);
			Pkt.WriteString("prairie_ride"); Pkt.WriteBool(false);
			Pkt.WriteString("sea"); Pkt.WriteBool(false);
			Pkt.WriteString("skeleton"); Pkt.WriteBool(false);
			Pkt.WriteString("skull_and_roses"); Pkt.WriteBool(false);
			Pkt.WriteString("stage"); Pkt.WriteBool(false);
			Pkt.WriteString("sunflowers"); Pkt.WriteBool(false);
			Pkt.WriteString("sunset"); Pkt.WriteBool(false);
			Pkt.WriteString("tides"); Pkt.WriteBool(false);
			Pkt.WriteString("unpacked"); Pkt.WriteBool(false);
			Pkt.WriteString("void"); Pkt.WriteBool(false);
			Pkt.WriteString("wanderer"); Pkt.WriteBool(false);
			Pkt.WriteString("wasteland"); Pkt.WriteBool(false);
			Pkt.WriteString("water"); Pkt.WriteBool(false);
			Pkt.WriteString("wind"); Pkt.WriteBool(false);
			Pkt.WriteString("wither"); Pkt.WriteBool(false);
		}
		{
			cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
			Pkt.WriteString("minecraft:damage_type");
			Pkt.WriteVarInt32(47);
			Pkt.WriteString("arrow"); Pkt.WriteBool(false);
			Pkt.WriteString("bad_respawn_point"); Pkt.WriteBool(false);
			Pkt.WriteString("cactus"); Pkt.WriteBool(false);
			Pkt.WriteString("campfire"); Pkt.WriteBool(false);
			Pkt.WriteString("cramming"); Pkt.WriteBool(false);
			Pkt.WriteString("dragon_breath"); Pkt.WriteBool(false);
			Pkt.WriteString("drown"); Pkt.WriteBool(false);
			Pkt.WriteString("dry_out"); Pkt.WriteBool(false);
			Pkt.WriteString("explosion"); Pkt.WriteBool(false);
			Pkt.WriteString("fall"); Pkt.WriteBool(false);
			Pkt.WriteString("falling_anvil"); Pkt.WriteBool(false);
			Pkt.WriteString("falling_block"); Pkt.WriteBool(false);
			Pkt.WriteString("falling_stalactite"); Pkt.WriteBool(false);
			Pkt.WriteString("fireball"); Pkt.WriteBool(false);
			Pkt.WriteString("fireworks"); Pkt.WriteBool(false);
			Pkt.WriteString("fly_into_wall"); Pkt.WriteBool(false);
			Pkt.WriteString("freeze"); Pkt.WriteBool(false);
			Pkt.WriteString("generic"); Pkt.WriteBool(false);
			Pkt.WriteString("generic_kill"); Pkt.WriteBool(false);
			Pkt.WriteString("hot_floor"); Pkt.WriteBool(false);
			Pkt.WriteString("indirect_magic"); Pkt.WriteBool(false);
			Pkt.WriteString("in_fire"); Pkt.WriteBool(false);
			Pkt.WriteString("in_wall"); Pkt.WriteBool(false);
			Pkt.WriteString("lava"); Pkt.WriteBool(false);
			Pkt.WriteString("lightning_bolt"); Pkt.WriteBool(false);
			Pkt.WriteString("magic"); Pkt.WriteBool(false);
			Pkt.WriteString("mob_attack"); Pkt.WriteBool(false);
			Pkt.WriteString("mob_attack_no_aggro"); Pkt.WriteBool(false);
			Pkt.WriteString("mob_projectile"); Pkt.WriteBool(false);
			Pkt.WriteString("on_fire"); Pkt.WriteBool(false);
			Pkt.WriteString("outside_border"); Pkt.WriteBool(false);
			Pkt.WriteString("out_of_world"); Pkt.WriteBool(false);
			Pkt.WriteString("player_attack"); Pkt.WriteBool(false);
			Pkt.WriteString("player_explosion"); Pkt.WriteBool(false);
			Pkt.WriteString("sonic_boom"); Pkt.WriteBool(false);
			Pkt.WriteString("spit"); Pkt.WriteBool(false);
			Pkt.WriteString("stalagmite"); Pkt.WriteBool(false);
			Pkt.WriteString("starve"); Pkt.WriteBool(false);
			Pkt.WriteString("sting"); Pkt.WriteBool(false);
			Pkt.WriteString("sweet_berry_bush"); Pkt.WriteBool(false);
			Pkt.WriteString("thorns"); Pkt.WriteBool(false);
			Pkt.WriteString("thrown"); Pkt.WriteBool(false);
			Pkt.WriteString("trident"); Pkt.WriteBool(false);
			Pkt.WriteString("unattributed_fireball"); Pkt.WriteBool(false);
			Pkt.WriteString("wind_charge"); Pkt.WriteBool(false);
			Pkt.WriteString("wither"); Pkt.WriteBool(false);
			Pkt.WriteString("wither_skull"); Pkt.WriteBool(false);
		}
		{
			cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
			Pkt.WriteString("minecraft:jukebox_song");
			Pkt.WriteVarInt32(19);
			Pkt.WriteString("13"); Pkt.WriteBool(false);
			Pkt.WriteString("cat"); Pkt.WriteBool(false);
			Pkt.WriteString("blocks"); Pkt.WriteBool(false);
			Pkt.WriteString("chirp"); Pkt.WriteBool(false);
			Pkt.WriteString("far"); Pkt.WriteBool(false);
			Pkt.WriteString("mall"); Pkt.WriteBool(false);
			Pkt.WriteString("mellohi"); Pkt.WriteBool(false);
			Pkt.WriteString("stal"); Pkt.WriteBool(false);
			Pkt.WriteString("strad"); Pkt.WriteBool(false);
			Pkt.WriteString("ward"); Pkt.WriteBool(false);
			Pkt.WriteString("11"); Pkt.WriteBool(false);
			Pkt.WriteString("wait"); Pkt.WriteBool(false);
			Pkt.WriteString("pigstep"); Pkt.WriteBool(false);
			Pkt.WriteString("otherside"); Pkt.WriteBool(false);
			Pkt.WriteString("5"); Pkt.WriteBool(false);
			Pkt.WriteString("relic"); Pkt.WriteBool(false);
			Pkt.WriteString("precipice"); Pkt.WriteBool(false);
			Pkt.WriteString("creator"); Pkt.WriteBool(false);
			Pkt.WriteString("creator_music_box"); Pkt.WriteBool(false);
		}
	}
}





void cProtocol_1_21::SendSelectKnownPacks()
{
	cPacketizer Pkt(*this, pktSelectKnownPacks);
	{
		Pkt.WriteVarInt32(2);
		Pkt.WriteString("minecraft");
		Pkt.WriteString("core");
		Pkt.WriteString("1.21");
		Pkt.WriteString("minecraft");
		Pkt.WriteString("core");
		Pkt.WriteString("1.21.1");
	}
}





void cProtocol_1_21::HandlePacketUseItem(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Hand);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Sequence);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Pitch);
	m_Client->HandleUseItem(Hand == MAIN_HAND);
	m_Client->SendAcknowledgeBlockChange(Sequence);
}





UInt32 cProtocol_1_21::GetProtocolBlockType(BlockState a_Block) const
{
	return Palette_1_21::From(a_Block);
}





UInt32 cProtocol_1_21::GetProtocolItemType(Item a_ItemID) const
{
	return Palette_1_21::From(a_ItemID);
}





Item cProtocol_1_21::GetItemFromProtocolID(UInt32 a_ProtocolID) const
{
	return Palette_1_21::ToItem(a_ProtocolID);
}





UInt32 cProtocol_1_21::GetProtocolSoundID(const AString & a_SoundName) const
{
	return Palette_1_21::FromSound(a_SoundName);
}

////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_21_2:

cProtocol::Version cProtocol_1_21_2::GetProtocolVersion() const
{
	return Version::v1_21_2;
}





UInt32 cProtocol_1_21_2::GetProtocolBlockType(BlockState a_Block) const
{
	return Palette_1_21_2::From(a_Block);
}





UInt32 cProtocol_1_21_2::GetProtocolItemType(Item a_ItemID) const
{
	return Palette_1_21_2::From(a_ItemID);
}





Item cProtocol_1_21_2::GetItemFromProtocolID(UInt32 a_ProtocolID) const
{
	return Palette_1_21_2::ToItem(a_ProtocolID);
}





void cProtocol_1_21_2::SendSelectKnownPacks()
{
	{
		cPacketizer Pkt(*this, pktSelectKnownPacks);
		Pkt.WriteVarInt32(2);
		Pkt.WriteString("minecraft");
		Pkt.WriteString("core");
		Pkt.WriteString("1.21.2");
		Pkt.WriteString("minecraft");
		Pkt.WriteString("core");
		Pkt.WriteString("1.21.3");
	}
}





void cProtocol_1_21_2::HandlePacketClientSettings(cByteBuffer & a_ByteBuffer)
{
	#define LEFT_HAND 0
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Locale);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   ViewDistance);
	HANDLE_READ(a_ByteBuffer, ReadVarInt,        int,     ChatFlags);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    ChatColors);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   SkinParts);
	HANDLE_READ(a_ByteBuffer, ReadVarInt,        UInt32,  MainHand);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    FilterText);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    AllowsListing);
	HANDLE_READ(a_ByteBuffer, ReadVarInt,        UInt32,  ParticleMode);

	m_Client->SetLocale(Locale);
	m_Client->SetViewDistance(ViewDistance);
	m_Client->GetPlayer()->SetSkinParts(SkinParts);
	m_Client->GetPlayer()->SetLeftHanded(MainHand == LEFT_HAND);
	m_Client->SetAllowListing(AllowsListing);
	m_Client->SetParticleMode(static_cast<enum eParticleModeSetting>(ParticleMode));
	// TODO: Handle chat flags and chat colors
}





void cProtocol_1_21_2::SendLoginSuccess()
{
	ASSERT(m_State == 2);  // State: login?

	// Enable compression:
	{
		cPacketizer Pkt(*this, pktStartCompression);
		Pkt.WriteVarInt32(CompressionThreshold);
	}

	m_CompressionEnabled = true;

	{
		cPacketizer Pkt(*this, pktLoginSuccess);
		Pkt.WriteUUID(m_Client->GetUUID());
		Pkt.WriteString(m_Client->GetUsername());
		const Json::Value & Properties = m_Client->GetProperties();
		Pkt.WriteVarInt32(Properties.size());
		for (auto & Node : Properties)
		{
			Pkt.WriteString(Node.get("name", "").asString());
			Pkt.WriteString(Node.get("value", "").asString());
			AString Signature = Node.get("signature", "").asString();
			if (Signature.empty())
			{
				Pkt.WriteBool(false);
			}
			else
			{
				Pkt.WriteBool(true);
				Pkt.WriteString(Signature);
			}
		}
	}
}





UInt32 cProtocol_1_21_2::GetPacketID(ePacketType a_PacketType) const
{
	switch (a_PacketType)
	{
		// Status packets
		case cProtocol::pktStatusResponse:       return 0x00;
		case cProtocol::pktPingResponse:         return 0x01;

		//  Login Packets
		case cProtocol::pktDisconnectDuringLogin:return 0x00;
		case cProtocol::pktEncryptionRequest:    return 0x01;
		case cProtocol::pktLoginSuccess:         return 0x02;
		case cProtocol::pktStartCompression:     return 0x03;
			// login query request 0x4
			// cookie request 0x5

		// Configuration
			// CookieRequestS2CPacket 0x0
		case cProtocol::pktConfigurationCustomPayload: return 0x01;
			//  Disconnect 0x02
		case cProtocol::pktConfigurationReady:   return 0x03;
			//  KeepAlive 0x04
			//  CommonPing 0x05
			// RestChat 0x06
		case cProtocol::pktConfigurationDynamicRegistries: return 0x07;
			// Resource pack remove 0x08
			// ResourcePackSend  0x09
			// StoreCookie 0x0A
			// ServerTransfer 0x0B
			// Features 0x0C
		case cProtocol::pktConfigurationTags:    return 0x0D;
		case cProtocol::pktSelectKnownPacks:     return 0x0E;
			// CustomReportDetailsS2CPacket 0x0F
			// ServerLinksS2CPacket 0x10

		//  Game packets
		case cProtocol::pktSpawnObject:          return 0x01;
		case cProtocol::pktSpawnMob:             return 0x01;
		case cProtocol::pktSpawnPainting:        return 0x01;
		case cProtocol::pktSpawnOtherPlayer:     return 0x01;
		case cProtocol::pktSpawnExperienceOrb:   return 0x02;
		case cProtocol::pktEntityAnimation:      return 0x03;
		case cProtocol::pktStatistics:           return 0x04;
		case cProtocol::pktPlayerActionResponse: return 0x05;
		// case cProtocol::pktBlockbreakingprogress:   return 0x06;
		case cProtocol::pktUpdateBlockEntity:    return 0x07;
		case cProtocol::pktBlockAction:          return 0x08;
		case cProtocol::pktBlockChange:          return 0x09;
		case cProtocol::pktBossBar:              return 0x0A;
		case cProtocol::pktDifficulty:           return 0x0B;
			//  ChunkSentS2CPacket 0x0C
			//  StartChunkSendS2CPacket 0xD
			//  ChunkBiomeDataS2CPacket 0x0E
			//  clear title 0x0F
			//  command suggestions here 0x10
		case cProtocol::pktCommnadTree:          return 0x11;
		case cProtocol::pktWindowClose:          return 0x12;
		case cProtocol::pktWindowItems:          return 0x13;  //  Inventory packet
		case cProtocol::pktWindowProperty:       return 0x14;  //  ScreenHandlerPropertyUpdateS2CPacket
		case cProtocol::pktInventorySlot:        return 0x15;  //  ScreenHandlerSlotUpdateS2CPacket
			//  CookieRequest 0x16
			//  cooldown update 0x17
			//  chat suggestions 0x18
		case cProtocol::pktCustomPayload:        return 0x19;
		case cProtocol::pktPluginMessage:        return 0x19;
			// EntityDamageS2CPacket 0x1A
			// DebugSample 0x1B
			// RemoveMessageS2CPacket 0x1C
		case cProtocol::pktDisconnectDuringGame: return 0x1D;
			//  ProfilelessChatMessageS2CPacket 0x1E
		case cProtocol::pktEntityStatus:         return 0x1F;
			// case EntityPositionSync 0x20
		case cProtocol::pktExplosion:            return 0x21;
		case cProtocol::pktUnloadChunk:          return 0x22;
		case cProtocol::pktGameMode:             return 0x23;
		case cProtocol::pktWeather:              return 0x23;
		case cProtocol::pktHorseWindowOpen:      return 0x24;
			// DamageTiltS2CPacket 0x25
			// wolrld border initalize 0x26
		case cProtocol::pktKeepAlive:            return 0x27;
			// chunk data packet 0x28
		case cProtocol::pktSoundParticleEffect:  return 0x29;  // world event
		case cProtocol::pktParticleEffect:       return 0x2A;
		case cProtocol::pktLightUpdate:          return 0x2B;
		case cProtocol::pktJoinGame:             return 0x2C;
			//  map update 0x2D
			//  set trade offers 0x2E
		case cProtocol::pktEntityRelMove:        return 0x2F;
		case cProtocol::pktEntityRelMoveLook:    return 0x30;
			// MoveMinecartAlongTrack 0x31
		case cProtocol::pktEntityLook:           return 0x32;
			//  vehicle move 0x33
			//  open written book 0x34
		case cProtocol::pktWindowOpen:           return 0x35;
		case cProtocol::pktUpdateSign:           return 0x36;
			//  CommonPingS2CPacket 0x37
			//  PingResultS2CPacket 0x38
			//  craft failed response 0x39
		case cProtocol::pktPlayerAbilities:      return 0x3A;
			//  ChatMessageS2CPacket 0x3B
			//  combat exit 0x3C
			//  comabt enter 0x3D
			//  death msg 0x3E
		case cProtocol::pktPlayerLstRemove:      return 0x3F;
		case cProtocol::pktPlayerList:           return 0x40;
			//  look at 0x41
		case cProtocol::pktPlayerMoveLook:       return 0x42;
			// player rotation 0x43
		case cProtocol::pktUnlockRecipe:         return 0x44;
			// recipe add 0x43
			// recipe remove 0x45
			// recipe settings 0x46
		case cProtocol::pktDestroyEntity:        return 0x47;
		case cProtocol::pktRemoveEntityEffect:   return 0x48;
			// ScoreboardScoreResetS2CPacket 0x49
			// ResourcePackRemoveS2CPacket 0x4A
		case cProtocol::pktResourcePack:         return 0x4B;
		case cProtocol::pktRespawn:              return 0x4C;
		case cProtocol::pktEntityHeadLook:       return 0x4D;
		case cProtocol::pktBlockChanges:         return 0x4E;
			// select advancment tab 0x4F
			// ServerMetadataS2CPacket 0x50
			// overlay msg 0x51
			// wb -- worldborder wb center changed 0x52
			// wb interpolate size 0x53
			// wb size changed 0x54
			// wb warning time changed 0x55
			// wb warning blocks changed 0x56
		case cProtocol::pktCameraSetTo:          return 0x57;
		case cProtocol::pktRenderDistanceCenter: return 0x58;
			//  chunk load distance 0x59
		case cProtocol::pktSetCursorItem:        return 0x5A;
		case cProtocol::pktSpawnPosition:        return 0x5B;
			//  scoreboard display 0x5C
		case cProtocol::pktEntityMeta:           return 0x5D;
			// entity attach 0x5E
		case cProtocol::pktEntityVelocity:       return 0x5F;
		case cProtocol::pktEntityEquipment:      return 0x60;
		case cProtocol::pktExperience:           return 0x61;
		case cProtocol::pktUpdateHealth:         return 0x62;
		case cProtocol::pktHeldItemChange:       return 0x63;
		case cProtocol::pktScoreboardObjective:  return 0x64;
		case cProtocol::pktAttachEntity:         return 0x65;
			// set player inventory 0x66
			// Teams 0x67
		case cProtocol::pktUpdateScore:          return 0x68;
			// simulation distance 0x69
			// subtitle 0x6A
		case cProtocol::pktTimeUpdate:           return 0x6B;
		case cProtocol::pktTitle:                return 0x6C;
			//  title fade 0x6D
			//  play sound from entity 0x6E
		case cProtocol::pktSoundEffect:          return 0x6F;
			//  EnterReconfigurationS2CPacket 0x70
			//  stop sound 0x71
			//  StoreCookies 0x72
		case cProtocol::pktChatRaw:              return 0x73;  //  Gamemessage
			//  player list header 0x74
			//  NbtQueryResponseS2CPacket 0x75
		case cProtocol::pktCollectEntity:        return 0x76;
		case cProtocol::pktTeleportEntity:       return 0x77;
			//  UpdateTickRateS2CPacket 0x78
			//  TickStepS2CPacket 0x79
			//  ServerTransfer 0x7A
			//  advancment update 0x7B
		case cProtocol::pktEntityProperties:     return 0x7C;
		case cProtocol::pktEntityEffect:         return 0x7D;
			//  sync recepies 0x7E
			//  sync tags 0x7F
			//  ProjectilePower 0x80
			// CustomReportDetailsS2CPacket 0x81
			// ServerLinksS2CPacket 0x82
		default: UNREACHABLE("unhandeled packet");
	}
}





void cProtocol_1_21_2::SendTags(void)
{
	{
		cPacketizer Pkt(*this, pktConfigurationTags);
		Pkt.WriteVarInt32(1);
		Pkt.WriteString("minecraft:worldgen/biome");
		Pkt.WriteVarInt32(3);
			Pkt.WriteString("minecraft:is_badlands");
			Pkt.WriteVarInt32(1);
				Pkt.WriteVarInt32(1);
			Pkt.WriteString("minecraft:is_savanna");
			Pkt.WriteVarInt32(1);
				Pkt.WriteVarInt32(2);
			Pkt.WriteString("minecraft:is_jungle");
			Pkt.WriteVarInt32(1);
				Pkt.WriteVarInt32(3);
			// indent -- Has to be here so CheckBasicStyle does not fail
		// indent
	}
}





bool cProtocol_1_21_2::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	switch (m_State)
	{
		case State::Status:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketStatusRequest(a_ByteBuffer); return true;
				case 0x01: HandlePacketStatusPing(a_ByteBuffer); return true;
			}
			break;
		}
		case State::Login:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketLoginStart(a_ByteBuffer); return true;
				case 0x01: HandlePacketLoginEncryptionResponse(a_ByteBuffer); return true;
				case 0x02: /* LoginQueryResponseC2SPacket */ return false;
				case 0x03: HandlePacketEnterConfiguration(a_ByteBuffer); return true;
				case 0x04: /* CookieResponseC2SPacket */ return false;
			}
			break;
		}

		case State::Configuration:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketClientSettings(a_ByteBuffer); return true;
				case 0x01: /* Cookie Response */ return false;
				case 0x02: HandlePacketPluginMessage(a_ByteBuffer); return true;
				case 0x03: HandlePacketReady(a_ByteBuffer); return true;
				case 0x04: HandlePacketKeepAlive(a_ByteBuffer); return true;
				case 0x05: /* CommonPongC2SPacket */ return false;
				case 0x06: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
				case 0x07: /* SelectKnownPacks */ return false;
			}
			break;
		}

		case State::Game:
		{
			switch (a_PacketType)
			{
				case 0x00: HandleConfirmTeleport(a_ByteBuffer); return true;
				case 0x01: /* query nbt packet */ return false;
				case 0x02: /* BundleItemSelected */ return false;
				case 0x03: /* update difficulty */ return false;
				case 0x04: /* MessageAcknowledgmentC2SPacket */ return false;
				case 0x05: HandlePacketCommandExecution(a_ByteBuffer); return true;
				case 0x06: /* ChatCommandSignedC2SPacket */ return false;
				case 0x07: HandlePacketChatMessage(a_ByteBuffer); return true;
				case 0x08: HandlePacketPlayerSession(a_ByteBuffer); return true;
				case 0x09: /* AcknowledgeChunksC2SPacket */ return false;
				case 0x0A: HandlePacketClientStatus(a_ByteBuffer); return true;
				case 0x0B: /* ClientTickEnd */ return true;
				case 0x0C: HandlePacketClientSettings(a_ByteBuffer); return true;
				case 0x0D: HandlePacketTabComplete(a_ByteBuffer); return true;
				case 0x0E: /* AcknowledgeReconfigurationC2SPacket */ return false;
				case 0x0F: /* ButtonClickC2SPacket */ return false;
				case 0x10: HandlePacketWindowClick(a_ByteBuffer); return true;
				case 0x11: HandlePacketWindowClose(a_ByteBuffer); return true;
				case 0x12: /* SlotChangedStateC2SPacket */ return false;
				case 0x13: /* CookieResponseC2SPacket */ return false;
				case 0x14: HandlePacketPluginMessage(a_ByteBuffer); return true;
				case 0x15: /* DebugSampleSubscriptionC2SPacket */ return false;
				case 0x16: HandlePacketBookUpdate(a_ByteBuffer); return true;  // not fully implemented
				case 0x17: /* QueryEntityNbtC2SPacket */ return false;
				case 0x18: HandlePacketUseEntity(a_ByteBuffer); return true;
				case 0x19: /* Jigsaw generating */ return false;
				case 0x1A: HandlePacketKeepAlive(a_ByteBuffer); return true;
				case 0x1B: /* Update difficulty lock */ return false;  // only used in single player
				case 0x1C: HandlePacketPlayerPos(a_ByteBuffer); return true;  // PositionAndOnGround
				case 0x1D: HandlePacketPlayerPosLook(a_ByteBuffer); return true;  // full
				case 0x1E: HandlePacketPlayerLook(a_ByteBuffer); return true;  // LookAndOnGround
				case 0x1F: HandlePacketPlayer(a_ByteBuffer); return true;
				case 0x20: HandlePacketVehicleMove(a_ByteBuffer); return true;
				case 0x21: HandlePacketBoatSteer(a_ByteBuffer); return true;
				case 0x22: /* pick from inventory */ return false;
				case 0x23: /* QueryPingC2SPacket */ return false;
				case 0x24: HandleCraftRecipe(a_ByteBuffer); return true;
				case 0x25: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
				case 0x26: HandlePacketBlockDig(a_ByteBuffer); return true;
				case 0x27: /* client command packet */ return false;
				case 0x28: HandlePacketSteerVehicle(a_ByteBuffer); return true;  // player input packet
				case 0x29: /* PlayPongC2SPacket */ return false;
				case 0x2A: /* Recipe Category Options */ return false;
				case 0x2B: HandlePacketCraftingBookData(a_ByteBuffer); return true;
				case 0x2C: HandlePacketNameItem(a_ByteBuffer); return true;
				case 0x2D: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
				case 0x2E: HandlePacketAdvancementTab(a_ByteBuffer); return true;
				case 0x2F: /* select villager trade */ return false;
				case 0x30: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
				case 0x31: HandlePacketSlotSelect(a_ByteBuffer); return true;
				case 0x32: /* update command block */ return false;
				case 0x33: /* update minecart command block */ return false;
				case 0x34: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
				case 0x35: /* Update jigsaw block */ return false;
				case 0x36: /* Update structure block */ return false;
				case 0x37: HandlePacketUpdateSign(a_ByteBuffer); return true;
				case 0x38: /* Update hand swing */ return false;
				case 0x39: /* Spectator teleport */ return false;
				case 0x3A: HandlePacketBlockPlace(a_ByteBuffer); return true;
				case 0x3B: HandlePacketUseItem(a_ByteBuffer); return true;
				default: break;
			}
		}
	}
	UNREACHABLE("");
}





void cProtocol_1_21_2::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the Join Game packet:
	{
		cServer * Server = cRoot::Get()->GetServer();
		cPacketizer Pkt(*this, pktJoinGame);
		Pkt.WriteBEUInt32(a_Player.GetUniqueID());
		Pkt.WriteBool(Server->IsHardcore());
		Pkt.WriteVarInt32(1);  // Number of dimensions
		Pkt.WriteString("overworld");
		Pkt.WriteVarInt32(static_cast<UInt32>(Server->GetMaxPlayers()));
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));  // simulation distance
		Pkt.WriteBool(false);  // Reduced debug info
		Pkt.WriteBool(true);   // Show deaths screen
		Pkt.WriteBool(false);  // Do Limited Crafting

		Pkt.WriteVarInt32(0);  // Dimension id
		Pkt.WriteString("minecraft:overworld");  // dimension name
		Pkt.WriteBEInt64(0);  // Seed

		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()));  // current game mode
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()));  // previous game mode

		Pkt.WriteBool(false);  // Debug world
		Pkt.WriteBool(false);  // Flat World

		Pkt.WriteBool(false);  // optional last death loc
		Pkt.WriteVarInt32(0);  // Portal Cooldown
		Pkt.WriteVarInt32(60);  // Sea level
		Pkt.WriteBool(false);  // Enforce secure chat
	}

	// Send the spawn position:
	{
		cPacketizer Pkt(*this, pktSpawnPosition);
		Pkt.WriteXZYPosition64(a_World.GetSpawnX(), a_World.GetSpawnY(), a_World.GetSpawnZ());
		Pkt.WriteBEFloat(0);  // Angle
	}
	// Send the server difficulty:
	{
		cPacketizer Pkt(*this, pktDifficulty);
		Pkt.WriteBEInt8(1);
		Pkt.WriteBool(false);  // Difficulty locked?
	}
}





void cProtocol_1_21_2::SendPlayerMoveLook(const Vector3d a_Pos, const float a_Yaw, const float a_Pitch, const bool a_IsRelative)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerMoveLook);

	Pkt.WriteVarInt32(++m_OutstandingTeleportId);

	// This teleport ID hasn't been confirmed yet
	m_IsTeleportIdConfirmed = false;

	Pkt.WriteBEDouble(a_Pos.x);
	Pkt.WriteBEDouble(a_Pos.y);
	Pkt.WriteBEDouble(a_Pos.z);
	Pkt.WriteBEDouble(0);
	Pkt.WriteBEDouble(0);
	Pkt.WriteBEDouble(0);
	Pkt.WriteBEFloat(a_Yaw);
	Pkt.WriteBEFloat(a_Pitch);

	if (a_IsRelative)
	{
		// Set all bits to 1 - makes everything relative
		Pkt.WriteBEInt32(-1);
	}
	else
	{
		// Set all bits to 0 - make everything absolute
		Pkt.WriteBEInt32(0);
	}


}





UInt32 cProtocol_1_21_2::GetProtocolMobType(eMonsterType a_MobType) const
{
	switch (a_MobType)
	{
		// Map invalid type to Giant for easy debugging (if this ever spawns, something has gone very wrong)
		case mtAllay: return 2;
		case mtArmadillo: return 4;
		case mtAxolotl: return 7;
		case mtBat: return 10;
		case mtBee: return 11;
		case mtBlaze: return 14;
		case mtBogged: return 16;
		case mtBreeze: return 17;
		case mtCamel: return 19;
		case mtCat: return 20;
		case mtCaveSpider: return 21;
		case mtChicken: return 25;
		case mtCod: return 26;
		case mtCow: return 28;
		case mtCreaking: return 29;
		case mtCreeper: return 31;
		case mtDolphin: return 34;
		case mtDonkey: return 35;
		case mtDrowned: return 37;
		case mtElderGuardian: return 39;
		case mtEnderman: return 40;
		case mtEndermite: return 41;
		case mtEnderDragon: return 42;
		case mtEvoker: return 45;
		case mtFox: return 53;
		case mtFrog: return 54;
		case mtGhast: return 56;
		case mtGiant: return 57;
		case mtGlowSquid: return 59;
		case mtGoat: return 60;
		case mtGuardian: return 61;
		case mtHoglin: return 62;
		case mtHorse: return 64;
		case mtHusk: return 65;
		case mtIllusioner: return 66;
		case mtLlama: return 76;
		case mtMagmaCube: return 78;
		case mtMooshroom: return 83;
		case mtMule: return 84;
		case mtOcelot: return 87;
		case mtPanda: return 92;
		case mtParrot: return 93;
		case mtPhantom: return 94;
		case mtPig: return 95;
		case mtPiglin: return 96;
		case mtPiglinBrute: return 97;
		case mtPillager: return 98;
		case mtPolarBear: return 99;
		case mtPufferfish: return 101;
		case mtRabbit: return 102;
		case mtRavager: return 103;
		case mtSalmon: return 104;
		case mtSheep: return 105;
		case mtShulker: return 106;
		case mtSilverfish: return 108;
		case mtSkeleton: return 109;
		case mtSkeletonHorse: return 110;
		case mtSlime: return 111;
		case mtSniffer: return 113;
		case mtSpider: return 118;
		case mtSquid: return 121;
		case mtStray: return 122;
		case mtStrider: return 123;
		case mtTadpole: return 124;
		case mtTraderLlama: return 128;
		case mtTropicalFish: return 130;
		case mtTurtle: return 131;
		case mtVex: return 132;
		case mtVillager: return 133;
		case mtVindicator: return 134;
		case mtWanderingTrader: return 135;
		case mtWarden: return 136;
		case mtWitch: return 138;
		case mtWither: return 139;
		case mtWitherSkeleton: return 140;
		case mtWolf: return 142;
		case mtZoglin: return 143;
		case mtZombie: return 144;
		case mtZombieHorse: return 145;
		case mtZombieVillager: return 146;
		case mtZombifiedPiglin: return 147;
		default: return 0;
	}
}





UInt8 cProtocol_1_21_2::GetProtocolEntityType(const cEntity & a_Entity) const
{
	using Type = cEntity::eEntityType;

	switch (a_Entity.GetEntityType())
	{
		case Type::etEnderCrystal: return 44;
		case Type::etPickup: return 69;
		case Type::etFallingBlock: return 50;
		case Type::etMinecart: return 82;
		case Type::etBoat: return 85;
		case Type::etTNT: return 126;
		case Type::etProjectile:
		{
			using PType = cProjectileEntity::eKind;
			const auto & Projectile = static_cast<const cProjectileEntity &>(a_Entity);

			switch (Projectile.GetProjectileKind())
			{
				case PType::pkArrow: return 6;
				case PType::pkSnowball: return 114;
				case PType::pkEgg: return 38;
				case PType::pkGhastFireball: return 112;
				case PType::pkFireCharge: return 51;
				case PType::pkEnderPearl: return 43;
				case PType::pkExpBottle: return 47;
				case PType::pkSplashPotion: return 100;
				case PType::pkFirework: return 52;
				case PType::pkWitherSkull: return 141;
			}
			break;
		}
		case Type::etFloater: return 149;
		case Type::etItemFrame: return 71;
		case Type::etLeashKnot: return 74;

		// Non-objects must not be sent
		case Type::etEntity:
		case Type::etPlayer:
		case Type::etMonster:
		case Type::etExpOrb:
		case Type::etPainting: break;
	}
	UNREACHABLE("Unhandled entity kind");
}





void cProtocol_1_21_2::SendDynamicRegistries()
{
	{
		{
			cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
			Pkt.WriteString("minecraft:dimension_type");
			Pkt.WriteVarInt32(1);
			Pkt.WriteString("minecraft:overworld");
			Pkt.WriteBool(true);
			cFastNBTWriter Writer(true);
			Writer.AddByte("piglin_safe", 1);
			Writer.AddByte("natural", 1);
			Writer.AddFloat("ambient_light", 1.0);
			Writer.AddInt("monster_spawn_block_light_limit", 0);
			Writer.AddString("infiniburn", "#infiniburn_overworld");
			Writer.AddByte("respawn_anchor_works", 1);
			Writer.AddByte("has_skylight", 1);
			Writer.AddByte("bed_works", 1);
			Writer.AddString("effects", "minecraft:overworld");
			Writer.AddByte("has_raids", 1);
			Writer.AddInt("logical_height", 256);
			Writer.AddDouble("coordinate_scale", 1.0);
			Writer.AddByte("ultrawarm", 0);
			Writer.AddByte("has_ceiling", 0);
			Writer.AddInt("min_y", 0);
			Writer.AddInt("height", 256);
			Writer.AddInt("monster_spawn_light_level", 0);
			Writer.AddInt("monster_spawn_block_light_limit", 0);
			Writer.Finish();
			Pkt.WriteBuf(Writer.GetResult());
		}
		{
			cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
			Pkt.WriteString("minecraft:worldgen/biome");
			Pkt.WriteVarInt32(64);
			Pkt.WriteString("plains"); Pkt.WriteBool(false);  // temporarily here
			Pkt.WriteString("badlands"); Pkt.WriteBool(false);
			Pkt.WriteString("bamboo_jungle"); Pkt.WriteBool(false);
			Pkt.WriteString("basalt_deltas"); Pkt.WriteBool(false);
			Pkt.WriteString("beach"); Pkt.WriteBool(false);
			Pkt.WriteString("birch_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("cherry_grove"); Pkt.WriteBool(false);
			Pkt.WriteString("cold_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("crimson_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("dark_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("deep_cold_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("deep_dark"); Pkt.WriteBool(false);
			Pkt.WriteString("deep_frozen_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("deep_lukewarm_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("deep_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("desert"); Pkt.WriteBool(false);
			Pkt.WriteString("dripstone_caves"); Pkt.WriteBool(false);
			Pkt.WriteString("end_barrens"); Pkt.WriteBool(false);
			Pkt.WriteString("end_highlands"); Pkt.WriteBool(false);
			Pkt.WriteString("end_midlands"); Pkt.WriteBool(false);
			Pkt.WriteString("eroded_badlands"); Pkt.WriteBool(false);
			Pkt.WriteString("flower_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("forest"); Pkt.WriteBool(false);
			Pkt.WriteString("frozen_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("frozen_peaks"); Pkt.WriteBool(false);
			Pkt.WriteString("frozen_river"); Pkt.WriteBool(false);
			Pkt.WriteString("grove"); Pkt.WriteBool(false);
			Pkt.WriteString("ice_spikes"); Pkt.WriteBool(false);
			Pkt.WriteString("jagged_peaks"); Pkt.WriteBool(false);
			Pkt.WriteString("jungle"); Pkt.WriteBool(false);
			Pkt.WriteString("lukewarm_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("lush_caves"); Pkt.WriteBool(false);
			Pkt.WriteString("mangrove_swamp"); Pkt.WriteBool(false);
			Pkt.WriteString("meadow"); Pkt.WriteBool(false);
			Pkt.WriteString("mushroom_fields"); Pkt.WriteBool(false);
			Pkt.WriteString("nether_wastes"); Pkt.WriteBool(false);
			Pkt.WriteString("ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("old_growth_birch_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("old_growth_pine_taiga"); Pkt.WriteBool(false);
			Pkt.WriteString("old_growth_spruce_taiga"); Pkt.WriteBool(false);
			// Pkt.WriteString("plains"); Pkt.WriteBool(false);
			Pkt.WriteString("river"); Pkt.WriteBool(false);
			Pkt.WriteString("savanna"); Pkt.WriteBool(false);
			Pkt.WriteString("savanna_plateau"); Pkt.WriteBool(false);
			Pkt.WriteString("small_end_islands"); Pkt.WriteBool(false);
			Pkt.WriteString("snowy_beach"); Pkt.WriteBool(false);
			Pkt.WriteString("snowy_plains"); Pkt.WriteBool(false);
			Pkt.WriteString("snowy_slopes"); Pkt.WriteBool(false);
			Pkt.WriteString("snowy_taiga"); Pkt.WriteBool(false);
			Pkt.WriteString("soul_sand_valley"); Pkt.WriteBool(false);
			Pkt.WriteString("sparse_jungle"); Pkt.WriteBool(false);
			Pkt.WriteString("stony_peaks"); Pkt.WriteBool(false);
			Pkt.WriteString("stony_shore"); Pkt.WriteBool(false);
			Pkt.WriteString("sunflower_plains"); Pkt.WriteBool(false);
			Pkt.WriteString("swamp"); Pkt.WriteBool(false);
			Pkt.WriteString("taiga"); Pkt.WriteBool(false);
			Pkt.WriteString("the_end"); Pkt.WriteBool(false);
			Pkt.WriteString("the_void"); Pkt.WriteBool(false);
			Pkt.WriteString("warm_ocean"); Pkt.WriteBool(false);
			Pkt.WriteString("warped_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("windswept_forest"); Pkt.WriteBool(false);
			Pkt.WriteString("windswept_gravelly_hills"); Pkt.WriteBool(false);
			Pkt.WriteString("windswept_hills"); Pkt.WriteBool(false);
			Pkt.WriteString("windswept_savanna"); Pkt.WriteBool(false);
			Pkt.WriteString("wooded_badlands"); Pkt.WriteBool(false);
		}
		{
			cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
			Pkt.WriteString("minecraft:wolf_variant");
			Pkt.WriteVarInt32(9);
			Pkt.WriteString("ashen"); Pkt.WriteBool(false);
			Pkt.WriteString("black"); Pkt.WriteBool(false);
			Pkt.WriteString("chestnut"); Pkt.WriteBool(false);
			Pkt.WriteString("pale"); Pkt.WriteBool(false);
			Pkt.WriteString("rusty"); Pkt.WriteBool(false);
			Pkt.WriteString("snowy"); Pkt.WriteBool(false);
			Pkt.WriteString("spotted"); Pkt.WriteBool(false);
			Pkt.WriteString("striped"); Pkt.WriteBool(false);
			Pkt.WriteString("woods"); Pkt.WriteBool(false);
		}
		{
			cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
			Pkt.WriteString("minecraft:painting_variant");
			Pkt.WriteVarInt32(50);
			Pkt.WriteString("alban"); Pkt.WriteBool(false);
			Pkt.WriteString("aztec"); Pkt.WriteBool(false);
			Pkt.WriteString("aztec2"); Pkt.WriteBool(false);
			Pkt.WriteString("backyard"); Pkt.WriteBool(false);
			Pkt.WriteString("baroque"); Pkt.WriteBool(false);
			Pkt.WriteString("bomb"); Pkt.WriteBool(false);
			Pkt.WriteString("bouquet"); Pkt.WriteBool(false);
			Pkt.WriteString("burning_skull"); Pkt.WriteBool(false);
			Pkt.WriteString("bust"); Pkt.WriteBool(false);
			Pkt.WriteString("cavebird"); Pkt.WriteBool(false);
			Pkt.WriteString("changing"); Pkt.WriteBool(false);
			Pkt.WriteString("cotan"); Pkt.WriteBool(false);
			Pkt.WriteString("courbet"); Pkt.WriteBool(false);
			Pkt.WriteString("creebet"); Pkt.WriteBool(false);
			Pkt.WriteString("donkey_kong"); Pkt.WriteBool(false);
			Pkt.WriteString("earth"); Pkt.WriteBool(false);
			Pkt.WriteString("endboss"); Pkt.WriteBool(false);
			Pkt.WriteString("fern"); Pkt.WriteBool(false);
			Pkt.WriteString("fighters"); Pkt.WriteBool(false);
			Pkt.WriteString("finding"); Pkt.WriteBool(false);
			Pkt.WriteString("fire"); Pkt.WriteBool(false);
			Pkt.WriteString("graham"); Pkt.WriteBool(false);
			Pkt.WriteString("humble"); Pkt.WriteBool(false);
			Pkt.WriteString("kebab"); Pkt.WriteBool(false);
			Pkt.WriteString("lowmist"); Pkt.WriteBool(false);
			Pkt.WriteString("match"); Pkt.WriteBool(false);
			Pkt.WriteString("meditative"); Pkt.WriteBool(false);
			Pkt.WriteString("orb"); Pkt.WriteBool(false);
			Pkt.WriteString("owlemons"); Pkt.WriteBool(false);
			Pkt.WriteString("passage"); Pkt.WriteBool(false);
			Pkt.WriteString("pigscene"); Pkt.WriteBool(false);
			Pkt.WriteString("plant"); Pkt.WriteBool(false);
			Pkt.WriteString("pointer"); Pkt.WriteBool(false);
			Pkt.WriteString("pond"); Pkt.WriteBool(false);
			Pkt.WriteString("pool"); Pkt.WriteBool(false);
			Pkt.WriteString("prairie_ride"); Pkt.WriteBool(false);
			Pkt.WriteString("sea"); Pkt.WriteBool(false);
			Pkt.WriteString("skeleton"); Pkt.WriteBool(false);
			Pkt.WriteString("skull_and_roses"); Pkt.WriteBool(false);
			Pkt.WriteString("stage"); Pkt.WriteBool(false);
			Pkt.WriteString("sunflowers"); Pkt.WriteBool(false);
			Pkt.WriteString("sunset"); Pkt.WriteBool(false);
			Pkt.WriteString("tides"); Pkt.WriteBool(false);
			Pkt.WriteString("unpacked"); Pkt.WriteBool(false);
			Pkt.WriteString("void"); Pkt.WriteBool(false);
			Pkt.WriteString("wanderer"); Pkt.WriteBool(false);
			Pkt.WriteString("wasteland"); Pkt.WriteBool(false);
			Pkt.WriteString("water"); Pkt.WriteBool(false);
			Pkt.WriteString("wind"); Pkt.WriteBool(false);
			Pkt.WriteString("wither"); Pkt.WriteBool(false);
		}
		{
			cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
			Pkt.WriteString("minecraft:damage_type");
			Pkt.WriteVarInt32(49);
			Pkt.WriteString("arrow"); Pkt.WriteBool(false);
			Pkt.WriteString("bad_respawn_point"); Pkt.WriteBool(false);
			Pkt.WriteString("cactus"); Pkt.WriteBool(false);
			Pkt.WriteString("campfire"); Pkt.WriteBool(false);
			Pkt.WriteString("cramming"); Pkt.WriteBool(false);
			Pkt.WriteString("dragon_breath"); Pkt.WriteBool(false);
			Pkt.WriteString("drown"); Pkt.WriteBool(false);
			Pkt.WriteString("dry_out"); Pkt.WriteBool(false);
			Pkt.WriteString("ender_pearl"); Pkt.WriteBool(false);
			Pkt.WriteString("explosion"); Pkt.WriteBool(false);
			Pkt.WriteString("fall"); Pkt.WriteBool(false);
			Pkt.WriteString("falling_anvil"); Pkt.WriteBool(false);
			Pkt.WriteString("falling_block"); Pkt.WriteBool(false);
			Pkt.WriteString("falling_stalactite"); Pkt.WriteBool(false);
			Pkt.WriteString("fireball"); Pkt.WriteBool(false);
			Pkt.WriteString("fireworks"); Pkt.WriteBool(false);
			Pkt.WriteString("fly_into_wall"); Pkt.WriteBool(false);
			Pkt.WriteString("freeze"); Pkt.WriteBool(false);
			Pkt.WriteString("generic"); Pkt.WriteBool(false);
			Pkt.WriteString("generic_kill"); Pkt.WriteBool(false);
			Pkt.WriteString("hot_floor"); Pkt.WriteBool(false);
			Pkt.WriteString("indirect_magic"); Pkt.WriteBool(false);
			Pkt.WriteString("in_fire"); Pkt.WriteBool(false);
			Pkt.WriteString("in_wall"); Pkt.WriteBool(false);
			Pkt.WriteString("lava"); Pkt.WriteBool(false);
			Pkt.WriteString("lightning_bolt"); Pkt.WriteBool(false);
			Pkt.WriteString("mace_smash"); Pkt.WriteBool(false);
			Pkt.WriteString("magic"); Pkt.WriteBool(false);
			Pkt.WriteString("mob_attack"); Pkt.WriteBool(false);
			Pkt.WriteString("mob_attack_no_aggro"); Pkt.WriteBool(false);
			Pkt.WriteString("mob_projectile"); Pkt.WriteBool(false);
			Pkt.WriteString("on_fire"); Pkt.WriteBool(false);
			Pkt.WriteString("outside_border"); Pkt.WriteBool(false);
			Pkt.WriteString("out_of_world"); Pkt.WriteBool(false);
			Pkt.WriteString("player_attack"); Pkt.WriteBool(false);
			Pkt.WriteString("player_explosion"); Pkt.WriteBool(false);
			Pkt.WriteString("sonic_boom"); Pkt.WriteBool(false);
			Pkt.WriteString("spit"); Pkt.WriteBool(false);
			Pkt.WriteString("stalagmite"); Pkt.WriteBool(false);
			Pkt.WriteString("starve"); Pkt.WriteBool(false);
			Pkt.WriteString("sting"); Pkt.WriteBool(false);
			Pkt.WriteString("sweet_berry_bush"); Pkt.WriteBool(false);
			Pkt.WriteString("thorns"); Pkt.WriteBool(false);
			Pkt.WriteString("thrown"); Pkt.WriteBool(false);
			Pkt.WriteString("trident"); Pkt.WriteBool(false);
			Pkt.WriteString("unattributed_fireball"); Pkt.WriteBool(false);
			Pkt.WriteString("wind_charge"); Pkt.WriteBool(false);
			Pkt.WriteString("wither"); Pkt.WriteBool(false);
			Pkt.WriteString("wither_skull"); Pkt.WriteBool(false);
		}
	}
}





void cProtocol_1_21_2::SendTimeUpdate(const cTickTimeLong a_WorldAge, const cTickTimeLong a_WorldDate, const bool a_DoDaylightCycle)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktTimeUpdate);
	Pkt.WriteBEInt64(a_WorldAge.count());
	Pkt.WriteBEInt64(a_WorldDate.count());
	Pkt.WriteBool(a_DoDaylightCycle);
}





void cProtocol_1_21_2::HandlePacketSteerVehicle(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, BitField);
	// TODO: parse data
}





void cProtocol_1_21_2::SendEntityPosition(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	const auto Delta = (a_Entity.GetPosition() * 32 * 128).Floor() - (a_Entity.GetLastSentPosition() * 32 * 128).Floor();

	// Ensure that the delta has enough precision and is within range of a BEInt16:
	if (
		Delta.HasNonZeroLength() &&
		cByteBuffer::CanBEInt16Represent(Delta.x) &&
		cByteBuffer::CanBEInt16Represent(Delta.y) &&
		cByteBuffer::CanBEInt16Represent(Delta.z)
	)
	{
		const auto Move = static_cast<Vector3<Int16>>(Delta);

		// Difference within limitations, use a relative move packet
		if (a_Entity.IsOrientationDirty())
		{
			cPacketizer Pkt(*this, pktEntityRelMoveLook);
			Pkt.WriteVarInt32(a_Entity.GetUniqueID());
			Pkt.WriteBEInt16(Move.x);
			Pkt.WriteBEInt16(Move.y);
			Pkt.WriteBEInt16(Move.z);
			Pkt.WriteByteAngle(a_Entity.GetYaw());
			Pkt.WriteByteAngle(a_Entity.GetPitch());
			Pkt.WriteBool(a_Entity.IsOnGround());
		}
		else
		{
			cPacketizer Pkt(*this, pktEntityRelMove);
			Pkt.WriteVarInt32(a_Entity.GetUniqueID());
			Pkt.WriteBEInt16(Move.x);
			Pkt.WriteBEInt16(Move.y);
			Pkt.WriteBEInt16(Move.z);
			Pkt.WriteBool(a_Entity.IsOnGround());
		}

		return;
	}

	// Too big or small a movement, do a teleport.

	cPacketizer Pkt(*this, pktTeleportEntity);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEDouble(a_Entity.GetPosX());
	Pkt.WriteBEDouble(a_Entity.GetPosY());
	Pkt.WriteBEDouble(a_Entity.GetPosZ());
	Pkt.WriteBEDouble(0);
	Pkt.WriteBEDouble(0);
	Pkt.WriteBEDouble(0);
	Pkt.WriteBEFloat(static_cast<float>(a_Entity.GetYaw()));
	Pkt.WriteBEFloat(static_cast<float>(a_Entity.GetPitch()));
	Pkt.WriteBEInt32(0);
	Pkt.WriteBool(a_Entity.IsOnGround());
}





void cProtocol_1_21_2::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_State == 3);  // In game mode?

	if (a_SlotNum == -1)
	{
		cPacketizer Pkt(*this, pktSetCursorItem);
		WriteItem(Pkt, a_Item);
		return;
	}

	cPacketizer Pkt(*this, pktInventorySlot);
	Pkt.WriteVarInt32(0);  // sync id
	Pkt.WriteVarInt32(0);  // revision
	Pkt.WriteBEInt16(a_SlotNum);
	WriteItem(Pkt, a_Item);
}





void cProtocol_1_21_2::SendRespawn(eDimension a_Dimension)
{
	cPacketizer Pkt(*this, pktRespawn);
	cPlayer * Player = m_Client->GetPlayer();

	Pkt.WriteVarInt32(0);  // dimension type key
	Pkt.WriteString("minecraft:overworld");  // world key
	Pkt.WriteBEUInt64(0);  // Appears to be a SHA256 od the world seed
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBool(false);  // debug world
	Pkt.WriteBool(false);  // flat world
	Pkt.WriteBool(false);  // optional last death pos
	Pkt.WriteVarInt32(0);  // portal cool down
	Pkt.WriteBEInt8(0x3);  // keep metadata and attributes
	Pkt.WriteVarInt32(60);  // Sea level
}





void cProtocol_1_21_2::HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Hand);

	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadXZYPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Face);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, CursorX);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, CursorY);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, CursorZ);
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, InsideBlock);
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, IsAgainstWorldBorder);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Sequence);
	m_Client->HandleRightClick({BlockX, BlockY, BlockZ}, FaceIntToBlockFace(Face), {FloorC(CursorX * 16), FloorC(CursorY * 16), FloorC(CursorZ * 16)}, Hand == 0);
	m_Client->SendAcknowledgeBlockChange(Sequence);
}





////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_21_4:

cProtocol::Version cProtocol_1_21_4::GetProtocolVersion() const
{
	return Version::v1_21_4;
}





void cProtocol_1_21_4::SendSelectKnownPacks()
{
	{
		cPacketizer Pkt(*this, pktSelectKnownPacks);
		Pkt.WriteVarInt32(1);
		Pkt.WriteString("minecraft");
		Pkt.WriteString("core");
		Pkt.WriteString("1.21.4");
	}
}





bool cProtocol_1_21_4::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	switch (m_State)
	{
		case State::Status:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketStatusRequest(a_ByteBuffer); return true;
				case 0x01: HandlePacketStatusPing(a_ByteBuffer); return true;
			}
			break;
		}

		case State::Login:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketLoginStart(a_ByteBuffer); return true;
				case 0x01: HandlePacketLoginEncryptionResponse(a_ByteBuffer); return true;
				case 0x02: /* LoginQueryResponseC2SPacket */ return false;
				case 0x03: HandlePacketEnterConfiguration(a_ByteBuffer); return true;
				case 0x04: /* CookieResponseC2SPacket */ return false;
			}
			break;
		}

		case State::Configuration:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketClientSettings(a_ByteBuffer); return true;
				case 0x01: /* Cookie Response */ return false;
				case 0x02: HandlePacketPluginMessage(a_ByteBuffer); return true;
				case 0x03: HandlePacketReady(a_ByteBuffer); return true;
				case 0x04: HandlePacketKeepAlive(a_ByteBuffer); return true;
				case 0x05: /* CommonPongC2SPacket */ return false;
				case 0x06: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
				case 0x07: /* SelectKnownPacks */ return false;
			}
			break;
		}

		case State::Game:
		{
			switch (a_PacketType)
			{
				case 0x00: HandleConfirmTeleport(a_ByteBuffer); return true;
				case 0x01: /* query nbt packet */ return false;
				case 0x02: /* BundleItemSelected */ return false;
				case 0x03: /* update difficulty */ return false;
				case 0x04: /* MessageAcknowledgmentC2SPacket */ return false;
				case 0x05: HandlePacketCommandExecution(a_ByteBuffer); return true;
				case 0x06: /* ChatCommandSignedC2SPacket */ return false;
				case 0x07: HandlePacketChatMessage(a_ByteBuffer); return true;
				case 0x08: HandlePacketPlayerSession(a_ByteBuffer); return true;
				case 0x09: /* AcknowledgeChunksC2SPacket */ return false;
				case 0x0A: HandlePacketClientStatus(a_ByteBuffer); return true;
				case 0x0B: /* ClientTickEnd */ return false;
				case 0x0C: HandlePacketClientSettings(a_ByteBuffer); return true;
				case 0x0D: HandlePacketTabComplete(a_ByteBuffer); return true;
				case 0x0E: /* AcknowledgeReconfigurationC2SPacket */ return false;
				case 0x0F: /* ButtonClickC2SPacket */ return false;
				case 0x10: HandlePacketWindowClick(a_ByteBuffer); return true;
				case 0x11: HandlePacketWindowClose(a_ByteBuffer); return true;
				case 0x12: /* SlotChangedStateC2SPacket */ return false;
				case 0x13: /* CookieResponseC2SPacket */ return false;
				case 0x14: HandlePacketPluginMessage(a_ByteBuffer); return true;
				case 0x15: /* DebugSampleSubscriptionC2SPacket */ return false;
				case 0x16: HandlePacketBookUpdate(a_ByteBuffer); return true;  // not fully implemented
				case 0x17: /* QueryEntityNbtC2SPacket */ return false;
				case 0x18: HandlePacketUseEntity(a_ByteBuffer); return true;
				case 0x19: /* Jigsaw generating */ return false;
				case 0x1A: HandlePacketKeepAlive(a_ByteBuffer); return true;
				case 0x1B: /* Update difficulty lock */ return false;  // only used in single player
				case 0x1C: HandlePacketPlayerPos(a_ByteBuffer); return true;  // PositionAndOnGround
				case 0x1D: HandlePacketPlayerPosLook(a_ByteBuffer); return true;  // full
				case 0x1E: HandlePacketPlayerLook(a_ByteBuffer); return true;  // LookAndOnGround
				case 0x1F: HandlePacketPlayer(a_ByteBuffer); return true;
				case 0x20: HandlePacketVehicleMove(a_ByteBuffer); return true;
				case 0x21: HandlePacketBoatSteer(a_ByteBuffer); return true;
				case 0x22: /* pick item from block */ return false;
				case 0x23: /* pick item from entity */ return false;
				case 0x24: /* QueryPingC2SPacket */ return false;
				case 0x25: HandleCraftRecipe(a_ByteBuffer); return true;
				case 0x26: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
				case 0x27: HandlePacketBlockDig(a_ByteBuffer); return true;
				case 0x28: /* client command packet */ return false;
				case 0x29: HandlePacketSteerVehicle(a_ByteBuffer); return true;  // player input packet
				case 0x2A: /* Player Loaded */ return false;
				case 0x2B: /* PlayPongC2SPacket */ return false;
				case 0x2C: /* Recipe Category Options */ return false;
				case 0x2D: HandlePacketCraftingBookData(a_ByteBuffer); return true;
				case 0x2E: HandlePacketNameItem(a_ByteBuffer); return true;
				case 0x2F: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
				case 0x30: HandlePacketAdvancementTab(a_ByteBuffer); return true;
				case 0x31: /* select villager trade */ return false;
				case 0x32: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
				case 0x33: HandlePacketSlotSelect(a_ByteBuffer); return true;
				case 0x34: /* update command block */ return false;
				case 0x35: /* update minecart command block */ return false;
				case 0x36: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
				case 0x37: /* Update jigsaw block */ return false;
				case 0x38: /* Update structure block */ return false;
				case 0x39: HandlePacketUpdateSign(a_ByteBuffer); return true;
				case 0x3A: /* Update hand swing */ return false;
				case 0x3B: /* Spectator teleport */ return false;
				case 0x3C: HandlePacketBlockPlace(a_ByteBuffer); return true;
				case 0x3D: HandlePacketUseItem(a_ByteBuffer); return true;
				default: break;
			}
		}
	}
	UNREACHABLE("");
}





UInt32 cProtocol_1_21_4::GetProtocolMobType(eMonsterType a_MobType) const
{
	switch (a_MobType)
	{
		// Map invalid type to Giant for easy debugging (if this ever spawns, something has gone very wrong)
		case mtAllay: return 2;
		case mtArmadillo: return 4;
		case mtAxolotl: return 7;
		case mtBat: return 10;
		case mtBee: return 11;
		case mtBlaze: return 14;
		case mtBogged: return 16;
		case mtBreeze: return 17;
		case mtCamel: return 19;
		case mtCat: return 20;
		case mtCaveSpider: return 21;
		case mtChicken: return 25;
		case mtCod: return 26;
		case mtCow: return 28;
		case mtCreaking: return 29;
		case mtCreeper: return 30;
		case mtDolphin: return 33;
		case mtDonkey: return 34;
		case mtDrowned: return 36;
		case mtElderGuardian: return 38;
		case mtEnderman: return 39;
		case mtEndermite: return 40;
		case mtEnderDragon: return 41;
		case mtEvoker: return 44;
		case mtFox: return 52;
		case mtFrog: return 53;
		case mtGhast: return 55;
		case mtGiant: return 56;
		case mtGlowSquid: return 58;
		case mtGoat: return 59;
		case mtGuardian: return 60;
		case mtHoglin: return 61;
		case mtHorse: return 63;
		case mtHusk: return 64;
		case mtIllusioner: return 65;
		case mtLlama: return 75;
		case mtMagmaCube: return 77;
		case mtMooshroom: return 82;
		case mtMule: return 83;
		case mtOcelot: return 86;
		case mtPanda: return 91;
		case mtParrot: return 92;
		case mtPhantom: return 93;
		case mtPig: return 94;
		case mtPiglin: return 95;
		case mtPiglinBrute: return 96;
		case mtPillager: return 97;
		case mtPolarBear: return 98;
		case mtPufferfish: return 100;
		case mtRabbit: return 101;
		case mtRavager: return 102;
		case mtSalmon: return 103;
		case mtSheep: return 104;
		case mtShulker: return 105;
		case mtSilverfish: return 107;
		case mtSkeleton: return 108;
		case mtSkeletonHorse: return 109;
		case mtSlime: return 110;
		case mtSniffer: return 112;
		case mtSpider: return 117;
		case mtSquid: return 120;
		case mtStray: return 121;
		case mtStrider: return 122;
		case mtTadpole: return 123;
		case mtTraderLlama: return 127;
		case mtTropicalFish: return 129;
		case mtTurtle: return 130;
		case mtVex: return 131;
		case mtVillager: return 132;
		case mtVindicator: return 133;
		case mtWanderingTrader: return 134;
		case mtWarden: return 135;
		case mtWitch: return 137;
		case mtWither: return 138;
		case mtWitherSkeleton: return 139;
		case mtWolf: return 141;
		case mtZoglin: return 142;
		case mtZombie: return 143;
		case mtZombieHorse: return 144;
		case mtZombieVillager: return 145;
		case mtZombifiedPiglin: return 146;
		default: return 0;
	}
}





UInt8 cProtocol_1_21_4::GetProtocolEntityType(const cEntity & a_Entity) const
{
	using Type = cEntity::eEntityType;

	switch (a_Entity.GetEntityType())
	{
		case Type::etEnderCrystal: return 43;
		case Type::etPickup: return 68;
		case Type::etFallingBlock: return 49;
		case Type::etMinecart: return 81;
		case Type::etBoat: return 84;
		case Type::etTNT: return 125;
		case Type::etProjectile:
		{
			using PType = cProjectileEntity::eKind;
			const auto & Projectile = static_cast<const cProjectileEntity &>(a_Entity);

			switch (Projectile.GetProjectileKind())
			{
				case PType::pkArrow: return 6;
				case PType::pkSnowball: return 113;
				case PType::pkEgg: return 37;
				case PType::pkGhastFireball: return 111;
				case PType::pkFireCharge: return 50;
				case PType::pkEnderPearl: return 42;
				case PType::pkExpBottle: return 46;
				case PType::pkSplashPotion: return 99;
				case PType::pkFirework: return 51;
				case PType::pkWitherSkull: return 140;
			}
			break;
		}
		case Type::etFloater: return 148;
		case Type::etItemFrame: return 70;
		case Type::etLeashKnot: return 73;

		// Non-objects must not be sent
		case Type::etEntity:
		case Type::etPlayer:
		case Type::etMonster:
		case Type::etExpOrb:
		case Type::etPainting: break;
	}
	UNREACHABLE("Unhandled entity kind");
}




