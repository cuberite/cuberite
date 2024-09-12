#include "Globals.h"
#include "Protocol_1_20.h"
#include "Packetizer.h"
#include <ClientHandle.h>





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
								Writer.AddFloat("exhaustion", 0.1F);
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




void cProtocol_1_20::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockChanges);
	Pkt.WriteBEInt32(a_ChunkX);
	Pkt.WriteBEInt32(a_ChunkZ);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Changes.size()));

	for (const auto & Change : a_Changes)
	{
		Int16 Coords = static_cast<Int16>(Change.m_RelY | (Change.m_RelZ << 8) | (Change.m_RelX << 12));
		UInt64 packed = Coords | (Change.m_BlockIdNew << 12);
		Pkt.WriteVarInt64(packed);
	}
}





void cProtocol_1_20::SendRespawn(eDimension a_Dimension)
{
	cPacketizer Pkt(*this, pktRespawn);
	cPlayer* Player = m_Client->GetPlayer();

	Pkt.WriteString("minecraft:overworld");  // dimension type key
	Pkt.WriteString("minecraft:overworld");  // world key
	Pkt.WriteBEUInt64(0);  // Appears to be a SHA256 od the world seed
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBool(false);  // debug world
	Pkt.WriteBool(false);  // flat world
	Pkt.WriteBEInt8(0x3);   // keep player attributes
	Pkt.WriteBool(false);  // optional last death pos
	Pkt.WriteVarInt32(0);
}





////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_20_2:

cProtocol::Version cProtocol_1_20_2::GetProtocolVersion() const
{
	return Version::v1_20_2;
}





UInt32 cProtocol_1_20_2::GetPacketID(ePacketType a_PacketType) const
{
	switch (a_PacketType)
	{
		/// Status packets
		case cProtocol::pktStatusResponse:       return 0x00;
		case cProtocol::pktPingResponse:         return 0x01;

		//  Login Packets
		case cProtocol::pktDisconnectDuringLogin:return 0x00;
		case cProtocol::pktEncryptionRequest:    return 0x01;
		case cProtocol::pktLoginSuccess:         return 0x02;
		case cProtocol::pktStartCompression:     return 0x03;

		// Configuration
		case cProtocol::pktConfigurationCustomPayload:return 0x00;
			//  Disconnect 0x01
		case cProtocol::pktConfigurationReady:   return 0x02;
			//  KeepAlive 0x03
			//  CommonPing 0x04
		case cProtocol::pktConfigurationDynamicRegistries: return 0x05;
			//  ResourcePackSend  0x06
			//  Features 0x07
			//  SynchronizeTags 0x08

		//  Game packets
		case cProtocol::pktSpawnObject:          return 0x01;
		case cProtocol::pktSpawnMob:             return 0x01;
		case cProtocol::pktSpawnPainting:        return 0x01;
		case cProtocol::pktSpawnOtherPlayer:     return 0x01;
		case cProtocol::pktSpawnExperienceOrb:   return 0x02;
		case cProtocol::pktEntityAnimation:      return 0x03;
		case cProtocol::pktStatistics:           return 0x04;
		case cProtocol::pktPlayerActionResponse: return 0x05;
		//  case cProtocol::pktBlockbreakingprogress:   return 0x06;
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
				//  cooldown update 0x16
				//  chat suggestions 0x17
		case cProtocol::pktCustomPayload:        return 0x18;
		case cProtocol::pktPluginMessage:        return 0x18;
				// EntityDamageS2CPacket 0x19
				// RemoveMessageS2CPacket 0x1A
				//case cProtocol::pktSoundEffect:          return 0x1B;
		case cProtocol::pktDisconnectDuringGame: return 0x1B;
				//  ProfilelessChatMessageS2CPacket 0x1C
		case cProtocol::pktEntityStatus:         return 0x1D;
		case cProtocol::pktExplosion:            return 0x1E;
		case cProtocol::pktUnloadChunk:          return 0x1F;
		case cProtocol::pktGameMode:             return 0x20;
		case cProtocol::pktWeather:              return 0x20;
		case cProtocol::pktHorseWindowOpen:      return 0x21;
				// DamageTiltS2CPacket 0x22
				// wolrld border initalize 0x23
		case cProtocol::pktKeepAlive:            return 0x24;
				// chunk data packet 0x25
		case cProtocol::pktSoundParticleEffect:  return 0x26;  // world event
		case cProtocol::pktParticleEffect:       return 0x27;
		case cProtocol::pktLightUpdate:          return 0x28;
		case cProtocol::pktJoinGame:             return 0x29;
				//  map update 0x2A
				//  set trade offers 0x2B
		case cProtocol::pktEntityRelMove:        return 0x2C;
		case cProtocol::pktEntityRelMoveLook:    return 0x2D;
		case cProtocol::pktEntityLook:           return 0x2E;
				//  vehicle move 0x2F
				//  open written book 0x30
		case cProtocol::pktWindowOpen:           return 0x31;
		case cProtocol::pktUpdateSign:           return 0x32;
				//  CommonPingS2CPacket 0x33
				//  PingResultS2CPacket 0x34
				//  craft failed response 0x35
		case cProtocol::pktPlayerAbilities:      return 0x36;
				//  ChatMessageS2CPacket 0x37
				//  combat exit 0x38
				//  comabt enter 0x39
				//  death msg 0x3A
		case cProtocol::pktPlayerLstRemove:      return 0x3B;
		case cProtocol::pktPlayerList:           return 0x3C;
				//  look at 0x3D
		case cProtocol::pktPlayerMoveLook:       return 0x3E;
		case cProtocol::pktUnlockRecipe:         return 0x3F;
		case cProtocol::pktDestroyEntity:        return 0x40;
		case cProtocol::pktRemoveEntityEffect:   return 0x41;
		case cProtocol::pktResourcePack:         return 0x42;
		case cProtocol::pktRespawn:              return 0x43;
		case cProtocol::pktEntityHeadLook:       return 0x44;
		case cProtocol::pktBlockChanges:         return 0x45;
				// select advancment tab 0x46
				// ServerMetadataS2CPacket 0x47
				// overlay msg 0x48
				// wb -- worldborder wb center changed 0x49
				// wb interpolate size 0x4A
				// wb size changed 0x4B
				// wb warning time changed 0x4C
				// wb warning blocks changed 0x4D
		case cProtocol::pktCameraSetTo:          return 0x4E;
		case cProtocol::pktHeldItemChange:       return 0x4F;
		case cProtocol::pktRenderDistanceCenter: return 0x50;
				//  chunk load distance 0x51
		case cProtocol::pktSpawnPosition:        return 0x52;
				//  scoreboard display 0x53
		case cProtocol::pktEntityMeta:           return 0x54;
				// entity attach 0x55
		case cProtocol::pktEntityVelocity:       return 0x56;
		case cProtocol::pktEntityEquipment:      return 0x57;
		case cProtocol::pktExperience:           return 0x58;
		case cProtocol::pktUpdateHealth:         return 0x59;
		case cProtocol::pktScoreboardObjective:  return 0x5A;
		case cProtocol::pktAttachEntity:         return 0x5B;
				// Teams 0x5C
		case cProtocol::pktUpdateScore:          return 0x5D;
				// simulation distance 0x5E
				// subtitle 0x5F
		case cProtocol::pktTimeUpdate:           return 0x60;
		case cProtocol::pktTitle:                return 0x61;
				//  title fade 0x62
				//  play sound from entity 0x63
				//  play sound 0x64
				//  EnterReconfigurationS2CPacket 0x65
				//  stop sound 0x66
		case cProtocol::pktChatRaw:              return 0x67; //  Gamemessage
				//  player list header 0x68
				//  NbtQueryResponseS2CPacket 0x69
		case cProtocol::pktCollectEntity:        return 0x6A;
		case cProtocol::pktTeleportEntity:       return 0x6B;
				//  advancment update 0x6C
		case cProtocol::pktEntityProperties:     return 0x6D;
		case cProtocol::pktEntityEffect:         return 0x6E;
				//  sync recepies 0x6F
				//  sync tags 0x70
		default: UNREACHABLE("unhandeled packet");
	}
}





