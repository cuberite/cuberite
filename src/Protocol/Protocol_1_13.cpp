
// Protocol_1_13.cpp

/*
Implements the 1.13 protocol classes:
- release 1.13 protocol (#393)
- release 1.13.1 protocol (#401)
- release 1.13.2 protocol (#404)
*/

#include "Globals.h"
#include "Protocol_1_13.h"

#include "../Entities/Boat.h"
#include "../Entities/EnderCrystal.h"
#include "../Entities/Minecart.h"
#include "../Entities/Pickup.h"
#include "../Entities/Player.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/FireworkEntity.h"
#include "../Entities/SplashPotionEntity.h"

#include "../Mobs/IncludeAllMonsters.h"

#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../JsonUtils.h"

#include "../Bindings/PluginManager.h"

#include "Palettes/Palette_1_13.h"
#include "Palettes/Palette_1_13_1.h"





#define HANDLE_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	do { \
		if (!ByteBuf.Proc(Var))\
		{\
			return;\
		} \
	} while (false)





#define HANDLE_PACKET_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	do { \
		{ \
			if (!ByteBuf.Proc(Var)) \
			{ \
				ByteBuf.CheckValid(); \
				return false; \
			} \
			ByteBuf.CheckValid(); \
		} \
	} while (false)





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_13:

void cProtocol_1_13::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cPacketizer Pkt(*this, pktBlockChange);
	Pkt.WriteXYZPosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteVarInt32(GetProtocolBlockType(a_BlockType, a_BlockMeta));
}





void cProtocol_1_13::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockChanges);
	Pkt.WriteBEInt32(a_ChunkX);
	Pkt.WriteBEInt32(a_ChunkZ);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Changes.size()));
	for (const auto & Change : a_Changes)
	{
		Int16 Coords = static_cast<Int16>(Change.m_RelY | (Change.m_RelZ << 8) | (Change.m_RelX << 12));
		Pkt.WriteBEInt16(Coords);
		Pkt.WriteVarInt32(GetProtocolBlockType(Change.m_BlockType, Change.m_BlockMeta));
	}  // for itr - a_Changes[]
}





void cProtocol_1_13::SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY)
{
	// TODO
}





void cProtocol_1_13::SendPaintingSpawn(const cPainting & a_Painting)
{
	// TODO
}





void cProtocol_1_13::SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data)
{
	// This packet is unchanged since 1.8
	// However we are hardcoding a string-to-id mapping inside there
	// TODO: make a virtual enum-to-id mapping
}





void cProtocol_1_13::SendScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	// TODO
}





void cProtocol_1_13::SendStatistics(const cStatManager & a_Manager)
{
	ASSERT(m_State == 3);  // In game mode?

	UInt32 Size = 0;
	a_Manager.ForEachStatisticType([this, &Size](const auto & Store)
	{
		for (const auto & Item : Store)
		{
			// Client balks at out-of-range values so there is no good default value
			// We're forced to not send the statistics this protocol version doesn't support

			if (GetProtocolStatisticType(Item.first) != static_cast<UInt32>(-1))
			{
				Size++;
			}
		}
	});

	// No need to check Size != 0
	// Assume that the vast majority of the time there's at least one statistic to send

	cPacketizer Pkt(*this, pktStatistics);
	Pkt.WriteVarInt32(Size);

	a_Manager.ForEachStatisticType([this, &Pkt](const cStatManager::CustomStore & Store)
	{
		for (const auto & Item : Store)
		{
			const auto ID = GetProtocolStatisticType(Item.first);
			if (ID == static_cast<UInt32>(-1))
			{
				// Unsupported, don't send:
				continue;
			}

			Pkt.WriteVarInt32(8);  // "Custom" category
			Pkt.WriteVarInt32(ID);
			Pkt.WriteVarInt32(static_cast<UInt32>(Item.second));
		}
	});
}





void cProtocol_1_13::SendTabCompletionResults(const AStringVector & a_Results)
{
	// TODO
}





