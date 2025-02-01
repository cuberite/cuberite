#include "Globals.h"
#include "Protocol_1_19.h"
#include "Packetizer.h"
#include "ClientHandle.h"
#include "Server.h"
#include "Entities/Player.h"
#include "Entities/Entity.h"
#include "Entities/Pickup.h"
#include "Entities/Minecart.h"
#include "../WorldStorage/FastNBT.h"
#include "../Root.h"
#include "../JsonUtils.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/Boat.h"
#include "../Entities/EnderCrystal.h"
#include "Palettes/Palette_1_19.h"


////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_19:

cProtocol::Version cProtocol_1_19::GetProtocolVersion() const
{
	return Version::v1_19;
}





UInt32 cProtocol_1_19::GetPacketID(ePacketType a_PacketType) const
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
		case cProtocol::pktSpawnMob:             return 0x00;
		case cProtocol::pktSpawnPainting:        return 0x00;
		case cProtocol::pktSpawnExperienceOrb:   return 0x01;
		case cProtocol::pktSpawnOtherPlayer:     return 0x02;
		case cProtocol::pktEntityAnimation:      return 0x03;
		case cProtocol::pktStatistics:           return 0x04;
		case cProtocol::pktPlayerActionResponse: return 0x05;
		//  case cProtocol::pktBlockbreakingprogress:   return 0x06;
		case cProtocol::pktUpdateBlockEntity:    return 0x07;
		case cProtocol::pktBlockAction:          return 0x08;
		case cProtocol::pktBlockChange:          return 0x09;
		case cProtocol::pktBossBar:              return 0x0A;
		case cProtocol::pktDifficulty:           return 0x0B;
			//  chat preview 0x0C
			//  clear title 0x0D
			//  command suggestions here 0x0E
		case cProtocol::pktCommnadTree:          return 0x0F;
		case cProtocol::pktWindowClose:          return 0x10;
		case cProtocol::pktWindowItems:          return 0x11;  //  Inventory packet
		case cProtocol::pktWindowProperty:       return 0x12;  //  ScreenHandlerPropertyUpdateS2CPacket
		case cProtocol::pktInventorySlot:        return 0x13;  //  ScreenHandlerSlotUpdateS2CPacket
			//  0x14 cooldown update
		case cProtocol::pktCustomPayload:        return 0x15;
		case cProtocol::pktPluginMessage:        return 0x15;
		case cProtocol::pktSoundEffect:          return 0x16;
		case cProtocol::pktDisconnectDuringGame: return 0x17;
		case cProtocol::pktEntityStatus:         return 0x18;
		case cProtocol::pktExplosion:            return 0x19;
		case cProtocol::pktUnloadChunk:          return 0x1A;
		case cProtocol::pktGameMode:             return 0x1B;
		case cProtocol::pktWeather:              return 0x1B;
		case cProtocol::pktHorseWindowOpen:      return 0x1C;
			// wolrld border initalize 0x1D
		case cProtocol::pktKeepAlive:            return 0x1E;
			// chunk data packet 0x1F
		case cProtocol::pktSoundParticleEffect:  return 0x20;  // world event
		case cProtocol::pktParticleEffect:       return 0x21;
		case cProtocol::pktLightUpdate:          return 0x22;
		case cProtocol::pktJoinGame:             return 0x23;
		case cProtocol::pktMapData:              return 0x24;
			//  set trade offers 0x25
		case cProtocol::pktEntityRelMove:        return 0x26;
		case cProtocol::pktEntityRelMoveLook:    return 0x27;
		case cProtocol::pktEntityLook:           return 0x28;
			//  vehicle move 0x29
			//  open written book 0x2A
		case cProtocol::pktWindowOpen:           return 0x2B;
		case cProtocol::pktUpdateSign:           return 0x2C;
			//  play ping 0x2D
			//  craft failed response 0x2E
		case cProtocol::pktPlayerAbilities:      return 0x2F;
			//  ChatMessageS2CPacket 0x30
			//  combat exit 0x31
			//  comabt enter 0x32
			//  death msg 0x33
		case cProtocol::pktPlayerList:           return 0x34;
			//  look at 0x35
		case cProtocol::pktPlayerMoveLook:       return 0x36;
		case cProtocol::pktUnlockRecipe:         return 0x37;
		case cProtocol::pktDestroyEntity:        return 0x38;
		case cProtocol::pktRemoveEntityEffect:   return 0x39;
		case cProtocol::pktResourcePack:         return 0x3A;
		case cProtocol::pktRespawn:              return 0x3B;
		case cProtocol::pktEntityHeadLook:       return 0x3C;
		case cProtocol::pktBlockChanges:         return 0x3D;
			// select advancment tab 0x3E
			// ServerMetadataS2CPacket 0x3F
			// overlay msg 0x40
			// wb -- worldborder wb center changed 0x41
			// wb interpolate size 0x42
			// wb size changed 0x43
			// wb warning time changed 0x44
			// wb warning blocks changed 0x45
		case cProtocol::pktCameraSetTo:          return 0x46;
		case cProtocol::pktHeldItemChange:       return 0x47;
		case cProtocol::pktRenderDistanceCenter: return 0x48;
			//  chunk load distance 0x49
		case cProtocol::pktSpawnPosition:        return 0x4A;
			//  ChatPreviewStateChangeS2CPacket 0x4B
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
		case cProtocol::pktChatRaw:              return 0x5F;  //  Gamemessage
			//  player list header 0x60
			//  NbtQueryResponseS2CPacket 0x61
		case cProtocol::pktCollectEntity:        return 0x62;
		case cProtocol::pktTeleportEntity:       return 0x63;
			//  advancment update 0x64
		case cProtocol::pktEntityProperties:     return 0x65;
		case cProtocol::pktEntityEffect:         return 0x66;
			//  sync recepies 0x67
			//  sync tags 0x68
		default: UNREACHABLE("unhandeled packet");
	}
}





bool cProtocol_1_19::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	if (m_State != State::Game)
	{
		return Super::HandlePacket(a_ByteBuffer, a_PacketType);
	}

	// Game
	switch (a_PacketType)
	{
		case 0x00: HandleConfirmTeleport(a_ByteBuffer); return true;
		case 0x01: /* query nbt packet */ return false;
		case 0x02: /* update difficulty */ return false;
		case 0x03: HandlePacketCommandExecution(a_ByteBuffer); return true;
		case 0x04: HandlePacketChatMessage(a_ByteBuffer); return true;
		case 0x05: /* RequestChatPreviewC2SPacket */ return false;
		case 0x06: HandlePacketClientStatus(a_ByteBuffer); return true;
		case 0x07: HandlePacketClientSettings(a_ByteBuffer); return true;
		case 0x08: HandlePacketTabComplete(a_ByteBuffer); return true;
		case 0x09: /* ButtonClickC2SPacket */ return false;
		case 0x0A: HandlePacketWindowClick(a_ByteBuffer); return true;
		case 0x0B: HandlePacketWindowClose(a_ByteBuffer); return true;
		case 0x0C: HandlePacketPluginMessage(a_ByteBuffer); return true;
		case 0x0D: HandlePacketBookUpdate(a_ByteBuffer); return true;  // not fully implemented
		case 0x0E: /* QueryEntityNbtC2SPacket */ return false;
		case 0x0F: HandlePacketUseEntity(a_ByteBuffer); return true;
		case 0x10: /* Jigsaw generating */ return false;
		case 0x11: HandlePacketKeepAlive(a_ByteBuffer); return true;
		case 0x12: /* Update difficulty lock */ return false;  // only used in single player
		case 0x13: HandlePacketPlayerPos(a_ByteBuffer); return true;  // PositionAndOnGround
		case 0x14: HandlePacketPlayerPosLook(a_ByteBuffer); return true;  // full
		case 0x15: HandlePacketPlayerLook(a_ByteBuffer); return true;  // LookAndOnGround
		case 0x16: HandlePacketPlayer(a_ByteBuffer); return true;
		case 0x17: HandlePacketVehicleMove(a_ByteBuffer); return true;
		case 0x18: HandlePacketBoatSteer(a_ByteBuffer); return true;
		case 0x19: /* pick from inventory */ return false;
		case 0x1A: HandleCraftRecipe(a_ByteBuffer); return true;
		case 0x1B: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
		case 0x1C: HandlePacketBlockDig(a_ByteBuffer); return true;
		case 0x1D: /* client command packet */ return false;
		case 0x1E: HandlePacketSteerVehicle(a_ByteBuffer); return true;  // player input packet
		case 0x1F: /* PlayPongC2SPacket */ return false;
		case 0x20: /* Recipe Category Options */ return false;
		case 0x21: HandlePacketCraftingBookData(a_ByteBuffer); return true;
		case 0x22: HandlePacketNameItem(a_ByteBuffer); return true;
		case 0x23: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
		case 0x24: HandlePacketAdvancementTab(a_ByteBuffer); return true;
		case 0x25: /* select villager trade */ return false;
		case 0x26: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
		case 0x27: HandlePacketSlotSelect(a_ByteBuffer); return true;
		case 0x28: HandlePacketCommandBlockUpdate(a_ByteBuffer); return true;
		case 0x29: /* update minecart command block */ return false;
		case 0x2A: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
		case 0x2B: /* Update jigsaw block */ return false;
		case 0x2C: /* Update structure block */ return false;
		case 0x2D: HandlePacketUpdateSign(a_ByteBuffer); return true;
		case 0x2E: /* Update hand swing */ return false;
		case 0x2F: /* Spectator teleport */ return false;
		case 0x30: HandlePacketBlockPlace(a_ByteBuffer); return true;
		case 0x31: HandlePacketUseItem(a_ByteBuffer); return true;
		default: break;
	}
	UNREACHABLE("");
}





UInt32 cProtocol_1_19::GetProtocolBlockType(BlockState a_Block) const
{
	// return Palette_1_19::From(a_Block);
	return 0;
}





UInt32 cProtocol_1_19::GetProtocolItemType(Item a_ItemID) const
{
	// return Palette_1_19::From(a_ItemID);
	return 0;
}





Item cProtocol_1_19::GetItemFromProtocolID(UInt32 a_ProtocolID) const
{
	// return Palette_1_19::ToItem(a_ProtocolID);
	return Item();
}





void cProtocol_1_19::SendAcknowledgeBlockChange(int a_SequenceId)
{
	cPacketizer Pkt(*this, pktPlayerActionResponse);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_SequenceId));
}





void cProtocol_1_19::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
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
			Writer.BeginCompound("minecraft:chat_type");
				Writer.AddString("type", "minecraft:chat_type");
				Writer.BeginList("value", eTagType::TAG_Compound);
					Writer.BeginCompound("");
						Writer.BeginCompound("element");
							Writer.BeginCompound("chat");
								Writer.BeginCompound("decoration");
									Writer.BeginList("parameters", eTagType::TAG_String);
										Writer.AddString("", "sender");
										Writer.AddString("", "content");
									Writer.EndList();
										Writer.BeginCompound("style");
										Writer.EndCompound();
									Writer.AddString("translation_key", "chat.type.text");
								Writer.EndCompound();
							Writer.EndCompound();

							Writer.BeginCompound("narration");
								Writer.BeginCompound("decoration");
									Writer.BeginList("parameters", eTagType::TAG_String);
										Writer.AddString("", "sender");
										Writer.AddString("", "content");
									Writer.EndList();
										Writer.BeginCompound("style");
										Writer.EndCompound();
									Writer.AddString("translation_key", "chat.type.narrate");
								Writer.EndCompound();
								Writer.AddString("priority", "chat");
							Writer.EndCompound();
						Writer.EndCompound();
					Writer.AddInt("id", 0);
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

		Pkt.WriteString("minecraft:overworld");  // dimension type
		Pkt.WriteString("minecraft:overworld");  // dimension id

		Pkt.WriteBEInt64(0);  // Seed
		Pkt.WriteVarInt32(static_cast<UInt32>(Server->GetMaxPlayers()));
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));  // simulation distance
		Pkt.WriteBool(false);  // Reduced debug info
		Pkt.WriteBool(true);   // Show deaths screen
		Pkt.WriteBool(false);  // Debug world
		Pkt.WriteBool(false);  // Flat World
		Pkt.WriteBool(false);  // optional last death loc
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





