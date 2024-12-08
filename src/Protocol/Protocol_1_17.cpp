#include "Globals.h"
#include "Protocol_1_17.h"
#include "Packetizer.h"
#include "../ClientHandle.h"
#include "../Server.h"
#include "../Entities/Player.h"
#include "../Entities/Entity.h"
#include "../WorldStorage/FastNBT.h"
#include "../Root.h"
#include "../UI/Window.h"


////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_17:

cProtocol::Version cProtocol_1_17::GetProtocolVersion() const
{
	return Version::v1_17;
}





UInt32 cProtocol_1_17::GetPacketID(ePacketType a_PacketType) const
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
			// subtitle 0x57
		case cProtocol::pktTimeUpdate:           return 0x58;
		case cProtocol::pktTitle:                return 0x59;
			//  title fade 0x5A
			//  play sound from entity 0x5B
			//  play sound 0x5C
			//  stop sound 0x5D
			//  player list header 0x5E
			//  tag query response 0x5F
		case cProtocol::pktCollectEntity:        return 0x60;
		case cProtocol::pktTeleportEntity:       return 0x61;
			//  advancment update 0x62
		case cProtocol::pktEntityProperties:     return 0x63;
		case cProtocol::pktEntityEffect:         return 0x64;
			//  sync recepies 0x65
			//  sync tags 0x66
		default: UNREACHABLE("unhandeled packet");
	}
}