void cProtocol_1_20_2::SendLoginSuccess(void)
{
	ASSERT(m_State == 2);  // State: login?

	// Enable compression:
	{
		cPacketizer Pkt(*this, pktStartCompression);
		Pkt.WriteVarInt32(CompressionThreshold);
	}

	m_CompressionEnabled = true;
	// m_State = State::Game;

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





bool cProtocol_1_20_2::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
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
				case 0x02: /*LoginQueryResponseC2SPacket*/ return false;
				case 0x03: HandlePacketEnterConfiguration(a_ByteBuffer); return true;
			}
			break;
		}

		case State::Configuration:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketClientSettings(a_ByteBuffer); return true;
				case 0x01: HandlePacketPluginMessage(a_ByteBuffer); return true;
				case 0x02: HandlePacketReady(a_ByteBuffer); return true;
				case 0x03: HandlePacketKeepAlive(a_ByteBuffer); return true;
				case 0x04: /*CommonPongC2SPacket*/ return false;
				case 0x05: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
			}
			break;
		}

		case State::Game:
		{
			switch (a_PacketType)
			{
				case 0x00: HandleConfirmTeleport(a_ByteBuffer); return true;
				case 0x01: /* query nbt packet */ return false;
				case 0x02: /* update difficulty */ return false;
				case 0x03: /* MessageAcknowledgmentC2SPacket */ return false;
				case 0x04: HandlePacketCommandExecution(a_ByteBuffer); return true;
				case 0x05: HandlePacketChatMessage(a_ByteBuffer); return true;
				case 0x06: HandlePacketPlayerSession(a_ByteBuffer); return true;
				case 0x07: /* AcknowledgeChunksC2SPacket */ return false;
				case 0x08: HandlePacketClientStatus(a_ByteBuffer); return true;
				case 0x09: HandlePacketClientSettings(a_ByteBuffer); return true;
				case 0x0A: HandlePacketTabComplete(a_ByteBuffer); return true;
				case 0x0B: /* AcknowledgeReconfigurationC2SPacket*/ return false;
				case 0x0C: /* ButtonClickC2SPacket */ return false;
				case 0x0D: HandlePacketWindowClick(a_ByteBuffer); return true;
				case 0x0E: HandlePacketWindowClose(a_ByteBuffer); return true;
				case 0x0F: HandlePacketPluginMessage(a_ByteBuffer); return true;
				case 0x10: HandlePacketBookUpdate(a_ByteBuffer); return true;  // not fully implemented
				case 0x11: /* QueryEntityNbtC2SPacket */ return false;
				case 0x12: HandlePacketUseEntity(a_ByteBuffer); return true;
				case 0x13: /* Jigsaw generating */ return false;
				case 0x14: HandlePacketKeepAlive(a_ByteBuffer); return true;
				case 0x15: /* Update difficulty lock */ return false;  // only used in single player
				case 0x16: HandlePacketPlayerPos(a_ByteBuffer); return true;  // PositionAndOnGround
				case 0x17: HandlePacketPlayerPosLook(a_ByteBuffer); return true; // full
				case 0x18: HandlePacketPlayerLook(a_ByteBuffer); return true; // LookAndOnGround
				case 0x19: HandlePacketPlayer(a_ByteBuffer); return true;
				case 0x1A: HandlePacketVehicleMove(a_ByteBuffer); return true;
				case 0x1B: HandlePacketBoatSteer(a_ByteBuffer); return true;
				case 0x1C: /* pick from inventory */ return false;
				case 0x1D: /*QueryPingC2SPacket*/ return false;
				case 0x1E: HandleCraftRecipe(a_ByteBuffer); return true;
				case 0x1F: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
				case 0x20: HandlePacketBlockDig(a_ByteBuffer); return true;
				case 0x21: /* client command packet */ return false;
				case 0x22: HandlePacketSteerVehicle(a_ByteBuffer); return true;  // player input packet
				case 0x23: /* PlayPongC2SPacket */ return false;
				case 0x24: /* Recipe Category Options */ return false;
				case 0x25: HandlePacketCraftingBookData(a_ByteBuffer); return true;
				case 0x26: HandlePacketNameItem(a_ByteBuffer); return true;
				case 0x27: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
				case 0x28: HandlePacketAdvancementTab(a_ByteBuffer); return true;
				case 0x29: /* select villager trade */ return false;
				case 0x2A: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
				case 0x2B: HandlePacketSlotSelect(a_ByteBuffer); return true;
				case 0x2C: /* update command block */ return false;
				case 0x2D: /* update minecart command block*/ return false;
				case 0x2E: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
				case 0x2F: /* Update jigsaw block */ return false;
				case 0x30: /* Update structure block */ return false;
				case 0x31: HandlePacketUpdateSign(a_ByteBuffer); return true;
				case 0x32: /* Update hand swing */ return false;
				case 0x33: /* Spectator teleport */ return false;
				case 0x34: HandlePacketBlockPlace(a_ByteBuffer); return true;
				case 0x35: HandlePacketUseItem(a_ByteBuffer); return true;
				default: break;
			}
		}
	}
}





void cProtocol_1_20_2::HandlePacketEnterConfiguration(cByteBuffer & a_ByteBuffer)
{
	m_State = State::Configuration;
	SendDynamicRegistries();
	m_Client->SendFinishConfiguration();
}





void cProtocol_1_20_2::SendFinishConfiguration()
{
	m_IsConfigReadySent = true;
	{
		cPacketizer Pkt(*this, pktConfigurationReady);
	}
}