void cProtocol_1_19::SendChatRaw(const AString & a_MessageRaw, eChatType a_Type)
{
	ASSERT(m_State == 3);  // In game mode?

	// Prevent chat messages that might trigger CVE-2021-44228
	if (a_MessageRaw.find("${") != std::string::npos)
	{
		return;
	}

	// Send the json string to the client:
	cPacketizer Pkt(*this, pktChatRaw);
	Pkt.WriteString(a_MessageRaw);
	Pkt.WriteVarInt32([a_Type]() -> UInt32
	{
		switch (a_Type)
		{
			case eChatType::ctChatBox: return 0;
			case eChatType::ctSystem: return 1;
			case eChatType::ctAboveActionBar: return 2;
		}
		UNREACHABLE("Unsupported chat type");
	}());

}





void cProtocol_1_19::SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount)
{
	ASSERT(m_State == 3);  // In game mode?

	const auto ParticleID = GetProtocolParticleID(a_ParticleName);

	cPacketizer Pkt(*this, pktParticleEffect);
	Pkt.WriteVarInt32(static_cast<UInt32>(ParticleID));

	Pkt.WriteBool(false);  // Long Distance
	Pkt.WriteBEDouble(a_Src.x);
	Pkt.WriteBEDouble(a_Src.y);
	Pkt.WriteBEDouble(a_Src.z);

	Pkt.WriteBEFloat(a_Offset.x);
	Pkt.WriteBEFloat(a_Offset.y);
	Pkt.WriteBEFloat(a_Offset.z);

	Pkt.WriteBEFloat(a_ParticleData);
	Pkt.WriteBEInt32(a_ParticleAmount);
}





void cProtocol_1_19::SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data)
{
	ASSERT(m_State == 3);  // In game mode?

	return;
	/*
	const auto ParticleID = GetProtocolParticleID(a_ParticleName);

	cPacketizer Pkt(*this, pktParticleEffect);
	Pkt.WriteVarInt32(static_cast<UInt32>(ParticleID));

	Pkt.WriteBool(false);  // Long Distance
	Pkt.WriteBEDouble(a_Src.x);
	Pkt.WriteBEDouble(a_Src.y);
	Pkt.WriteBEDouble(a_Src.z);

	Pkt.WriteBEFloat(a_Offset.x);
	Pkt.WriteBEFloat(a_Offset.y);
	Pkt.WriteBEFloat(a_Offset.z);

	Pkt.WriteBEFloat(a_ParticleData);
	Pkt.WriteBEInt32(a_ParticleAmount);
	*/
	// TODO implement particle specific data
}





void cProtocol_1_19::SendEntitySpawn(const cEntity & a_Entity, const UInt8 a_ObjectType, const Int32 a_ObjectData)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSpawnObject);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());

	// TODO: Bad way to write a UUID, and it's not a true UUID, but this is functional for now.
	Pkt.WriteBEUInt64(0);
	Pkt.WriteBEUInt64(a_Entity.GetUniqueID());
	Pkt.WriteVarInt32(a_ObjectType);
	Pkt.WriteBEDouble(a_Entity.GetPosX());
	Pkt.WriteBEDouble(a_Entity.GetPosY());
	Pkt.WriteBEDouble(a_Entity.GetPosZ());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetHeadYaw());
	Pkt.WriteVarInt32(static_cast<UInt32>(a_ObjectData));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedZ() * 400));
}





void cProtocol_1_19::SendSpawnMob(const cMonster & a_Mob)
{
	ASSERT(m_State == 3);  // In game mode?

	const auto MobType = GetProtocolMobType(a_Mob.GetMobType());

	// If the type is not valid in this protocol bail out:
	if (MobType == 0)
	{
		return;
	}

	cPacketizer Pkt(*this, pktSpawnMob);
	Pkt.WriteVarInt32(a_Mob.GetUniqueID());
	// TODO: Bad way to write a UUID, and it's not a true UUID, but this is functional for now.
	Pkt.WriteBEUInt64(0);
	Pkt.WriteBEUInt64(a_Mob.GetUniqueID());
	Pkt.WriteVarInt32(static_cast<Byte>(MobType));
	Vector3d LastSentPos = a_Mob.GetLastSentPosition();
	Pkt.WriteBEDouble(LastSentPos.x);
	Pkt.WriteBEDouble(LastSentPos.y);
	Pkt.WriteBEDouble(LastSentPos.z);
	Pkt.WriteByteAngle(a_Mob.GetPitch());
	Pkt.WriteByteAngle(a_Mob.GetYaw());
	Pkt.WriteByteAngle(a_Mob.GetHeadYaw());	 // Doesn't seem to be used
	Pkt.WriteVarInt32(0);  // Entity data not used for mobs
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedZ() * 400));
}





void cProtocol_1_19::SendPaintingSpawn(const cPainting & a_Painting)
{
	ASSERT(m_State == 3);  // In game mode?
	LOGWARNING("Paintings not implemented in 1.19+");
	return;
}





const int MAX_ENC_LEN = 512;  // Maximum size of the encrypted message; should be 128, but who knows...
void cProtocol_1_19::HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer)
{
	UInt32 EncKeyLength, EncNonceLength = 0;
	ContiguousByteBuffer EncKey;
	ContiguousByteBuffer EncNonce;

	ContiguousByteBuffer SignatureData;
	if (!a_ByteBuffer.ReadVarInt(EncKeyLength))
	{
		return;
	}
	if (!a_ByteBuffer.ReadSome(EncKey, EncKeyLength))
	{
		return;
	}
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, HasNounce);
	if (HasNounce)
	{
		if (!a_ByteBuffer.ReadVarInt(EncNonceLength))
		{
			return;
		}

		if (!a_ByteBuffer.ReadSome(EncNonce, EncNonceLength))
		{
			return;
		}
	}
	else
	{
		HANDLE_READ(a_ByteBuffer, ReadBEInt64, Int64, salt);
		HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, SignatureLength);
		if (!a_ByteBuffer.ReadSome(SignatureData, SignatureLength))
		{
			return;
		}
	}

	if ((EncKeyLength > MAX_ENC_LEN) || (EncNonceLength > MAX_ENC_LEN))
	{
		LOGD("Too long encryption");
		m_Client->Kick("Hacked client");
		return;
	}

	cRsaPrivateKey & rsaDecryptor = cRoot::Get()->GetServer()->GetPrivateKey();
	int res = 0;
	if (HasNounce)
	{
		// Decrypt EncNonce using privkey
		UInt32 DecryptedNonce[MAX_ENC_LEN / sizeof(Int32)];
		res = rsaDecryptor.Decrypt(EncNonce, reinterpret_cast<Byte *>(DecryptedNonce), sizeof(DecryptedNonce));
		if (res != 4)
		{
			LOGD("Bad nonce length: got %d, exp %d", res, 4);
			m_Client->Kick("Hacked client");
			return;
		}
		if (ntohl(DecryptedNonce[0]) != static_cast<unsigned>(reinterpret_cast<uintptr_t>(this)))
		{
			LOGD("Bad nonce value");
			m_Client->Kick("Hacked client");
			return;
		}
	}
	else
	{
		LOGWARN("Player %s sent a signed nounce, The verification of these is not implemented. Allowing the player to join anyway.", m_Client->GetUsername());
	}

	// Decrypt the symmetric encryption key using privkey:
	Byte DecryptedKey[MAX_ENC_LEN];
	res = rsaDecryptor.Decrypt(EncKey, DecryptedKey, sizeof(DecryptedKey));
	if (res != 16)
	{
		LOGD("Bad key length");
		m_Client->Kick("Hacked client");
		return;
	}

	StartEncryption(DecryptedKey);
	m_Client->HandleLogin();
}





void cProtocol_1_19::HandlePacketLoginStart(cByteBuffer & a_ByteBuffer)
{
	AString Username;
	if (!a_ByteBuffer.ReadVarUTF8String(Username))
	{
		m_Client->Kick("Bad username");
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadBool, bool, HasPublicKey);
	if (HasPublicKey)
	{
		ContiguousByteBuffer PublicKey;
		ContiguousByteBuffer KeySignature;
		HANDLE_READ(a_ByteBuffer, ReadBEInt64, Int64, ExpiresAtEpochMilis);
		HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, PubKeyLen);
		if (!a_ByteBuffer.ReadSome(PublicKey, PubKeyLen))
		{
			return;
		}
		HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, KeySignatureLen);
		if (!a_ByteBuffer.ReadSome(KeySignature, KeySignatureLen))
		{
			return;
		}

		auto time = std::chrono::system_clock::now();  // get the current time
		auto since_epoch = time.time_since_epoch();  // get the duration since epoch
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch).count();

		if (millis > ExpiresAtEpochMilis)
		{
			LOGWARN("Expired profile public key");
		}

		// TODO: validate key

		// UNREACHABLE("not implemented");
	}
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





void cProtocol_1_19::SendLoginSuccess(void)
{
	ASSERT(m_State == 2);  // State: login?

	// Enable compression:
	{
		cPacketizer Pkt(*this, pktStartCompression);
		Pkt.WriteVarInt32(CompressionThreshold);
	}

	m_State = State::Game;

	{
		cPacketizer Pkt(*this, pktLoginSuccess);
		Pkt.WriteUUID(m_Client->GetUUID());
		Pkt.WriteString(m_Client->GetUsername());
		Pkt.WriteVarInt32(0);  // number of Profile Properties
	}
}





void cProtocol_1_19::SendPlayerListAddPlayer(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerList);
	Pkt.WriteVarInt32(0);
	Pkt.WriteVarInt32(1);
	Pkt.WriteUUID(a_Player.GetUUID());
	Pkt.WriteString(a_Player.GetPlayerListName());

	const Json::Value & Properties = a_Player.GetClientHandle()->GetProperties();
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

	Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetEffectiveGameMode()));
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetClientHandle()->GetPing()));
	Pkt.WriteBool(false);
	Pkt.WriteBool(false);
}





void cProtocol_1_19::SendRespawn(eDimension a_Dimension)
{
	cPacketizer Pkt(*this, pktRespawn);
	cPlayer * Player = m_Client->GetPlayer();

	Pkt.WriteString("minecraft:overworld");  // dimension type key
	Pkt.WriteString("minecraft:overworld");  // world key
	Pkt.WriteBEUInt64(0);  // Appears to be a SHA256 od the world seed
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBool(false);  // debug world
	Pkt.WriteBool(false);  // flat world
	Pkt.WriteBool(true);   // keep player attributes
	Pkt.WriteBool(false);  // optional last death pos
}





void cProtocol_1_19::HandlePacketChatMessage(cByteBuffer & a_ByteBuffer)
{
	ContiguousByteBuffer sigdata;

	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Message);
	HANDLE_READ(a_ByteBuffer, ReadBEInt64,       Int64,   timestamp);
	HANDLE_READ(a_ByteBuffer, ReadBEInt64,       Int64,  sig_salt);
	HANDLE_READ(a_ByteBuffer, ReadVarInt32,      UInt32, sig_data_len);
	if (!a_ByteBuffer.ReadSome(sigdata, sig_data_len))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadBool, bool, previwed);

	m_Client->HandleChat(Message);
}





void cProtocol_1_19::HandlePacketCommandExecution(cByteBuffer & a_ByteBuffer)
{
	ContiguousByteBuffer sigdata;

	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Command);
	HANDLE_READ(a_ByteBuffer, ReadBEInt64,       Int64,   timestamp);


	HANDLE_READ(a_ByteBuffer, ReadBEInt64,       Int64,  sig_salt);
	HANDLE_READ(a_ByteBuffer, ReadVarInt32,      UInt32, arg_count);
	for (size_t i = 0; i < arg_count; i++)
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Argument);
		HANDLE_READ(a_ByteBuffer, ReadVarInt32,      UInt32, Arg_sig_len);
		ContiguousByteBuffer bfr;
		if (!a_ByteBuffer.ReadSome(bfr, Arg_sig_len))
		{
			return;
		}
	}

	HANDLE_READ(a_ByteBuffer, ReadBool, bool, previwed);

	m_Client->HandleChat("/" + Command);
}





