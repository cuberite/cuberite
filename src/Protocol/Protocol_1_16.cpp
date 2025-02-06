#include "Globals.h"
#include "Protocol_1_16.h"
#include "Packetizer.h"
#include "../ClientHandle.h"
#include "../Server.h"
#include "../Entities/Player.h"
#include "../Entities/Entity.h"
#include "../WorldStorage/FastNBT.h"
#include "../Root.h"


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
		case cProtocol::pktCommandTree:          return 0x11;
			//  confirm gui action 0x12
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
		case cProtocol::pktKeepAlive:            return 0x20;
			// chunk data 0x21
		case cProtocol::pktSoundParticleEffect:  return 0x22;  // world event
		case cProtocol::pktParticleEffect:       return 0x23;
		case cProtocol::pktLightUpdate:          return 0x24;
		case cProtocol::pktJoinGame:             return 0x25;
		case cProtocol::pktMapData:              return 0x26;
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
		case 0x26: HandlePacketCommandBlockUpdate(a_ByteBuffer); return true;
		case 0x27: /* update minecart command block */ return false;
		case 0x28: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
		case 0x29: /* Update jigsaw block */ return false;
		case 0x2A: /* Update structure block */ return false;
		case 0x2B: HandlePacketUpdateSign(a_ByteBuffer); return true;
		case 0x2C: /* Update hand swing */ return false;
		case 0x2D: /* Spectator teleport */ return false;
		case 0x2E: HandlePacketUseItem(a_ByteBuffer); return true;
		default: break;
	}
	UNREACHABLE("");
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
		// Pkt.WriteString("the_nether");
		// Pkt.WriteString("the_end");

		cFastNBTWriter Writer;
		Writer.BeginList("dimension", eTagType::TAG_Compound);
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
		Writer.Finish();
		Pkt.WriteBuf(Writer.GetResult());

		Pkt.WriteString("overworld");  // dimension type key
		Pkt.WriteString("overworld");  // dimension id

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
	Pkt.WriteUUID(cUUID());

}





void cProtocol_1_16::SendThunderbolt(Vector3i a_Origin)
{
	LOGWARNING("Thunderbolts not implemented in 1.16+");
	return;
}





void cProtocol_1_16::HandlePacketPlayerAbilities(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Flags);

	// Convert the bitfield into individual boolean flags:
	bool IsFlying = false;
	if ((Flags & 2) != 0)
	{
		IsFlying = true;
	}

	m_Client->HandlePlayerAbilities(IsFlying, 0, 0);
}





int cProtocol_1_16::GetProtocolParticleID(const AString & a_ParticleName) const
{
	static const std::unordered_map<AString, int> ParticleMap
	{
		// Initialize the ParticleMap:
		{ "ambient_entity_effect",  0 },
		{ "angry_villager",         1 },
		{ "barrier",                2 },
		{ "block",                  3 },
		{ "bubble",                 4 },
		{ "cloud",                  5 },
		{ "crit",                   6 },
		{ "damage_indicator",       7 },
		{ "dragon_breath",          8 },
		{ "dripping_lava",          9 },
		{ "falling_lava",           10 },
		{ "landing_lava",           11 },
		{ "dripping_water",         12 },
		{ "falling_water",          13 },
		{ "dust",                   14 },
		{ "effect",                 15 },
		{ "elder_guardian",         16 },
		{ "enchanted_hit",          17 },
		{ "enchant",                18 },
		{ "end_rod",                19 },
		{ "entity_effect",          20 },
		{ "explosion_emitter",      21 },
		{ "explosion",              22 },
		{ "falling_dust",           23 },
		{ "firework",               24 },
		{ "fishing",                25 },
		{ "flame",                  26 },
		{ "soul_fire_flame",        27 },
		{ "soul",                   28 },
		{ "flash",                  29 },
		{ "happy_villager",         30 },
		{ "composter",              31 },
		{ "heart",                  32 },
		{ "instant_effect",         33 },
		{ "item",                   34 },
		{ "item_slime",             35 },
		{ "item_snowball",          36 },
		{ "large_smoke",            37 },
		{ "lava",                   38 },
		{ "mycelium",               39 },
		{ "note",                   40 },
		{ "poof",                   41 },
		{ "portal",                 42 },
		{ "rain",                   43 },
		{ "smoke",                  44 },
		{ "sneeze",                 45 },
		{ "spit",                   46 },
		{ "squid_ink",              47 },
		{ "sweep_attack",           48 },
		{ "totem_of_undying",       49 },
		{ "underwater",             50 },
		{ "splash",                 51 },
		{ "witch",                  52 },
		{ "bubble_pop",             53 },
		{ "current_down",           54 },
		{ "bubble_column_up",       55 },
		{ "nautilus",               56 },
		{ "dolphin",                57 },
		{ "campfire_cosy_smoke",    58 },
		{ "campfire_signal_smoke",  59 },
		{ "dripping_honey",         60 },
		{ "falling_honey",          61 },
		{ "landing_honey",          62 },
		{ "falling_nectar",         63 },
		{ "ash",                    64 },
		{ "crimson_spore",          65 },
		{ "warped_spore",           66 },
		{ "dripping_obsidian_tear", 67 },
		{ "falling_obsidian_tear",  68 },
		{ "landing_obsidian_tear",  69 },
		{ "reverse_portal",         70 },
		{ "white_ash",              71 }
	};


	const auto ParticleName = StrToLower(a_ParticleName);
	const auto FindResult = ParticleMap.find(ParticleName);
	if (FindResult == ParticleMap.end())
	{
		LOGWARNING("Unknown particle: %s", a_ParticleName.c_str());
		ASSERT(!"Unknown particle");
		return 0;
	}

	return FindResult->second;
}