UInt8 cProtocol_1_17::GetEntityMetadataID(EntityMetadata a_Metadata) const
{
	const UInt8 Entity = 8;
	const UInt8 Living = Entity + 7;
	const UInt8 Insentient = Living + 1;
	const UInt8 Ageable = Insentient + 1;
	const UInt8 AbstractHorse = Ageable + 1;
	const UInt8 ChestedHorse = AbstractHorse + 1;
	const UInt8 TameableAnimal = Ageable + 2;
	const UInt8 Minecart = Entity + 6;
	const UInt8 RaidParticipent = Insentient + 1;

	switch (a_Metadata)
	{
		case EntityMetadata::EntityFlags:                           return 0;
		case EntityMetadata::EntityAir:                             return 1;
		case EntityMetadata::EntityCustomName:                      return 2;
		case EntityMetadata::EntityCustomNameVisible:               return 3;
		case EntityMetadata::EntitySilent:                          return 4;
		case EntityMetadata::EntityNoGravity:                       return 5;
		case EntityMetadata::EntityPose:                            return 6;
		case EntityMetadata::FrozenTicks:                           return 7;
		case EntityMetadata::ThrowableItem:                         return Entity;
		case EntityMetadata::PotionThrown:                          return Entity;
		case EntityMetadata::FallingBlockPosition:                  return Entity;
		case EntityMetadata::AreaEffectCloudRadius:                 return Entity;
		case EntityMetadata::AreaEffectCloudColor:                  return Entity + 1;
		case EntityMetadata::AreaEffectCloudSinglePointEffect:      return Entity + 2;
		case EntityMetadata::AreaEffectCloudParticleId:             return Entity + 3;
		case EntityMetadata::ArrowFlags:                            return Entity;
		case EntityMetadata::PiercingLevel:                         return Entity + 2;
		case EntityMetadata::TippedArrowColor:                      return Entity + 3;
		case EntityMetadata::BoatLastHitTime:                       return Entity;
		case EntityMetadata::BoatForwardDirection:                  return Entity + 1;
		case EntityMetadata::BoatDamageTaken:                       return Entity + 2;
		case EntityMetadata::BoatType:                              return Entity + 3;
		case EntityMetadata::BoatLeftPaddleTurning:                 return Entity + 4;
		case EntityMetadata::BoatRightPaddleTurning:                return Entity + 5;
		case EntityMetadata::BoatSplashTimer:                       return Entity + 6;
		case EntityMetadata::EnderCrystalBeamTarget:                return Entity;
		case EntityMetadata::EnderCrystalShowBottom:                return Entity + 1;
		case EntityMetadata::WitherSkullInvulnerable:               return Entity;
		case EntityMetadata::FireworkInfo:                          return Entity;
		case EntityMetadata::FireworkBoostedEntityId:               return Entity + 1;
		case EntityMetadata::FireworkFromCrossbow:                  return Entity + 2;
		case EntityMetadata::ItemFrameItem:                         return Entity;
		case EntityMetadata::ItemFrameRotation:                     return Entity + 1;
		case EntityMetadata::ItemItem:                              return Entity;
		case EntityMetadata::LivingActiveHand:                      return Entity;
		case EntityMetadata::LivingHealth:                          return Entity + 1;
		case EntityMetadata::LivingPotionEffectColor:               return Entity + 2;
		case EntityMetadata::LivingPotionEffectAmbient:             return Entity + 3;
		case EntityMetadata::LivingNumberOfArrows:                  return Entity + 4;
			//  LivingStingerCount + 5
			//  LivingSleppingPos + 6
		case EntityMetadata::PlayerAdditionalHearts:                return Living;
		case EntityMetadata::PlayerScore:                           return Living + 1;
		case EntityMetadata::PlayerDisplayedSkinParts:              return Living + 2;
		case EntityMetadata::PlayerMainHand:                        return Living + 3;
			//  PlayerRightShoudlerEntity + 4
			//  PlayerRightShoudlerEntity + 5
		case EntityMetadata::ArmorStandStatus:                      return Living;
		case EntityMetadata::ArmorStandHeadRotation:                return Living + 1;
		case EntityMetadata::ArmorStandBodyRotation:                return Living + 2;
		case EntityMetadata::ArmorStandLeftArmRotation:             return Living + 3;
		case EntityMetadata::ArmorStandRightArmRotation:            return Living + 4;
		case EntityMetadata::ArmorStandLeftLegRotation:             return Living + 5;
		case EntityMetadata::ArmorStandRightLegRotation:            return Living + 6;
		case EntityMetadata::InsentientFlags:                       return Living;
		case EntityMetadata::BatHanging:                            return Insentient;
		case EntityMetadata::AgeableIsBaby:                         return Insentient;
		case EntityMetadata::AbstractHorseFlags:                    return Ageable;
		case EntityMetadata::AbstractHorseOwner:                    return Ageable + 1;
		case EntityMetadata::HorseVariant:                          return AbstractHorse;
		case EntityMetadata::ChestedHorseChested:                   return AbstractHorse;
		case EntityMetadata::LlamaStrength:                         return ChestedHorse;
		case EntityMetadata::LlamaCarpetColor:                      return ChestedHorse + 1;
		case EntityMetadata::LlamaVariant:                          return ChestedHorse + 2;
		case EntityMetadata::PigHasSaddle:                          return Ageable;
		case EntityMetadata::PigTotalCarrotOnAStickBoost:           return Ageable + 1;
		case EntityMetadata::RabbitType:                            return Ageable;
		case EntityMetadata::PolarBearStanding:                     return Ageable;
		case EntityMetadata::SheepFlags:                            return Ageable;
		case EntityMetadata::TameableAnimalFlags:                   return Ageable;
		case EntityMetadata::TameableAnimalOwner:                   return Ageable + 1;
		case EntityMetadata::OcelotType:                            return TameableAnimal;
		case EntityMetadata::WolfDamageTaken:                       return TameableAnimal;
		case EntityMetadata::WolfBegging:                           return TameableAnimal + 1;
		case EntityMetadata::WolfCollarColour:                      return TameableAnimal + 2;
		case EntityMetadata::VillagerProfession:                    return Ageable;
		case EntityMetadata::IronGolemPlayerCreated:                return Insentient;
		case EntityMetadata::ShulkerFacingDirection:                return Insentient;
		case EntityMetadata::ShulkerAttachmentFallingBlockPosition: return Insentient + 1;
		case EntityMetadata::ShulkerShieldHeight:                   return Insentient + 2;
		case EntityMetadata::BlazeOnFire:                           return Insentient;
		case EntityMetadata::CreeperState:                          return Insentient;
		case EntityMetadata::CreeperPowered:                        return Insentient + 1;
		case EntityMetadata::CreeperIgnited:                        return Insentient + 2;
		case EntityMetadata::GuardianStatus:                        return Insentient;
		case EntityMetadata::GuardianTarget:                        return Insentient + 1;
		case EntityMetadata::IllagerFlags:                          return Insentient;
		case EntityMetadata::SpeIlagerSpell:                        return Insentient + 1;
		case EntityMetadata::VexFlags:                              return Insentient;
		case EntityMetadata::AbstractSkeletonArmsSwinging:          return Insentient;
		case EntityMetadata::SpiderClimbing:                        return Insentient;
		case EntityMetadata::WitchAggresive:                        return Insentient;
		case EntityMetadata::WitherFirstHeadTarget:                 return Insentient;
		case EntityMetadata::WitherSecondHeadTarget:                return Insentient + 1;
		case EntityMetadata::WitherThirdHeadTarget:                 return Insentient + 2;
		case EntityMetadata::WitherInvulnerableTimer:               return Insentient + 3;
		case EntityMetadata::ZombieIsBaby:                          return Insentient;
		case EntityMetadata::ZombieHandsRisedUp:                    return Insentient + 2;
		case EntityMetadata::ZombieVillagerConverting:              return Insentient + 4;
		case EntityMetadata::ZombieVillagerProfession:              return Insentient + 5;
		case EntityMetadata::EndermanCarriedBlock:                  return Insentient;
		case EntityMetadata::EndermanScreaming:                     return Insentient + 1;
		case EntityMetadata::EnderDragonDragonPhase:                return Insentient;
		case EntityMetadata::GhastAttacking:                        return Insentient;
		case EntityMetadata::SlimeSize:                             return Insentient;
		case EntityMetadata::MinecartShakingPower:                  return Entity;
		case EntityMetadata::MinecartShakingDirection:              return Entity + 1;
		case EntityMetadata::MinecartShakingMultiplier:             return Entity + 2;
		case EntityMetadata::MinecartBlockIDMeta:                   return Entity + 3;
		case EntityMetadata::MinecartBlockY:                        return Entity + 4;
		case EntityMetadata::MinecartShowBlock:                     return Entity + 5;
		case EntityMetadata::MinecartCommandBlockCommand:           return Minecart;
		case EntityMetadata::MinecartCommandBlockLastOutput:        return Minecart + 1;
		case EntityMetadata::MinecartFurnacePowered:                return Minecart;
		case EntityMetadata::TNTPrimedFuseTime:                     return Entity;
		case EntityMetadata::TridentLoyaltyLevel:                   return Entity + 3;
		case EntityMetadata::MooshroomType:                         return Ageable;
		case EntityMetadata::WitchDrinking:                         return RaidParticipent;

		case EntityMetadata::AreaEffectCloudParticleParameter1:
		case EntityMetadata::AreaEffectCloudParticleParameter2:
		case EntityMetadata::ZombieUnusedWasType: break;

		default:
			break;
	}
	UNREACHABLE("Retrieved invalid metadata for protocol");
}