void cProtocol_1_20_2::SendDynamicRegistries()
{
	{
		cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
		AString dmgsrc[] = {"in_fire","lightning_bolt", "on_fire","lava","hot_floor", "in_wall","cramming", "drown","starve",	"cactus", "fall","fly_into_wall","out_of_world","generic","magic","wither","dragon_breath","dry_out","sweet_berry_bush","freeze","stalagmite","outside_border","generic_kill"};
		int dmgids[] =     {18       ,22             ,27        ,21     ,17        ,19        ,3           ,5     ,33          ,2       ,8      ,14             ,28            ,16       ,23     ,40      ,4              ,6        ,35                ,15      ,32          ,5              ,7};
		{
			cFastNBTWriter Writer = cFastNBTWriter(true);
			Writer.BeginCompound("minecraft:damage_type");
				Writer.AddString("type", "minecraft:damage_type");
				Writer.BeginList("value", eTagType::TAG_Compound);
				int id = 0;
					for each (auto ds in dmgsrc)
					{
						Writer.BeginCompound("");
							Writer.BeginCompound("element");
								Writer.AddString("effects", "burning");
								Writer.AddFloat("exhaustion", 0.1F);
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

	}
}





void cProtocol_1_20_2::HandlePacketLoginStart(cByteBuffer & a_ByteBuffer)
{
	AString Username;
	if (!a_ByteBuffer.ReadVarUTF8String(Username))
	{
		m_Client->Kick("Bad username");
		return;
	}
	HANDLE_READ(a_ByteBuffer,ReadUUID,cUUID,ProfileID);
	if (!m_Client->HandleHandshake(Username))
	{
		// The client is not welcome here, they have been sent a Kick packet already
		return;
	}

	m_Client->SetUsername(std::move(Username));

	// If auth is required, then send the encryption request:
	if (const auto Server = cRoot::Get()->GetServer(); Server->ShouldAuthenticate())
	{
		cPacketizer Pkt(*this, pktEncryptionRequest);
		Pkt.WriteString(Server->GetServerID());
		const auto PubKeyDer = Server->GetPublicKeyDER();
		Pkt.WriteVarInt32(static_cast<UInt32>(PubKeyDer.size()));
		Pkt.WriteBuf(PubKeyDer);
		Pkt.WriteVarInt32(4);
		Pkt.WriteBEInt32(static_cast<int>(reinterpret_cast<intptr_t>(this)));  // Using 'this' as the cryptographic nonce, so that we don't have to generate one each time :)
		return;
	}

	m_Client->HandleLogin();
}





void cProtocol_1_20_2::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
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

		Pkt.WriteString("minecraft:overworld"); // dimension type
		Pkt.WriteString("minecraft:overworld"); // dimension id
		Pkt.WriteBEInt64(0);  // Seed

		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()));  // current game mode
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()));  // previous game mode

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





void cProtocol_1_20_2::HandlePacketReady(cByteBuffer & a_ByteBuffer)
{
	m_State = Game;
	m_Client->FinishAuthenticate();
}





void cProtocol_1_20_2::SendPluginMessage(const AString & a_Channel, const ContiguousByteBufferView a_Message)
{
	ASSERT(m_State == 3 || m_State == 4);  // In game mode?

	if (m_State == cProtocol::State::Configuration && !m_IsConfigReadySent)
	{
		{
			cPacketizer Pkt(*this, pktConfigurationCustomPayload);
			Pkt.WriteString(a_Channel);
			Pkt.WriteBuf(a_Message);
		}
	}
	else
	{
		cPacketizer Pkt(*this, pktPluginMessage);
		Pkt.WriteString(a_Channel);
		Pkt.WriteBuf(a_Message);
	}
}





void cProtocol_1_20_2::SendRespawn(eDimension a_Dimension)
{
	cPacketizer Pkt(*this, pktRespawn);
	cPlayer* Player = m_Client->GetPlayer();

	Pkt.WriteString("minecraft:overworld");  // dimension type key
	Pkt.WriteString("minecraft:overworld");  // world key
	Pkt.WriteBEUInt64(0);  // Appears to be a SHA256 od the world seed
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBool(false);  // debug world
	Pkt.WriteBool(false);  // flat world
	Pkt.WriteBool(false);  // optional last death pos
	Pkt.WriteVarInt32(0); // portal cool down
	Pkt.WriteBEInt8(0x3); // keep metadata and attributes 
}





////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_20_3:

cProtocol::Version cProtocol_1_20_3::GetProtocolVersion() const
{
	return Version::v1_20_3;
}