UInt8 cProtocol_1_16::GetProtocolEntityType(eEntityType a_Type) const
{
	using Type = eEntityType;

	switch (a_Type)
	{
		case Type::etAreaEffectCloud:      return 0;
		case Type::etArmorStand:           return 1;
		case Type::etArrow:                return 2;
		case Type::etBat:                  return 3;
		case Type::etBee:                  return 4;
		case Type::etBlaze:                return 5;
		case Type::etCat:                  return 7;
		case Type::etCaveSpider:           return 8;
		case Type::etChicken:              return 9;
		case Type::etCod:                  return 10;
		case Type::etCow:                  return 11;
		case Type::etCreeper:              return 12;
		case Type::etDolphin:              return 13;
		case Type::etDonkey:               return 14;
		case Type::etDragonFireball:       return 15;
		case Type::etDrowned:              return 16;
		case Type::etElderGuardian:        return 17;
		case Type::etEndCrystal:           return 18;
		case Type::etEnderDragon:          return 19;
		case Type::etEnderman:             return 20;
		case Type::etEndermite:            return 21;
		case Type::etEvoker:               return 22;
		case Type::etEvokerFangs:          return 23;
		case Type::etExperienceOrb:        return 24;
		case Type::etEyeOfEnder:           return 25;
		case Type::etFallingBlock:         return 26;
		case Type::etFireworkRocket:       return 27;
		case Type::etFox:                  return 28;
		case Type::etGhast:                return 29;
		case Type::etGiant:                return 30;
		case Type::etGuardian:             return 31;
		case Type::etHoglin:               return 32;
		case Type::etHorse:                return 33;
		case Type::etHusk:                 return 34;
		case Type::etIllusioner:           return 35;
		case Type::etIronGolem:            return 36;
		case Type::etItem:                 return 37;
		case Type::etItemFrame:            return 38;
		case Type::etFireball:             return 39;
		case Type::etLeashKnot:            return 40;
		case Type::etLightningBolt:        return 41;
		case Type::etLlama:                return 42;
		case Type::etLlamaSpit:            return 43;
		case Type::etMagmaCube:            return 44;
		case Type::etMinecart:             return 45;
		case Type::etChestMinecart:        return 46;
		case Type::etCommandBlockMinecart: return 47;
		case Type::etFurnaceMinecart:      return 48;
		case Type::etHopperMinecart:       return 49;
		case Type::etSpawnerMinecart:      return 50;
		case Type::etTntMinecart:          return 51;
		case Type::etMule:                 return 52;
		case Type::etMooshroom:            return 53;
		case Type::etOcelot:               return 54;
		case Type::etPainting:             return 55;
		case Type::etPanda:                return 56;
		case Type::etParrot:               return 57;
		case Type::etPhantom:              return 58;
		case Type::etPig:                  return 59;
		case Type::etPiglin:               return 60;
		case Type::etPillager:             return 61;
		case Type::etPolarBear:            return 62;
		case Type::etTnt:                  return 63;
		case Type::etPufferfish:           return 64;
		case Type::etRabbit:               return 65;
		case Type::etRavager:              return 66;
		case Type::etSalmon:               return 67;
		case Type::etSheep:                return 68;
		case Type::etShulker:              return 69;
		case Type::etShulkerBullet:        return 70;
		case Type::etSilverfish:           return 71;
		case Type::etSkeleton:             return 72;
		case Type::etSkeletonHorse:        return 73;
		case Type::etSlime:                return 74;
		case Type::etSmallFireball:        return 75;
		case Type::etSnowGolem:            return 76;
		case Type::etSnowball:             return 77;
		case Type::etSpectralArrow:        return 78;
		case Type::etSpider:               return 79;
		case Type::etSquid:                return 80;
		case Type::etStray:                return 81;
		case Type::etStrider:              return 82;
		case Type::etEgg:                  return 83;
		case Type::etEnderPearl:           return 84;
		case Type::etExperienceBottle:     return 85;
		case Type::etPotion:               return 86;
		case Type::etTrident:              return 87;
		case Type::etTraderLlama:          return 88;
		case Type::etTropicalFish:         return 89;
		case Type::etTurtle:               return 90;
		case Type::etVex:                  return 91;
		case Type::etVillager:             return 92;
		case Type::etVindicator:           return 93;
		case Type::etWanderingTrader:      return 94;
		case Type::etWitch:                return 95;
		case Type::etWither:               return 96;
		case Type::etWitherSkeleton:       return 97;
		case Type::etWitherSkull:          return 98;
		case Type::etWolf:                 return 99;
		case Type::etZoglin:               return 100;
		case Type::etZombie:               return 101;
		case Type::etZombieHorse:          return 102;
		case Type::etZombieVillager:       return 103;
		case Type::etZombifiedPiglin:      return 104;
		case Type::etPlayer:               return 105;
		case Type::etFishingBobber:        return 106;
	}
	UNREACHABLE("Unhandled entity kind");
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_16_1:

cProtocol::Version cProtocol_1_16_1::GetProtocolVersion() const
{
	return Version::v1_16_1;
}





void cProtocol_1_16_1::HandlePacketUseEntity(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, EntityID);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, Type);

	switch (Type)
	{
		case 0:
		{
			HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, Hand);
			HANDLE_READ(a_ByteBuffer, ReadBool,     bool, IsSneaking);

			if (Hand == MAIN_HAND)  // TODO: implement handling of off-hand actions; ignore them for now to avoid processing actions twice
			{
				m_Client->HandleUseEntity(EntityID, false);
			}
			break;
		}
		case 1:
		{
			HANDLE_READ(a_ByteBuffer, ReadBool,     bool, IsSneaking);
			m_Client->HandleUseEntity(EntityID, true);
			break;
		}
		case 2:
		{
			HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, TargetX);
			HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, TargetY);
			HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, TargetZ);
			HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, Hand);
			HANDLE_READ(a_ByteBuffer, ReadBool,     bool, IsSneaking);

			// TODO: Do anything
			break;
		}
		default:
		{
			ASSERT(!"Unhandled use entity type!");
			return;
		}
	}
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
		case cProtocol::pktCommandTree:          return 0x10;
			//  confirm gui action 0x11
		case cProtocol::pktWindowClose:          return 0x12;
		case cProtocol::pktWindowItems:          return 0x13;  //  Inventory packet
		case cProtocol::pktWindowProperty:       return 0x14;  //  ScreenHandlerPropertyUpdateS2CPacket
		case cProtocol::pktInventorySlot:        return 0x15;  //  ScreenHandlerSlotUpdateS2CPacket
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
		case cProtocol::pktMapData:              return 0x25;
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
		// Pkt.WriteString("the_nether");
		// Pkt.WriteString("the_end");
		{
			cFastNBTWriter Writer;
			Writer.BeginCompound("minecraft:dimension_type");
				Writer.AddString("type", "minecraft:dimension_type");
				Writer.BeginList("value", eTagType::TAG_Compound);
					Writer.BeginCompound("");
					Writer.AddString("name", "minecraft:overworld");
					Writer.AddInt("id", 0);
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
			Writer.Finish();
			Pkt.WriteBuf(Writer.GetResult());
		}

		Pkt.WriteString("overworld");  // dimension id

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