void cProtocol_1_19::HandlePacketBlockDig(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Status);

	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadXZYPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Face);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Sequence);
	m_Client->HandleLeftClick({BlockX, BlockY, BlockZ}, FaceIntToBlockFace(Face), Status);
	m_Client->GetPlayer()->GetWorld()->FlushPendingBlockChanges();
	m_Client->SendAcknowledgeBlockChange(Sequence);
}





void cProtocol_1_19::HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer)
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
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Sequence);
	m_Client->HandleRightClick({BlockX, BlockY, BlockZ}, FaceIntToBlockFace(Face), {FloorC(CursorX * 16), FloorC(CursorY * 16), FloorC(CursorZ * 16)}, Hand == 0);
	m_Client->GetPlayer()->GetWorld()->FlushPendingBlockChanges();
	m_Client->SendAcknowledgeBlockChange(Sequence);
}





void cProtocol_1_19::HandlePacketUseItem(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Hand);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Sequence);
	m_Client->HandleUseItem(Hand == MAIN_HAND);
	m_Client->SendAcknowledgeBlockChange(Sequence);
}





void cProtocol_1_19::HandlePacketSetBeaconEffect(cByteBuffer & a_ByteBuffer)
{
	UInt32 ToSetEff1 = 0, ToSetEff2 = 0;
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, HasEffect1);
	if (HasEffect1)
	{
		HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, Effect1);
		ToSetEff1 = Effect1;
	}
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, HasEffect2);
	if (HasEffect2)
	{
		HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, Effect2);
		ToSetEff2 = Effect2;
	}
	m_Client->HandleBeaconSelection(ToSetEff1, ToSetEff2);
}





UInt32 cProtocol_1_19::GetProtocolMobType(eMonsterType a_MobType) const
{
	switch (a_MobType)
	{
		// Map invalid type to Giant for easy debugging (if this ever spawns, something has gone very wrong)
		case mtInvalidType:           return 31;
		case mtAllay: return 0;
		case mtAxolotl: return 4;
		case mtBat: return 5;
		case mtBee: return 6;
		case mtBlaze: return 7;
		case mtCat: return 10;
		case mtCaveSpider: return 11;
		case mtChicken: return 12;
		case mtCod: return 13;
		case mtCow: return 14;
		case mtCreeper: return 15;
		case mtDolphin: return 16;
		case mtDonkey: return 17;
		case mtDrowned: return 19;
		case mtElderGuardian: return 20;
		case mtEnderDragon: return 22;
		case mtEnderman: return 23;
		case mtEndermite: return 24;
		case mtEvoker: return 25;
		case mtFox: return 31;
		case mtFrog: return 32;
		case mtGhast: return 33;
		case mtGiant: return 34;
		case mtGlowSquid: return 36;
		case mtGoat: return 37;
		case mtGuardian: return 38;
		case mtHoglin: return 39;
		case mtHorse: return 40;
		case mtHusk: return 41;
		case mtIllusioner: return 42;
		case mtLlama: return 49;
		case mtMagmaCube: return 51;
		case mtMule: return 60;
		case mtMooshroom: return 61;
		case mtOcelot: return 62;
		case mtPanda: return 64;
		case mtParrot: return 65;
		case mtPhantom: return 66;
		case mtPig: return 67;
		case mtPiglin: return 68;
		case mtPiglinBrute: return 69;
		case mtPillager: return 70;
		case mtPolarBear: return 71;
		case mtPufferfish: return 73;
		case mtRabbit: return 74;
		case mtRavager: return 75;
		case mtSalmon: return 76;
		case mtSheep: return 77;
		case mtShulker: return 78;
		case mtSilverfish: return 80;
		case mtSkeleton: return 81;
		case mtSkeletonHorse: return 82;
		case mtSlime: return 83;
		case mtSpider: return 88;
		case mtSquid: return 89;
		case mtStray: return 90;
		case mtStrider: return 91;
		case mtTadpole: return 92;
		case mtTraderLlama: return 98;
		case mtTropicalFish: return 99;
		case mtTurtle: return 100;
		case mtVex: return 101;
		case mtVillager: return 102;
		case mtVindicator: return 103;
		case mtWanderingTrader: return 104;
		case mtWarden: return 105;
		case mtWitch: return 106;
		case mtWither: return 107;
		case mtWitherSkeleton: return 108;
		case mtWolf: return 110;
		case mtZoglin: return 111;
		case mtZombie: return 112;
		case mtZombieHorse: return 113;
		case mtZombieVillager: return 114;
		case mtZombifiedPiglin: return 115;

		default:                      return 0;
	}
}





int cProtocol_1_19::GetProtocolParticleID(const AString & a_ParticleName) const
{
	static const std::unordered_map<AString, int> ParticleMap
	{
		// Initialize the ParticleMap:
		{ "ambient_entity_effect",    0 },
		{ "angry_villager",           1 },
		{ "ash",                      71 },
		{ "block",                    2 },
		{ "block_marker",             3 },
		{ "bubble",                   4 },
		{ "bubble_column_up",         61 },
		{ "bubble_pop",               59 },
		{ "campfire_cosy_smoke",      64 },
		{ "campfire_signal_smoke",    65 },
		{ "cloud",                    5 },
		{ "composter",                36 },
		{ "crimson_spore",            72 },
		{ "crit",                     6 },
		{ "current_down",             60 },
		{ "damage_indicator",         7 },
		{ "dolphin",                  63 },
		{ "dragon_breath",            8 },
		{ "dripping_dripstone_lava",  82 },
		{ "dripping_dripstone_water", 84 },
		{ "dripping_honey",           66 },
		{ "dripping_lava",            9 },
		{ "dripping_obsidian_tear",   75 },
		{ "dripping_water",           12 },
		{ "dust",                     14 },
		{ "dust_color_transition",    15 },
		{ "effect",                   16 },
		{ "elder_guardian",           17 },
		{ "electric_spark",           90 },
		{ "enchant",                  19 },
		{ "enchanted_hit",            18 },
		{ "end_rod",                  20 },
		{ "entity_effect",            21 },
		{ "explosion",                23 },
		{ "explosion_emitter",        22 },
		{ "falling_dripstone_lava",   83 },
		{ "falling_dripstone_water",  85 },
		{ "falling_dust",             25 },
		{ "falling_honey",            67 },
		{ "falling_lava",             10 },
		{ "falling_nectar",           69 },
		{ "falling_obsidian_tear",    76 },
		{ "falling_spore_blossom",    70 },
		{ "falling_water",            13 },
		{ "firework",                 26 },
		{ "fishing",                  27 },
		{ "flame",                    28 },
		{ "flash",                    34 },
		{ "glow",                     87 },
		{ "glow_squid_ink",           86 },
		{ "happy_villager",           35 },
		{ "heart",                    37 },
		{ "instant_effect",           38 },
		{ "item",                     39 },
		{ "item_slime",               41 },
		{ "item_snowball",            42 },
		{ "landing_honey",            68 },
		{ "landing_lava",             11 },
		{ "landing_obsidian_tear",    77 },
		{ "large_smoke",              43 },
		{ "lava",                     44 },
		{ "mycelium",                 45 },
		{ "nautilus",                 62 },
		{ "note",                     46 },
		{ "poof",                     47 },
		{ "portal",                   48 },
		{ "rain",                     49 },
		{ "reverse_portal",           78 },
		{ "scrape",                   91 },
		{ "sculk_charge",             30 },
		{ "sculk_charge_pop",         31 },
		{ "sculk_soul",               29 },
		{ "shriek",                   92 },
		{ "small_flame",              80 },
		{ "smoke",                    50 },
		{ "sneeze",                   51 },
		{ "snowflake",                81 },
		{ "sonic_boom",               24 },
		{ "soul",                     33 },
		{ "soul_fire_flame",          32 },
		{ "spit",                     52 },
		{ "splash",                   57 },
		{ "spore_blossom_air",        74 },
		{ "squid_ink",                53 },
		{ "sweep_attack",             54 },
		{ "totem_of_undying",         55 },
		{ "underwater",               56 },
		{ "vibration",                40 },
		{ "warped_spore",             73 },
		{ "wax_off",                  89 },
		{ "wax_on",                   88 },
		{ "white_ash",                79 },
		{ "witch",                    58 },
	};


	const auto ParticleName = StrToLower(a_ParticleName);
	const auto FindResult = ParticleMap.find(ParticleName);
	if (FindResult == ParticleMap.end())
	{
		LOGWARNING("Unknown particle: %s", a_ParticleName.c_str());
		// ASSERT(!"Unknown particle");  // some particles where renamed so they throw this error
		return 0;
	}

	return FindResult->second;
}





UInt8 cProtocol_1_19::GetProtocolEntityType(const cEntity & a_Entity) const
{
	using Type = cEntity::eEntityType;

	switch (a_Entity.GetEntityType())
	{
		case Type::etEnderCrystal: return 21;
		case Type::etPickup: return 44;
		case Type::etFallingBlock: return 29;
		case Type::etMinecart: return 53;
		case Type::etBoat: return 8;
		case Type::etTNT: return 72;
		case Type::etProjectile:
		{
			using PType = cProjectileEntity::eKind;
			const auto & Projectile = static_cast<const cProjectileEntity &>(a_Entity);

			switch (Projectile.GetProjectileKind())
			{
				case PType::pkArrow: return 3;
				case PType::pkSnowball: return 86;
				case PType::pkEgg: return 93;
				case PType::pkGhastFireball: return 46;
				case PType::pkFireCharge: return 84;
				case PType::pkEnderPearl: return 94;
				case PType::pkExpBottle: return 95;
				case PType::pkSplashPotion: return 96;
				case PType::pkFirework: return 30;
				case PType::pkWitherSkull: return 109;
			}
			break;
		}
		case Type::etFloater: return 117;
		case Type::etItemFrame: return 45;
		case Type::etLeashKnot: return 47;

		// Non-objects must not be sent
		case Type::etEntity:
		case Type::etPlayer:
		case Type::etMonster:
		case Type::etExpOrb:
		case Type::etPainting: break;
	}
	UNREACHABLE("Unhandled entity kind");
}





void cProtocol_1_19::SendSoundEffect(const AString & a_SoundName, Vector3d a_Origin, float a_Volume, float a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSoundEffect);
	Pkt.WriteString(a_SoundName);
	Pkt.WriteVarInt32(0);  // Master sound category (may want to be changed to a parameter later)
	Pkt.WriteBEInt32(static_cast<Int32>(a_Origin.x * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Origin.y * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Origin.z * 8.0));
	Pkt.WriteBEFloat(a_Volume);
	Pkt.WriteBEFloat(a_Pitch);
	Pkt.WriteBEInt64(0);
}





