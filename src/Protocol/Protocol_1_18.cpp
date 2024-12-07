#include "Globals.h"
#include "Protocol_1_18.h"
#include "Packetizer.h"
#include "../ClientHandle.h"
#include "../Server.h"
#include "../Entities/Player.h"
#include "../Entities/Entity.h"
#include "../WorldStorage/FastNBT.h"
#include "../Root.h"




////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_18:

cProtocol::Version cProtocol_1_18::GetProtocolVersion() const
{
	return Version::v1_18;
}





UInt32 cProtocol_1_18::GetPacketID(ePacketType a_PacketType) const
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

		//  Game packets
		case cProtocol::pktSpawnObject:          return 0x00;
		case cProtocol::pktSpawnExperienceOrb:   return 0x01;
		case cProtocol::pktSpawnMob:             return 0x02;
		case cProtocol::pktSpawnPainting:        return 0x03;
		case cProtocol::pktSpawnOtherPlayer:     return 0x04;
			// vibration 0x05
		case cProtocol::pktEntityAnimation:      return 0x06;
		case cProtocol::pktStatistics:           return 0x07;
		case cProtocol::pktPlayerActionResponse: return 0x08;
		//  case cProtocol::pktBlockbreakingprogress:   return 0x09;
		case cProtocol::pktUpdateBlockEntity:    return 0x0A;
		case cProtocol::pktBlockAction:          return 0x0B;
		case cProtocol::pktBlockChange:          return 0x0C;
		case cProtocol::pktBossBar:              return 0x0D;
		case cProtocol::pktDifficulty:           return 0x0E;
		case cProtocol::pktChatRaw:              return 0x0F;  //  Gamemessage
			//  clear title 0x10
			//  command suggestions here 0x11
		case cProtocol::pktCommnadTree:          return 0x12;
		case cProtocol::pktWindowClose:          return 0x13;
		case cProtocol::pktWindowItems:          return 0x14;  //  Inventory packet
		case cProtocol::pktWindowProperty:       return 0x15;  //  ScreenHandlerPropertyUpdateS2CPacket
		case cProtocol::pktInventorySlot:        return 0x16;  //  ScreenHandlerSlotUpdateS2CPacket
			//  0x17 cooldown update
		case cProtocol::pktCustomPayload:        return 0x18;
		case cProtocol::pktPluginMessage:        return 0x18;
		case cProtocol::pktSoundEffect:          return 0x19;
		case cProtocol::pktDisconnectDuringGame: return 0x1A;
		case cProtocol::pktEntityStatus:         return 0x1B;
		case cProtocol::pktExplosion:            return 0x1C;
		case cProtocol::pktUnloadChunk:          return 0x1D;
		case cProtocol::pktGameMode:             return 0x1E;
		case cProtocol::pktWeather:              return 0x1E;
		case cProtocol::pktHorseWindowOpen:      return 0x1F;
			// wolrld border initalize 0x20
		case cProtocol::pktKeepAlive:            return 0x21;
			// chunk data packet 0x22
		case cProtocol::pktSoundParticleEffect:  return 0x23;  // world event
		case cProtocol::pktParticleEffect:       return 0x24;
		case cProtocol::pktLightUpdate:          return 0x25;
		case cProtocol::pktJoinGame:             return 0x26;
		case cProtocol::pktMapData:              return 0x27;
			//  set trade offers 0x28
		case cProtocol::pktEntityRelMove:        return 0x29;
		case cProtocol::pktEntityRelMoveLook:    return 0x2A;
		case cProtocol::pktEntityLook:           return 0x2B;
			// --  entity packet 0x2A
			//  vehicle move 0x2C
			//  open written book 0x2D
		case cProtocol::pktWindowOpen:           return 0x2E;
		case cProtocol::pktUpdateSign:           return 0x2F;
			//  play ping 0x30
			//  craft failed response 0x31
		case cProtocol::pktPlayerAbilities:      return 0x32;
			//  combat exit 0x33
			//  comabt enter 0x34
			//  death msg 0x35
		case cProtocol::pktPlayerList:           return 0x36;
			//  look at 0x37
		case cProtocol::pktPlayerMoveLook:       return 0x38;
		case cProtocol::pktUnlockRecipe:         return 0x39;
		case cProtocol::pktDestroyEntity:        return 0x3A;
		case cProtocol::pktRemoveEntityEffect:   return 0x3B;
		case cProtocol::pktResourcePack:         return 0x3C;
		case cProtocol::pktRespawn:              return 0x3D;
		case cProtocol::pktEntityHeadLook:       return 0x3E;
		case cProtocol::pktBlockChanges:         return 0x3F;
			// select advancment tab 0x40
			// overlay msg 0x41
			// wb -- worldborder wb center changed 0x42
			// wb interpolate size 0x43
			// wb size changed 0x44
			// wb warning time changed 0x45
			// wb warning blocks changed 0x46
		case cProtocol::pktCameraSetTo:          return 0x47;
		case cProtocol::pktHeldItemChange:       return 0x48;
		case cProtocol::pktRenderDistanceCenter: return 0x49;
			//  chunk load distance 0x4A
		case cProtocol::pktSpawnPosition:        return 0x4B;
			//  scoreboard display 0x4C
		case cProtocol::pktEntityMeta:           return 0x4D;
		case cProtocol::pktLeashEntity:          return 0x4E;
		case cProtocol::pktEntityVelocity:       return 0x4F;
		case cProtocol::pktEntityEquipment:      return 0x50;
		case cProtocol::pktExperience:           return 0x51;
		case cProtocol::pktUpdateHealth:         return 0x52;
		case cProtocol::pktScoreboardObjective:  return 0x53;
		case cProtocol::pktAttachEntity:         return 0x54;
			// Teams 0x55
		case cProtocol::pktUpdateScore:          return 0x56;
			// simulation distance 0x57
			// subtitle 0x58
		case cProtocol::pktTimeUpdate:           return 0x59;
		case cProtocol::pktTitle:                return 0x5A;
			//  title fade 0x5B
			//  play sound from entity 0x5C
			//  play sound 0x5D
			//  stop sound 0x5E
			//  player list header 0x5F
			//  tag query response 0x60
		case cProtocol::pktCollectEntity:        return 0x61;
		case cProtocol::pktTeleportEntity:       return 0x62;
			//  advancment update 0x63
		case cProtocol::pktEntityProperties:     return 0x64;
		case cProtocol::pktEntityEffect:         return 0x65;
			//  sync recepies 0x66
			//  sync tags 0x67
		default: UNREACHABLE("unhandeled packet");
	}
}





