#include "Globals.h"
#include "Protocol_1_16.h"
#include "Packetizer.h"


static const UInt32 CompressionThreshold = 256;  // After how large a packet should we compress it.

////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_16:

UInt32 cProtocol_1_16::GetPacketID(ePacketType a_PacketType) const
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
		case cProtocol::pktEntityAnimation:      return 0x05;
		case cProtocol::pktStatistics:           return 0x06;
		case cProtocol::pktPlayerActionResponse: return 0x07;
		//  case cProtocol::pktBlockbreakingprogress:   return 0x08;
		case cProtocol::pktUpdateBlockEntity:    return 0x09;
		case cProtocol::pktBlockAction:          return 0x0A;
		case cProtocol::pktBlockChange:          return 0x0B;
		case cProtocol::pktBossBar:              return 0x0C;
		case cProtocol::pktDifficulty:           return 0x0D;
		case cProtocol::pktChatRaw:              return 0x0E;  //  Gamemessage
		case cProtocol::pktBlockChanges:         return 0x0F;
			//  command suggestions here 0x10
			//  command tree 0x11
			//  confirm gui action 0x12
		case cProtocol::pktWindowClose:          return 0x13;
		case cProtocol::pktWindowItems:          return 0x14; //  Inventory packet
		case cProtocol::pktWindowProperty:       return 0x15; //  ScreenHandlerPropertyUpdateS2CPacket
		case cProtocol::pktInventorySlot:        return 0x16; //  ScreenHandlerSlotUpdateS2CPacket
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
		case cProtocol::pktKeepAlive:            return 0x20;
			// chunk data 0x21
		case cProtocol::pktSoundParticleEffect:  return 0x22;  // world event
		case cProtocol::pktParticleEffect:       return 0x23;
		case cProtocol::pktLightUpdate:          return 0x24;
		case cProtocol::pktJoinGame:             return 0x25;
			//  map update 0x26
			//  set trade offers 0x27 
		case cProtocol::pktEntityRelMove:        return 0x28;
		case cProtocol::pktEntityRelMoveLook:    return 0x29;
		case cProtocol::pktEntityLook:           return 0x2A;
			//  entity packet 0x2B
			//  vehicle move 0x2C
			//  open written book 0x2D
		case cProtocol::pktWindowOpen:           return 0x2E;
		case cProtocol::pktUpdateSign:           return 0x2F;
			//  craft failed response 0x30
		case cProtocol::pktPlayerAbilities:      return 0x31;
			//  combat events 0x32 
		case cProtocol::pktPlayerList:           return 0x33;
			//  look at 0x34
		case cProtocol::pktPlayerMoveLook:       return 0x35;
		case cProtocol::pktUnlockRecipe:         return 0x36;
		case cProtocol::pktDestroyEntity:        return 0x37;
		case cProtocol::pktRemoveEntityEffect:   return 0x38;
		case cProtocol::pktResourcePack:         return 0x39;
		case cProtocol::pktRespawn:              return 0x3A;
		case cProtocol::pktEntityHeadLook:       return 0x3B;
			// select advancment tab 0x3C
		case cProtocol::pktWorldBorder:          return 0x3D;
		case cProtocol::pktCameraSetTo:          return 0x3E;
		case cProtocol::pktHeldItemChange:       return 0x3F;
		case cProtocol::pktRenderDistanceCenter: return 0x40;
			//  chunk load distance 0x41
		case cProtocol::pktSpawnPosition:        return 0x42;
			//  scoreboard display 0x43
		case cProtocol::pktEntityMeta:           return 0x44;
		case cProtocol::pktEntityVelocity:       return 0x46;
		case cProtocol::pktEntityEquipment:      return 0x47;
		case cProtocol::pktExperience:           return 0x48;
		case cProtocol::pktUpdateHealth:         return 0x49;
		case cProtocol::pktScoreboardObjective:  return 0x4A;
		case cProtocol::pktAttachEntity:         return 0x4B;
			// Teams 0x4C
		case cProtocol::pktUpdateScore:          return 0x4D;
		case cProtocol::pktTimeUpdate:           return 0x4E;
		case cProtocol::pktTitle:                return 0x4F;
			//  play sound from entity 0x50
			//  play sound 0x51
			//  stop sound 0x52
			//  player list header 0x53
			//  tag query response 0x54 
		case cProtocol::pktCollectEntity:        return 0x55;
		case cProtocol::pktTeleportEntity:       return 0x56;
			//  advancment update 0x57
		case cProtocol::pktEntityProperties:     return 0x58;
		case cProtocol::pktEntityEffect:         return 0x59;
			//  sync recepies 0x5A
			//  sync tags 0x5B
		default: UNREACHABLE("unhandeled packet");
	}
}