UInt32 cProtocol_1_20_3::GetPacketID(ePacketType a_PacketType) const
{
	switch (a_PacketType)
	{
		/// Status packets
		case cProtocol::pktStatusResponse:       return 0x00;
		case cProtocol::pktPingResponse:         return 0x01;

		//  Login Packets
		case cProtocol::pktDisconnectDuringLogin:return 0x00;
		case cProtocol::pktEncryptionRequest:    return 0x01;
		case cProtocol::pktLoginSuccess:         return 0x02;
		case cProtocol::pktStartCompression:     return 0x03;

		// Configuration
		case cProtocol::pktConfigurationCustomPayload:return 0x00;
			//  Disconnect 0x01
		case cProtocol::pktConfigurationReady:   return 0x02;
			//  KeepAlive 0x03
			//  CommonPing 0x04
		case cProtocol::pktConfigurationDynamicRegistries: return 0x05;
			//	Resource pack remove 0x06
			//  ResourcePackSend  0x07
			//  Features 0x08
			//  SynchronizeTags 0x09

		//  Game packets
	    case cProtocol::pktSpawnObject:          return 0x01;
        case cProtocol::pktSpawnMob:             return 0x01;
        case cProtocol::pktSpawnPainting:        return 0x01;
        case cProtocol::pktSpawnOtherPlayer:     return 0x01;
        case cProtocol::pktSpawnExperienceOrb:   return 0x02;
        case cProtocol::pktEntityAnimation:      return 0x03;
        case cProtocol::pktStatistics:           return 0x04;
        case cProtocol::pktPlayerActionResponse: return 0x05;
        //  case cProtocol::pktBlockbreakingprogress:   return 0x06;
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
                        //  cooldown update 0x16
                        //  chat suggestions 0x17
        case cProtocol::pktCustomPayload:        return 0x18;
        case cProtocol::pktPluginMessage:        return 0x18;
                        // EntityDamageS2CPacket 0x19
                        // RemoveMessageS2CPacket 0x1A
                        //case cProtocol::pktSoundEffect:          return 0x1B;
        case cProtocol::pktDisconnectDuringGame: return 0x1B;
                        //  ProfilelessChatMessageS2CPacket 0x1C
        case cProtocol::pktEntityStatus:         return 0x1D;
        case cProtocol::pktExplosion:            return 0x1E;
        case cProtocol::pktUnloadChunk:          return 0x1F;
        case cProtocol::pktGameMode:             return 0x20;
        case cProtocol::pktWeather:              return 0x20;
        case cProtocol::pktHorseWindowOpen:      return 0x21;
                        // DamageTiltS2CPacket 0x22
                        // wolrld border initalize 0x23
        case cProtocol::pktKeepAlive:            return 0x24;
                        // chunk data packet 0x25
        case cProtocol::pktSoundParticleEffect:  return 0x26;  // world event
        case cProtocol::pktParticleEffect:       return 0x27;
        case cProtocol::pktLightUpdate:          return 0x28;
        case cProtocol::pktJoinGame:             return 0x29;
                        //  map update 0x2A
                        //  set trade offers 0x2B
        case cProtocol::pktEntityRelMove:        return 0x2C;
        case cProtocol::pktEntityRelMoveLook:    return 0x2D;
        case cProtocol::pktEntityLook:           return 0x2E;
                        //  vehicle move 0x2F
                        //  open written book 0x30
        case cProtocol::pktWindowOpen:           return 0x31;
        case cProtocol::pktUpdateSign:           return 0x32;
                        //  CommonPingS2CPacket 0x33
                        //  PingResultS2CPacket 0x34
                        //  craft failed response 0x35
        case cProtocol::pktPlayerAbilities:      return 0x36;
                        //  ChatMessageS2CPacket 0x37
                        //  combat exit 0x38
                        //  comabt enter 0x39
                        //  death msg 0x3A
        case cProtocol::pktPlayerLstRemove:      return 0x3B;
        case cProtocol::pktPlayerList:           return 0x3C;
                        //  look at 0x3D
        case cProtocol::pktPlayerMoveLook:       return 0x3E;
        case cProtocol::pktUnlockRecipe:         return 0x3F;
        case cProtocol::pktDestroyEntity:        return 0x40;
        case cProtocol::pktRemoveEntityEffect:   return 0x41;
			// ScoreboardScoreResetS2CPacket 0x42
			// ResourcePackRemoveS2CPacket 0x43
        case cProtocol::pktResourcePack:         return 0x44;
        case cProtocol::pktRespawn:              return 0x45;
        case cProtocol::pktEntityHeadLook:       return 0x46;
        case cProtocol::pktBlockChanges:         return 0x47;
                        // select advancment tab 0x48
                        // ServerMetadataS2CPacket 0x49
                        // overlay msg 0x4A
                        // wb -- worldborder wb center changed 0x4B
                        // wb interpolate size 0x4C
                        // wb size changed 0x4D
                        // wb warning time changed 0x4E
                        // wb warning blocks changed 0x4F
        case cProtocol::pktCameraSetTo:          return 0x50;
        case cProtocol::pktHeldItemChange:       return 0x51;
        case cProtocol::pktRenderDistanceCenter: return 0x52;
                        //  chunk load distance 0x53
        case cProtocol::pktSpawnPosition:        return 0x54;
                        //  scoreboard display 0x55
        case cProtocol::pktEntityMeta:           return 0x56;
                        // entity attach 0x57
        case cProtocol::pktEntityVelocity:       return 0x58;
        case cProtocol::pktEntityEquipment:      return 0x59;
        case cProtocol::pktExperience:           return 0x5A;
        case cProtocol::pktUpdateHealth:         return 0x5B;
        case cProtocol::pktScoreboardObjective:  return 0x5C;
        case cProtocol::pktAttachEntity:         return 0x5D;
                        // Teams 0x5E
        case cProtocol::pktUpdateScore:          return 0x5F;
                        // simulation distance 0x60
                        // subtitle 0x61
        case cProtocol::pktTimeUpdate:           return 0x62;
        case cProtocol::pktTitle:                return 0x63;
                        //  title fade 0x64
                        //  play sound from entity 0x65
                        //  play sound 0x66
                        //  EnterReconfigurationS2CPacket 0x67
                        //  stop sound 0x68
        case cProtocol::pktChatRaw:              return 0x69; //  Gamemessage
                        //  player list header 0x6A
                        //  NbtQueryResponseS2CPacket 0x6B
        case cProtocol::pktCollectEntity:        return 0x6C;
			// UpdateTickRateS2CPacket 0x6D
			// TickStepS2CPacket 0x6E
        case cProtocol::pktTeleportEntity:       return 0x6F;
                        //  advancment update 0x70
        case cProtocol::pktEntityProperties:     return 0x71;
        case cProtocol::pktEntityEffect:         return 0x72;
                        //  sync recepies 0x73
                        //  sync tags 0x74
		default: UNREACHABLE("unhandeled packet");
	}
}





bool cProtocol_1_20_3::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	if (m_State != State::Game)
	{
		return Super::HandlePacket(a_ByteBuffer, a_PacketType);
	}

	
	switch (a_PacketType)
	{
        case 0x00: HandleConfirmTeleport(a_ByteBuffer); return true;
        case 0x01: /* query nbt packet */ return false;
        case 0x02: /* update difficulty */ return false;
        case 0x03: /* MessageAcknowledgmentC2SPacket */ return false;
        case 0x04: HandlePacketCommandExecution(a_ByteBuffer); return true;
        case 0x05: HandlePacketChatMessage(a_ByteBuffer); return true;
        case 0x06: HandlePacketPlayerSession(a_ByteBuffer); return true;
        case 0x07: /* AcknowledgeChunksC2SPacket */ return false;
        case 0x08: HandlePacketClientStatus(a_ByteBuffer); return true;
        case 0x09: HandlePacketClientSettings(a_ByteBuffer); return true;
        case 0x0A: HandlePacketTabComplete(a_ByteBuffer); return true;
        case 0x0B: /* AcknowledgeReconfigurationC2SPacket*/ return false;
        case 0x0C: /* ButtonClickC2SPacket */ return false;
        case 0x0D: HandlePacketWindowClick(a_ByteBuffer); return true;
        case 0x0E: HandlePacketWindowClose(a_ByteBuffer); return true;
		case 0x0F: /* SlotChangedStateC2SPacket */ return false;
        case 0x10: HandlePacketPluginMessage(a_ByteBuffer); return true;
        case 0x11: HandlePacketBookUpdate(a_ByteBuffer); return true;  // not fully implemented
        case 0x12: /* QueryEntityNbtC2SPacket */ return false;
        case 0x13: HandlePacketUseEntity(a_ByteBuffer); return true;
        case 0x14: /* Jigsaw generating */ return false;
        case 0x15: HandlePacketKeepAlive(a_ByteBuffer); return true;
        case 0x16: /* Update difficulty lock */ return false;  // only used in single player
        case 0x17: HandlePacketPlayerPos(a_ByteBuffer); return true;  // PositionAndOnGround
        case 0x18: HandlePacketPlayerPosLook(a_ByteBuffer); return true; // full
        case 0x19: HandlePacketPlayerLook(a_ByteBuffer); return true; // LookAndOnGround
        case 0x1A: HandlePacketPlayer(a_ByteBuffer); return true;
        case 0x1B: HandlePacketVehicleMove(a_ByteBuffer); return true;
        case 0x1C: HandlePacketBoatSteer(a_ByteBuffer); return true;
        case 0x1D: /* pick from inventory */ return false;
        case 0x1E: /*QueryPingC2SPacket*/ return false;
        case 0x1F: HandleCraftRecipe(a_ByteBuffer); return true;
        case 0x20: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
        case 0x21: HandlePacketBlockDig(a_ByteBuffer); return true;
        case 0x22: /* client command packet */ return false;
        case 0x23: HandlePacketSteerVehicle(a_ByteBuffer); return true;  // player input packet
        case 0x24: /* PlayPongC2SPacket */ return false;
        case 0x25: /* Recipe Category Options */ return false;
        case 0x26: HandlePacketCraftingBookData(a_ByteBuffer); return true;
        case 0x27: HandlePacketNameItem(a_ByteBuffer); return true;
        case 0x28: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
        case 0x29: HandlePacketAdvancementTab(a_ByteBuffer); return true;
        case 0x2A: /* select villager trade */ return false;
        case 0x2B: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
        case 0x2C: HandlePacketSlotSelect(a_ByteBuffer); return true;
        case 0x2D: /* update command block */ return false;
        case 0x2E: /* update minecart command block*/ return false;
        case 0x2F: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
        case 0x30: /* Update jigsaw block */ return false;
        case 0x31: /* Update structure block */ return false;
        case 0x32: HandlePacketUpdateSign(a_ByteBuffer); return true;
        case 0x33: /* Update hand swing */ return false;
        case 0x34: /* Spectator teleport */ return false;
        case 0x35: HandlePacketBlockPlace(a_ByteBuffer); return true;
        case 0x36: HandlePacketUseItem(a_ByteBuffer); return true;
		default: break;
	}	
}