bool cProtocol_1_17::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
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
		case 0x07: /* ButtonClickC2SPacket */ return false;
		case 0x08: HandlePacketWindowClick(a_ByteBuffer); return true;
		case 0x09: HandlePacketWindowClose(a_ByteBuffer); return true;
		case 0x0A: HandlePacketPluginMessage(a_ByteBuffer); return true;
		case 0x0B: HandlePacketBookUpdate(a_ByteBuffer); return true;  // not fully implemented
		case 0x0C: /* QueryEntityNbtC2SPacket */ return false;
		case 0x0D: HandlePacketUseEntity(a_ByteBuffer); return true;
		case 0x0E: /* Jigsaw generating */ return false;
		case 0x0F: HandlePacketKeepAlive(a_ByteBuffer); return true;
		case 0x10: /* Update difficulty lock */ return false;  // only used in single player
		case 0x11: HandlePacketPlayerPos(a_ByteBuffer); return true;  // PositionAndOnGround
		case 0x12: HandlePacketPlayerPosLook(a_ByteBuffer); return true;  // full
		case 0x13: HandlePacketPlayerLook(a_ByteBuffer); return true;  // LookAndOnGround
		case 0x14: HandlePacketPlayer(a_ByteBuffer); return true;
		case 0x15: HandlePacketVehicleMove(a_ByteBuffer); return true;
		case 0x16: HandlePacketBoatSteer(a_ByteBuffer); return true;
		case 0x17: /* pick from inventory */ return false;
		case 0x18: HandleCraftRecipe(a_ByteBuffer); return true;
		case 0x19: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
		case 0x1A: HandlePacketBlockDig(a_ByteBuffer); return true;
		case 0x1B: /* client command packet */ return false;
		case 0x1C: HandlePacketSteerVehicle(a_ByteBuffer); return true;  // player input packet
		case 0x1D: /* PlayPongC2SPacket */ return false;
		case 0x1E: /* Recipe Category Options */ return false;
		case 0x1F: HandlePacketCraftingBookData(a_ByteBuffer); return true;
		case 0x20: HandlePacketNameItem(a_ByteBuffer); return true;
		case 0x21: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
		case 0x22: HandlePacketAdvancementTab(a_ByteBuffer); return true;
		case 0x23: /* select villager trade */ return false;
		case 0x24: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
		case 0x25: HandlePacketSlotSelect(a_ByteBuffer); return true;
		case 0x26: /* update command block */ return false;
		case 0x27: /* update minecart command block */ return false;
		case 0x28: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
		case 0x29: /* Update jigsaw block */ return false;
		case 0x2A: /* Update structure block */ return false;
		case 0x2B: HandlePacketUpdateSign(a_ByteBuffer); return true;
		case 0x2C: /* Update hand swing */ return false;
		case 0x2D: /* Spectator teleport */ return false;
		case 0x2E: HandlePacketBlockPlace(a_ByteBuffer); return true;
		case 0x2F: HandlePacketUseItem(a_ByteBuffer); return true;
		default: break;
	}
	UNREACHABLE("");
}





