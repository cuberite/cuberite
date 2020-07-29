
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
	SendBlockChange<&Palette_1_13::FromBlock>(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
}





template <auto Palette>
void cProtocol_1_13::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockChange);
	Pkt.WriteXYZPosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteVarInt32(Palette(PaletteUpgrade::FromBlock(a_BlockType, a_BlockMeta)));
}





void cProtocol_1_13::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	SendBlockChanges<&Palette_1_13::FromBlock>(a_ChunkX, a_ChunkZ, a_Changes);
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





void cProtocol_1_13::SendPluginMessage(const AString & a_Channel, const AString & a_Message)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPluginMessage);
	Pkt.WriteString(a_Channel);
	Pkt.WriteString(a_Message);
}





void cProtocol_1_13::SendScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	// TODO
}





void cProtocol_1_13::SendStatistics(const cStatManager & a_Manager)
{
	// TODO
}





void cProtocol_1_13::SendTabCompletionResults(const AStringVector & a_Results)
{
	// TODO
}





void cProtocol_1_13::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	// TODO
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
		SendPluginMessage("minecraft:brand", "Cuberite");
		return;
	}

	// Read the plugin message and relay to clienthandle:
	AString Data;
	VERIFY(a_ByteBuffer.ReadString(Data, a_ByteBuffer.GetReadableSpace() - 1));  // Always succeeds
	m_Client->HandlePluginMessage(Channel, Data);
}