void cProtocol_1_19::HandlePacketWindowClick(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  WindowID);
	HANDLE_READ(a_ByteBuffer, ReadVarInt32,  UInt32,  StateID);
	HANDLE_READ(a_ByteBuffer, ReadBEInt16,  Int16,  SlotNum);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  Button);
	HANDLE_READ(a_ByteBuffer, ReadVarInt32,  UInt32,  Mode);
	HANDLE_READ(a_ByteBuffer, ReadVarInt32,  UInt32,  ArrLen);
	std::vector<std::pair<Int16, cItem>> items;
	for (UInt32 i = 0; i < ArrLen; ++i)
	{
		cItem Item;
		HANDLE_READ(a_ByteBuffer, ReadBEInt16,  Int16,  CurrSlotNum);
		ReadItem(a_ByteBuffer, Item, 1000000);  // HACK to make it work
		items.emplace_back(CurrSlotNum, Item);
	}
	cItem Item;
	ReadItem(a_ByteBuffer, Item, 0);

	/** The slot number that the client uses to indicate "outside the window". */
	static const Int16 SLOT_NUM_OUTSIDE = -999;

	// Convert Button, Mode, SlotNum and HeldItem into eClickAction:
	eClickAction Action;
	switch ((Mode << 8) | Button)
	{
		case 0x0000: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caLeftClick  : caLeftClickOutside;  break;
		case 0x0001: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caRightClick : caRightClickOutside; break;
		case 0x0100: Action = caShiftLeftClick;  break;
		case 0x0101: Action = caShiftRightClick; break;
		case 0x0200: Action = caNumber1;         break;
		case 0x0201: Action = caNumber2;         break;
		case 0x0202: Action = caNumber3;         break;
		case 0x0203: Action = caNumber4;         break;
		case 0x0204: Action = caNumber5;         break;
		case 0x0205: Action = caNumber6;         break;
		case 0x0206: Action = caNumber7;         break;
		case 0x0207: Action = caNumber8;         break;
		case 0x0208: Action = caNumber9;         break;
		case 0x0302: Action = caMiddleClick;     break;
		case 0x0400: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caLeftClickOutsideHoldNothing  : caDropKey;     break;
		case 0x0401: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caRightClickOutsideHoldNothing : caCtrlDropKey; break;
		case 0x0500: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caLeftPaintBegin               : caUnknown;     break;
		case 0x0501: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caLeftPaintProgress            : caUnknown;     break;
		case 0x0502: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caLeftPaintEnd                 : caUnknown;     break;
		case 0x0504: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caRightPaintBegin              : caUnknown;     break;
		case 0x0505: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caRightPaintProgress           : caUnknown;     break;
		case 0x0506: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caRightPaintEnd                : caUnknown;     break;
		case 0x0508: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caMiddlePaintBegin             : caUnknown;     break;
		case 0x0509: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caMiddlePaintProgress          : caUnknown;     break;
		case 0x050a: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caMiddlePaintEnd               : caUnknown;     break;
		case 0x0600: Action = caDblClick; break;
		default:
		{
			LOGWARNING("Unhandled window click mode / button combination: %d (0x%x)", (Mode << 8) | Button, (Mode << 8) | Button);
			Action = caUnknown;
			break;
		}
	}

	m_Client->HandleWindowClick(WindowID, SlotNum, Action, Item);
}





Int32 cProtocol_1_19::GetProtocolCommandArgumentID(eCommandParserType a_ParserType) const
{
	switch (a_ParserType)
	{
		case eCommandParserType::Bool:              return 0;
		case eCommandParserType::Float:             return 1;
		case eCommandParserType::Double:            return 2;
		case eCommandParserType::Integer:           return 3;
		case eCommandParserType::Long:              return 4;
		case eCommandParserType::String:            return 5;
		case eCommandParserType::Entity:            return 6;
		case eCommandParserType::GameProfile:       return 7;
		case eCommandParserType::BlockPos:          return 8;
		case eCommandParserType::ColumnPos:         return 9;
		case eCommandParserType::Vec3:              return 10;
		case eCommandParserType::Vec2:              return 11;
		case eCommandParserType::BlockState:        return 12;
		case eCommandParserType::BlockPredicate:    return 13;
		case eCommandParserType::ItemStack:         return 14;
		case eCommandParserType::ItemPredicate:     return 15;
		case eCommandParserType::Color:             return 16;
		case eCommandParserType::Component:         return 17;
		case eCommandParserType::Message:           return 18;
		case eCommandParserType::NbtCompoundTag:    return 19;
		case eCommandParserType::NbtTag:            return 20;
		case eCommandParserType::NbtPath:           return 21;
		case eCommandParserType::Objective:         return 22;
		case eCommandParserType::ObjectiveCriteria: return 23;
		case eCommandParserType::Operation:         return 24;
		case eCommandParserType::Particle:          return 25;
		case eCommandParserType::Angle:             return 26;
		case eCommandParserType::Rotation:          return 27;
		case eCommandParserType::ScoreboardSlot:    return 28;
		case eCommandParserType::ScoreHolder:       return 29;
		case eCommandParserType::Swizzle:           return 30;
		case eCommandParserType::Team:              return 31;
		case eCommandParserType::ItemSlot:          return 32;
		case eCommandParserType::ResourceLocation:  return 33;
		case eCommandParserType::Function:          return 35;
		case eCommandParserType::EntityAnchor:      return 36;
		case eCommandParserType::IntRange:          return 37;
		case eCommandParserType::FloatRange:        return 38;
		case eCommandParserType::Dimension:         return 41;
		// case eCommandParserType::Time:              return 42;
		case eCommandParserType::ResourceOrTag:     return 43;
		case eCommandParserType::Resource:          return 44;
		case eCommandParserType::TemplateMirror:    return 45;
		case eCommandParserType::TemplateRotation:  return 46;
		case eCommandParserType::Uuid:              return 47;
		default: return -1;
	}
}
////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_19_1:

cProtocol::Version cProtocol_1_19_1::GetProtocolVersion() const
{
	return Version::v1_19_1;
}





void cProtocol_1_19_1::HandlePacketLoginStart(cByteBuffer & a_ByteBuffer)
{
	AString Username;
	if (!a_ByteBuffer.ReadVarUTF8String(Username))
	{
		m_Client->Kick("Bad username");
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadBool, bool, HasPublicKey);
	if (HasPublicKey)
	{
		ContiguousByteBuffer PublicKey;
		ContiguousByteBuffer KeySignature;
		HANDLE_READ(a_ByteBuffer, ReadBEInt64, Int64, ExpiresAtEpochMilis);
		HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, PubKeyLen);
		if (!a_ByteBuffer.ReadSome(PublicKey, PubKeyLen))
		{
			return;
		}
		HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, KeySignatureLen);
		if (!a_ByteBuffer.ReadSome(KeySignature, KeySignatureLen))
		{
			return;
		}

		auto time = std::chrono::system_clock::now();  // get the current time
		auto since_epoch = time.time_since_epoch();  // get the duration since epoch
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch).count();

		if (millis > ExpiresAtEpochMilis)
		{
			LOGWARN("Expired profile public key");
		}

		// TODO: validate key

		// UNREACHABLE("not implemented");
	}
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, HasUUID);
	if (HasUUID)
	{
		HANDLE_READ(a_ByteBuffer, ReadUUID, cUUID, ProfileID);
	}
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





UInt32 cProtocol_1_19_1::GetPacketID(ePacketType a_PacketType) const
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
		case cProtocol::pktSpawnMob:             return 0x00;
		case cProtocol::pktSpawnPainting:        return 0x00;
		case cProtocol::pktSpawnExperienceOrb:   return 0x01;
		case cProtocol::pktSpawnOtherPlayer:     return 0x02;
		case cProtocol::pktEntityAnimation:      return 0x03;
		case cProtocol::pktStatistics:           return 0x04;
		case cProtocol::pktPlayerActionResponse: return 0x05;
		//  case cProtocol::pktBlockbreakingprogress:   return 0x06;
		case cProtocol::pktUpdateBlockEntity:    return 0x07;
		case cProtocol::pktBlockAction:          return 0x08;
		case cProtocol::pktBlockChange:          return 0x09;
		case cProtocol::pktBossBar:              return 0x0A;
		case cProtocol::pktDifficulty:           return 0x0B;
			//  chat preview 0x0C
			//  clear title 0x0D
			//  command suggestions here 0x0E
		case cProtocol::pktCommnadTree:          return 0x0F;
		case cProtocol::pktWindowClose:          return 0x10;
		case cProtocol::pktWindowItems:          return 0x11;  //  Inventory packet
		case cProtocol::pktWindowProperty:       return 0x12;  //  ScreenHandlerPropertyUpdateS2CPacket
		case cProtocol::pktInventorySlot:        return 0x13;  //  ScreenHandlerSlotUpdateS2CPacket
			//  0x14 cooldown update
			//  chat suggestions 0x15
		case cProtocol::pktCustomPayload:        return 0x16;
		case cProtocol::pktPluginMessage:        return 0x16;
		case cProtocol::pktSoundEffect:          return 0x17;
			// HideMessageS2CPacket 0x18
		case cProtocol::pktDisconnectDuringGame: return 0x19;
		case cProtocol::pktEntityStatus:         return 0x1A;
		case cProtocol::pktExplosion:            return 0x1B;
		case cProtocol::pktUnloadChunk:          return 0x1C;
		case cProtocol::pktGameMode:             return 0x1D;
		case cProtocol::pktWeather:              return 0x1D;
		case cProtocol::pktHorseWindowOpen:      return 0x1E;
			// wolrld border initalize 0x1F
		case cProtocol::pktKeepAlive:            return 0x20;
			// chunk data packet 0x21
		case cProtocol::pktSoundParticleEffect:  return 0x22;  // world event
		case cProtocol::pktParticleEffect:       return 0x23;
		case cProtocol::pktLightUpdate:          return 0x24;
		case cProtocol::pktJoinGame:             return 0x25;
		case cProtocol::pktMapData:              return 0x26;
			//  set trade offers 0x27
		case cProtocol::pktEntityRelMove:        return 0x28;
		case cProtocol::pktEntityRelMoveLook:    return 0x29;
		case cProtocol::pktEntityLook:           return 0x2A;
			//  vehicle move 0x2B
			//  open written book 0x2C
		case cProtocol::pktWindowOpen:           return 0x2D;
		case cProtocol::pktUpdateSign:           return 0x2E;
			//  play ping 0x2F
			//  craft failed response 0x30
		case cProtocol::pktPlayerAbilities:      return 0x31;
			//  MessageHeaderS2CPacket 0x32
			//  ChatMessageS2CPacket 0x33
			//  combat exit 0x34
			//  comabt enter 0x35
			//  death msg 0x36
		case cProtocol::pktPlayerList:           return 0x37;
			//  look at 0x38
		case cProtocol::pktPlayerMoveLook:       return 0x39;
		case cProtocol::pktUnlockRecipe:         return 0x3A;
		case cProtocol::pktDestroyEntity:        return 0x3B;
		case cProtocol::pktRemoveEntityEffect:   return 0x3C;
		case cProtocol::pktResourcePack:         return 0x3D;
		case cProtocol::pktRespawn:              return 0x3E;
		case cProtocol::pktEntityHeadLook:       return 0x3F;
		case cProtocol::pktBlockChanges:         return 0x40;
			// select advancment tab 0x41
			// ServerMetadataS2CPacket 0x42
			// overlay msg 0x43
			// wb -- worldborder wb center changed 0x44
			// wb interpolate size 0x45
			// wb size changed 0x46
			// wb warning time changed 0x47
			// wb warning blocks changed 0x48
		case cProtocol::pktCameraSetTo:          return 0x49;
		case cProtocol::pktHeldItemChange:       return 0x4A;
		case cProtocol::pktRenderDistanceCenter: return 0x4B;
			//  chunk load distance 0x4C
		case cProtocol::pktSpawnPosition:        return 0x4D;
			//  ChatPreviewStateChangeS2CPacket 0x4E
			//  scoreboard display 0x4F
		case cProtocol::pktEntityMeta:           return 0x50;
		case cProtocol::pktLeashEntity:          return 0x51;
		case cProtocol::pktEntityVelocity:       return 0x52;
		case cProtocol::pktEntityEquipment:      return 0x53;
		case cProtocol::pktExperience:           return 0x54;
		case cProtocol::pktUpdateHealth:         return 0x55;
		case cProtocol::pktScoreboardObjective:  return 0x56;
		case cProtocol::pktAttachEntity:         return 0x57;
			// Teams 0x58
		case cProtocol::pktUpdateScore:          return 0x59;
			// simulation distance 0x5A
			// subtitle 0x5B
		case cProtocol::pktTimeUpdate:           return 0x5C;
		case cProtocol::pktTitle:                return 0x5D;
			//  title fade 0x5E
			//  play sound from entity 0x5F
			//  play sound 0x60
			//  stop sound 0x61
		case cProtocol::pktChatRaw:              return 0x62;  //  Gamemessage
			//  player list header 0x63
			//  NbtQueryResponseS2CPacket 0x64
		case cProtocol::pktCollectEntity:        return 0x65;
		case cProtocol::pktTeleportEntity:       return 0x66;
			//  advancment update 0x67
		case cProtocol::pktEntityProperties:     return 0x68;
		case cProtocol::pktEntityEffect:         return 0x69;
			//  sync recepies 0x6A
			//  sync tags 0x6B
		default: UNREACHABLE("unhandeled packet");
	}
}