void cProtocol_1_18::HandlePacketClientSettings(cByteBuffer & a_ByteBuffer)
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

	m_Client->SetLocale(Locale);
	m_Client->SetViewDistance(ViewDistance);
	m_Client->GetPlayer()->SetSkinParts(SkinParts);
	m_Client->GetPlayer()->SetLeftHanded(MainHand == LEFT_HAND);
	m_Client->SetAllowListing(AllowsListing);
	// TODO: Handle chat flags and chat colors
}





void cProtocol_1_18::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
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
		// Pkt.WriteString("the_nether");
		// Pkt.WriteString("the_end");
		{
			cFastNBTWriter Writer;
			Writer.BeginCompound("minecraft:dimension_type");
				Writer.AddString("type", "minecraft:dimension_type");
				Writer.BeginList("value", eTagType::TAG_Compound);
					Writer.BeginCompound("");

						Writer.BeginCompound("element");

						Writer.AddByte("piglin_safe", 1);
						Writer.AddByte("natural", 1);
						Writer.AddFloat("ambient_light", 1.0);
						Writer.AddString("infiniburn", "infiniburn_overworld");
						Writer.AddByte("respawn_anchor_works", 1);
						Writer.AddByte("has_skylight", 1);
						Writer.AddByte("bed_works", 1);
						Writer.AddString("effects", "minecraft:overworld");
						Writer.AddByte("has_raids", 1);
						Writer.AddInt("logical_height", 256);
						Writer.AddDouble("coordinate_scale", 1.0);
						Writer.AddByte("ultrawarm", 1);
						Writer.AddByte("has_ceiling", 1);
						Writer.AddInt("min_y", 0);
						Writer.AddInt("height", 256);
						Writer.EndCompound();

						Writer.AddInt("id", 0);
						Writer.AddString("name", "minecraft:overworld");
					Writer.EndCompound();
				Writer.EndList();
			Writer.EndCompound();
			Writer.BeginCompound("minecraft:worldgen/biome");
				Writer.AddString("type", "minecraft:worldgen/biome");
				Writer.BeginList("value", eTagType::TAG_Compound);
					Writer.BeginCompound("");
					Writer.AddString("name", "minecraft:plains");
					Writer.AddInt("id", 0);
						Writer.BeginCompound("element");
							Writer.AddString("precipitation", "rain");
								Writer.BeginCompound("effects");
								Writer.AddInt("sky_color", 7907327);
								Writer.AddInt("water_fog_color", 329011);
								Writer.AddInt("fog_color", 12638463);
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
							Writer.AddString("category", "plains");
						Writer.EndCompound();
					Writer.AddString("name", "minecraft:plains");
					Writer.AddInt("id", 0);
					Writer.EndCompound();
				Writer.EndList();
			Writer.EndCompound();
			Writer.Finish();
			Pkt.WriteBuf(Writer.GetResult());
		}

		// Dimensional type
		{
			cFastNBTWriter Writer;
			Writer.AddByte("piglin_safe", 1);
			Writer.AddByte("natural", 1);
			Writer.AddFloat("ambient_light", 1.0);
			Writer.AddString("infiniburn", "infiniburn_overworld");
			Writer.AddByte("respawn_anchor_works", 1);
			Writer.AddByte("has_skylight", 1);
			Writer.AddByte("bed_works", 1);
			Writer.AddString("effects", "minecraft:overworld");
			Writer.AddByte("has_raids", 1);
			Writer.AddInt("logical_height", 256);
			Writer.AddDouble("coordinate_scale", 1.0);
			Writer.AddByte("ultrawarm", 1);
			Writer.AddByte("has_ceiling", 1);
			Writer.AddInt("min_y", 0);
			Writer.AddInt("height", 256);
			Writer.Finish();
			Pkt.WriteBuf(Writer.GetResult());
		}

		Pkt.WriteString("overworld");  // dimension id

		Pkt.WriteBEInt64(0);  // Seed
		Pkt.WriteVarInt32(static_cast<UInt32>(Server->GetMaxPlayers()));
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));  // simulation distance
		Pkt.WriteBool(false);  // Reduced debug info
		Pkt.WriteBool(true);   // Show deaths screen
		Pkt.WriteBool(false);  // Debug world
		Pkt.WriteBool(false);  // Flat World
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
//  cProtocol_1_18_2:

cProtocol::Version cProtocol_1_18_2::GetProtocolVersion() const
{
	return Version::v1_18_2;
}