void cProtocol_1_17::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
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





void cProtocol_1_17::HandlePacketClientSettings(cByteBuffer & a_ByteBuffer)
{
	#define LEFT_HAND 0
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Locale);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   ViewDistance);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   ChatFlags);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    ChatColors);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   SkinParts);
	HANDLE_READ(a_ByteBuffer, ReadVarInt,        UInt32,  MainHand);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    FilterText);

	m_Client->SetLocale(Locale);
	m_Client->SetViewDistance(ViewDistance);
	m_Client->GetPlayer()->SetSkinParts(SkinParts);
	m_Client->GetPlayer()->SetLeftHanded(MainHand == LEFT_HAND);
	// TODO: Handle chat flags and chat colors
}





void cProtocol_1_17::SendPlayerMoveLook(const Vector3d a_Pos, const float a_Yaw, const float a_Pitch, const bool a_IsRelative)
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
	Pkt.WriteBool(false);  // should dismount

	// This teleport ID hasn't been confirmed yet
	m_IsTeleportIdConfirmed = false;
}





void cProtocol_1_17::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_State == 3);  // In game mode?
	sSetBlockVector vec_copy = sSetBlockVector(a_Changes);

	// Splits the vector into chunk section and send each separately
	std::vector<std::pair<int, sSetBlockVector>> YSplit;

	std::sort(
		vec_copy.begin(), vec_copy.end());
	int OldY = std::numeric_limits<int>::max();
	int Index = -1;
	for (const auto & a_Change : vec_copy)
	{
		int SectionY = a_Change.m_RelY / 16;
		if (OldY != SectionY)
		{
			YSplit.emplace_back(SectionY, sSetBlockVector());
			Index++;
			OldY = SectionY;
			YSplit[static_cast<UInt64>(Index)].second.push_back(a_Change);
		}
		else
		{
			YSplit[static_cast<UInt64>(Index)].second.push_back(a_Change);
		}
	}

	for (const auto & [Y, rel_changes] : YSplit)
	{
		cPacketizer Pkt(*this, pktBlockChanges);
		UInt64 encoded_pos =
			((static_cast<UInt64>(a_ChunkX & 0x3FFFFF)) << 42) |
			((static_cast<UInt64>(a_ChunkZ & 0x3FFFFF)) << 20) |
			(Y & 0xFFFFF);

		Pkt.WriteBEUInt64(encoded_pos);
		Pkt.WriteBool(true);
		Pkt.WriteVarInt32(static_cast<UInt32>(rel_changes.size()));

		for (const auto & Change : rel_changes)
		{
			UInt64 Coords = static_cast<UInt64>(Change.m_RelY % 16 | (Change.m_RelZ << 4) | (Change.m_RelX << 8));
			UInt64 packed = Coords | (GetProtocolBlockType(Change.m_Block) << 12);
			Pkt.WriteVarInt64(packed);
		}
	}
}