void cProtocol_1_13::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	ASSERT(m_State == 3);  // In game mode?
	cPacketizer Pkt(*this, pktUpdateBlockEntity);

	Pkt.WriteXYZPosition64(a_BlockEntity.GetPosX(), a_BlockEntity.GetPosY(), a_BlockEntity.GetPosZ());

	Byte Action = 0;
	switch (a_BlockEntity.GetBlockType())
	{
		case E_BLOCK_MOB_SPAWNER:       Action = 1;  break;  // Update mob spawner spinny mob thing
		case E_BLOCK_COMMAND_BLOCK:     Action = 2;  break;  // Update command block text
		case E_BLOCK_BEACON:            Action = 3;  break;  // Update beacon entity
		case E_BLOCK_HEAD:              Action = 4;  break;  // Update Mobhead entity
		// case E_BLOCK_CONDUIT:        Action = 5;  break;  // Update Conduit entity
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_WALL_BANNER:       Action = 6;  break;  // Update banner entity
		// case Structure Block:        Action = 7;  break;  // Update Structure tile entity
		case E_BLOCK_END_GATEWAY:       Action = 8;  break;  // Update destination for a end gateway entity
		case E_BLOCK_SIGN_POST:         Action = 9;  break;  // Update sign entity
		// case E_BLOCK_SHULKER_BOX:    Action = 10; break;  // sets shulker box - not used just here if anyone is confused from reading the protocol wiki
		case E_BLOCK_BED:               Action = 11; break;  // Update bed color

		case E_BLOCK_ENCHANTMENT_TABLE: Action = 0; break;  // The ones with a action of 0 is just a workaround to send the block entities to a client.
		case E_BLOCK_END_PORTAL:        Action = 0; break;  // Todo: 18.09.2020 - remove this when block entities are transmitted in the ChunkData packet - 12xx12

		default: ASSERT(!"Unhandled or unimplemented BlockEntity update request!"); break;
	}
	Pkt.WriteBEUInt8(Action);

	WriteBlockEntity(Pkt, a_BlockEntity);
}





bool cProtocol_1_13::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	if (m_State != 3)
	{
		return Super::HandlePacket(a_ByteBuffer, a_PacketType);
	}

	// Game
	switch (a_PacketType)
	{
		case 0x00: HandleConfirmTeleport(a_ByteBuffer); return true;
		case 0x05: HandlePacketTabComplete(a_ByteBuffer); return true;
		case 0x02: HandlePacketChatMessage(a_ByteBuffer); return true;
		case 0x03: HandlePacketClientStatus(a_ByteBuffer); return true;
		case 0x04: HandlePacketClientSettings(a_ByteBuffer); return true;
		case 0x06: break;  // Confirm transaction - not used in Cuberite
		case 0x07: HandlePacketEnchantItem(a_ByteBuffer); return true;
		case 0x08: HandlePacketWindowClick(a_ByteBuffer); return true;
		case 0x09: HandlePacketWindowClose(a_ByteBuffer); return true;
		case 0x0a: HandlePacketPluginMessage(a_ByteBuffer); return true;
		case 0x0d: HandlePacketUseEntity(a_ByteBuffer); return true;
		case 0x0e: HandlePacketKeepAlive(a_ByteBuffer); return true;
		case 0x0f: HandlePacketPlayer(a_ByteBuffer); return true;
		case 0x10: HandlePacketPlayerPos(a_ByteBuffer); return true;
		case 0x11: HandlePacketPlayerPosLook(a_ByteBuffer); return true;
		case 0x12: HandlePacketPlayerLook(a_ByteBuffer); return true;
		case 0x13: HandlePacketVehicleMove(a_ByteBuffer); return true;
		case 0x14: HandlePacketBoatSteer(a_ByteBuffer); return true;
		case 0x15: break;  // Pick item - not yet implemented
		case 0x16: break;  // Craft Recipe Request - not yet implemented
		case 0x17: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
		case 0x18: HandlePacketBlockDig(a_ByteBuffer); return true;
		case 0x19: HandlePacketEntityAction(a_ByteBuffer); return true;
		case 0x1a: HandlePacketSteerVehicle(a_ByteBuffer); return true;
		case 0x1b: HandlePacketCraftingBookData(a_ByteBuffer); return true;
		case 0x1d: break;  // Resource pack status - not yet implemented
		case 0x1e: HandlePacketAdvancementTab(a_ByteBuffer); return true;
		case 0x20: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
		case 0x21: HandlePacketSlotSelect(a_ByteBuffer); return true;
		case 0x24: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
		case 0x26: HandlePacketUpdateSign(a_ByteBuffer); return true;
		case 0x27: HandlePacketAnimation(a_ByteBuffer); return true;
		case 0x28: HandlePacketSpectate(a_ByteBuffer); return true;
		case 0x29: HandlePacketBlockPlace(a_ByteBuffer); return true;
		case 0x2a: HandlePacketUseItem(a_ByteBuffer); return true;
	}

	return Super::HandlePacket(a_ByteBuffer, a_PacketType);
}





