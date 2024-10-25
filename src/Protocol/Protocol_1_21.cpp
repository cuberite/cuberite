#include "Globals.h"
#include "Protocol_1_21.h"
#include "Packetizer.h"
#include <ClientHandle.h>
#include "WorldStorage/FastNBT.h"
#include "Palettes/Palette_1_21.h"


////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_21:

cProtocol::Version cProtocol_1_21::GetProtocolVersion() const
{
	return Version::v1_21;
}





UInt32 cProtocol_1_21::GetPacketID(ePacketType a_PacketType) const
{
	switch (m_State)
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
		//default: ASSERT(false,"Invalid m_State: %d", m_State); break;
	}
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
			Writer.AddInt("monster_spawn_light_level",0);
			Writer.AddInt("monster_spawn_block_light_limit",0);
			Writer.Finish();
			Pkt.WriteBuf(Writer.GetResult());
		}
		{
			cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
			Pkt.WriteString("minecraft:worldgen/biome");
			Pkt.WriteVarInt32(64);
			Pkt.WriteString("plains"); Pkt.WriteBool(false); // temporarily here
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
			//Pkt.WriteString("plains"); Pkt.WriteBool(false);
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



////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_21_2:

cProtocol::Version cProtocol_1_21_2::GetProtocolVersion() const
{
	return Version::v1_21_2;
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