bool cProtocol_1_16::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	LOG("Handling packet 0x%x", a_PacketType);
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
		case 0x03: HandlePacketChatMessage(a_ByteBuffer); return true;
		case 0x04: HandlePacketClientStatus(a_ByteBuffer); return true;
		case 0x05: HandlePacketClientSettings(a_ByteBuffer); return true;
		case 0x06: HandlePacketTabComplete(a_ByteBuffer); return true;
		case 0x07: break;  // Confirm transaction - not used in Cuberite
		case 0x08: HandlePacketEnchantItem(a_ByteBuffer); return true;
		case 0x09: HandlePacketWindowClick(a_ByteBuffer); return true;
		case 0x0A: HandlePacketWindowClose(a_ByteBuffer); return true;
		case 0x0B: HandlePacketPluginMessage(a_ByteBuffer); return true;
		case 0x0C: HandlePacketBookUpdate(a_ByteBuffer); return true;  // not fully implemented
		case 0x0E: HandlePacketUseEntity(a_ByteBuffer); return true;
		case 0x0F: /* Jigsaw generating */ return false;
		case 0x10: HandlePacketKeepAlive(a_ByteBuffer); return true;
		case 0x11: /* Update difficulty lock */ return false;  // only used in single player
		case 0x12: HandlePacketPlayerPos(a_ByteBuffer); return true;
		case 0x13: HandlePacketPlayerPosLook(a_ByteBuffer); return true;
		case 0x14: HandlePacketPlayerLook(a_ByteBuffer); return true;
		case 0x15: HandlePacketPlayer(a_ByteBuffer); return true;
		case 0x16: HandlePacketVehicleMove(a_ByteBuffer); return true;
		case 0x17: HandlePacketBoatSteer(a_ByteBuffer); return true;
		case 0x18: /* pick from inventory */ return false;
		case 0x19: HandleCraftRecipe(a_ByteBuffer); return true;
		case 0x1A: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
		case 0x1B: HandlePacketBlockDig(a_ByteBuffer); return true;
		case 0x1C: /* client command packet */ return false;
		case 0x1D: HandlePacketSteerVehicle(a_ByteBuffer); return true;  // player input packet
		case 0x1E: /* Recipe Category Options */ return false;
		case 0x1F: HandlePacketCraftingBookData(a_ByteBuffer); return true;
		case 0x20: HandlePacketNameItem(a_ByteBuffer); return true;
		case 0x21: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
		case 0x22: HandlePacketAdvancementTab(a_ByteBuffer); return true;
		case 0x23: /* select villager trade */ return false;
		case 0x24: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
		case 0x25: HandlePacketSlotSelect(a_ByteBuffer); return true;
		case 0x26: /* update command block */ return false;
		case 0x27: /* update minecart command block*/ return false;
		case 0x28: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
		case 0x29: /* Update jigsaw block */ return false;
		case 0x2A: /* Update structure block */ return false;
		case 0x2B: HandlePacketUpdateSign(a_ByteBuffer); return true;
		case 0x2C: /* Update hand swing */ return false;
		case 0x2D: /* Spectator teleport */ return false;
		// case 0x2D: HandlePacketBlockPlace(a_ByteBuffer); return true;
		case 0x2E: HandlePacketUseItem(a_ByteBuffer); return true;
		default: break;
	}
}