void cProtocol_1_13::HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Channel);

	// If the plugin channel is recognized vanilla, handle it directly:
	if (Channel.substr(0, 15) == "minecraft:brand")
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Brand);
		m_Client->SetClientBrand(Brand);

		// Send back our brand, including the length:
		SendPluginMessage("minecraft:brand", "\x08""Cuberite");
		return;
	}

	// Read the plugin message and relay to clienthandle:
	AString Data;
	VERIFY(a_ByteBuffer.ReadString(Data, a_ByteBuffer.GetReadableSpace() - 1));  // Always succeeds
	m_Client->HandlePluginMessage(Channel, Data);
}





void cProtocol_1_13::HandlePacketSetBeaconEffect(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, Effect1);
	HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, Effect2);
	m_Client->HandleBeaconSelection(
		static_cast<int>(Effect1), static_cast<int>(Effect2)
	);
}





cProtocol::Version cProtocol_1_13::GetProtocolVersion()
{
	return Version::v1_13;
}





UInt32 cProtocol_1_13::GetPacketID(ePacketType a_PacketType)
{
	switch (a_PacketType)
	{
		case pktAttachEntity:         return 0x46;
		case pktBlockChanges:         return 0x0f;
		case pktCameraSetTo:          return 0x3c;
		case pktChatRaw:              return 0x0e;
		case pktCollectEntity:        return 0x4f;
		case pktDestroyEntity:        return 0x35;
		case pktDisconnectDuringGame: return 0x1b;
		case pktEditSign:             return 0x2c;
		case pktEntityEffect:         return 0x53;
		case pktEntityEquipment:      return 0x42;
		case pktEntityHeadLook:       return 0x39;
		case pktEntityLook:           return 0x2a;
		case pktEntityMeta:           return 0x3f;
		case pktEntityProperties:     return 0x52;
		case pktEntityRelMove:        return 0x28;
		case pktEntityRelMoveLook:    return 0x29;
		case pktEntityStatus:         return 0x1c;
		case pktEntityVelocity:       return 0x41;
		case pktExperience:           return 0x43;
		case pktExplosion:            return 0x1e;
		case pktGameMode:             return 0x20;
		case pktHeldItemChange:       return 0x3d;
		case pktInventorySlot:        return 0x17;
		case pktJoinGame:             return 0x25;
		case pktKeepAlive:            return 0x21;
		case pktLeashEntity:          return 0x40;
		case pktMapData:              return 0x26;
		case pktParticleEffect:       return 0x24;
		case pktPlayerAbilities:      return 0x2e;
		case pktPlayerList:           return 0x30;
		case pktPlayerMaxSpeed:       return 0x52;
		case pktPlayerMoveLook:       return 0x32;
		case pktPluginMessage:        return 0x19;
		case pktRemoveEntityEffect:   return 0x36;
		case pktRespawn:              return 0x38;
		case pktScoreboardObjective:  return 0x45;
		case pktSoundEffect:          return 0x1a;
		case pktSoundParticleEffect:  return 0x23;
		case pktSpawnPosition:        return 0x49;
		case pktTabCompletionResults: return 0x10;
		case pktTeleportEntity:       return 0x50;
		case pktTimeUpdate:           return 0x4a;
		case pktTitle:                return 0x4b;
		case pktUnloadChunk:          return 0x1f;
		case pktUnlockRecipe:         return 0x32;
		case pktUpdateHealth:         return 0x44;
		case pktUpdateScore:          return 0x48;
		case pktUpdateSign:           return GetPacketID(pktUpdateBlockEntity);
		case pktUseBed:               return 0x33;
		case pktWindowClose:          return 0x13;
		case pktWindowItems:          return 0x15;
		case pktWindowOpen:           return 0x14;
		case pktWindowProperty:       return 0x16;
		default: return Super::GetPacketID(a_PacketType);
	}
}