void cProtocol_1_20_3::SendChatRaw(const AString & a_MessageRaw, eChatType a_Type)
{
	ASSERT(m_State == 3);  // In game mode?

	UNUSED(@@)
	// Prevent chat messages that might trigger CVE-2021-44228
	if (a_MessageRaw.find("${") != std::string::npos)
	{
		return;
	}
	cFastNBTWriter Writer(true);
	Writer.AddString("text", a_MessageRaw);
	Writer.Finish();
	// Send the json string to the client:
	cPacketizer Pkt(*this, pktChatRaw);
	Pkt.WriteBuf(Writer.GetResult());
	Pkt.WriteBool(a_Type == ctAboveActionBar);
}





void cProtocol_1_20_3::SendInitialChunksComing()
{
	{
		cPacketizer Pkt(*this, pktWeather);
		Pkt.WriteBEUInt8(static_cast<UInt8>(eGameStateReason::InitialChunksComing));
		Pkt.WriteBEFloat(0);  // Unused
	}
}





void cProtocol_1_20_3::WriteEntityMetadata(cPacketizer & a_Pkt, const EntityMetadata a_Metadata, const EntityMetadataType a_FieldType) const
{
	a_Pkt.WriteBEUInt8(GetEntityMetadataID(a_Metadata));	      // Index
	auto v = static_cast<UInt32>(a_FieldType);
	if (v >= 2) // temp fix 
	{
		v++;
	}
	a_Pkt.WriteVarInt32(v);        // Type
}





void cProtocol_1_20_3::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const
{
	// Common metadata:
	Int8 Flags = 0;
	if (a_Entity.IsOnFire())
	{
		Flags |= 0x01;
	}
	if (a_Entity.IsCrouched())
	{
		Flags |= 0x02;
	}
	if (a_Entity.IsSprinting())
	{
		Flags |= 0x08;
	}
	if (a_Entity.IsRclking())
	{
		Flags |= 0x10;
	}
	if (a_Entity.IsInvisible())
	{
		Flags |= 0x20;
	}
	/*
	if (a_Entity.IsGlowing())
	{
		Flags |= 0x40;
	}
	*/
	if (a_Entity.IsElytraFlying())
	{
		Flags |= 0x80;
	}

	WriteEntityMetadata(a_Pkt, EntityMetadata::EntityFlags, EntityMetadataType::Byte);
	a_Pkt.WriteBEInt8(Flags);

	switch (a_Entity.GetEntityType())
	{
		case cEntity::etPlayer:
		{
			auto & Player = static_cast<const cPlayer &>(a_Entity);

			// TODO Set player custom name to their name.
			// Then it's possible to move the custom name of mobs to the entities
			// and to remove the "special" player custom name.
			WriteEntityMetadata(a_Pkt, EntityMetadata::EntityCustomName, EntityMetadataType::OptChat);
			a_Pkt.WriteBool(true);

			cFastNBTWriter Writer(true);
			Writer.AddString("text", Player.GetName());
			Writer.Finish();
			a_Pkt.WriteBuf(Writer.GetResult());

			WriteEntityMetadata(a_Pkt, EntityMetadata::LivingHealth, EntityMetadataType::Float);
			a_Pkt.WriteBEFloat(static_cast<float>(Player.GetHealth()));

			WriteEntityMetadata(a_Pkt, EntityMetadata::PlayerDisplayedSkinParts, EntityMetadataType::Byte);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Player.GetSkinParts()));

			WriteEntityMetadata(a_Pkt, EntityMetadata::PlayerMainHand, EntityMetadataType::Byte);
			a_Pkt.WriteBEUInt8(Player.IsLeftHanded() ? 0 : 1);
			break;
		}
		case cEntity::etPickup:
		{
			WriteEntityMetadata(a_Pkt, EntityMetadata::ItemItem, EntityMetadataType::Item);
			WriteItem(a_Pkt, static_cast<const cPickup &>(a_Entity).GetItem());
			break;
		}
		case cEntity::etMinecart:
		{
			WriteEntityMetadata(a_Pkt, EntityMetadata::MinecartShakingPower, EntityMetadataType::VarInt);

			// The following expression makes Minecarts shake more with less health or higher damage taken
			auto & Minecart = static_cast<const cMinecart &>(a_Entity);
			auto maxHealth = a_Entity.GetMaxHealth();
			auto curHealth = a_Entity.GetHealth();
			a_Pkt.WriteVarInt32(static_cast<UInt32>((maxHealth - curHealth) * Minecart.LastDamage() * 4));

			WriteEntityMetadata(a_Pkt, EntityMetadata::MinecartShakingDirection, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(1);  // (doesn't seem to effect anything)

			WriteEntityMetadata(a_Pkt, EntityMetadata::MinecartShakingMultiplier, EntityMetadataType::Float);
			a_Pkt.WriteBEFloat(static_cast<float>(Minecart.LastDamage() + 10));  // or damage taken

			if (Minecart.GetPayload() == cMinecart::mpNone)
			{
				auto & RideableMinecart = static_cast<const cRideableMinecart &>(Minecart);
				const cItem & MinecartContent = RideableMinecart.GetContent();
				if (!MinecartContent.IsEmpty())
				{
					WriteEntityMetadata(a_Pkt, EntityMetadata::MinecartBlockIDMeta, EntityMetadataType::VarInt);
					int Content = MinecartContent.m_ItemType;
					Content |= MinecartContent.m_ItemDamage << 8;
					a_Pkt.WriteVarInt32(static_cast<UInt32>(Content));

					WriteEntityMetadata(a_Pkt, EntityMetadata::MinecartBlockY, EntityMetadataType::VarInt);
					a_Pkt.WriteVarInt32(static_cast<UInt32>(RideableMinecart.GetBlockHeight()));

					WriteEntityMetadata(a_Pkt, EntityMetadata::MinecartShowBlock, EntityMetadataType::Boolean);
					a_Pkt.WriteBool(true);
				}
			}
			else if (Minecart.GetPayload() == cMinecart::mpFurnace)
			{
				WriteEntityMetadata(a_Pkt, EntityMetadata::MinecartFurnacePowered, EntityMetadataType::Boolean);
				a_Pkt.WriteBool(static_cast<const cMinecartWithFurnace &>(Minecart).IsFueled());
			}
			break;
		}  // case etMinecart

		case cEntity::etProjectile:
		{
			auto & Projectile = static_cast<const cProjectileEntity &>(a_Entity);
			switch (Projectile.GetProjectileKind())
			{
				case cProjectileEntity::pkArrow:
				{
					WriteEntityMetadata(a_Pkt, EntityMetadata::ArrowFlags, EntityMetadataType::Byte);
					a_Pkt.WriteBEInt8(static_cast<const cArrowEntity &>(Projectile).IsCritical() ? 1 : 0);

					// TODO: Piercing level
					break;
				}
				case cProjectileEntity::pkFirework:
				{
					// TODO
					break;
				}
				case cProjectileEntity::pkSplashPotion:
				{
					// TODO
				}
				default:
				{
					break;
				}
			}
			break;
		}  // case etProjectile

		case cEntity::etMonster:
		{
			WriteMobMetadata(a_Pkt, static_cast<const cMonster &>(a_Entity));
			break;
		}

		case cEntity::etBoat:
		{
			auto & Boat = static_cast<const cBoat &>(a_Entity);

			WriteEntityMetadata(a_Pkt, EntityMetadata::BoatLastHitTime, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Boat.GetLastDamage()));

			WriteEntityMetadata(a_Pkt, EntityMetadata::BoatForwardDirection, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Boat.GetForwardDirection()));

			WriteEntityMetadata(a_Pkt, EntityMetadata::BoatDamageTaken, EntityMetadataType::Float);
			a_Pkt.WriteBEFloat(Boat.GetDamageTaken());

			WriteEntityMetadata(a_Pkt, EntityMetadata::BoatType, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Boat.GetMaterial()));

			WriteEntityMetadata(a_Pkt, EntityMetadata::BoatRightPaddleTurning, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Boat.IsRightPaddleUsed());

			WriteEntityMetadata(a_Pkt, EntityMetadata::BoatLeftPaddleTurning, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(static_cast<bool>(Boat.IsLeftPaddleUsed()));

			WriteEntityMetadata(a_Pkt, EntityMetadata::BoatSplashTimer, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(0);

			break;
		}  // case etBoat

		case cEntity::etItemFrame:
		{
			// TODO
			break;
		}  // case etItemFrame

		case cEntity::etEnderCrystal:
		{
			const auto & EnderCrystal = static_cast<const cEnderCrystal &>(a_Entity);
			if (EnderCrystal.DisplaysBeam())
			{
				WriteEntityMetadata(a_Pkt, EntityMetadata::EnderCrystalBeamTarget, EntityMetadataType::OptPosition);
				a_Pkt.WriteBool(true);  // Dont do a second check if it should display the beam
				a_Pkt.WriteXYZPosition64(EnderCrystal.GetBeamTarget());
			}
			WriteEntityMetadata(a_Pkt, EntityMetadata::EnderCrystalShowBottom, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(EnderCrystal.ShowsBottom());
			break;
		}  // case etEnderCrystal

		default:
		{
			break;
		}
	}
}





