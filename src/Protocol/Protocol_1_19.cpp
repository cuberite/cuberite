#include "Globals.h"
#include "Protocol_1_19.h"
#include "Packetizer.h"





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
		/// Status packets
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
			//  command tree 0x0F
		case cProtocol::pktWindowClose:          return 0x10;
		case cProtocol::pktWindowItems:          return 0x11; //  Inventory packet
		case cProtocol::pktWindowProperty:       return 0x12; //  ScreenHandlerPropertyUpdateS2CPacket
		case cProtocol::pktInventorySlot:        return 0x13; //  ScreenHandlerSlotUpdateS2CPacket
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
			//  map update 0x24
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
			// entity attach 0x4E
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
		//case 0x08: HandlePacketEnchantItem(a_ByteBuffer); return true;
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
		case 0x14: HandlePacketPlayerPosLook(a_ByteBuffer); return true; // full
		case 0x15: HandlePacketPlayerLook(a_ByteBuffer); return true; // LookAndOnGround
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
		case 0x28: /* update command block */ return false;
		case 0x29: /* update minecart command block*/ return false;
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
		//Pkt.WriteString("the_nether");
		//Pkt.WriteString("the_end");
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
	Pkt.WriteString(JsonUtils::SerializeSingleValueJsonObject("text", a_MessageRaw));
	Pkt.WriteVarInt32([a_Type]() -> signed char
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
	Pkt.WriteBEInt32(a_ObjectData);
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedZ() * 400));
}




const int MAX_ENC_LEN = 512;  // Maximum size of the encrypted message; should be 128, but who knows...
void cProtocol_1_19::HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer)
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
	HANDLE_READ(a_ByteBuffer,ReadBool,bool,IsSignature);
	if (!IsSignature)
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
		HANDLE_READ(a_ByteBuffer,ReadBEInt64,INT64,salt);
		HANDLE_READ(a_ByteBuffer,ReadVarInt32,UInt32,SignatureLength);
		if (!a_ByteBuffer.ReadSome(SignatureData, SignatureLength))
		{
			return;
		}
		UNREACHABLE("not implemented");
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



void cProtocol_1_19::HandlePacketLoginStart(cByteBuffer & a_ByteBuffer)
{
	AString Username;
	if (!a_ByteBuffer.ReadVarUTF8String(Username))
	{
		m_Client->Kick("Bad username");
		return;
	}

	HANDLE_READ(a_ByteBuffer,ReadBool,bool,HasPublicKey);
	if (HasPublicKey)
	{
		ContiguousByteBuffer PublicKey;
		ContiguousByteBuffer KeySignature;
		HANDLE_READ(a_ByteBuffer,ReadBEInt64,Int64,ExpiresAtEpochMilis);
		HANDLE_READ(a_ByteBuffer,ReadVarInt32,UInt32,PubKeyLen);
		if (!a_ByteBuffer.ReadSome(PublicKey, PubKeyLen))
		{
			return;
		}
		HANDLE_READ(a_ByteBuffer,ReadVarInt32,UInt32,KeySignatureLen);
		if (!a_ByteBuffer.ReadSome(KeySignature, KeySignatureLen))
		{
			return;
		}

		auto time = std::chrono::system_clock::now(); // get the current time
		auto since_epoch = time.time_since_epoch(); // get the duration since epoch
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch).count();

		if (millis > ExpiresAtEpochMilis)
		{
			LOGWARN("Expired profile public key");
		}

		// TODO: validate key

		UNREACHABLE("not implemented");
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
		Pkt.WriteVarInt32(0);  // number of Profile Properites
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
	cPlayer* Player = m_Client->GetPlayer();

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





////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_19_1:

cProtocol::Version cProtocol_1_19_1::GetProtocolVersion() const
{
	return Version::v1_19_1;
}






////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_19_3:

cProtocol::Version cProtocol_1_19_3::GetProtocolVersion() const
{
	return Version::v1_19_3;
}






////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_19_4:

cProtocol::Version cProtocol_1_19_4::GetProtocolVersion() const
{
	return Version::v1_19_4;
}