void cProtocol_1_19_1::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
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
					Writer.AddInt("id", 0);
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

		Pkt.WriteString("minecraft:overworld");  // dimension type
		Pkt.WriteString("minecraft:overworld");  // dimension id

		Pkt.WriteBEInt64(0);  // Seed
		Pkt.WriteVarInt32(static_cast<UInt32>(Server->GetMaxPlayers()));
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));  // simulation distance
		Pkt.WriteBool(false);  // Reduced debug info
		Pkt.WriteBool(true);   // Show deaths screen
		Pkt.WriteBool(false);  // Debug world
		Pkt.WriteBool(false);  // Flat World
		Pkt.WriteBool(false);  // optional last death loc
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





bool cProtocol_1_19_1::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	if (m_State != State::Game)
	{
		return Super::HandlePacket(a_ByteBuffer, a_PacketType);
	}

	// Game
	switch (a_PacketType)
	{
		case 0x00: HandleConfirmTeleport(a_ByteBuffer); return true;
		case 0x01: /* query nbt packet */ return false;
		case 0x02: /* update difficulty */ return false;
		case 0x03: /* MessageAcknowledgmentC2SPacket */ return false;
		case 0x04: HandlePacketCommandExecution(a_ByteBuffer); return true;
		case 0x05: HandlePacketChatMessage(a_ByteBuffer); return true;
		case 0x06: /* RequestChatPreviewC2SPacket */ return false;
		case 0x07: HandlePacketClientStatus(a_ByteBuffer); return true;
		case 0x08: HandlePacketClientSettings(a_ByteBuffer); return true;
		case 0x09: HandlePacketTabComplete(a_ByteBuffer); return true;
		case 0x0A: /* ButtonClickC2SPacket */ return false;
		case 0x0B: HandlePacketWindowClick(a_ByteBuffer); return true;
		case 0x0C: HandlePacketWindowClose(a_ByteBuffer); return true;
		case 0x0D: HandlePacketPluginMessage(a_ByteBuffer); return true;
		case 0x0E: HandlePacketBookUpdate(a_ByteBuffer); return true;  // not fully implemented
		case 0x0F: /* QueryEntityNbtC2SPacket */ return false;
		case 0x10: HandlePacketUseEntity(a_ByteBuffer); return true;
		case 0x11: /* Jigsaw generating */ return false;
		case 0x12: HandlePacketKeepAlive(a_ByteBuffer); return true;
		case 0x13: /* Update difficulty lock */ return false;  // only used in single player
		case 0x14: HandlePacketPlayerPos(a_ByteBuffer); return true;  // PositionAndOnGround
		case 0x15: HandlePacketPlayerPosLook(a_ByteBuffer); return true;  // full
		case 0x16: HandlePacketPlayerLook(a_ByteBuffer); return true;  // LookAndOnGround
		case 0x17: HandlePacketPlayer(a_ByteBuffer); return true;
		case 0x18: HandlePacketVehicleMove(a_ByteBuffer); return true;
		case 0x19: HandlePacketBoatSteer(a_ByteBuffer); return true;
		case 0x1A: /* pick from inventory */ return false;
		case 0x1B: HandleCraftRecipe(a_ByteBuffer); return true;
		case 0x1C: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
		case 0x1D: HandlePacketBlockDig(a_ByteBuffer); return true;
		case 0x1E: /* client command packet */ return false;
		case 0x1F: HandlePacketSteerVehicle(a_ByteBuffer); return true;  // player input packet
		case 0x20: /* PlayPongC2SPacket */ return false;
		case 0x21: /* Recipe Category Options */ return false;
		case 0x22: HandlePacketCraftingBookData(a_ByteBuffer); return true;
		case 0x23: HandlePacketNameItem(a_ByteBuffer); return true;
		case 0x24: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
		case 0x25: HandlePacketAdvancementTab(a_ByteBuffer); return true;
		case 0x26: /* select villager trade */ return false;
		case 0x27: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
		case 0x28: HandlePacketSlotSelect(a_ByteBuffer); return true;
		case 0x29: HandlePacketCommandBlockUpdate(a_ByteBuffer); return true;
		case 0x2A: /* update minecart command block */ return false;
		case 0x2B: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
		case 0x2C: /* Update jigsaw block */ return false;
		case 0x2D: /* Update structure block */ return false;
		case 0x2E: HandlePacketUpdateSign(a_ByteBuffer); return true;
		case 0x2F: /* Update hand swing */ return false;
		case 0x30: /* Spectator teleport */ return false;
		case 0x31: HandlePacketBlockPlace(a_ByteBuffer); return true;
		case 0x32: HandlePacketUseItem(a_ByteBuffer); return true;
		default: break;
	}
	UNREACHABLE("");
}





void cProtocol_1_19_1::HandlePacketCommandExecution(cByteBuffer & a_ByteBuffer)
{
	ContiguousByteBuffer sigdata;

	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Command);
	HANDLE_READ(a_ByteBuffer, ReadBEInt64,       Int64,   timestamp);


	HANDLE_READ(a_ByteBuffer, ReadBEInt64,       Int64,  sig_salt);
	HANDLE_READ(a_ByteBuffer, ReadVarInt32,      UInt32, arg_count);
	for (size_t i = 0; i < arg_count; i++)
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Argument);
		HANDLE_READ(a_ByteBuffer, ReadVarInt32,      UInt32, Arg_sig_len);
		ContiguousByteBuffer bfr;
		if (!a_ByteBuffer.ReadSome(bfr, Arg_sig_len))
		{
			return;
		}
	}

	HANDLE_READ(a_ByteBuffer, ReadBool, bool, previwed);

	HANDLE_READ(a_ByteBuffer, ReadVarInt32,      UInt32, last_seen_sg_count);
	for (size_t i = 0; i < last_seen_sg_count; i++)
	{
		HANDLE_READ(a_ByteBuffer, ReadUUID, cUUID, profileid);
		HANDLE_READ(a_ByteBuffer, ReadVarInt32,      UInt32, sig_data_count);
		ContiguousByteBuffer sig_data;
		if (!a_ByteBuffer.ReadSome(sig_data, sig_data_count))
		{
			return;
		}
	}
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, HasLastRecieved);
	if (HasLastRecieved)
	{
		HANDLE_READ(a_ByteBuffer, ReadUUID, cUUID, profileid);
		HANDLE_READ(a_ByteBuffer, ReadVarInt32,      UInt32, sig_data_count);
		ContiguousByteBuffer sig_data;
		if (!a_ByteBuffer.ReadSome(sig_data, sig_data_count))
		{
			return;
		}
	}


	m_Client->HandleChat("/" + Command);
}




////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_19_3:

cProtocol::Version cProtocol_1_19_3::GetProtocolVersion() const
{
	return Version::v1_19_3;
}





UInt32 cProtocol_1_19_3::GetPacketID(ePacketType a_PacketType) const
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
		case cProtocol::pktSpawnMob:             return 0x00;
		case cProtocol::pktSpawnPainting:        return 0x00;
		case cProtocol::pktSpawnExperienceOrb:   return 0x01;
		case cProtocol::pktSpawnOtherPlayer:     return 0x02;
		case cProtocol::pktEntityAnimation:      return 0x03;
		case cProtocol::pktStatistics:           return 0x04;
		case cProtocol::pktPlayerActionResponse: return 0x05;
		//  case cProtocol::pktBlockbreakingprogress:   return 0x06;
		case cProtocol::pktUpdateBlockEntity:    return 0x07;
		case cProtocol::pktBlockAction:          return 0x08;
		case cProtocol::pktBlockChange:          return 0x09;
		case cProtocol::pktBossBar:              return 0x0A;
		case cProtocol::pktDifficulty:           return 0x0B;
			//  clear title 0x0C
			//  command suggestions here 0x0D
		case cProtocol::pktCommnadTree:          return 0x0E;
		case cProtocol::pktWindowClose:          return 0x0F;
		case cProtocol::pktWindowItems:          return 0x10;  //  Inventory packet
		case cProtocol::pktWindowProperty:       return 0x11;  //  ScreenHandlerPropertyUpdateS2CPacket
		case cProtocol::pktInventorySlot:        return 0x12;  //  ScreenHandlerSlotUpdateS2CPacket
			//  cooldown update 0x13
			//  chat suggestions 0x14
		case cProtocol::pktCustomPayload:        return 0x15;
		case cProtocol::pktPluginMessage:        return 0x15;
			// RemoveMessageS2CPacket 0x16
		case cProtocol::pktDisconnectDuringGame: return 0x17;
			//  ProfilelessChatMessageS2CPacket 0x18
		case cProtocol::pktEntityStatus:         return 0x19;
		case cProtocol::pktExplosion:            return 0x1A;
		case cProtocol::pktUnloadChunk:          return 0x1B;
		case cProtocol::pktGameMode:             return 0x1C;
		case cProtocol::pktWeather:              return 0x1C;
		case cProtocol::pktHorseWindowOpen:      return 0x1D;
			// wolrld border initalize 0x1E
		case cProtocol::pktKeepAlive:            return 0x1F;
			// chunk data packet 0x20
		case cProtocol::pktSoundParticleEffect:  return 0x21;  // world event
		case cProtocol::pktParticleEffect:       return 0x22;
		case cProtocol::pktLightUpdate:          return 0x23;
		case cProtocol::pktJoinGame:             return 0x24;
		case cProtocol::pktMapData:              return 0x25;
			//  set trade offers 0x26
		case cProtocol::pktEntityRelMove:        return 0x27;
		case cProtocol::pktEntityRelMoveLook:    return 0x28;
		case cProtocol::pktEntityLook:           return 0x29;
			//  vehicle move 0x2A
			//  open written book 0x2B
		case cProtocol::pktWindowOpen:           return 0x2C;
		case cProtocol::pktUpdateSign:           return 0x2D;
			//  play ping 0x2E
			//  craft failed response 0x2F
		case cProtocol::pktPlayerAbilities:      return 0x30;
			//  ChatMessageS2CPacket 0x31
			//  combat exit 0x32
			//  comabt enter 0x33
			//  death msg 0x34
		case cProtocol::pktPlayerLstRemove:      return 0x35;
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
			// ServerMetadataS2CPacket 0x41
			// overlay msg 0x42
			// wb -- worldborder wb center changed 0x43
			// wb interpolate size 0x44
			// wb size changed 0x45
			// wb warning time changed 0x46
			// wb warning blocks changed 0x47
		case cProtocol::pktCameraSetTo:          return 0x48;
		case cProtocol::pktHeldItemChange:       return 0x49;
		case cProtocol::pktRenderDistanceCenter: return 0x4A;
			//  chunk load distance 0x4B
		case cProtocol::pktSpawnPosition:        return 0x4C;
			//  scoreboard display 0x4D
		case cProtocol::pktEntityMeta:           return 0x4E;
		case cProtocol::pktLeashEntity:          return 0x4F;
		case cProtocol::pktEntityVelocity:       return 0x50;
		case cProtocol::pktEntityEquipment:      return 0x51;
		case cProtocol::pktExperience:           return 0x52;
		case cProtocol::pktUpdateHealth:         return 0x53;
		case cProtocol::pktScoreboardObjective:  return 0x54;
		case cProtocol::pktAttachEntity:         return 0x55;
			// Teams 0x56
		case cProtocol::pktUpdateScore:          return 0x57;
			// simulation distance 0x58
			// subtitle 0x59
		case cProtocol::pktTimeUpdate:           return 0x5A;
		case cProtocol::pktTitle:                return 0x5B;
			//  title fade 0x5C
			//  play sound from entity 0x5D
		case cProtocol::pktSoundEffect:          return 0x5E;
			//  stop sound 0x5F
		case cProtocol::pktChatRaw:              return 0x60;  //  Gamemessage
			//  player list header 0x61
			//  NbtQueryResponseS2CPacket 0x62
		case cProtocol::pktCollectEntity:        return 0x63;
		case cProtocol::pktTeleportEntity:       return 0x64;
			//  advancment update 0x65
		case cProtocol::pktEntityProperties:     return 0x66;
			// FeaturesS2CPacket 0x67
		case cProtocol::pktEntityEffect:         return 0x68;
			//  sync recepies 0x69
			//  sync tags 0x6A
		default: UNREACHABLE("unhandeled packet");
	}
}