void cProtocol_1_16::SendLoginSuccess(void)
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
		auto uuid = m_Client->GetUUID().ToRaw();
		auto arr = reinterpret_cast<int*>(&uuid);
		Pkt.WriteBEInt32(arr[0]);
		Pkt.WriteBEInt32(arr[1]);
		Pkt.WriteBEInt32(arr[2]);
		Pkt.WriteBEInt32(arr[3]);
		//  Pkt.WriteString(m_Client->GetUUID().ToLongString());
		Pkt.WriteString(m_Client->GetUsername());
	}
}





void cProtocol_1_16::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the Join Game packet:
	{
		cServer * Server = cRoot::Get()->GetServer();
		cPacketizer Pkt(*this, pktJoinGame);
		Pkt.WriteBEUInt32(a_Player.GetUniqueID());
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()) | (Server->IsHardcore() ? 0x08 : 0));
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()));
		Pkt.WriteVarInt32(1);  // Number of dimensions
		Pkt.WriteString("overworld");
		//Pkt.WriteString("the_nether");
		//Pkt.WriteString("the_end");

		cFastNBTWriter Writer;
		Writer.BeginList("dimension",eTagType::TAG_Compound);
		Writer.BeginCompound("");
		Writer.AddString("name", "overworld");
		Writer.AddFloat("ambient_light", 1.0);
		Writer.AddString("infiniburn", "infiniburn_overworld");
		Writer.AddInt("logical_height", 256);
		Writer.AddByte("has_raids", 1);
		Writer.AddByte("respawn_anchor_works", 1);
		Writer.AddByte("bed_works", 1);
		Writer.AddByte("piglin_safe", 1);
		Writer.AddByte("shrunk", 1);
		Writer.AddByte("natural", 1);
		Writer.AddByte("ultrawarm", 1);
		Writer.AddByte("has_ceiling", 1);
		Writer.AddByte("has_skylight", 1);
		Writer.EndCompound();
		Writer.EndList();
		//Writer.EndCompound();
		Writer.Finish();
		Pkt.WriteBuf(Writer.GetResult());

		Pkt.WriteString("overworld"); // dimension type key
		Pkt.WriteString("overworld"); // dimension id

		Pkt.WriteBEInt64(0);  // Seed
		Pkt.WriteBEUInt8(static_cast<UInt8>(Clamp<size_t>(Server->GetMaxPlayers(), 0, 255)));
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));
		Pkt.WriteBool(false);  // Reduced debug info
		Pkt.WriteBool(true);   // Show deaths screen
		Pkt.WriteBool(false);  // Debug world
		Pkt.WriteBool(false);  // Death screen
	}

	// Send the spawn position:
	{
		cPacketizer Pkt(*this, pktSpawnPosition);
		Pkt.WriteXZYPosition64(a_World.GetSpawnX(), a_World.GetSpawnY(), a_World.GetSpawnZ());
	}
	// Send the server difficulty:
	{
		cPacketizer Pkt(*this, pktDifficulty);
		Pkt.WriteBEInt8(1);
		Pkt.WriteBool(false);  // Difficulty locked?
	}
}





void cProtocol_1_16::WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity) const
{
	if (a_Entity.IsPlayer())
	{
		const auto & Player = static_cast<const cPlayer &>(a_Entity);

		a_Pkt.WriteBEInt32(1);  // Count.
		a_Pkt.WriteString("generic.movement_speed");
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

		a_Pkt.WriteBEInt32(0);
	}
}





void cProtocol_1_16::SendRespawn(eDimension a_Dimension)
{
	cPacketizer Pkt(*this, pktRespawn);
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteString("overworld");
	Pkt.WriteString("overworld");
	Pkt.WriteBEUInt64(0);  // Appears to be a SHA256 od the world seed
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBool(false);  // debug world
	Pkt.WriteBool(false);  // flat world
	Pkt.WriteBool(true);   // keep player attributes
}





void cProtocol_1_16::HandlePacketUseItem(cByteBuffer & a_ByteBuffer)
{
	#define MAIN_HAND 0
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Hand);
	HANDLE_READ(a_ByteBuffer, ReadXZYPosition64, Vector3i, BlockPos);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Direction);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Xoffset);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Yoffset);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Zoffset);
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, bl);

	m_Client->HandleUseItem(Hand == MAIN_HAND);
}