cProtocol::Version cProtocol_1_13::GetProtocolVersion()
{
	return Version::Version_1_13;
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





UInt8 cProtocol_1_13::GetEntityMetadataID(eEntityMetadata a_Metadata)
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
		case eEntityMetadata::EntityFlags:                           return 0;
		case eEntityMetadata::EntityAir:                             return 1;
		case eEntityMetadata::EntityCustomName:                      return 2;
		case eEntityMetadata::EntityCustomNameVisible:               return 3;
		case eEntityMetadata::EntitySilent:                          return 4;
		case eEntityMetadata::EntityNoGravity:                       return 5;
		case eEntityMetadata::PotionThrown:                          return Entity;
		case eEntityMetadata::FallingBlockPosition:                  return Entity;
		case eEntityMetadata::AreaEffectCloudRadius:                 return Entity;
		case eEntityMetadata::AreaEffectCloudColor:                  return Entity + 1;
		case eEntityMetadata::AreaEffectCloudSinglePointEffect:      return Entity + 2;
		case eEntityMetadata::AreaEffectCloudParticleId:             return Entity + 3;
		case eEntityMetadata::ArrowFlags:                            return Entity;
		case eEntityMetadata::TippedArrowColor:                      return Entity + 1;
		case eEntityMetadata::BoatLastHitTime:                       return Entity;
		case eEntityMetadata::BoatForwardDirection:                  return Entity + 1;
		case eEntityMetadata::BoatDamageTaken:                       return Entity + 2;
		case eEntityMetadata::BoatType:                              return Entity + 3;
		case eEntityMetadata::BoatLeftPaddleTurning:                 return Entity + 4;
		case eEntityMetadata::BoatRightPaddleTurning:                return Entity + 5;
		case eEntityMetadata::BoatSplashTimer:                       return Entity + 6;
		case eEntityMetadata::EnderCrystalBeamTarget:                return Entity;
		case eEntityMetadata::EnderCrystalShowBottom:                return Entity + 1;
		case eEntityMetadata::WitherSkullInvulnerable:               return Entity;
		case eEntityMetadata::FireworkInfo:                          return Entity;
		case eEntityMetadata::FireworkBoostedEntityId:               return Entity + 1;
		case eEntityMetadata::ItemFrameItem:                         return Entity;
		case eEntityMetadata::ItemFrameRotation:                     return Entity + 1;
		case eEntityMetadata::ItemItem:                              return Entity;
		case eEntityMetadata::LivingActiveHand:                      return Entity;
		case eEntityMetadata::LivingHealth:                          return Entity + 1;
		case eEntityMetadata::LivingPotionEffectColor:               return Entity + 2;
		case eEntityMetadata::LivingPotionEffectAmbient:             return Entity + 3;
		case eEntityMetadata::LivingNumberOfArrows:                  return Entity + 4;
		case eEntityMetadata::PlayerAdditionalHearts:                return Living;
		case eEntityMetadata::PlayerScore:                           return Living + 1;
		case eEntityMetadata::PlayerDisplayedSkinParts:              return Living + 2;
		case eEntityMetadata::PlayerMainHand:                        return Living + 3;
		case eEntityMetadata::ArmorStandStatus:                      return Living;
		case eEntityMetadata::ArmorStandHeadRotation:                return Living + 1;
		case eEntityMetadata::ArmorStandBodyRotation:                return Living + 2;
		case eEntityMetadata::ArmorStandLeftArmRotation:             return Living + 3;
		case eEntityMetadata::ArmorStandRightArmRotation:            return Living + 4;
		case eEntityMetadata::ArmorStandLeftLegRotation:             return Living + 5;
		case eEntityMetadata::ArmorStandRightLegRotation:            return Living + 6;
		case eEntityMetadata::InsentientFlags:                       return Living;
		case eEntityMetadata::BatHanging:                            return Insentient;
		case eEntityMetadata::AgeableIsBaby:                         return Insentient;
		case eEntityMetadata::AbstractHorseFlags:                    return Ageable;
		case eEntityMetadata::AbstractHorseOwner:                    return Ageable + 1;
		case eEntityMetadata::HorseVariant:                          return AbstractHorse;
		case eEntityMetadata::HorseArmour:                           return AbstractHorse + 1;
		case eEntityMetadata::ChestedHorseChested:                   return AbstractHorse;
		case eEntityMetadata::LlamaStrength:                         return ChestedHorse;
		case eEntityMetadata::LlamaCarpetColor:                      return ChestedHorse + 1;
		case eEntityMetadata::LlamaVariant:                          return ChestedHorse + 2;
		case eEntityMetadata::PigHasSaddle:                          return Ageable;
		case eEntityMetadata::PigTotalCarrotOnAStickBoost:           return Ageable + 1;
		case eEntityMetadata::RabbitType:                            return Ageable;
		case eEntityMetadata::PolarBearStanding:                     return Ageable;
		case eEntityMetadata::SheepFlags:                            return Ageable;
		case eEntityMetadata::TameableAnimalFlags:                   return Ageable;
		case eEntityMetadata::TameableAnimalOwner:                   return Ageable + 1;
		case eEntityMetadata::OcelotType:                            return TameableAnimal;
		case eEntityMetadata::WolfDamageTaken:                       return TameableAnimal;
		case eEntityMetadata::WolfBegging:                           return TameableAnimal + 1;
		case eEntityMetadata::WolfCollarColour:                      return TameableAnimal + 2;
		case eEntityMetadata::VillagerProfession:                    return Ageable;
		case eEntityMetadata::IronGolemPlayerCreated:                return Insentient;
		case eEntityMetadata::ShulkerFacingDirection:                return Insentient;
		case eEntityMetadata::ShulkerAttachmentFallingBlockPosition: return Insentient + 1;
		case eEntityMetadata::ShulkerShieldHeight:                   return Insentient + 2;
		case eEntityMetadata::BlazeOnFire:                           return Insentient;
		case eEntityMetadata::CreeperState:                          return Insentient;
		case eEntityMetadata::CreeperPowered:                        return Insentient + 1;
		case eEntityMetadata::CreeperIgnited:                        return Insentient + 2;
		case eEntityMetadata::GuardianStatus:                        return Insentient;
		case eEntityMetadata::GuardianTarget:                        return Insentient + 1;
		case eEntityMetadata::IllagerFlags:                          return Insentient;
		case eEntityMetadata::SpeIlagerSpell:                        return Insentient + 1;
		case eEntityMetadata::VexFlags:                              return Insentient;
		case eEntityMetadata::SpiderClimbing:                        return Insentient;
		case eEntityMetadata::WitchAggresive:                        return Insentient;
		case eEntityMetadata::WitherFirstHeadTarget:                 return Insentient;
		case eEntityMetadata::WitherSecondHeadTarget:                return Insentient + 1;
		case eEntityMetadata::WitherThirdHeadTarget:                 return Insentient + 2;
		case eEntityMetadata::WitherInvulnerableTimer:               return Insentient + 3;
		case eEntityMetadata::ZombieIsBaby:                          return Insentient;
		case eEntityMetadata::ZombieHandsRisedUp:                    return Insentient + 2;
		case eEntityMetadata::ZombieVillagerConverting:              return Insentient + 4;
		case eEntityMetadata::ZombieVillagerProfession:              return Insentient + 5;
		case eEntityMetadata::EndermanCarriedBlock:                  return Insentient;
		case eEntityMetadata::EndermanScreaming:                     return Insentient + 1;
		case eEntityMetadata::EnderDragonDragonPhase:                return Insentient;
		case eEntityMetadata::GhastAttacking:                        return Insentient;
		case eEntityMetadata::SlimeSize:                             return Insentient;
		case eEntityMetadata::MinecartShakingPower:                  return Entity;
		case eEntityMetadata::MinecartShakingDirection:              return Entity + 1;
		case eEntityMetadata::MinecartShakingMultiplier:             return Entity + 2;
		case eEntityMetadata::MinecartBlockIDMeta:                   return Entity + 3;
		case eEntityMetadata::MinecartBlockY:                        return Entity + 4;
		case eEntityMetadata::MinecartShowBlock:                     return Entity + 5;
		case eEntityMetadata::MinecartCommandBlockCommand:           return Minecart;
		case eEntityMetadata::MinecartCommandBlockLastOutput:        return Minecart + 1;
		case eEntityMetadata::MinecartFurnacePowered:                return Minecart;
		case eEntityMetadata::TNTPrimedFuseTime:                     return Entity;

		case eEntityMetadata::EntityPose:
		case eEntityMetadata::AreaEffectCloudParticleParameter1:
		case eEntityMetadata::AreaEffectCloudParticleParameter2:
		case eEntityMetadata::AbstractSkeletonArmsSwinging:
		case eEntityMetadata::ZombieUnusedWasType: break;
	}

	UNREACHABLE("Retrieved invalid metadata for protocol");
}