void cProtocol_1_19_3::HandlePacketLoginStart(cByteBuffer & a_ByteBuffer)
{
	AString Username;
	if (!a_ByteBuffer.ReadVarUTF8String(Username))
	{
		m_Client->Kick("Bad username");
		return;
	}
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, HasUUID);
	if (HasUUID)
	{
		HANDLE_READ(a_ByteBuffer, ReadUUID, cUUID, ProfileID);
	}
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





bool cProtocol_1_19_3::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	if (m_State != State::Game)
	{
		return Super::HandlePacket(a_ByteBuffer, a_PacketType);
	}

	// Game
	switch (a_PacketType)
	{
		case 0x00: HandleConfirmTeleport(a_ByteBuffer); return true;
		case 0x01: /* query nbt packet */ return false;
		case 0x02: /* update difficulty */ return false;
		case 0x03: /* MessageAcknowledgmentC2SPacket */ return false;
		case 0x04: HandlePacketCommandExecution(a_ByteBuffer); return true;
		case 0x05: HandlePacketChatMessage(a_ByteBuffer); return true;
		case 0x06: HandlePacketClientStatus(a_ByteBuffer); return true;
		case 0x07: HandlePacketClientSettings(a_ByteBuffer); return true;
		case 0x08: HandlePacketTabComplete(a_ByteBuffer); return true;
		case 0x09: /* ButtonClickC2SPacket */ return false;
		case 0x0A: HandlePacketWindowClick(a_ByteBuffer); return true;
		case 0x0B: HandlePacketWindowClose(a_ByteBuffer); return true;
		case 0x0C: HandlePacketPluginMessage(a_ByteBuffer); return true;
		case 0x0D: HandlePacketBookUpdate(a_ByteBuffer); return true;  // not fully implemented
		case 0x0E: /* QueryEntityNbtC2SPacket */ return false;
		case 0x0F: HandlePacketUseEntity(a_ByteBuffer); return true;
		case 0x10: /* Jigsaw generating */ return false;
		case 0x11: HandlePacketKeepAlive(a_ByteBuffer); return true;
		case 0x12: /* Update difficulty lock */ return false;  // only used in single player
		case 0x13: HandlePacketPlayerPos(a_ByteBuffer); return true;  // PositionAndOnGround
		case 0x14: HandlePacketPlayerPosLook(a_ByteBuffer); return true;  // full
		case 0x15: HandlePacketPlayerLook(a_ByteBuffer); return true;  // LookAndOnGround
		case 0x16: HandlePacketPlayer(a_ByteBuffer); return true;
		case 0x17: HandlePacketVehicleMove(a_ByteBuffer); return true;
		case 0x18: HandlePacketBoatSteer(a_ByteBuffer); return true;
		case 0x19: /* pick from inventory */ return false;
		case 0x1A: HandleCraftRecipe(a_ByteBuffer); return true;
		case 0x1B: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
		case 0x1C: HandlePacketBlockDig(a_ByteBuffer); return true;
		case 0x1D: /* client command packet */ return false;
		case 0x1E: HandlePacketSteerVehicle(a_ByteBuffer); return true;  // player input packet
		case 0x1F: /* PlayPongC2SPacket */ return false;
		case 0x20: /* PlayerSessionC2SPacket */ return false;
		case 0x21: /* Recipe Category Options */ return false;
		case 0x22: HandlePacketCraftingBookData(a_ByteBuffer); return true;
		case 0x23: HandlePacketNameItem(a_ByteBuffer); return true;
		case 0x24: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
		case 0x25: HandlePacketAdvancementTab(a_ByteBuffer); return true;
		case 0x26: /* select villager trade */ return false;
		case 0x27: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
		case 0x28: HandlePacketSlotSelect(a_ByteBuffer); return true;
		case 0x29: HandlePacketCommandBlockUpdate(a_ByteBuffer); return true;
		case 0x2A: /* update minecart command block */ return false;
		case 0x2B: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
		case 0x2C: /* Update jigsaw block */ return false;
		case 0x2D: /* Update structure block */ return false;
		case 0x2E: HandlePacketUpdateSign(a_ByteBuffer); return true;
		case 0x2F: /* Update hand swing */ return false;
		case 0x30: /* Spectator teleport */ return false;
		case 0x31: HandlePacketBlockPlace(a_ByteBuffer); return true;
		case 0x32: HandlePacketUseItem(a_ByteBuffer); return true;
		default: break;
	}
	UNREACHABLE("");
}





void cProtocol_1_19_3::SendChatRaw(const AString & a_MessageRaw, eChatType a_Type)
{
	ASSERT(m_State == 3);  // In game mode?

	// Prevent chat messages that might trigger CVE-2021-44228
	if (a_MessageRaw.find("${") != std::string::npos)
	{
		return;
	}

	// Send the json string to the client:
	cPacketizer Pkt(*this, pktChatRaw);
	Pkt.WriteString(a_MessageRaw);
	Pkt.WriteBool(false);
}





void cProtocol_1_19_3::SendSoundEffect(const AString & a_SoundName, Vector3d a_Origin, float a_Volume, float a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSoundEffect);
	Pkt.WriteVarInt32(GetProtocolSoundID(a_SoundName));  // sound id
	Pkt.WriteVarInt32(0);  // Master sound category (may want to be changed to a parameter later)
	Pkt.WriteBEInt32(static_cast<Int32>(a_Origin.x * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Origin.y * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Origin.z * 8.0));
	Pkt.WriteBEFloat(a_Volume);
	Pkt.WriteBEFloat(a_Pitch);
	Pkt.WriteBEInt64(0);  // seed
}





void cProtocol_1_19_3::SendPlayerListAddPlayer(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerList);
	Pkt.WriteBEInt8(static_cast<Int8>(PlayerListAction::AddPlayer) | static_cast<Int8>(PlayerListAction::UpdateGameMode) | static_cast<Int8>(PlayerListAction::UpdateListed) | static_cast<Int8>(PlayerListAction::UpdateLatency));

	Pkt.WriteVarInt32(1);
	Pkt.WriteUUID(a_Player.GetUUID());
	Pkt.WriteString(a_Player.GetPlayerListName());

	const Json::Value & Properties = a_Player.GetClientHandle()->GetProperties();
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
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetEffectiveGameMode()));
	Pkt.WriteBool(true);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetClientHandle()->GetPing()));
}





void cProtocol_1_19_3::SendPlayerListInitChat(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerList);
	Pkt.WriteBEInt8(static_cast<Int8>(PlayerListAction::InitializeChat));

	auto player_session_data = a_Player.GetClientHandle()->GetPlayerSessionData();

	Pkt.WriteVarInt32(1);
	Pkt.WriteUUID(a_Player.GetUUID());
	Pkt.WriteBool(player_session_data.IsPopulated());
	if (player_session_data.IsPopulated())
	{
		Pkt.WriteUUID(player_session_data.GetSessionUUID());
		Pkt.WriteBEInt64(player_session_data.GetExpiresAtEpochMiliscond());
		Pkt.WriteLengthPrefixedBuf(player_session_data.GetPublicKey());
		Pkt.WriteLengthPrefixedBuf(player_session_data.GetKeySignature());
	}
}





void cProtocol_1_19_3::SendPlayerListUpdatePing()
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerList);
	Pkt.WriteBEInt8(static_cast<UInt32>(PlayerListAction::UpdateLatency));

	const auto World = m_Client->GetPlayer()->GetWorld();
	Pkt.WriteVarInt32(static_cast<UInt32>(World->GetPlayerCount()));
	World->ForEachPlayer([&Pkt](cPlayer & a_Player)
	{
		Pkt.WriteUUID(a_Player.GetUUID());
		Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetClientHandle()->GetPing()));
		return false;
	});
}





void cProtocol_1_19_3::SendPlayerListUpdateGameMode(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerList);
	Pkt.WriteBEInt8(static_cast<Int8>(PlayerListAction::UpdateGameMode));
	Pkt.WriteVarInt32(1);
	Pkt.WriteUUID(a_Player.GetUUID());
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetEffectiveGameMode()));
}





void cProtocol_1_19_3::SendPlayerListRemovePlayer(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerLstRemove);
	Pkt.WriteVarInt32(1);
	Pkt.WriteUUID(a_Player.GetUUID());
}





void cProtocol_1_19_3::WriteEntityMetadata(cPacketizer & a_Pkt, const EntityMetadata a_Metadata, const EntityMetadataType a_FieldType) const
{
	a_Pkt.WriteBEUInt8(GetEntityMetadataID(a_Metadata));	      // Index
	a_Pkt.WriteVarInt32(static_cast<UInt32>(a_FieldType));        // Type
}





void cProtocol_1_19_3::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const
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
			WriteEntityMetadata(a_Pkt, EntityMetadata::EntityCustomName, EntityMetadataType::OptChat);  // no longer optional ????
			a_Pkt.WriteString(JsonUtils::SerializeSingleValueJsonObject("text", Player.GetName()));	 // needs to be json formatted

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
					a_Pkt.WriteVarInt32(GetProtocolItemType(MinecartContent.m_ItemType));

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
					break;
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





void cProtocol_1_19_3::HandlePacketCommandExecution(cByteBuffer & a_ByteBuffer)
{
	ContiguousByteBuffer sigdata;

	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Command);
	HANDLE_READ(a_ByteBuffer, ReadBEInt64,       Int64,   timestamp);


	HANDLE_READ(a_ByteBuffer, ReadBEInt64,       Int64,  sig_salt);
	HANDLE_READ(a_ByteBuffer, ReadVarInt32,      UInt32, arg_count);
	for (size_t i = 0; i < arg_count; i++)
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Argument);
		ContiguousByteBuffer bfr;
		if (!a_ByteBuffer.ReadSome(bfr, 256))
		{
			return;
		}
	}

	// Acknowledgment ???
	HANDLE_READ(a_ByteBuffer, ReadVarInt32,      UInt32, offset);

	ContiguousByteBuffer bitsetvalue;
	a_ByteBuffer.ReadSome(bitsetvalue, 3);  // temp fix

	m_Client->HandleChat("/" + Command);
}





void cProtocol_1_19_3::HandlePacketChatMessage(cByteBuffer & a_ByteBuffer)
{
	ContiguousByteBuffer sigdata;

	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Message);
	HANDLE_READ(a_ByteBuffer, ReadBEInt64,       Int64,   timestamp);
	HANDLE_READ(a_ByteBuffer, ReadBEInt64,       Int64,  sig_salt);

	HANDLE_READ(a_ByteBuffer, ReadBool,       bool,  HasMessageSigData);
	if (HasMessageSigData)
	{
		if (!a_ByteBuffer.ReadSome(sigdata, 256))
		{
			return;
		}
	}

	// Acknowledgment ???
	HANDLE_READ(a_ByteBuffer, ReadVarInt32,      UInt32, offset);

	ContiguousByteBuffer bitsetvalue;
	a_ByteBuffer.ReadSome(bitsetvalue, 3);  // temp fix

	m_Client->HandleChat(Message);
}