void cProtocol_1_17::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktInventorySlot);
	Pkt.WriteBEInt8(a_WindowID);
	Pkt.WriteVarInt32(0);  // revision
	Pkt.WriteBEInt16(a_SlotNum);
	WriteItem(Pkt, a_Item);
}





void cProtocol_1_17::SendRespawn(eDimension a_Dimension)
{
	cPacketizer Pkt(*this, pktRespawn);
	cPlayer * Player = m_Client->GetPlayer();
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
	Pkt.WriteString("overworld");
	Pkt.WriteBEUInt64(0);  // Appears to be a SHA256 od the world seed
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteBool(false);  // debug world
	Pkt.WriteBool(false);  // flat world
	Pkt.WriteBool(true);   // keep player attributes
}





void cProtocol_1_17::WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity) const
{
	if (a_Entity.IsPlayer())
	{
		const auto & Player = static_cast<const cPlayer &>(a_Entity);

		a_Pkt.WriteVarInt32(1);  // Count.
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

		a_Pkt.WriteVarInt32(0);
	}
}





void cProtocol_1_17::SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY)
{
	{
		cPacketizer Pkt(*this, pktMapData);
		Pkt.WriteVarInt32(a_Map.GetID());
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Map.GetScale()));
		Pkt.WriteBool(false);  // Is map locked TODO: Implement map locking
		Pkt.WriteBool(true);
		Pkt.WriteVarInt32(static_cast<UInt32>(a_Map.GetDecorators().size()));
		for (const auto & Decorator : a_Map.GetDecorators())
		{
			Pkt.WriteVarInt32(static_cast<UInt32>(Decorator.GetType()));
			Pkt.WriteBEUInt8(static_cast<UInt8>(Decorator.GetPixelX()));
			Pkt.WriteBEUInt8(static_cast<UInt8>(Decorator.GetPixelZ()));
			Pkt.WriteBEUInt8(static_cast<UInt8>(Decorator.GetRot()));
			Pkt.WriteBool(false);  // TODO: Implement display names
		}
		// TODO: Remove hardcoded values
		Pkt.WriteBEUInt8(128);
		Pkt.WriteBEUInt8(128);
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_DataStartX));
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_DataStartY));
		Pkt.WriteVarInt32(static_cast<UInt32>(a_Map.GetData().size()));
		for (unsigned char itr : a_Map.GetData())
		{
			Pkt.WriteBEUInt8(itr);
		}
	}
}