UInt32 cProtocol_1_13::GetProtocolMobType(eMonsterType a_MobType)
{
	switch (a_MobType)
	{
		// Map invalid type to Giant for easy debugging (if this ever spawns, something has gone very wrong)
		case mtInvalidType:           return 27;
		case mtBat:                   return 3;
		case mtBlaze:                 return 4;
		case mtCaveSpider:            return 6;
		case mtChicken:               return 7;
		case mtCow:                   return 9;
		case mtCreeper:               return 10;
		case mtEnderDragon:           return 17;
		case mtEnderman:              return 18;
		case mtGhast:                 return 26;
		case mtGiant:                 return 27;
		case mtGuardian:              return 28;
		case mtHorse:                 return 29;
		case mtIronGolem:             return 80;
		case mtMagmaCube:             return 38;
		case mtMooshroom:             return 47;
		case mtOcelot:                return 48;
		case mtPig:                   return 51;
		case mtRabbit:                return 56;
		case mtSheep:                 return 58;
		case mtSilverfish:            return 61;
		case mtSkeleton:              return 62;
		case mtSlime:                 return 64;
		case mtSnowGolem:             return 66;
		case mtSpider:                return 69;
		case mtSquid:                 return 70;
		case mtVillager:              return 79;
		case mtWitch:                 return 82;
		case mtWither:                return 83;
		case mtWitherSkeleton:        return 84;
		case mtWolf:                  return 86;
		case mtZombie:                return 87;
		case mtZombiePigman:          return 53;
		case mtZombieVillager:        return 89;
	}
	UNREACHABLE("Unsupported mob type");
}





UInt8 cProtocol_1_13::GetEntityMetadataID(EntityMetadata a_Metadata)
{
	const UInt8 Entity = 6;
	const UInt8 Living = Entity + 5;
	const UInt8 Insentient = Living + 1;
	const UInt8 Ageable = Insentient + 1;
	const UInt8 AbstractHorse = Ageable + 2;
	const UInt8 ChestedHorse = AbstractHorse + 1;
	const UInt8 TameableAnimal = Ageable + 2;
	const UInt8 Minecart = Entity + 6;

	switch (a_Metadata)
	{
		case EntityMetadata::EntityFlags:                           return 0;
		case EntityMetadata::EntityAir:                             return 1;
		case EntityMetadata::EntityCustomName:                      return 2;
		case EntityMetadata::EntityCustomNameVisible:               return 3;
		case EntityMetadata::EntitySilent:                          return 4;
		case EntityMetadata::EntityNoGravity:                       return 5;
		case EntityMetadata::PotionThrown:                          return Entity;
		case EntityMetadata::FallingBlockPosition:                  return Entity;
		case EntityMetadata::AreaEffectCloudRadius:                 return Entity;
		case EntityMetadata::AreaEffectCloudColor:                  return Entity + 1;
		case EntityMetadata::AreaEffectCloudSinglePointEffect:      return Entity + 2;
		case EntityMetadata::AreaEffectCloudParticleId:             return Entity + 3;
		case EntityMetadata::ArrowFlags:                            return Entity;
		case EntityMetadata::TippedArrowColor:                      return Entity + 1;
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
		case EntityMetadata::ItemFrameItem:                         return Entity;
		case EntityMetadata::ItemFrameRotation:                     return Entity + 1;
		case EntityMetadata::ItemItem:                              return Entity;
		case EntityMetadata::LivingActiveHand:                      return Entity;
		case EntityMetadata::LivingHealth:                          return Entity + 1;
		case EntityMetadata::LivingPotionEffectColor:               return Entity + 2;
		case EntityMetadata::LivingPotionEffectAmbient:             return Entity + 3;
		case EntityMetadata::LivingNumberOfArrows:                  return Entity + 4;
		case EntityMetadata::PlayerAdditionalHearts:                return Living;
		case EntityMetadata::PlayerScore:                           return Living + 1;
		case EntityMetadata::PlayerDisplayedSkinParts:              return Living + 2;
		case EntityMetadata::PlayerMainHand:                        return Living + 3;
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
		case EntityMetadata::HorseArmour:                           return AbstractHorse + 1;
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

		case EntityMetadata::EntityPose:
		case EntityMetadata::AreaEffectCloudParticleParameter1:
		case EntityMetadata::AreaEffectCloudParticleParameter2:
		case EntityMetadata::AbstractSkeletonArmsSwinging:
		case EntityMetadata::ZombieUnusedWasType: break;
	}

	UNREACHABLE("Retrieved invalid metadata for protocol");
}