void cProtocol_1_19_3::HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer)
{
	UInt32 EncKeyLength, EncNonceLength;
	ContiguousByteBuffer EncKey;
	ContiguousByteBuffer EncNonce;

	ContiguousByteBuffer SignatureData;
	if (!a_ByteBuffer.ReadVarInt(EncKeyLength))
	{
		return;
	}

	if (!a_ByteBuffer.ReadSome(EncKey, EncKeyLength))
	{
		return;
	}

	if (!a_ByteBuffer.ReadVarInt(EncNonceLength))
	{
		return;
	}

	if (!a_ByteBuffer.ReadSome(EncNonce, EncNonceLength))
	{
		return;
	}

	if ((EncKeyLength > MAX_ENC_LEN) || (EncNonceLength > MAX_ENC_LEN))
	{
		LOGD("Too long encryption");
		m_Client->Kick("Hacked client");
		return;
	}

	// Decrypt EncNonce using privkey
	cRsaPrivateKey & rsaDecryptor = cRoot::Get()->GetServer()->GetPrivateKey();
	UInt32 DecryptedNonce[MAX_ENC_LEN / sizeof(Int32)];
	int res = rsaDecryptor.Decrypt(EncNonce, reinterpret_cast<Byte *>(DecryptedNonce), sizeof(DecryptedNonce));
	if (res != 4)
	{
		LOGD("Bad nonce length: got %d, exp %d", res, 4);
		m_Client->Kick("Hacked client");
		return;
	}
	if (ntohl(DecryptedNonce[0]) != static_cast<unsigned>(reinterpret_cast<uintptr_t>(this)))
	{
		LOGD("Bad nonce value");
		m_Client->Kick("Hacked client");
		return;
	}

	// Decrypt the symmetric encryption key using privkey:
	Byte DecryptedKey[MAX_ENC_LEN];
	res = rsaDecryptor.Decrypt(EncKey, DecryptedKey, sizeof(DecryptedKey));
	if (res != 16)
	{
		LOGD("Bad key length");
		m_Client->Kick("Hacked client");
		return;
	}

	StartEncryption(DecryptedKey);
	m_Client->HandleLogin();
}





////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_19_4:

cProtocol::Version cProtocol_1_19_4::GetProtocolVersion() const
{
	return Version::v1_19_4;
}





UInt32 cProtocol_1_19_4::GetPacketID(ePacketType a_PacketType) const
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
		case cProtocol::pktSpawnObject:          return 0x01;
		case cProtocol::pktSpawnMob:             return 0x01;
		case cProtocol::pktSpawnPainting:        return 0x01;
		case cProtocol::pktSpawnExperienceOrb:   return 0x02;
		case cProtocol::pktSpawnOtherPlayer:     return 0x03;
		case cProtocol::pktEntityAnimation:      return 0x04;
		case cProtocol::pktStatistics:           return 0x05;
		case cProtocol::pktPlayerActionResponse: return 0x06;
		//  case cProtocol::pktBlockbreakingprogress:   return 0x07;
		case cProtocol::pktUpdateBlockEntity:    return 0x08;
		case cProtocol::pktBlockAction:          return 0x09;
		case cProtocol::pktBlockChange:          return 0x0A;
		case cProtocol::pktBossBar:              return 0x0B;
		case cProtocol::pktDifficulty:           return 0x0C;
			//  ChunkBiomeDataS2CPacket 0x0D
			//  clear title 0x0E
			//  command suggestions here 0x0F
		case cProtocol::pktCommnadTree:          return 0x10;
		case cProtocol::pktWindowClose:          return 0x11;
		case cProtocol::pktWindowItems:          return 0x12;  //  Inventory packet
		case cProtocol::pktWindowProperty:       return 0x13;  //  ScreenHandlerPropertyUpdateS2CPacket
		case cProtocol::pktInventorySlot:        return 0x14;  //  ScreenHandlerSlotUpdateS2CPacket
			//  cooldown update 0x15
			//  chat suggestions 0x16
		case cProtocol::pktCustomPayload:        return 0x17;
		case cProtocol::pktPluginMessage:        return 0x17;
			// EntityDamageS2CPacket 0x18
			// RemoveMessageS2CPacket 0x19
		case cProtocol::pktDisconnectDuringGame: return 0x1A;
			//  ProfilelessChatMessageS2CPacket 0x1B
		case cProtocol::pktEntityStatus:         return 0x1C;
		case cProtocol::pktExplosion:            return 0x1D;
		case cProtocol::pktUnloadChunk:          return 0x1E;
		case cProtocol::pktGameMode:             return 0x1F;
		case cProtocol::pktWeather:              return 0x1F;
		case cProtocol::pktHorseWindowOpen:      return 0x20;
			// DamageTiltS2CPacket 0x21
			// wolrld border initalize 0x22
		case cProtocol::pktKeepAlive:            return 0x23;
			// chunk data packet 0x24
		case cProtocol::pktSoundParticleEffect:  return 0x25;  // world event
		case cProtocol::pktParticleEffect:       return 0x26;
		case cProtocol::pktLightUpdate:          return 0x27;
		case cProtocol::pktJoinGame:             return 0x28;
		case cProtocol::pktMapData:              return 0x29;
			//  set trade offers 0x2A
		case cProtocol::pktEntityRelMove:        return 0x2B;
		case cProtocol::pktEntityRelMoveLook:    return 0x2C;
		case cProtocol::pktEntityLook:           return 0x2D;
			//  vehicle move 0x2E
			//  open written book 0x2F
		case cProtocol::pktWindowOpen:           return 0x30;
		case cProtocol::pktUpdateSign:           return 0x31;
			//  play ping 0x32
			//  craft failed response 0x33
		case cProtocol::pktPlayerAbilities:      return 0x34;
			//  ChatMessageS2CPacket 0x35
			//  combat exit 0x36
			//  comabt enter 0x37
			//  death msg 0x38
		case cProtocol::pktPlayerLstRemove:      return 0x39;
		case cProtocol::pktPlayerList:           return 0x3A;
			//  look at 0x3B
		case cProtocol::pktPlayerMoveLook:       return 0x3C;
		case cProtocol::pktUnlockRecipe:         return 0x3D;
		case cProtocol::pktDestroyEntity:        return 0x3E;
		case cProtocol::pktRemoveEntityEffect:   return 0x3F;
		case cProtocol::pktResourcePack:         return 0x40;
		case cProtocol::pktRespawn:              return 0x41;
		case cProtocol::pktEntityHeadLook:       return 0x42;
		case cProtocol::pktBlockChanges:         return 0x43;
			// select advancment tab 0x44
			// ServerMetadataS2CPacket 0x45
			// overlay msg 0x46
			// wb -- worldborder wb center changed 0x47
			// wb interpolate size 0x48
			// wb size changed 0x49
			// wb warning time changed 0x4A
			// wb warning blocks changed 0x4B
		case cProtocol::pktCameraSetTo:          return 0x4C;
		case cProtocol::pktHeldItemChange:       return 0x4D;
		case cProtocol::pktRenderDistanceCenter: return 0x4E;
			//  chunk load distance 0x4F
		case cProtocol::pktSpawnPosition:        return 0x50;
			//  scoreboard display 0x51
		case cProtocol::pktEntityMeta:           return 0x52;
		case cProtocol::pktLeashEntity:          return 0x53;
		case cProtocol::pktEntityVelocity:       return 0x54;
		case cProtocol::pktEntityEquipment:      return 0x55;
		case cProtocol::pktExperience:           return 0x56;
		case cProtocol::pktUpdateHealth:         return 0x57;
		case cProtocol::pktScoreboardObjective:  return 0x58;
		case cProtocol::pktAttachEntity:         return 0x59;
			// Teams 0x5A
		case cProtocol::pktUpdateScore:          return 0x5B;
			// simulation distance 0x5C
			// subtitle 0x5D
		case cProtocol::pktTimeUpdate:           return 0x5E;
		case cProtocol::pktTitle:                return 0x5F;
			//  title fade 0x60
			//  play sound from entity 0x61
		case cProtocol::pktSoundEffect:          return 0x62;
			//  stop sound 0x63
		case cProtocol::pktChatRaw:              return 0x64;  //  Gamemessage
			//  player list header 0x65
			//  NbtQueryResponseS2CPacket 0x66
		case cProtocol::pktCollectEntity:        return 0x67;
		case cProtocol::pktTeleportEntity:       return 0x68;
			//  advancment update 0x69
		case cProtocol::pktEntityProperties:     return 0x6A;
			// FeaturesS2CPacket 0x6B
		case cProtocol::pktEntityEffect:         return 0x6C;
			//  sync recepies 0x6D
			//  sync tags 0x6E
		default: UNREACHABLE("unhandeled packet");
	}
}





bool cProtocol_1_19_4::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	if (m_State != State::Game)
	{
		return Super::HandlePacket(a_ByteBuffer, a_PacketType);
	}

	// Game
	switch (a_PacketType)
	{
		case 0x00: HandleConfirmTeleport(a_ByteBuffer); return true;
		case 0x01: /* query nbt packet */ return false;
		case 0x02: /* update difficulty */ return false;
		case 0x03: /* MessageAcknowledgmentC2SPacket */ return false;
		case 0x04: HandlePacketCommandExecution(a_ByteBuffer); return true;
		case 0x05: HandlePacketChatMessage(a_ByteBuffer); return true;
		case 0x06: HandlePacketPlayerSession(a_ByteBuffer); return true;
		case 0x07: HandlePacketClientStatus(a_ByteBuffer); return true;
		case 0x08: HandlePacketClientSettings(a_ByteBuffer); return true;
		case 0x09: HandlePacketTabComplete(a_ByteBuffer); return true;
		case 0x0A: /* ButtonClickC2SPacket */ return false;
		case 0x0B: HandlePacketWindowClick(a_ByteBuffer); return true;
		case 0x0C: HandlePacketWindowClose(a_ByteBuffer); return true;
		case 0x0D: HandlePacketPluginMessage(a_ByteBuffer); return true;
		case 0x0E: HandlePacketBookUpdate(a_ByteBuffer); return true;  // not fully implemented
		case 0x0F: /* QueryEntityNbtC2SPacket */ return false;
		case 0x10: HandlePacketUseEntity(a_ByteBuffer); return true;
		case 0x11: /* Jigsaw generating */ return false;
		case 0x12: HandlePacketKeepAlive(a_ByteBuffer); return true;
		case 0x13: /* Update difficulty lock */ return false;  // only used in single player
		case 0x14: HandlePacketPlayerPos(a_ByteBuffer); return true;  // PositionAndOnGround
		case 0x15: HandlePacketPlayerPosLook(a_ByteBuffer); return true;  // full
		case 0x16: HandlePacketPlayerLook(a_ByteBuffer); return true;  // LookAndOnGround
		case 0x17: HandlePacketPlayer(a_ByteBuffer); return true;
		case 0x18: HandlePacketVehicleMove(a_ByteBuffer); return true;
		case 0x19: HandlePacketBoatSteer(a_ByteBuffer); return true;
		case 0x1A: /* pick from inventory */ return false;
		case 0x1B: HandleCraftRecipe(a_ByteBuffer); return true;
		case 0x1C: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
		case 0x1D: HandlePacketBlockDig(a_ByteBuffer); return true;
		case 0x1E: /* client command packet */ return false;
		case 0x1F: HandlePacketSteerVehicle(a_ByteBuffer); return true;  // player input packet
		case 0x20: /* PlayPongC2SPacket */ return false;
		case 0x21: /* Recipe Category Options */ return false;
		case 0x22: HandlePacketCraftingBookData(a_ByteBuffer); return true;
		case 0x23: HandlePacketNameItem(a_ByteBuffer); return true;
		case 0x24: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
		case 0x25: HandlePacketAdvancementTab(a_ByteBuffer); return true;
		case 0x26: /* select villager trade */ return false;
		case 0x27: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
		case 0x28: HandlePacketSlotSelect(a_ByteBuffer); return true;
		case 0x29: HandlePacketCommandBlockUpdate(a_ByteBuffer); return true;
		case 0x2A: /* update minecart command block */ return false;
		case 0x2B: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
		case 0x2C: /* Update jigsaw block */ return false;
		case 0x2D: /* Update structure block */ return false;
		case 0x2E: HandlePacketUpdateSign(a_ByteBuffer); return true;
		case 0x2F: /* Update hand swing */ return false;
		case 0x30: /* Spectator teleport */ return false;
		case 0x31: HandlePacketBlockPlace(a_ByteBuffer); return true;
		case 0x32: HandlePacketUseItem(a_ByteBuffer); return true;
		default: break;
	}
	UNREACHABLE("");
}