void cProtocol_1_20_3::SendDisconnect(const AString & a_Reason)
{
	switch (m_State)
	{
		case State::Login:
		{
			cPacketizer Pkt(*this, pktDisconnectDuringLogin);
			Pkt.WriteString(JsonUtils::SerializeSingleValueJsonObject("text", a_Reason));
			break;
		}
		case State::Game:
		{
			cPacketizer Pkt(*this, pktDisconnectDuringGame);
			cFastNBTWriter writer(true);
			writer.AddString("text",a_Reason);
			writer.Finish();
			Pkt.WriteBuf(writer.GetResult());
			break;
		}
		default:
		{
			FLOGERROR(
				"Tried to send disconnect in invalid game state {0}",
				static_cast<int>(m_State)
			);
		}
	}
}






////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_20_5:

cProtocol::Version cProtocol_1_20_5::GetProtocolVersion() const
{
	return Version::v1_20_5;
}





bool cProtocol_1_20_5::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
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
                case 0x02: /* update difficulty */ return false;
                case 0x03: /* MessageAcknowledgmentC2SPacket */ return false;
                case 0x04: HandlePacketCommandExecution(a_ByteBuffer); return true;
				case 0x05: /* ChatCommandSignedC2SPacket */ return false;
                case 0x06: HandlePacketChatMessage(a_ByteBuffer); return true;
                case 0x07: HandlePacketPlayerSession(a_ByteBuffer); return true;
                case 0x08: /* AcknowledgeChunksC2SPacket */ return false;
                case 0x09: HandlePacketClientStatus(a_ByteBuffer); return true;
                case 0x0A: HandlePacketClientSettings(a_ByteBuffer); return true;
                case 0x0B: HandlePacketTabComplete(a_ByteBuffer); return true;
                case 0x0C: /* AcknowledgeReconfigurationC2SPacket*/ return false;
                case 0x0D: /* ButtonClickC2SPacket */ return false;
                case 0x0E: HandlePacketWindowClick(a_ByteBuffer); return true;
                case 0x0F: HandlePacketWindowClose(a_ByteBuffer); return true;
                case 0x10: /* SlotChangedStateC2SPacket */ return false;
				case 0x11: /* CookieResponseC2SPacket */ return false;
                case 0x12: HandlePacketPluginMessage(a_ByteBuffer); return true;
				case 0x13: /* DebugSampleSubscriptionC2SPacket */ return false;
                case 0x14: HandlePacketBookUpdate(a_ByteBuffer); return true;  // not fully implemented
                case 0x15: /* QueryEntityNbtC2SPacket */ return false;
                case 0x16: HandlePacketUseEntity(a_ByteBuffer); return true;
                case 0x17: /* Jigsaw generating */ return false;
                case 0x18: HandlePacketKeepAlive(a_ByteBuffer); return true;
                case 0x19: /* Update difficulty lock */ return false;  // only used in single player
                case 0x1A: HandlePacketPlayerPos(a_ByteBuffer); return true;  // PositionAndOnGround
                case 0x1B: HandlePacketPlayerPosLook(a_ByteBuffer); return true; // full
                case 0x1C: HandlePacketPlayerLook(a_ByteBuffer); return true; // LookAndOnGround
                case 0x1D: HandlePacketPlayer(a_ByteBuffer); return true;
                case 0x1E: HandlePacketVehicleMove(a_ByteBuffer); return true;
                case 0x1F: HandlePacketBoatSteer(a_ByteBuffer); return true;
                case 0x20: /* pick from inventory */ return false;
                case 0x21: /*QueryPingC2SPacket*/ return false;
                case 0x22: HandleCraftRecipe(a_ByteBuffer); return true;
                case 0x23: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
                case 0x24: HandlePacketBlockDig(a_ByteBuffer); return true;
                case 0x25: /* client command packet */ return false;
                case 0x26: HandlePacketSteerVehicle(a_ByteBuffer); return true;  // player input packet
                case 0x27: /* PlayPongC2SPacket */ return false;
                case 0x28: /* Recipe Category Options */ return false;
                case 0x29: HandlePacketCraftingBookData(a_ByteBuffer); return true;
                case 0x2A: HandlePacketNameItem(a_ByteBuffer); return true;
                case 0x2B: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
                case 0x2C: HandlePacketAdvancementTab(a_ByteBuffer); return true;
                case 0x2D: /* select villager trade */ return false;
                case 0x2E: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
                case 0x2F: HandlePacketSlotSelect(a_ByteBuffer); return true;
                case 0x30: /* update command block */ return false;
                case 0x31: /* update minecart command block*/ return false;
                case 0x32: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
                case 0x33: /* Update jigsaw block */ return false;
                case 0x34: /* Update structure block */ return false;
                case 0x35: HandlePacketUpdateSign(a_ByteBuffer); return true;
                case 0x36: /* Update hand swing */ return false;
                case 0x37: /* Spectator teleport */ return false;
                case 0x38: HandlePacketBlockPlace(a_ByteBuffer); return true;
                case 0x39: HandlePacketUseItem(a_ByteBuffer); return true;
				default: break;
			}
		}
	}
}