UInt32 cProtocol_1_17::GetProtocolMobType(eMonsterType a_MobType) const
{
	switch (a_MobType)
	{
		// Map invalid type to Giant for easy debugging (if this ever spawns, something has gone very wrong)
		case mtInvalidType:           return 31;
		case mtAxolotl: return 3;
		case mtBat: return 4;
		case mtBee: return 5;
		case mtBlaze: return 6;
		case mtCat: return 8;
		case mtCaveSpider: return 9;
		case mtChicken: return 10;
		case mtCod: return 11;
		case mtCow: return 12;
		case mtCreeper: return 13;
		case mtDolphin: return 14;
		case mtDonkey: return 15;
		case mtDrowned: return 17;
		case mtElderGuardian: return 18;
		case mtEnderDragon: return 20;
		case mtEnderman: return 21;
		case mtEndermite: return 22;
		case mtEvoker: return 23;
		case mtFox: return 29;
		case mtGhast: return 30;
		case mtGiant: return 31;
		case mtGlowSquid: return 33;
		case mtGoat: return 34;
		case mtGuardian: return 35;
		case mtHoglin: return 36;
		case mtHorse: return 37;
		case mtHusk: return 38;
		case mtIllusioner: return 39;
		case mtLlama: return 46;
		case mtMagmaCube: return 48;
		case mtMule: return 57;
		case mtMooshroom: return 58;
		case mtOcelot: return 59;
		case mtPanda: return 61;
		case mtParrot: return 62;
		case mtPhantom: return 63;
		case mtPig: return 64;
		case mtPiglin: return 65;
		case mtPiglinBrute: return 66;
		case mtPillager: return 67;
		case mtPolarBear: return 68;
		case mtPufferfish: return 70;
		case mtRabbit: return 71;
		case mtRavager: return 72;
		case mtSalmon: return 73;
		case mtSheep: return 74;
		case mtShulker: return 75;
		case mtSilverfish: return 77;
		case mtSkeleton: return 78;
		case mtSkeletonHorse: return 79;
		case mtSlime: return 80;
		case mtSpider: return 85;
		case mtSquid: return 86;
		case mtStray: return 87;
		case mtStrider: return 88;
		case mtTraderLlama: return 94;
		case mtTropicalFish: return 95;
		case mtTurtle: return 96;
		case mtVex: return 97;
		case mtVindicator: return 99;
		case mtWanderingTrader: return 100;
		case mtWitch: return 101;
		case mtWither: return 102;
		case mtWitherSkeleton: return 103;
		case mtWolf: return 105;
		case mtZoglin: return 106;
		case mtZombie: return 107;
		case mtZombieHorse: return 108;
		case mtZombieVillager: return 109;
		case mtZombifiedPiglin: return 110;

		default:                      return 0;
	}
}





int cProtocol_1_17::GetProtocolParticleID(const AString & a_ParticleName) const
{
	static const std::unordered_map<AString, int> ParticleMap
	{
		// Initialize the ParticleMap:
		{ "ambient_entity_effect",    0 },
		{ "angry_villager",           1 },
		{ "barrier",                  2 },
		{ "light",                    3 },
		{ "block",                    4 },
		{ "bubble",                   5 },
		{ "cloud",                    6 },
		{ "crit",                     7 },
		{ "damage_indicator",         8 },
		{ "dragon_breath",            9 },
		{ "dripping_lava",            10 },
		{ "falling_lava",             11 },
		{ "landing_lava",             12 },
		{ "dripping_water",           13 },
		{ "falling_water",            14 },
		{ "dust",                     15 },
		{ "dust_color_transition",    16 },
		{ "effect",                   17 },
		{ "elder_guardian",           18 },
		{ "enchanted_hit",            19 },
		{ "enchant",                  20 },
		{ "end_rod",                  21 },
		{ "entity_effect",            22 },
		{ "explosion_emitter",        23 },
		{ "explosion",                24 },
		{ "falling_dust",             25 },
		{ "firework",                 26 },
		{ "fishing",                  27 },
		{ "flame",                    28 },
		{ "soul_fire_flame",          29 },
		{ "soul",                     30 },
		{ "flash",                    31 },
		{ "happy_villager",           32 },
		{ "composter",                33 },
		{ "heart",                    34 },
		{ "instant_effect",           35 },
		{ "item",                     36 },
		{ "vibration",                37 },
		{ "item_slime",               38 },
		{ "item_snowball",            39 },
		{ "large_smoke",              40 },
		{ "lava",                     41 },
		{ "mycelium",                 42 },
		{ "note",                     43 },
		{ "poof",                     44 },
		{ "portal",                   45 },
		{ "rain",                     46 },
		{ "smoke",                    47 },
		{ "sneeze",                   48 },
		{ "spit",                     49 },
		{ "squid_ink",                50 },
		{ "sweep_attack",             51 },
		{ "totem_of_undying",         52 },
		{ "underwater",               53 },
		{ "splash",                   54 },
		{ "witch",                    55 },
		{ "bubble_pop",               56 },
		{ "current_down",             57 },
		{ "bubble_column_up",         58 },
		{ "nautilus",                 59 },
		{ "dolphin",                  60 },
		{ "campfire_cosy_smoke",      61 },
		{ "campfire_signal_smoke",    62 },
		{ "dripping_honey",           63 },
		{ "falling_honey",            64 },
		{ "landing_honey",            65 },
		{ "falling_nectar",           66 },
		{ "falling_spore_blossom",    67 },
		{ "ash",                      68 },
		{ "crimson_spore",            69 },
		{ "warped_spore",             70 },
		{ "spore_blossom_air",        71 },
		{ "dripping_obsidian_tear",   72 },
		{ "falling_obsidian_tear",    73 },
		{ "landing_obsidian_tear",    74 },
		{ "reverse_portal",           75 },
		{ "white_ash",                76 },
		{ "small_flame",              77 },
		{ "snowflake",                78 },
		{ "dripping_dripstone_lava",  79 },
		{ "falling_dripstone_lava",   80 },
		{ "dripping_dripstone_water", 81 },
		{ "falling_dripstone_water",  82 },
		{ "glow_squid_ink",           83 },
		{ "glow",                     84 },
		{ "wax_on",                   85 },
		{ "wax_off",                  86 },
		{ "electric_spark",           87 },
		{ "scrape",                   88 },
	};


	const auto ParticleName = StrToLower(a_ParticleName);
	const auto FindResult = ParticleMap.find(ParticleName);
	if (FindResult == ParticleMap.end())
	{
		LOGWARNING("Unknown particle: %s", a_ParticleName.c_str());
		// ASSERT(!"Unknown particle");  // some particles where renamed so they throw this error, its disabled
		return 0;
	}

	return FindResult->second;
}