UInt8 cProtocol_1_13::GetEntityMetadataID(EntityMetadataType a_FieldType)
{
	switch (a_FieldType)
	{
		case EntityMetadataType::Byte:         return 0;
		case EntityMetadataType::VarInt:       return 1;
		case EntityMetadataType::Float:        return 2;
		case EntityMetadataType::String:       return 3;
		case EntityMetadataType::Chat:         return 4;
		case EntityMetadataType::OptChat:      return 5;
		case EntityMetadataType::Item:         return 6;
		case EntityMetadataType::Boolean:      return 7;
		case EntityMetadataType::Rotation:     return 8;
		case EntityMetadataType::Position:     return 9;
		case EntityMetadataType::OptPosition:  return 10;
		case EntityMetadataType::Direction:    return 11;
		case EntityMetadataType::OptUUID:      return 12;
		case EntityMetadataType::OptBlockID:   return 13;
		case EntityMetadataType::NBT:          return 14;
		case EntityMetadataType::Particle:     return 15;
		case EntityMetadataType::VillagerData: return 16;
		case EntityMetadataType::OptVarInt:    return 17;
		case EntityMetadataType::Pose:         return 18;
	}

	UNREACHABLE("Translated invalid metadata type for protocol");
}





std::pair<short, short> cProtocol_1_13::GetItemFromProtocolID(UInt32 a_ProtocolID)
{
	return PaletteUpgrade::ToItem(Palette_1_13::ToItem(a_ProtocolID));
}





UInt32 cProtocol_1_13::GetProtocolBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta)
{
	return Palette_1_13::FromBlock(PaletteUpgrade::FromBlock(a_BlockType, a_Meta));
}





UInt32 cProtocol_1_13::GetProtocolItemType(short a_ItemID, short a_ItemDamage)
{
	return Palette_1_13::FromItem(PaletteUpgrade::FromItem(a_ItemID, a_ItemDamage));
}





UInt32 cProtocol_1_13::GetProtocolStatisticType(Statistic a_Statistic)
{
	return Palette_1_13::From(a_Statistic);
}





bool cProtocol_1_13::ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes)
{
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt16, Int16, ItemID);
	if (ItemID == -1)
	{
		// The item is empty, no more data follows
		a_Item.Empty();
		return true;
	}

	const auto Translated = GetItemFromProtocolID(ToUnsigned(ItemID));
	a_Item.m_ItemType = Translated.first;
	a_Item.m_ItemDamage = Translated.second;

	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt8, Int8, ItemCount);
	a_Item.m_ItemCount = ItemCount;
	if (ItemCount <= 0)
	{
		a_Item.Empty();
	}

	AString Metadata;
	if (!a_ByteBuffer.ReadString(Metadata, a_ByteBuffer.GetReadableSpace() - a_KeepRemainingBytes - 1) || (Metadata.size() == 0) || (Metadata[0] == 0))
	{
		// No metadata
		return true;
	}

	ParseItemMetadata(a_Item, Metadata);
	return true;
}





void cProtocol_1_13::WriteItem(cPacketizer & a_Pkt, const cItem & a_Item)
{
	short ItemType = a_Item.m_ItemType;
	ASSERT(ItemType >= -1);  // Check validity of packets in debug runtime
	if (ItemType <= 0)
	{
		// Fix, to make sure no invalid values are sent.
		ItemType = -1;
	}

	if (a_Item.IsEmpty())
	{
		a_Pkt.WriteBEInt16(-1);
		return;
	}

	// Normal item
	a_Pkt.WriteBEInt16(static_cast<Int16>(GetProtocolItemType(a_Item.m_ItemType, a_Item.m_ItemDamage)));
	a_Pkt.WriteBEInt8(a_Item.m_ItemCount);

	// TODO: NBT
	a_Pkt.WriteBEInt8(0);
}





void cProtocol_1_13::WriteEntityMetadata(cPacketizer & a_Pkt, const EntityMetadata a_Metadata, const EntityMetadataType a_FieldType)
{
	a_Pkt.WriteBEUInt8(GetEntityMetadataID(a_Metadata));  // Index
	a_Pkt.WriteBEUInt8(GetEntityMetadataID(a_FieldType));  // Type
}