UInt32 cProtocol_1_20_5::GetPacketID(ePacketType a_PacketType) const
{
	switch (a_PacketType)
	{
		/// Status packets
		case cProtocol::pktStatusResponse:       return 0x00;
		case cProtocol::pktPingResponse:         return 0x01;

		//  Login Packets
		case cProtocol::pktDisconnectDuringLogin:return 0x00;
		case cProtocol::pktEncryptionRequest:    return 0x01;
		case cProtocol::pktLoginSuccess:         return 0x02;
		case cProtocol::pktStartCompression:     return 0x03;
			//login query request 0x4
			//cookie request 0x5

		// Configuration
			//CookieRequestS2CPacket 0x0
		case cProtocol::pktConfigurationCustomPayload: return 0x01;
			//  Disconnect 0x02
		case cProtocol::pktConfigurationReady:   return 0x03;
			//  KeepAlive 0x04
			//  CommonPing 0x05
			// RestChat 0x06
		case cProtocol::pktConfigurationDynamicRegistries: return 0x07;
			//	Resource pack remove 0x08
			//  ResourcePackSend  0x09
			//	StoreCookie 0x0A
			//  ServerTransfer 0x0B
			//  Features 0x0C
			//  SynchronizeTags 0x0D
		case cProtocol::pktSelectKnownPacks:     return 0x0E;

		//  Game packets
        case cProtocol::pktSpawnObject:          return 0x01;
        case cProtocol::pktSpawnMob:             return 0x01;
        case cProtocol::pktSpawnPainting:        return 0x01;
        case cProtocol::pktSpawnOtherPlayer:     return 0x01;
        case cProtocol::pktSpawnExperienceOrb:   return 0x02;
        case cProtocol::pktEntityAnimation:      return 0x03;
        case cProtocol::pktStatistics:           return 0x04;
        case cProtocol::pktPlayerActionResponse: return 0x05;
        //  case cProtocol::pktBlockbreakingprogress:   return 0x06;
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
        case cProtocol::pktExplosion:            return 0x20;
        case cProtocol::pktUnloadChunk:          return 0x21;
        case cProtocol::pktGameMode:             return 0x22;
        case cProtocol::pktWeather:              return 0x22;
        case cProtocol::pktHorseWindowOpen:      return 0x23;
                        // DamageTiltS2CPacket 0x24
                        // wolrld border initalize 0x25
        case cProtocol::pktKeepAlive:            return 0x26;
                        // chunk data packet 0x27
        case cProtocol::pktSoundParticleEffect:  return 0x28;  // world event
        case cProtocol::pktParticleEffect:       return 0x29;
        case cProtocol::pktLightUpdate:          return 0x2A;
        case cProtocol::pktJoinGame:             return 0x2B;
                        //  map update 0x2C
                        //  set trade offers 0x2D
        case cProtocol::pktEntityRelMove:        return 0x2E;
        case cProtocol::pktEntityRelMoveLook:    return 0x2F;
        case cProtocol::pktEntityLook:           return 0x30;
                        //  vehicle move 0x31
                        //  open written book 0x32
        case cProtocol::pktWindowOpen:           return 0x33;
        case cProtocol::pktUpdateSign:           return 0x34;
                        //  CommonPingS2CPacket 0x35
                        //  PingResultS2CPacket 0x36
                        //  craft failed response 0x37
        case cProtocol::pktPlayerAbilities:      return 0x38;
                        //  ChatMessageS2CPacket 0x39
                        //  combat exit 0x3A
                        //  comabt enter 0x3B
                        //  death msg 0x3C
        case cProtocol::pktPlayerLstRemove:      return 0x3D;
        case cProtocol::pktPlayerList:           return 0x3E;
                        //  look at 0x3F
        case cProtocol::pktPlayerMoveLook:       return 0x40;
        case cProtocol::pktUnlockRecipe:         return 0x41;
        case cProtocol::pktDestroyEntity:        return 0x42;
        case cProtocol::pktRemoveEntityEffect:   return 0x43;
                        // ScoreboardScoreResetS2CPacket 0x44
                        // ResourcePackRemoveS2CPacket 0x45
        case cProtocol::pktResourcePack:         return 0x46;
        case cProtocol::pktRespawn:              return 0x47;
        case cProtocol::pktEntityHeadLook:       return 0x48;
        case cProtocol::pktBlockChanges:         return 0x49;
                        // select advancment tab 0x4A
                        // ServerMetadataS2CPacket 0x4B
                        // overlay msg 0x4C
                        // wb -- worldborder wb center changed 0x4D
                        // wb interpolate size 0x4E
                        // wb size changed 0x4F
                        // wb warning time changed 0x50
                        // wb warning blocks changed 0x51
        case cProtocol::pktCameraSetTo:          return 0x52;
        case cProtocol::pktHeldItemChange:       return 0x53;
        case cProtocol::pktRenderDistanceCenter: return 0x54;
                        //  chunk load distance 0x55
        case cProtocol::pktSpawnPosition:        return 0x56;
                        //  scoreboard display 0x57
        case cProtocol::pktEntityMeta:           return 0x58;
                        // entity attach 0x59
        case cProtocol::pktEntityVelocity:       return 0x5A;
        case cProtocol::pktEntityEquipment:      return 0x5B;
        case cProtocol::pktExperience:           return 0x5C;
        case cProtocol::pktUpdateHealth:         return 0x5D;
        case cProtocol::pktScoreboardObjective:  return 0x5E;
        case cProtocol::pktAttachEntity:         return 0x5F;
                        // Teams 0x60
        case cProtocol::pktUpdateScore:          return 0x61;
                        // simulation distance 0x62
                        // subtitle 0x63
        case cProtocol::pktTimeUpdate:           return 0x64;
        case cProtocol::pktTitle:                return 0x65;
                        //  title fade 0x66
                        //  play sound from entity 0x67
                        //  play sound 0x68
                        //  EnterReconfigurationS2CPacket 0x69
                        //  stop sound 0x6A
						//  StoreCookies 0x6B
        case cProtocol::pktChatRaw:              return 0x6C; //  Gamemessage
                        //  player list header 0x6D
                        //  NbtQueryResponseS2CPacket 0x6E
        case cProtocol::pktCollectEntity:        return 0x6F;
                        // UpdateTickRateS2CPacket 0x70
                        // TickStepS2CPacket 0x71
						// ServerTransfer 0x72
        case cProtocol::pktTeleportEntity:       return 0x73;
                        //  advancment update 0x74
        case cProtocol::pktEntityProperties:     return 0x75;
        case cProtocol::pktEntityEffect:         return 0x76;
                        //  sync recepies 0x77
                        //  sync tags 0x78
						//  ProjectilePower 0x79
		default: UNREACHABLE("unhandeled packet");
	}
}