cProtocol::Version cProtocol_1_16::GetProtocolVersion() const
{
	return Version::v1_16;
}





void cProtocol_1_16::SendChatRaw(const AString & a_MessageRaw, eChatType a_Type)
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
	Pkt.WriteBEInt8([a_Type]() -> signed char
	{
		switch (a_Type)
		{
			case eChatType::ctChatBox: return 0;
			case eChatType::ctSystem: return 1;
			case eChatType::ctAboveActionBar: return 2;
		}
		UNREACHABLE("Unsupported chat type");
	}());
	Pkt.WriteUUID(cUUID::cUUID());

}






////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_16_1:

cProtocol::Version cProtocol_1_16_1::GetProtocolVersion() const
{
	return Version::v1_16_1;
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_16_2:

cProtocol::Version cProtocol_1_16_2::GetProtocolVersion() const
{
	return Version::v1_16_2;
}





UInt32 cProtocol_1_16_2::GetPacketID(ePacketType a_PacketType) const
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
		case cProtocol::pktEntityAnimation:      return 0x05;
		case cProtocol::pktStatistics:           return 0x06;
		case cProtocol::pktPlayerActionResponse: return 0x07;
		//  case cProtocol::pktBlockbreakingprogress:   return 0x08;
		case cProtocol::pktUpdateBlockEntity:    return 0x09;
		case cProtocol::pktBlockAction:          return 0x0A;
		case cProtocol::pktBlockChange:          return 0x0B;
		case cProtocol::pktBossBar:              return 0x0C;
		case cProtocol::pktDifficulty:           return 0x0D;
		case cProtocol::pktChatRaw:              return 0x0E;  //  Gamemessage
			//  command suggestions here 0x0F
			//  command tree 0x10
			//  confirm gui action 0x11
		case cProtocol::pktWindowClose:          return 0x12;
		case cProtocol::pktWindowItems:          return 0x13; //  Inventory packet
		case cProtocol::pktWindowProperty:       return 0x14; //  ScreenHandlerPropertyUpdateS2CPacket
		case cProtocol::pktInventorySlot:        return 0x15; //  ScreenHandlerSlotUpdateS2CPacket
			//  0x16 cooldown update
		case cProtocol::pktCustomPayload:        return 0x17;
		case cProtocol::pktPluginMessage:        return 0x17;
		case cProtocol::pktSoundEffect:          return 0x18;
		case cProtocol::pktDisconnectDuringGame: return 0x19;
		case cProtocol::pktEntityStatus:         return 0x1A;
		case cProtocol::pktExplosion:            return 0x1B;
		case cProtocol::pktUnloadChunk:          return 0x1C;
		case cProtocol::pktGameMode:             return 0x1D;
		case cProtocol::pktWeather:              return 0x1D;
		case cProtocol::pktHorseWindowOpen:      return 0x1E;
		case cProtocol::pktKeepAlive:            return 0x1F;
			// chunk data packet 0x20
		case cProtocol::pktSoundParticleEffect:  return 0x21;  // world event
		case cProtocol::pktParticleEffect:       return 0x22;
		case cProtocol::pktLightUpdate:          return 0x23;
		case cProtocol::pktJoinGame:             return 0x24;
			//  map update 0x25
			//  set trade offers 0x26 
		case cProtocol::pktEntityRelMove:        return 0x27;
		case cProtocol::pktEntityRelMoveLook:    return 0x28;
		case cProtocol::pktEntityLook:           return 0x29;
			//  entity packet 0x2A
			//  vehicle move 0x2B
			//  open written book 0x2C
		case cProtocol::pktWindowOpen:           return 0x2D;
		case cProtocol::pktUpdateSign:           return 0x2E;
			//  craft failed response 0x2F
		case cProtocol::pktPlayerAbilities:      return 0x30;
			//  combat events 0x31 
		case cProtocol::pktPlayerList:           return 0x32;
			//  look at 0x33
		case cProtocol::pktPlayerMoveLook:       return 0x34;
		case cProtocol::pktUnlockRecipe:         return 0x35;
		case cProtocol::pktDestroyEntity:        return 0x36;
		case cProtocol::pktRemoveEntityEffect:   return 0x37;
		case cProtocol::pktResourcePack:         return 0x38;
		case cProtocol::pktRespawn:              return 0x39;
		case cProtocol::pktEntityHeadLook:       return 0x3A;
		case cProtocol::pktBlockChanges:         return 0x3B;
			// select advancment tab 0x3C
		case cProtocol::pktWorldBorder:          return 0x3D;
		case cProtocol::pktCameraSetTo:          return 0x3E;
		case cProtocol::pktHeldItemChange:       return 0x3F;
		case cProtocol::pktRenderDistanceCenter: return 0x40;
			//  chunk load distance 0x41
		case cProtocol::pktSpawnPosition:        return 0x42;
			//  scoreboard display 0x43
		case cProtocol::pktEntityMeta:           return 0x44;
		case cProtocol::pktEntityVelocity:       return 0x46;
		case cProtocol::pktEntityEquipment:      return 0x47;
		case cProtocol::pktExperience:           return 0x48;
		case cProtocol::pktUpdateHealth:         return 0x49;
		case cProtocol::pktScoreboardObjective:  return 0x4A;
		case cProtocol::pktAttachEntity:         return 0x4B;
			// Teams 0x4C
		case cProtocol::pktUpdateScore:          return 0x4D;
		case cProtocol::pktTimeUpdate:           return 0x4E;
		case cProtocol::pktTitle:                return 0x4F;
			//  play sound from entity 0x50
			//  play sound 0x51
			//  stop sound 0x52
			//  player list header 0x53
			//  tag query response 0x54 
		case cProtocol::pktCollectEntity:        return 0x55;
		case cProtocol::pktTeleportEntity:       return 0x56;
			//  advancment update 0x57
		case cProtocol::pktEntityProperties:     return 0x58;
		case cProtocol::pktEntityEffect:         return 0x59;
			//  sync recepies 0x5A
			//  sync tags 0x5B
		default: UNREACHABLE("unhandeled packet");
	}
}