UInt8 cProtocol_1_13::GetEntityMetadataID(eEntityMetadataType a_FieldType)
{
	switch (a_FieldType)
	{
		case eEntityMetadataType::Byte:         return 0;
		case eEntityMetadataType::VarInt:       return 1;
		case eEntityMetadataType::Float:        return 2;
		case eEntityMetadataType::String:       return 3;
		case eEntityMetadataType::Chat:         return 4;
		case eEntityMetadataType::OptChat:      return 5;
		case eEntityMetadataType::Item:         return 6;
		case eEntityMetadataType::Boolean:      return 7;
		case eEntityMetadataType::Rotation:     return 8;
		case eEntityMetadataType::Position:     return 9;
		case eEntityMetadataType::OptPosition:  return 10;
		case eEntityMetadataType::Direction:    return 11;
		case eEntityMetadataType::OptUUID:      return 12;
		case eEntityMetadataType::OptBlockID:   return 13;
		case eEntityMetadataType::NBT:          return 14;
		case eEntityMetadataType::Particle:     return 15;
		case eEntityMetadataType::VillagerData: return 16;
		case eEntityMetadataType::OptVarInt:    return 17;
		case eEntityMetadataType::Pose:         return 18;
	}

	UNREACHABLE("Translated invalid metadata type for protocol");
}





std::pair<short, short> cProtocol_1_13::GetItemFromProtocolID(UInt32 a_ProtocolID)
{
	return PaletteUpgrade::ToItem(Palette_1_13::ToItem(a_ProtocolID));
}





UInt32 cProtocol_1_13::GetProtocolIDFromItem(short a_ItemID, short a_ItemDamage)
{
	return Palette_1_13::FromItem(PaletteUpgrade::FromItem(a_ItemID, a_ItemDamage));
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
	a_Pkt.WriteBEInt16(GetProtocolIDFromItem(a_Item.m_ItemType, a_Item.m_ItemDamage));
	a_Pkt.WriteBEInt8(a_Item.m_ItemCount);

	// TODO: NBT
	a_Pkt.WriteBEInt8(0);
}