void cProtocol_1_20_5::HandlePacketLoginStart(cByteBuffer & a_ByteBuffer)
{
	AString Username;
	if (!a_ByteBuffer.ReadVarUTF8String(Username))
	{
		m_Client->Kick("Bad username");
		return;
	}

	HANDLE_READ(a_ByteBuffer,ReadUUID,cUUID,ProfileID);
	if (!m_Client->HandleHandshake(Username))
	{
		// The client is not welcome here, they have been sent a Kick packet already
		return;
	}

	m_Client->SetUsername(std::move(Username));

	// Encryption can now be used regardless of is auth is required. Encryption is now always requested
	const auto Server = cRoot::Get()->GetServer();
	{
		cPacketizer Pkt(*this, pktEncryptionRequest);
		Pkt.WriteString(Server->GetServerID());
		const auto PubKeyDer = Server->GetPublicKeyDER();
		Pkt.WriteVarInt32(static_cast<UInt32>(PubKeyDer.size()));
		Pkt.WriteBuf(PubKeyDer);
		Pkt.WriteVarInt32(4);
		Pkt.WriteBEInt32(static_cast<int>(reinterpret_cast<intptr_t>(this)));  // Using 'this' as the cryptographic nonce, so that we don't have to generate one each time :)
		Pkt.WriteBool(Server->ShouldAuthenticate());
	}

	if (!Server->ShouldAuthenticate())
	{
		m_Client->HandleLogin();
	}
}





void cProtocol_1_20_5::SendDynamicRegistries()
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
			Pkt.WriteVarInt32(1);
			Pkt.WriteString("minecraft:plains");
			Pkt.WriteBool(false);
		}
		{
			cPacketizer Pkt(*this, pktConfigurationDynamicRegistries);
			Pkt.WriteString("minecraft:damage_type");
			Pkt.WriteVarInt32(45);
			Pkt.WriteString("arrow"); Pkt.WriteBool(false);
			Pkt.WriteString("bad_respawn_point"); Pkt.WriteBool(false);
			Pkt.WriteString("cactus"); Pkt.WriteBool(false);
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
			Pkt.WriteString("wither"); Pkt.WriteBool(false);
			Pkt.WriteString("wither_skull"); Pkt.WriteBool(false);
		}	

	}
}





void cProtocol_1_20_5::SendSelectKnownPacks()
{
	cPacketizer Pkt(*this, pktSelectKnownPacks);
	{
		Pkt.WriteVarInt32(1);
		Pkt.WriteString("minecraft");
		Pkt.WriteString("core");
		Pkt.WriteString("1.20.5");
	}	
}





void cProtocol_1_20_5::HandlePacketEnterConfiguration(cByteBuffer & a_ByteBuffer)
{
	m_State = State::Configuration;
	SendDynamicRegistries();
	SendSelectKnownPacks();
	m_Client->SendFinishConfiguration();
}





void cProtocol_1_20_5::WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity) const
{
	if (a_Entity.IsPlayer())
	{
		const auto & Player = static_cast<const cPlayer &>(a_Entity);

		a_Pkt.WriteVarInt32(1);  // Count.
		a_Pkt.WriteVarInt32(17); // generic.speed
		a_Pkt.WriteBEDouble(0.1 * Player.GetNormalMaxSpeed());  // The default game speed is 0.1, multiply that value by the relative speed.

		// It seems the modifiers aren't conditionally activated; their effects are applied immediately!
		// We have to keep on re-sending this packet when the client notifies us of sprint start and end, and so on. Strange.

		if (Player.IsSprinting())
		{
			a_Pkt.WriteVarInt32(1);  // Modifier count.
			a_Pkt.WriteBEUInt64(0x662a6b8dda3e4c1c);
			a_Pkt.WriteBEUInt64(0x881396ea6097278d);  // UUID of the modifier (sprinting speed boost).
			a_Pkt.WriteBEDouble(Player.GetSprintingMaxSpeed() - Player.GetNormalMaxSpeed());
			a_Pkt.WriteBEUInt8(2);
		}
		else
		{
			a_Pkt.WriteVarInt32(0);
		}
	}
	else
	{
		// const cMonster & Mob = (const cMonster &)a_Entity;

		// TODO: Send properties and modifiers based on the mob type

		a_Pkt.WriteVarInt32(0);
	}
}





void cProtocol_1_20_5::SendLoginSuccess(void)
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
		Pkt.WriteBool(true); //  strict error handling
	}
}





void cProtocol_1_20_5::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
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

		Pkt.WriteVarInt32(0); // Dimension id
		Pkt.WriteString("minecraft:overworld"); // dimension name
		Pkt.WriteBEInt64(0);  // Seed

		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()));  // current game mode
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()));  // previous game mode

		Pkt.WriteBool(false);  // Debug world
		Pkt.WriteBool(false);  // Flat World

		Pkt.WriteBool(false);  // optional last death loc
		Pkt.WriteVarInt32(0);  // Portal Cooldown
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





void cProtocol_1_20_5::HandlePacketCommandExecution(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Command);

	m_Client->HandleChat("/" + Command);
}





void cProtocol_1_20_5::SendRespawn(eDimension a_Dimension)
{
	cPacketizer Pkt(*this, pktRespawn);
	cPlayer* Player = m_Client->GetPlayer();

	Pkt.WriteVarInt32(0);  // dimension type key
	Pkt.WriteString("minecraft:overworld");  // world key
	Pkt.WriteBEUInt64(0);  // Appears to be a SHA256 od the world seed
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBool(false);  // debug world
	Pkt.WriteBool(false);  // flat world
	Pkt.WriteBool(false);  // optional last death pos
	Pkt.WriteVarInt32(0); // portal cool down
	Pkt.WriteBEInt8(0x3); // keep metadata and attributes 
}