void cProtocol_1_19_4::HandlePacketPlayerSession(cByteBuffer & a_ByteBuffer)
{
	ContiguousByteBuffer pubkey;
	ContiguousByteBuffer KeySig;
	HANDLE_READ(a_ByteBuffer, ReadUUID, cUUID, SessionID);
	HANDLE_READ(a_ByteBuffer, ReadBEInt64, Int64, ExpiresAtEpoch);
	HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, PubKeyLen);
	if (!a_ByteBuffer.ReadSome(pubkey, PubKeyLen))
	{
		return;
	}
	HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, KeySigLen);
	if (!a_ByteBuffer.ReadSome(KeySig, KeySigLen))
	{
		return;
	}
	if (PubKeyLen > 512)
	{
		m_Client->Kick("Public key too long");
		return;
	}
	if (KeySigLen > 4096)
	{
		m_Client->Kick("Key signature too long");
		return;
	}
	m_Client->HandlePlayerSession(SessionID, ExpiresAtEpoch, pubkey, KeySig);
}





void cProtocol_1_19_4::SendRespawn(eDimension a_Dimension)
{
	cPacketizer Pkt(*this, pktRespawn);
	cPlayer * Player = m_Client->GetPlayer();

	Pkt.WriteString("minecraft:overworld");  // dimension type key
	Pkt.WriteString("minecraft:overworld");  // world key
	Pkt.WriteBEUInt64(0);  // Appears to be a SHA256 od the world seed
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBool(false);  // debug world
	Pkt.WriteBool(false);  // flat world
	Pkt.WriteBEInt8(0x3);   // keep player attributes
	Pkt.WriteBool(false);  // optional last death pos
}





void cProtocol_1_19_4::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
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

		AString dmgsrc[] = {"in_fire","lightning_bolt", "on_fire","lava","hot_floor", "in_wall","cramming", "drown","starve", "cactus", "fall","fly_into_wall","out_of_world","generic","magic","wither","dragon_breath","dry_out","sweet_berry_bush","freeze","stalagmite","player_attack"};
		int dmgids[] =     {18,          22,                 27,          21,      17,             19,        3,              5,          33,        2,           8,       14,                28,               16,         23,        40,         4,                6,           35,                   15,        32,             34};
		{
			cFastNBTWriter Writer;
			Writer.BeginCompound("minecraft:damage_type");
				Writer.AddString("type", "minecraft:damage_type");
				Writer.BeginList("value", eTagType::TAG_Compound);
				int id = 0;
					for (const auto & ds : dmgsrc)
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
					Writer.AddInt("id", 0);
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
							Writer.AddByte("has_precipitation", 1);
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

		Pkt.WriteString("minecraft:overworld");  // dimension type
		Pkt.WriteString("minecraft:overworld");  // dimension id

		Pkt.WriteBEInt64(0);  // Seed
		Pkt.WriteVarInt32(static_cast<UInt32>(Server->GetMaxPlayers()));
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));  // simulation distance
		Pkt.WriteBool(false);  // Reduced debug info
		Pkt.WriteBool(true);   // Show deaths screen
		Pkt.WriteBool(false);  // Debug world
		Pkt.WriteBool(false);  // Flat World
		Pkt.WriteBool(false);  // optional last death loc
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





void cProtocol_1_19_4::SendPlayerMoveLook(const Vector3d a_Pos, const float a_Yaw, const float a_Pitch, const bool a_IsRelative)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerMoveLook);
	Pkt.WriteBEDouble(a_Pos.x);
	Pkt.WriteBEDouble(a_Pos.y);
	Pkt.WriteBEDouble(a_Pos.z);
	Pkt.WriteBEFloat(a_Yaw);
	Pkt.WriteBEFloat(a_Pitch);

	if (a_IsRelative)
	{
		// Set all bits to 1 - makes everything relative
		Pkt.WriteBEUInt8(static_cast<UInt8>(-1));
	}
	else
	{
		// Set all bits to 0 - make everything absolute
		Pkt.WriteBEUInt8(0);
	}

	Pkt.WriteVarInt32(++m_OutstandingTeleportId);

	// This teleport ID hasn't been confirmed yet
	m_IsTeleportIdConfirmed = false;
}





void cProtocol_1_19_4::WriteEntityMetadata(cPacketizer & a_Pkt, const EntityMetadata a_Metadata, const EntityMetadataType a_FieldType) const
{
	a_Pkt.WriteBEUInt8(GetEntityMetadataID(a_Metadata));	      // Index
	auto v = static_cast<UInt32>(a_FieldType);
	if (v >= 2)  // temp fix
	{
		v++;
	}
	a_Pkt.WriteVarInt32(v);        // Type
}





void cProtocol_1_19_4::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const
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
			a_Pkt.WriteString(JsonUtils::SerializeSingleValueJsonObject("text", Player.GetName()));	 // needs to be json formatted

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
					a_Pkt.WriteVarInt32(GetProtocolItemType(MinecartContent.m_ItemType));

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
					break;
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





UInt32 cProtocol_1_19_4::GetProtocolMobType(eMonsterType a_MobType) const
{
	switch (a_MobType)
	{
		// Map invalid type to Giant for easy debugging (if this ever spawns, something has gone very wrong)
		case mtAllay: return 0;
		case mtAxolotl: return 4;
		case mtBat: return 5;
		case mtBee: return 6;
		case mtBlaze: return 7;
		case mtCamel: return 10;
		case mtCat: return 11;
		case mtCaveSpider: return 12;
		case mtChicken: return 15;
		case mtCod: return 16;
		case mtCow: return 18;
		case mtCreeper: return 19;
		case mtDolphin: return 20;
		case mtDonkey: return 21;
		case mtDrowned: return 23;
		case mtElderGuardian: return 25;
		case mtEnderDragon: return 27;
		case mtEnderman: return 29;
		case mtEndermite: return 30;
		case mtEvoker: return 31;
		case mtFox: return 38;
		case mtFrog: return 39;
		case mtGhast: return 41;
		case mtGiant: return 42;
		case mtGlowSquid: return 44;
		case mtGoat: return 45;
		case mtGuardian: return 46;
		case mtHoglin: return 47;
		case mtHorse: return 49;
		case mtHusk: return 50;
		case mtIllusioner: return 51;
		case mtLlama: return 60;
		case mtMagmaCube: return 62;
		case mtMooshroom: return 65;
		case mtMule: return 66;
		case mtOcelot: return 67;
		case mtPanda: return 69;
		case mtParrot: return 70;
		case mtPhantom: return 71;
		case mtPig: return 72;
		case mtPiglin: return 73;
		case mtPiglinBrute: return 74;
		case mtPillager: return 75;
		case mtPolarBear: return 76;
		case mtPufferfish: return 78;
		case mtRabbit: return 79;
		case mtRavager: return 80;
		case mtSalmon: return 81;
		case mtSheep: return 82;
		case mtShulker: return 83;
		case mtSilverfish: return 85;
		case mtSkeleton: return 86;
		case mtSkeletonHorse: return 87;
		case mtSlime: return 88;
		case mtSniffer: return 90;
		case mtSpider: return 95;
		case mtSquid: return 96;
		case mtStray: return 97;
		case mtStrider: return 98;
		case mtTadpole: return 99;
		case mtTraderLlama: return 103;
		case mtTropicalFish: return 105;
		case mtTurtle: return 106;
		case mtVex: return 107;
		case mtVillager: return 108;
		case mtVindicator: return 109;
		case mtWanderingTrader: return 110;
		case mtWarden: return 111;
		case mtWitch: return 112;
		case mtWither: return 113;
		case mtWitherSkeleton: return 114;
		case mtWolf: return 116;
		case mtZoglin: return 117;
		case mtZombie: return 118;
		case mtZombieHorse: return 119;
		case mtZombieVillager: return 120;
		case mtZombifiedPiglin: return 121;

		default:                      return 0;
	}
}





UInt8 cProtocol_1_19_4::GetProtocolEntityType(const cEntity & a_Entity) const
{
	using Type = cEntity::eEntityType;

	switch (a_Entity.GetEntityType())
	{
		case Type::etEnderCrystal: return 26;
		case Type::etPickup: return 54;
		case Type::etFallingBlock: return 36;
		case Type::etMinecart: return 64;
		case Type::etBoat: return 9;
		case Type::etTNT: return 101;
		case Type::etProjectile:
		{
			using PType = cProjectileEntity::eKind;
			const auto & Projectile = static_cast<const cProjectileEntity &>(a_Entity);

			switch (Projectile.GetProjectileKind())
			{
				case PType::pkArrow: return 3;
				case PType::pkSnowball: return 92;
				case PType::pkEgg: return 24;
				case PType::pkGhastFireball: return 57;
				case PType::pkFireCharge: return 89;
				case PType::pkEnderPearl: return 94;
				case PType::pkExpBottle: return 33;
				case PType::pkSplashPotion: return 77;
				case PType::pkFirework: return 37;
				case PType::pkWitherSkull: return 115;
			}
			break;
		}
		case Type::etFloater: return 123;
		case Type::etItemFrame: return 56;
		case Type::etLeashKnot: return 58;

		// Non-objects must not be sent
		case Type::etEntity:
		case Type::etPlayer:
		case Type::etMonster:
		case Type::etExpOrb:
		case Type::etPainting: break;
	}
	UNREACHABLE("Unhandled entity kind");
}





Int32 cProtocol_1_19_4::GetProtocolCommandArgumentID(eCommandParserType a_ParserType) const
{
	switch (a_ParserType)
	{
		case eCommandParserType::Bool:              return 0;
		case eCommandParserType::Float:             return 1;
		case eCommandParserType::Double:            return 2;
		case eCommandParserType::Integer:           return 3;
		case eCommandParserType::Long:              return 4;
		case eCommandParserType::String:            return 5;
		case eCommandParserType::Entity:            return 6;
		case eCommandParserType::GameProfile:       return 7;
		case eCommandParserType::BlockPos:          return 8;
		case eCommandParserType::ColumnPos:         return 9;
		case eCommandParserType::Vec3:              return 10;
		case eCommandParserType::Vec2:              return 11;
		case eCommandParserType::BlockState:        return 12;
		case eCommandParserType::BlockPredicate:    return 13;
		case eCommandParserType::ItemStack:         return 14;
		case eCommandParserType::ItemPredicate:     return 15;
		case eCommandParserType::Color:             return 16;
		case eCommandParserType::Component:         return 17;
		case eCommandParserType::Message:           return 18;
		case eCommandParserType::NbtCompoundTag:    return 19;
		case eCommandParserType::NbtTag:            return 20;
		case eCommandParserType::NbtPath:           return 21;
		case eCommandParserType::Objective:         return 22;
		case eCommandParserType::ObjectiveCriteria: return 23;
		case eCommandParserType::Operation:         return 24;
		case eCommandParserType::Particle:          return 25;
		case eCommandParserType::Angle:             return 26;
		case eCommandParserType::Rotation:          return 27;
		case eCommandParserType::ScoreboardSlot:    return 28;
		case eCommandParserType::ScoreHolder:       return 29;
		case eCommandParserType::Swizzle:           return 30;
		case eCommandParserType::Team:              return 31;
		case eCommandParserType::ItemSlot:          return 32;
		case eCommandParserType::ResourceLocation:  return 33;
		case eCommandParserType::Function:          return 34;
		case eCommandParserType::EntityAnchor:      return 35;
		case eCommandParserType::IntRange:          return 36;
		case eCommandParserType::FloatRange:        return 37;
		case eCommandParserType::Dimension:         return 38;
		case eCommandParserType::Gamemode:          return 39;
		case eCommandParserType::Time:              return 40;
		case eCommandParserType::ResourceOrTag:     return 41;
		case eCommandParserType::ResourceOrTagKey:  return 42;
		case eCommandParserType::Resource:          return 43;
		case eCommandParserType::ResourceKey:       return 44;
		case eCommandParserType::TemplateMirror:    return 45;
		case eCommandParserType::TemplateRotation:  return 46;
		case eCommandParserType::Heightmap:         return 47;
		case eCommandParserType::Uuid:              return 48;
		default: return -1;
	}
}