void cProtocol_1_13::WriteEntityMetadata(cPacketizer & a_Pkt, const eEntityMetadata a_Metadata, const eEntityMetadataType a_FieldType)
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

	WriteEntityMetadata(a_Pkt, eEntityMetadata::EntityFlags, eEntityMetadataType::Byte);
	a_Pkt.WriteBEInt8(Flags);

	switch (a_Entity.GetEntityType())
	{
		case cEntity::etPlayer:
		{
			auto & Player = static_cast<const cPlayer &>(a_Entity);

			// TODO Set player custom name to their name.
			// Then it's possible to move the custom name of mobs to the entities
			// and to remove the "special" player custom name.
			WriteEntityMetadata(a_Pkt, eEntityMetadata::EntityCustomName, eEntityMetadataType::String);
			a_Pkt.WriteString(Player.GetName());

			WriteEntityMetadata(a_Pkt, eEntityMetadata::LivingHealth, eEntityMetadataType::Float);
			a_Pkt.WriteBEFloat(static_cast<float>(Player.GetHealth()));

			WriteEntityMetadata(a_Pkt, eEntityMetadata::PlayerDisplayedSkinParts, eEntityMetadataType::Byte);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Player.GetSkinParts()));

			WriteEntityMetadata(a_Pkt, eEntityMetadata::PlayerMainHand, eEntityMetadataType::Byte);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Player.GetMainHand()));
			break;
		}
		case cEntity::etPickup:
		{
			WriteEntityMetadata(a_Pkt, eEntityMetadata::ItemItem, eEntityMetadataType::Item);
			WriteItem(a_Pkt, static_cast<const cPickup &>(a_Entity).GetItem());
			break;
		}
		case cEntity::etMinecart:
		{
			WriteEntityMetadata(a_Pkt, eEntityMetadata::MinecartShakingPower, eEntityMetadataType::VarInt);

			// The following expression makes Minecarts shake more with less health or higher damage taken
			auto & Minecart = static_cast<const cMinecart &>(a_Entity);
			auto maxHealth = a_Entity.GetMaxHealth();
			auto curHealth = a_Entity.GetHealth();
			a_Pkt.WriteVarInt32(static_cast<UInt32>((maxHealth - curHealth) * Minecart.LastDamage() * 4));

			WriteEntityMetadata(a_Pkt, eEntityMetadata::MinecartShakingDirection, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(1);  // (doesn't seem to effect anything)

			WriteEntityMetadata(a_Pkt, eEntityMetadata::MinecartShakingMultiplier, eEntityMetadataType::Float);
			a_Pkt.WriteBEFloat(static_cast<float>(Minecart.LastDamage() + 10));  // or damage taken

			if (Minecart.GetPayload() == cMinecart::mpNone)
			{
				auto & RideableMinecart = static_cast<const cRideableMinecart &>(Minecart);
				const cItem & MinecartContent = RideableMinecart.GetContent();
				if (!MinecartContent.IsEmpty())
				{
					WriteEntityMetadata(a_Pkt, eEntityMetadata::MinecartBlockIDMeta, eEntityMetadataType::VarInt);
					int Content = MinecartContent.m_ItemType;
					Content |= MinecartContent.m_ItemDamage << 8;
					a_Pkt.WriteVarInt32(static_cast<UInt32>(Content));

					WriteEntityMetadata(a_Pkt, eEntityMetadata::MinecartBlockY, eEntityMetadataType::VarInt);
					a_Pkt.WriteVarInt32(static_cast<UInt32>(RideableMinecart.GetBlockHeight()));

					WriteEntityMetadata(a_Pkt, eEntityMetadata::MinecartShowBlock, eEntityMetadataType::Boolean);
					a_Pkt.WriteBool(true);
				}
			}
			else if (Minecart.GetPayload() == cMinecart::mpFurnace)
			{
				WriteEntityMetadata(a_Pkt, eEntityMetadata::MinecartFurnacePowered, eEntityMetadataType::Boolean);
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
					WriteEntityMetadata(a_Pkt, eEntityMetadata::ArrowFlags, eEntityMetadataType::Byte);
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

			WriteEntityMetadata(a_Pkt, eEntityMetadata::BoatLastHitTime, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Boat.GetLastDamage()));

			WriteEntityMetadata(a_Pkt, eEntityMetadata::BoatForwardDirection, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Boat.GetForwardDirection()));

			WriteEntityMetadata(a_Pkt, eEntityMetadata::BoatDamageTaken, eEntityMetadataType::Float);
			a_Pkt.WriteBEFloat(Boat.GetDamageTaken());

			WriteEntityMetadata(a_Pkt, eEntityMetadata::BoatType, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Boat.GetMaterial()));

			WriteEntityMetadata(a_Pkt, eEntityMetadata::BoatRightPaddleTurning, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Boat.IsRightPaddleUsed());

			WriteEntityMetadata(a_Pkt, eEntityMetadata::BoatLeftPaddleTurning, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(static_cast<bool>(Boat.IsLeftPaddleUsed()));

			WriteEntityMetadata(a_Pkt, eEntityMetadata::BoatSplashTimer, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(0);

			break;
		}  // case etBoat

		case cEntity::etItemFrame:
		{
			// TODO
			break;
		}  // case etItemFrame

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
		WriteEntityMetadata(a_Pkt, eEntityMetadata::EntityCustomName, eEntityMetadataType::OptChat);
		a_Pkt.WriteBool(true);
		a_Pkt.WriteString(a_Mob.GetCustomName());

		WriteEntityMetadata(a_Pkt, eEntityMetadata::EntityCustomNameVisible, eEntityMetadataType::Boolean);
		a_Pkt.WriteBool(a_Mob.IsCustomNameAlwaysVisible());
	}

	WriteEntityMetadata(a_Pkt, eEntityMetadata::LivingHealth, eEntityMetadataType::Float);
	a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));

	switch (a_Mob.GetMobType())
	{
		case mtBat:
		{
			auto & Bat = static_cast<const cBat &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::BatHanging, eEntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(Bat.IsHanging() ? 1 : 0);
			break;
		}  // case mtBat

		case mtChicken:
		{
			auto & Chicken = static_cast<const cChicken &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::AgeableIsBaby, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Chicken.IsBaby());
			break;
		}  // case mtChicken

		case mtCow:
		{
			auto & Cow = static_cast<const cCow &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::AgeableIsBaby, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Cow.IsBaby());
			break;
		}  // case mtCow

		case mtCreeper:
		{
			auto & Creeper = static_cast<const cCreeper &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::CreeperState, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(Creeper.IsBlowing() ? 1 : static_cast<UInt32>(-1));  // (idle or "blowing")

			WriteEntityMetadata(a_Pkt, eEntityMetadata::CreeperPowered, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Creeper.IsCharged());

			WriteEntityMetadata(a_Pkt, eEntityMetadata::CreeperIgnited, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Creeper.IsBurnedWithFlintAndSteel());
			break;
		}  // case mtCreeper

		case mtEnderman:
		{
			auto & Enderman = static_cast<const cEnderman &>(a_Mob);
			WriteEntityMetadata(a_Pkt, eEntityMetadata::EndermanCarriedBlock, eEntityMetadataType::OptBlockID);
			UInt32 Carried = 0;
			Carried |= static_cast<UInt32>(Enderman.GetCarriedBlock() << 4);
			Carried |= Enderman.GetCarriedMeta();
			a_Pkt.WriteVarInt32(Carried);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::EndermanScreaming, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Enderman.IsScreaming());
			break;
		}  // case mtEnderman

		case mtGhast:
		{
			auto & Ghast = static_cast<const cGhast &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::GhastAttacking, eEntityMetadataType::Boolean);
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
			WriteEntityMetadata(a_Pkt, eEntityMetadata::AbstractHorseFlags, eEntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(Flags);

			// Regular horses
			int Appearance = 0;
			Appearance = Horse.GetHorseColor();
			Appearance |= Horse.GetHorseStyle() << 8;
			WriteEntityMetadata(a_Pkt, eEntityMetadata::HorseVariant, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Appearance));  // Color / style

			WriteEntityMetadata(a_Pkt, eEntityMetadata::HorseArmour, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Horse.GetHorseArmour()));

			WriteEntityMetadata(a_Pkt, eEntityMetadata::AgeableIsBaby, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Horse.IsBaby());
			break;
		}  // case mtHorse

		case mtMagmaCube:
		{
			auto & MagmaCube = static_cast<const cMagmaCube &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::SlimeSize, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(MagmaCube.GetSize()));
			break;
		}  // case mtMagmaCube

		case mtOcelot:
		{
			auto & Ocelot = static_cast<const cOcelot &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::AgeableIsBaby, eEntityMetadataType::Boolean);
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
			WriteEntityMetadata(a_Pkt, eEntityMetadata::TameableAnimalFlags, eEntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(OcelotStatus);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::OcelotType, eEntityMetadataType::Byte);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Ocelot.GetOcelotType()));

			break;
		}  // case mtOcelot

		case mtPig:
		{
			auto & Pig = static_cast<const cPig &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::AgeableIsBaby, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Pig.IsBaby());

			WriteEntityMetadata(a_Pkt, eEntityMetadata::PigHasSaddle, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Pig.IsSaddled());

			// PIG_TOTAL_CARROT_ON_A_STICK_BOOST in 1.11.1 only
			break;
		}  // case mtPig

		case mtRabbit:
		{
			auto & Rabbit = static_cast<const cRabbit &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::AgeableIsBaby, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Rabbit.IsBaby());

			WriteEntityMetadata(a_Pkt, eEntityMetadata::RabbitType, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Rabbit.GetRabbitType()));
			break;
		}  // case mtRabbit

		case mtSheep:
		{
			auto & Sheep = static_cast<const cSheep &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::AgeableIsBaby, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Sheep.IsBaby());

			Int8 SheepMetadata = 0;
			SheepMetadata = static_cast<Int8>(Sheep.GetFurColor());
			if (Sheep.IsSheared())
			{
				SheepMetadata |= 0x10;
			}
			WriteEntityMetadata(a_Pkt, eEntityMetadata::SheepFlags, eEntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(SheepMetadata);
			break;
		}  // case mtSheep

		case mtSlime:
		{
			auto & Slime = static_cast<const cSlime &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::SlimeSize, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Slime.GetSize()));
			break;
		}  // case mtSlime

		case mtVillager:
		{
			auto & Villager = static_cast<const cVillager &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::AgeableIsBaby, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Villager.IsBaby());

			WriteEntityMetadata(a_Pkt, eEntityMetadata::VillagerProfession, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Villager.GetVilType()));
			break;
		}  // case mtVillager

		case mtWitch:
		{
			auto & Witch = static_cast<const cWitch &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::WitchAggresive, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Witch.IsAngry());
			break;
		}  // case mtWitch

		case mtWither:
		{
			auto & Wither = static_cast<const cWither &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::WitherInvulnerableTimer, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(Wither.GetWitherInvulnerableTicks());

			// TODO: Use boss bar packet for health
			break;
		}  // case mtWither

		case mtWolf:
		{
			auto & Wolf = static_cast<const cWolf &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::AgeableIsBaby, eEntityMetadataType::Boolean);
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
			WriteEntityMetadata(a_Pkt, eEntityMetadata::TameableAnimalFlags, eEntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(WolfStatus);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::WolfDamageTaken, eEntityMetadataType::Float);
			a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));  // TODO Not use the current health

			WriteEntityMetadata(a_Pkt, eEntityMetadata::WolfBegging, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Wolf.IsBegging());

			WriteEntityMetadata(a_Pkt, eEntityMetadata::WolfCollarColour, eEntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Wolf.GetCollarColor()));
			break;
		}  // case mtWolf

		case mtZombie:
		{
			// XXX Zombies were also split into new sublcasses; this doesn't handle that.

			auto & Zombie = static_cast<const cZombie &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::ZombieIsBaby, eEntityMetadataType::Boolean);
			a_Pkt.WriteBool(Zombie.IsBaby());
			break;
		}  // case mtZombie

		case mtZombiePigman:
		{
			auto & ZombiePigman = static_cast<const cZombiePigman &>(a_Mob);

			WriteEntityMetadata(a_Pkt, eEntityMetadata::AgeableIsBaby, eEntityMetadataType::Boolean);
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

void cProtocol_1_13_1::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	Super::SendBlockChange<&Palette_1_13_1::FromBlock>(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
}





void cProtocol_1_13_1::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	Super::SendBlockChanges<&Palette_1_13_1::FromBlock>(a_ChunkX, a_ChunkZ, a_Changes);
}





cProtocol::Version cProtocol_1_13_1::GetProtocolVersion()
{
	return Version::Version_1_13_1;
}





std::pair<short, short> cProtocol_1_13_1::GetItemFromProtocolID(UInt32 a_ProtocolID)
{
	return PaletteUpgrade::ToItem(Palette_1_13_1::ToItem(a_ProtocolID));
}





UInt32 cProtocol_1_13_1::GetProtocolIDFromItem(short a_ItemID, short a_ItemDamage)
{
	return Palette_1_13_1::FromItem(PaletteUpgrade::FromItem(a_ItemID, a_ItemDamage));
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_13_2:

cProtocol::Version cProtocol_1_13_2::GetProtocolVersion()
{
	return Version::Version_1_13_2;
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
	a_Pkt.WriteVarInt32(GetProtocolIDFromItem(a_Item.m_ItemType, a_Item.m_ItemDamage));
	a_Pkt.WriteBEInt8(a_Item.m_ItemCount);

	// TODO: NBT
	a_Pkt.WriteBEInt8(0);
}
