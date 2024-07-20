#include "Globals.h"
#include "Protocol_1_20.h"
#include "Packetizer.h"





////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_20:

cProtocol::Version cProtocol_1_20::GetProtocolVersion() const
{
	return Version::v1_20;
}




void cProtocol_1_20::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the Join Game packet:
	{
		cServer * Server = cRoot::Get()->GetServer();
		cPacketizer Pkt(*this, pktJoinGame);
		Pkt.WriteBEUInt32(a_Player.GetUniqueID());
		Pkt.WriteBool(Server->IsHardcore());
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()));  // current game mode
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()));  // previous game mode
		Pkt.WriteVarInt32(1);  // Number of dimensions
		Pkt.WriteString("overworld");
		AString dmgsrc[] = {"in_fire","lightning_bolt", "on_fire","lava","hot_floor", "in_wall","cramming", "drown","starve",	"cactus", "fall","fly_into_wall","out_of_world","generic","magic","wither","dragon_breath","dry_out","sweet_berry_bush","freeze","stalagmite","outside_border","generic_kill"};
		int dmgids[] =     {18       ,22             ,27        ,21     ,17        ,19        ,3           ,5     ,33          ,2       ,8      ,14             ,28            ,16       ,23     ,40      ,4              ,6        ,35                ,15      ,32          ,5              ,7};
		{
			cFastNBTWriter Writer;
			Writer.BeginCompound("minecraft:damage_type");
				Writer.AddString("type", "minecraft:damage_type");
				Writer.BeginList("value", eTagType::TAG_Compound);
				int id = 0;
					for each (auto ds in dmgsrc)
					{
						Writer.BeginCompound("");
							Writer.BeginCompound("element");
								Writer.AddString("effects", "burning");
								Writer.AddFloat("exhaustion", 0.1);
								Writer.AddString("message_id", "inFire");
								Writer.AddString("scaling", "when_caused_by_living_non_player");
							Writer.EndCompound();
						Writer.AddInt("id", dmgids[id]);
						Writer.AddString("name", "minecraft:"+ds);
						Writer.EndCompound();
						id++;
					}

				Writer.EndList();
			Writer.EndCompound();
			Writer.BeginCompound("minecraft:chat_type");
				Writer.AddString("type", "minecraft:chat_type");
				Writer.BeginList("value", eTagType::TAG_Compound);
					Writer.BeginCompound("");
						Writer.BeginCompound("element");
							Writer.BeginCompound("chat");
									Writer.BeginList("parameters", eTagType::TAG_String);
										Writer.AddString("", "sender");
										Writer.AddString("", "content");
									Writer.EndList();
										Writer.BeginCompound("style");
										Writer.EndCompound();
									Writer.AddString("translation_key", "chat.type.text");
							Writer.EndCompound();

							Writer.BeginCompound("narration");
									Writer.BeginList("parameters", eTagType::TAG_String);
										Writer.AddString("", "sender");
										Writer.AddString("", "content");
									Writer.EndList();
										Writer.BeginCompound("style");
										Writer.EndCompound();
									Writer.AddString("translation_key", "chat.type.narrate");
							Writer.EndCompound();
						Writer.EndCompound();
					Writer.AddInt("id",0);
					Writer.AddString("name", "minecraft:chat");
					Writer.EndCompound();
				Writer.EndList();
			Writer.EndCompound();
			Writer.BeginCompound("minecraft:dimension_type");
				Writer.AddString("type", "minecraft:dimension_type");
				Writer.BeginList("value", eTagType::TAG_Compound);
					Writer.BeginCompound("");

						Writer.BeginCompound("element");

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
							Writer.BeginCompound("monster_spawn_light_level");

							Writer.AddString("type", "minecraft:uniform");
								Writer.BeginCompound("value");

								Writer.AddInt("min_inclusive", 0);
								Writer.AddInt("max_inclusive", 7);

								Writer.EndCompound();

							Writer.EndCompound();

						Writer.EndCompound();

						Writer.AddInt("id",0);
						Writer.AddString("name", "minecraft:overworld");
					Writer.EndCompound();
				Writer.EndList();
			Writer.EndCompound();
			Writer.BeginCompound("minecraft:worldgen/biome");
				Writer.AddString("type", "minecraft:worldgen/biome");
				Writer.BeginList("value", eTagType::TAG_Compound);
					Writer.BeginCompound("");
					Writer.AddString("name", "minecraft:plains");
					Writer.AddInt("id",0);
						Writer.BeginCompound("element");
							Writer.AddString("precipitation", "rain");
								Writer.BeginCompound("effects");
								Writer.AddInt("sky_color", 7907327);
								Writer.AddInt("water_fog_color", 329011);
								Writer.AddInt("fog_color" ,12638463);
								Writer.AddInt("water_color", 4159204);
									Writer.BeginCompound("mood_sound");
									Writer.AddInt("tick_delay", 6000);
									Writer.AddDouble("offset", 2.0);
									Writer.AddString("sound", "minecraft:ambient.cave");
									Writer.AddInt("block_search_extent", 8);
									Writer.EndCompound();
								Writer.EndCompound();
							Writer.AddFloat("depth", -1.0f);
							Writer.AddFloat("temperature", 0.5f);
							Writer.AddFloat("scale", 0.1f);
							Writer.AddFloat("downfall", 0.5f);
							Writer.AddByte("has_precipitation",1);
							Writer.AddString("category", "plains");
						Writer.EndCompound();
					Writer.AddString("name", "minecraft:plains");
					Writer.AddInt("id",0);
					Writer.EndCompound();
				Writer.EndList();
			Writer.EndCompound();
			Writer.Finish();
			Pkt.WriteBuf(Writer.GetResult());
		}

		////dimensional type
		//{
		//	cFastNBTWriter Writer;
		//	Writer.AddByte("piglin_safe", 1);
		//	Writer.AddByte("natural", 1);
		//	Writer.AddFloat("ambient_light", 1.0);
		//	Writer.AddString("infiniburn", "infiniburn_overworld");
		//	Writer.AddByte("respawn_anchor_works", 1);
		//	Writer.AddByte("has_skylight", 1);
		//	Writer.AddByte("bed_works", 1);
		//	Writer.AddString("effects", "minecraft:overworld");
		//	Writer.AddByte("has_raids", 1);
		//	Writer.AddInt("logical_height", 256);
		//	Writer.AddDouble("coordinate_scale", 1.0);
		//	Writer.AddByte("ultrawarm", 1);
		//	Writer.AddByte("has_ceiling", 1);
		//	Writer.AddInt("min_y", 0);
		//	Writer.AddInt("height", 256);
		//	Writer.Finish();
		//	Pkt.WriteBuf(Writer.GetResult());
		//}


		Pkt.WriteString("minecraft:overworld"); // dimension type
		Pkt.WriteString("minecraft:overworld"); // dimension id

		Pkt.WriteBEInt64(0);  // Seed
		Pkt.WriteVarInt32(static_cast<UInt32>(Server->GetMaxPlayers()));
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));  
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));  // simulation distance
		Pkt.WriteBool(false);  // Reduced debug info
		Pkt.WriteBool(true);   // Show deaths screen
		Pkt.WriteBool(false);  // Debug world
		Pkt.WriteBool(false);  // Flat World
		Pkt.WriteBool(false);  // optional last death loc
		Pkt.WriteVarInt32(0);  // Portal Cooldown
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





////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_20_2:

cProtocol::Version cProtocol_1_20_2::GetProtocolVersion() const
{
	return Version::v1_20_2;
}





////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_20_3:

cProtocol::Version cProtocol_1_20_3::GetProtocolVersion() const
{
	return Version::v1_20_3;
}





////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_20_5:

cProtocol::Version cProtocol_1_20_5::GetProtocolVersion() const
{
	return Version::v1_20_5;
}