UInt8 cProtocol_1_17::GetProtocolEntityType(const cEntity & a_Entity) const
{
	using Type = cEntity::eEntityType;

	switch (a_Entity.GetEntityType())
	{
		case Type::etEnderCrystal: return 19;
		case Type::etPickup: return 41;
		case Type::etFallingBlock: return 27;
		case Type::etMinecart: return 50;
		case Type::etBoat: return 7;
		case Type::etTNT: return 69;
		case Type::etProjectile:
		{
			using PType = cProjectileEntity::eKind;
			const auto & Projectile = static_cast<const cProjectileEntity &>(a_Entity);

			switch (Projectile.GetProjectileKind())
			{
				case PType::pkArrow: return 2;
				case PType::pkSnowball: return 83;
				case PType::pkEgg: return 89;
				case PType::pkGhastFireball: return 43;
				case PType::pkFireCharge: return 81;
				case PType::pkEnderPearl: return 90;
				case PType::pkExpBottle: return 91;
				case PType::pkSplashPotion: return 92;
				case PType::pkFirework: return 28;
				case PType::pkWitherSkull: return 104;
			}
			break;
		}
		case Type::etFloater: return 112;
		case Type::etItemFrame: return 42;
		case Type::etLeashKnot: return 44;

		// Non-objects must not be sent
		case Type::etEntity:
		case Type::etPlayer:
		case Type::etMonster:
		case Type::etExpOrb:
		case Type::etPainting: break;
	}
	UNREACHABLE("Unhandled entity kind");
}





void cProtocol_1_17::HandlePacketWindowClick(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  WindowID);
	HANDLE_READ(a_ByteBuffer, ReadBEInt16,  Int16,  SlotNum);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  Button);
	HANDLE_READ(a_ByteBuffer, ReadVarInt32,  UInt32,  Mode);
	HANDLE_READ(a_ByteBuffer, ReadVarInt32,  UInt32,  ArrLen);
	std::vector<cItem> items;
	for (UInt32 i = 0; i < ArrLen; ++i)
	{
		cItem Item;
		ReadItem(a_ByteBuffer, Item, 0);
		items.push_back(Item);
	}
	UNREACHABLE("todo parse this packet properly");
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




////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_17_1:

cProtocol::Version cProtocol_1_17_1::GetProtocolVersion() const
{
	return Version::v1_17_1;
}





void cProtocol_1_17_1::SendWholeInventory(const cWindow & a_Window, const cItem & a_CursorStack)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktWindowItems);
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_Window.GetWindowID()));
	Pkt.WriteVarInt32(0);  // revision
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Window.GetNumSlots()));
	cItems Slots;
	a_Window.GetSlots(*(m_Client->GetPlayer()), Slots);
	for (const auto & Slot : Slots)
	{
		WriteItem(Pkt, Slot);
	}  // for itr - Slots[]
	WriteItem(Pkt, a_CursorStack);
}