void cProtocol_1_16_2::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
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
		//Pkt.WriteString("the_nether");
		//Pkt.WriteString("the_end");
		{
			cFastNBTWriter Writer;
			Writer.BeginCompound("minecraft:dimension_type");
				Writer.AddString("type", "minecraft:dimension_type");
				Writer.BeginList("value", eTagType::TAG_Compound);
					Writer.BeginCompound("");
					Writer.AddString("name", "minecraft:overworld");
					Writer.AddInt("id",0);
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

						Writer.EndCompound();
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
							Writer.AddString("category", "plains");
						Writer.EndCompound();
					Writer.EndCompound();
				Writer.EndList();
			Writer.EndCompound();
			Writer.Finish();
			Pkt.WriteBuf(Writer.GetResult());
		}

		//dimensional type
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
			Writer.Finish();
			Pkt.WriteBuf(Writer.GetResult());
		}

		Pkt.WriteString("overworld"); // dimension id

		Pkt.WriteBEInt64(0);  // Seed
		Pkt.WriteVarInt32(static_cast<UInt32>(Server->GetMaxPlayers()));
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));
		Pkt.WriteBool(false);  // Reduced debug info
		Pkt.WriteBool(true);   // Show deaths screen
		Pkt.WriteBool(false);  // Debug world
		Pkt.WriteBool(false);  // Flat World
	}

	// Send the spawn position:
	{
		cPacketizer Pkt(*this, pktSpawnPosition);
		Pkt.WriteXZYPosition64(a_World.GetSpawnX(), a_World.GetSpawnY(), a_World.GetSpawnZ());
	}
	// Send the server difficulty:
	{
		cPacketizer Pkt(*this, pktDifficulty);
		Pkt.WriteBEInt8(1);
		Pkt.WriteBool(false);  // Difficulty locked?
	}
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_16_3:

cProtocol::Version cProtocol_1_16_3::GetProtocolVersion() const
{
	return Version::v1_16_3;
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_16_4:

cProtocol::Version cProtocol_1_16_4::GetProtocolVersion() const
{
	return Version::v1_16_4;
}