void cProtocol_1_13::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity)
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
			WriteEntityMetadata(a_Pkt, EntityMetadata::EntityCustomName, EntityMetadataType::String);
			a_Pkt.WriteString(Player.GetName());

			WriteEntityMetadata(a_Pkt, EntityMetadata::LivingHealth, EntityMetadataType::Float);
			a_Pkt.WriteBEFloat(static_cast<float>(Player.GetHealth()));

			WriteEntityMetadata(a_Pkt, EntityMetadata::PlayerDisplayedSkinParts, EntityMetadataType::Byte);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Player.GetSkinParts()));

			WriteEntityMetadata(a_Pkt, EntityMetadata::PlayerMainHand, EntityMetadataType::Byte);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Player.GetMainHand()));
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
			WriteEntityMetadata(a_Pkt, EntityMetadata::EnderCrystalBeamTarget, EntityMetadataType::OptPosition);
			a_Pkt.WriteBool(EnderCrystal.DisplaysBeam());
			if (EnderCrystal.DisplaysBeam())
			{
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





void cProtocol_1_13::WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob)
{
	// Living Enitiy Metadata
	if (a_Mob.HasCustomName())
	{
		// TODO: As of 1.9 _all_ entities can have custom names; should this be moved up?
		WriteEntityMetadata(a_Pkt, EntityMetadata::EntityCustomName, EntityMetadataType::OptChat);
		a_Pkt.WriteBool(true);
		a_Pkt.WriteString(a_Mob.GetCustomName());

		WriteEntityMetadata(a_Pkt, EntityMetadata::EntityCustomNameVisible, EntityMetadataType::Boolean);
		a_Pkt.WriteBool(a_Mob.IsCustomNameAlwaysVisible());
	}

	WriteEntityMetadata(a_Pkt, EntityMetadata::LivingHealth, EntityMetadataType::Float);
	a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));

	switch (a_Mob.GetMobType())
	{
		case mtBat:
		{
			auto & Bat = static_cast<const cBat &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::BatHanging, EntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(Bat.IsHanging() ? 1 : 0);
			break;
		}  // case mtBat

		case mtChicken:
		{
			auto & Chicken = static_cast<const cChicken &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Chicken.IsBaby());
			break;
		}  // case mtChicken

		case mtCow:
		{
			auto & Cow = static_cast<const cCow &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Cow.IsBaby());
			break;
		}  // case mtCow

		case mtCreeper:
		{
			auto & Creeper = static_cast<const cCreeper &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::CreeperState, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(Creeper.IsBlowing() ? 1 : static_cast<UInt32>(-1));  // (idle or "blowing")

			WriteEntityMetadata(a_Pkt, EntityMetadata::CreeperPowered, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Creeper.IsCharged());

			WriteEntityMetadata(a_Pkt, EntityMetadata::CreeperIgnited, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Creeper.IsBurnedWithFlintAndSteel());
			break;
		}  // case mtCreeper

		case mtEnderman:
		{
			auto & Enderman = static_cast<const cEnderman &>(a_Mob);
			WriteEntityMetadata(a_Pkt, EntityMetadata::EndermanCarriedBlock, EntityMetadataType::OptBlockID);
			UInt32 Carried = 0;
			Carried |= static_cast<UInt32>(Enderman.GetCarriedBlock() << 4);
			Carried |= Enderman.GetCarriedMeta();
			a_Pkt.WriteVarInt32(Carried);

			WriteEntityMetadata(a_Pkt, EntityMetadata::EndermanScreaming, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Enderman.IsScreaming());
			break;
		}  // case mtEnderman

		case mtGhast:
		{
			auto & Ghast = static_cast<const cGhast &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::GhastAttacking, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Ghast.IsCharging());
			break;
		}  // case mtGhast

		case mtHorse:
		{
			// XXX This behaves incorrectly with different varients; horses have different entity IDs now

			// Abstract horse
			auto & Horse = static_cast<const cHorse &>(a_Mob);

			Int8 Flags = 0;
			if (Horse.IsTame())
			{
				Flags |= 0x02;
			}
			if (Horse.IsSaddled())
			{
				Flags |= 0x04;
			}
			if (Horse.IsInLoveCooldown())
			{
				Flags |= 0x08;
			}
			if (Horse.IsEating())
			{
				Flags |= 0x10;
			}
			if (Horse.IsRearing())
			{
				Flags |= 0x20;
			}
			if (Horse.IsMthOpen())
			{
				Flags |= 0x40;
			}
			WriteEntityMetadata(a_Pkt, EntityMetadata::AbstractHorseFlags, EntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(Flags);

			// Regular horses
			int Appearance = 0;
			Appearance = Horse.GetHorseColor();
			Appearance |= Horse.GetHorseStyle() << 8;
			WriteEntityMetadata(a_Pkt, EntityMetadata::HorseVariant, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Appearance));  // Color / style

			WriteEntityMetadata(a_Pkt, EntityMetadata::HorseArmour, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Horse.GetHorseArmour()));

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Horse.IsBaby());
			break;
		}  // case mtHorse

		case mtMagmaCube:
		{
			auto & MagmaCube = static_cast<const cMagmaCube &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::SlimeSize, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(MagmaCube.GetSize()));
			break;
		}  // case mtMagmaCube

		case mtOcelot:
		{
			auto & Ocelot = static_cast<const cOcelot &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Ocelot.IsBaby());

			Int8 OcelotStatus = 0;
			if (Ocelot.IsSitting())
			{
				OcelotStatus |= 0x1;
			}
			if (Ocelot.IsTame())
			{
				OcelotStatus |= 0x4;
			}
			WriteEntityMetadata(a_Pkt, EntityMetadata::TameableAnimalFlags, EntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(OcelotStatus);

			WriteEntityMetadata(a_Pkt, EntityMetadata::OcelotType, EntityMetadataType::Byte);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Ocelot.GetOcelotType()));

			break;
		}  // case mtOcelot

		case mtPig:
		{
			auto & Pig = static_cast<const cPig &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Pig.IsBaby());

			WriteEntityMetadata(a_Pkt, EntityMetadata::PigHasSaddle, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Pig.IsSaddled());

			// PIG_TOTAL_CARROT_ON_A_STICK_BOOST in 1.11.1 only
			break;
		}  // case mtPig

		case mtRabbit:
		{
			auto & Rabbit = static_cast<const cRabbit &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Rabbit.IsBaby());

			WriteEntityMetadata(a_Pkt, EntityMetadata::RabbitType, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Rabbit.GetRabbitType()));
			break;
		}  // case mtRabbit

		case mtSheep:
		{
			auto & Sheep = static_cast<const cSheep &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Sheep.IsBaby());

			Int8 SheepMetadata = 0;
			SheepMetadata = static_cast<Int8>(Sheep.GetFurColor());
			if (Sheep.IsSheared())
			{
				SheepMetadata |= 0x10;
			}
			WriteEntityMetadata(a_Pkt, EntityMetadata::SheepFlags, EntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(SheepMetadata);
			break;
		}  // case mtSheep

		case mtSlime:
		{
			auto & Slime = static_cast<const cSlime &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::SlimeSize, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Slime.GetSize()));
			break;
		}  // case mtSlime

		case mtVillager:
		{
			auto & Villager = static_cast<const cVillager &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Villager.IsBaby());

			WriteEntityMetadata(a_Pkt, EntityMetadata::VillagerProfession, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Villager.GetVilType()));
			break;
		}  // case mtVillager

		case mtWitch:
		{
			auto & Witch = static_cast<const cWitch &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::WitchAggresive, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Witch.IsAngry());
			break;
		}  // case mtWitch

		case mtWither:
		{
			auto & Wither = static_cast<const cWither &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::WitherInvulnerableTimer, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(Wither.GetWitherInvulnerableTicks());

			// TODO: Use boss bar packet for health
			break;
		}  // case mtWither

		case mtWolf:
		{
			auto & Wolf = static_cast<const cWolf &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Wolf.IsBaby());

			Int8 WolfStatus = 0;
			if (Wolf.IsSitting())
			{
				WolfStatus |= 0x1;
			}
			if (Wolf.IsAngry())
			{
				WolfStatus |= 0x2;
			}
			if (Wolf.IsTame())
			{
				WolfStatus |= 0x4;
			}
			WriteEntityMetadata(a_Pkt, EntityMetadata::TameableAnimalFlags, EntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(WolfStatus);

			WriteEntityMetadata(a_Pkt, EntityMetadata::WolfDamageTaken, EntityMetadataType::Float);
			a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));  // TODO Not use the current health

			WriteEntityMetadata(a_Pkt, EntityMetadata::WolfBegging, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Wolf.IsBegging());

			WriteEntityMetadata(a_Pkt, EntityMetadata::WolfCollarColour, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Wolf.GetCollarColor()));
			break;
		}  // case mtWolf

		case mtZombie:
		{
			// XXX Zombies were also split into new sublcasses; this doesn't handle that.

			auto & Zombie = static_cast<const cZombie &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::ZombieIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Zombie.IsBaby());
			break;
		}  // case mtZombie

		case mtZombiePigman:
		{
			auto & ZombiePigman = static_cast<const cZombiePigman &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(ZombiePigman.IsBaby());
			break;
		}  // case mtZombiePigman

		case mtBlaze:
		case mtEnderDragon:
		case mtGuardian:
		case mtIronGolem:
		case mtSnowGolem:
		case mtSpider:
		case mtZombieVillager:
		{
			// TODO: Mobs with extra fields that aren't implemented
			break;
		}

		case mtMooshroom:
		case mtCaveSpider:
		{
			// Not mentioned on http://wiki.vg/Entities
			break;
		}

		case mtGiant:
		case mtSilverfish:
		case mtSkeleton:
		case mtSquid:
		case mtWitherSkeleton:
		{
			// Mobs with no extra fields
			break;
		}

		case mtInvalidType:
		{
			ASSERT(!"cProtocol_1_13::WriteMobMetadata: Recieved mob of invalid type");
			break;
		}
	}  // switch (a_Mob.GetType())
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_13_1:

cProtocol::Version cProtocol_1_13_1::GetProtocolVersion()
{
	return Version::v1_13_1;
}





std::pair<short, short> cProtocol_1_13_1::GetItemFromProtocolID(UInt32 a_ProtocolID)
{
	return PaletteUpgrade::ToItem(Palette_1_13_1::ToItem(a_ProtocolID));
}





UInt32 cProtocol_1_13_1::GetProtocolBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta)
{
	return Palette_1_13_1::FromBlock(PaletteUpgrade::FromBlock(a_BlockType, a_Meta));
}





UInt32 cProtocol_1_13_1::GetProtocolItemType(short a_ItemID, short a_ItemDamage)
{
	return Palette_1_13_1::FromItem(PaletteUpgrade::FromItem(a_ItemID, a_ItemDamage));
}





UInt32 cProtocol_1_13_1::GetProtocolStatisticType(Statistic a_Statistic)
{
	return Palette_1_13_1::From(a_Statistic);
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_13_2:

cProtocol::Version cProtocol_1_13_2::GetProtocolVersion()
{
	return Version::v1_13_2;
}





bool cProtocol_1_13_2::ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes)
{
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBool, bool, Present);
	if (!Present)
	{
		// The item is empty, no more data follows
		a_Item.Empty();
		return true;
	}

	HANDLE_PACKET_READ(a_ByteBuffer, ReadVarInt32, UInt32, ItemID);
	const auto Translated = GetItemFromProtocolID(ItemID);
	a_Item.m_ItemType = Translated.first;
	a_Item.m_ItemDamage = Translated.second;

	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt8, Int8, ItemCount);
	a_Item.m_ItemCount = ItemCount;
	if (ItemCount <= 0)
	{
		a_Item.Empty();
	}

	AString Metadata;
	if (!a_ByteBuffer.ReadString(Metadata, a_ByteBuffer.GetReadableSpace() - a_KeepRemainingBytes - 1) || (Metadata.size() == 0) || (Metadata[0] == 0))
	{
		// No metadata
		return true;
	}

	ParseItemMetadata(a_Item, Metadata);
	return true;
}





void cProtocol_1_13_2::WriteItem(cPacketizer & a_Pkt, const cItem & a_Item)
{
	short ItemType = a_Item.m_ItemType;
	ASSERT(ItemType >= -1);  // Check validity of packets in debug runtime
	if (ItemType <= 0)
	{
		// Fix, to make sure no invalid values are sent.
		ItemType = -1;
	}

	if (a_Item.IsEmpty())
	{
		a_Pkt.WriteBool(false);
		return;
	}

	// Item present
	a_Pkt.WriteBool(true);

	// Normal item
	a_Pkt.WriteVarInt32(GetProtocolItemType(a_Item.m_ItemType, a_Item.m_ItemDamage));
	a_Pkt.WriteBEInt8(a_Item.m_ItemCount);

	// TODO: NBT
	a_Pkt.WriteBEInt8(0);
}
