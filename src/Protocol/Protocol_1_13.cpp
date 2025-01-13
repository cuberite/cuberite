
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

#include "../CompositeChat.h"
#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../JsonUtils.h"
#include "../WorldStorage/FastNBT.h"
#include "WorldStorage/NamespaceSerializer.h"

#include "../Bindings/PluginManager.h"
#include "Entities/FallingBlock.h"
#include "Entities/Floater.h"

#include "Palettes/Palette_1_13.h"
#include "Palettes/Palette_1_13_1.h"





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_13:

void cProtocol_1_13::SendBlockChange(Vector3i a_BlockPos, BlockState a_Block)
{
	cPacketizer Pkt(*this, pktBlockChange);
	Pkt.WriteXYZPosition64(a_BlockPos);
	Pkt.WriteVarInt32(GetProtocolBlockType(a_Block));
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
		Pkt.WriteVarInt32(GetProtocolBlockType(Change.m_Block));
	}
}





void cProtocol_1_13::SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY)
{
	{
		cPacketizer Pkt(*this, pktMapData);
		Pkt.WriteVarInt32(a_Map.GetID());
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Map.GetScale()));

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
		// TODO: Implement proper map scaling
		Pkt.WriteBEUInt8(128);
		Pkt.WriteBEUInt8(128);
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_DataStartX));
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_DataStartY));
		Pkt.WriteVarInt32(static_cast<UInt32>(a_Map.GetData().size()));
		for (auto itr = a_Map.GetData().cbegin(); itr != a_Map.GetData().cend(); ++itr)
		{
			Pkt.WriteBEUInt8(*itr);
		}
	}
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
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktScoreboardObjective);
	Pkt.WriteString(a_Name);
	Pkt.WriteBEUInt8(a_Mode);
	if ((a_Mode == 0) || (a_Mode == 2))
	{
		Pkt.WriteString(a_DisplayName);
		Pkt.WriteVarInt32(0);
	}
}





void cProtocol_1_13::SendCommandTree()
{
	// TODO: rework the whole command system to support new format
	// https://wiki.vg/Command_Data

#define NEW_COMMAND_TREE 1
#if NEW_COMMAND_TREE
	{
		cPacketizer Pkt(*this, pktCommnadTree);
		cRoot::Get()->GetPluginManager()->GetRootCommandNode()->WriteCommandTree(Pkt, *this);
	}
#else
	{
		AStringVector commands;
		class cCallback :
			public cPluginManager::cCommandEnumCallback
		{
		public:
			cCallback(void) {}

			virtual bool Command(const AString & a_Command, const cPlugin * a_Plugin, const AString & a_Permission, const AString & a_HelpString) override
			{
			UNUSED(a_Plugin);
			UNUSED(a_Permission);
			UNUSED(a_HelpString);
				m_Commands.push_back(a_Command.substr(1, std::string::npos));  // Commands are sent with out slashes
				return false;
			}

			AStringList m_Commands;
		} Callback;
		cRoot::Get()->GetPluginManager()->ForEachCommand(Callback);
		{
			cPacketizer Pkt(*this, pktCommnadTree);
			Pkt.WriteVarInt32(static_cast<UInt32>(Callback.m_Commands.size()) + 1);  // + 1 for the root node
			for (const AString & var : Callback.m_Commands)
			{
				Pkt.WriteVarInt32(1);  // Flags
				Pkt.WriteVarInt32(0);  // Size of Array of child nodes
				Pkt.WriteString(var);
			}
			// Root Node
			Pkt.WriteVarInt32(0);  // Flags
			Pkt.WriteVarInt32(static_cast<UInt32>(Callback.m_Commands.size()));  // Size of Array of child nodes. Every Command is a child
			for (size_t i = 0; i < Callback.m_Commands.size(); i++)
			{
				Pkt.WriteVarInt32(static_cast<UInt32>(i));  // Indexes of children in the array
			}

			Pkt.WriteVarInt32(static_cast<UInt32>(Callback.m_Commands.size()));  // root index
		}
	}
#endif
}





void cProtocol_1_13::HandlePacketTabComplete(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt32,      UInt32,     CompletionId);
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString,    PartialCommand);

	m_Client->HandleTabCompletion(PartialCommand, CompletionId);
}





void cProtocol_1_13::SendStatistics(const StatisticsManager & a_Manager)
{
	ASSERT(m_State == 3);  // In game mode?

	UInt32 Size = 0;

	for (const auto & [Statistic, Value] : a_Manager.Custom)
	{
		// Client balks at out-of-range values so there is no good default value.
		// We're forced to not send the statistics this protocol version doesn't support.

		if (GetProtocolStatisticType(Statistic) != static_cast<UInt32>(-1))
		{
			Size++;
		}
	}

	// No need to check Size != 0
	// Assume that the vast majority of the time there's at least one statistic to send

	cPacketizer Pkt(*this, pktStatistics);
	Pkt.WriteVarInt32(Size);

	for (const auto & [Statistic, Value] : a_Manager.Custom)
	{
		const auto ID = GetProtocolStatisticType(Statistic);
		if (ID == static_cast<UInt32>(-1))
		{
			// Unsupported, don't send:
			continue;
		}

		Pkt.WriteVarInt32(8);  // "Custom" category.
		Pkt.WriteVarInt32(ID);
		Pkt.WriteVarInt32(static_cast<UInt32>(Value));
	}
}





void cProtocol_1_13::SendSpawnEntity(const cEntity & a_Entity)
{
	Int32 EntityData = /* Default: velocity present flag */ 1;
	const auto EntityType = GetProtocolEntityType(a_Entity);

	if (a_Entity.IsMinecart())
	{
		const auto & Cart = static_cast<const cMinecart &>(a_Entity);
		EntityData = static_cast<Int32>(Cart.GetPayload());
	}
	else if (a_Entity.IsItemFrame())
	{
		const auto & Frame = static_cast<const cItemFrame &>(a_Entity);
		EntityData = static_cast<Int32>(Frame.GetProtocolFacing());
	}
	else if (a_Entity.IsFallingBlock())
	{
		const auto & Block = static_cast<const cFallingBlock &>(a_Entity);
		auto NumericBlock = GetProtocolBlockType(Block.GetBlock());
		EntityData = static_cast<Int32>(NumericBlock);
	}
	else if (a_Entity.IsFloater())
	{
		const auto & Floater = static_cast<const cFloater &>(a_Entity);
		EntityData = static_cast<Int32>(Floater.GetOwnerID());
	}
	else if (a_Entity.IsProjectile())
	{
		using PType = cProjectileEntity::eKind;
		const auto & Projectile = static_cast<const cProjectileEntity &>(a_Entity);

		if (Projectile.GetProjectileKind() == PType::pkArrow)
		{
			const auto & Arrow = static_cast<const cArrowEntity &>(Projectile);
			EntityData = static_cast<Int32>(Arrow.GetCreatorUniqueID() + 1);
		}
	}

	SendEntitySpawn(a_Entity, EntityType, EntityData);
}





void cProtocol_1_13::SendTabCompletionResults(const AStringVector & a_Results, UInt32 CompletionId)
{
	{
		//  TODO: Implement Start and Length
		cPacketizer Pkt(*this, pktTabCompletionResults);
		Pkt.WriteVarInt32(CompletionId);
		Pkt.WriteVarInt32(0);  //  Start
		Pkt.WriteVarInt32(0);  //  Length
		Pkt.WriteVarInt32(static_cast<UInt32>(a_Results.size()));
		for (const AString & Match : a_Results)
		{
			Pkt.WriteString(Match);
			Pkt.WriteBool(false);  // Has Tooltip
		}
	}
}





void cProtocol_1_13::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	ASSERT(m_State == 3);  // In game mode?

	Byte Action;
	switch (a_BlockEntity.GetBlockType())
	{
		case BlockType::EnchantingTable:   Action = 0; break;  // The ones with a action of 0 is just a workaround to send the block entities to a client.
		case BlockType::EndPortal:         Action = 0; break;  // Todo: 18.09.2020 - remove this when block entities are transmitted in the ChunkData packet - 12xx12
		case BlockType::Spawner:           Action = 1;  break;  // Update mob spawner spinny mob thing
		case BlockType::CommandBlock:
		case BlockType::ChainCommandBlock:
		case BlockType::RepeatingCommandBlock: Action = 2;  break;  // Update command block text
		case BlockType::Beacon:            Action = 3;  break;  // Update beacon entity
		case BlockType::CreeperHead:
		case BlockType::CreeperWallHead:
		case BlockType::DragonHead:
		case BlockType::DragonWallHead:
		case BlockType::PlayerHead:
		case BlockType::PlayerWallHead:
		case BlockType::SkeletonSkull:
		case BlockType::SkeletonWallSkull:
		case BlockType::WitherSkeletonSkull:
		case BlockType::WitherSkeletonWallSkull:
		case BlockType::ZombieHead:
		case BlockType::ZombieWallHead:    Action = 4;  break;  // Update Mobhead entity
		// case BlockType::CONDUIT:        Action = 5;  break;  // Update Conduit entity
		case BlockType::BlackBanner:
		case BlockType::BlueBanner:
		case BlockType::BrownBanner:
		case BlockType::CyanBanner:
		case BlockType::GrayBanner:
		case BlockType::GreenBanner:
		case BlockType::LightBlueBanner:
		case BlockType::LightGrayBanner:
		case BlockType::LimeBanner:
		case BlockType::MagentaBanner:
		case BlockType::OrangeBanner:
		case BlockType::PinkBanner:
		case BlockType::PurpleBanner:
		case BlockType::RedBanner:
		case BlockType::WhiteBanner:
		case BlockType::YellowBanner:

		case BlockType::BlackWallBanner:
		case BlockType::BlueWallBanner:
		case BlockType::BrownWallBanner:
		case BlockType::CyanWallBanner:
		case BlockType::GrayWallBanner:
		case BlockType::GreenWallBanner:
		case BlockType::LightBlueWallBanner:
		case BlockType::LightGrayWallBanner:
		case BlockType::LimeWallBanner:
		case BlockType::MagentaWallBanner:
		case BlockType::OrangeWallBanner:
		case BlockType::PinkWallBanner:
		case BlockType::PurpleWallBanner:
		case BlockType::RedWallBanner:
		case BlockType::WhiteWallBanner:
		case BlockType::YellowWallBanner:  Action = 6;  break;  // Update banner entity
		// case Structure Block:           Action = 7;  break;  // Update Structure tile entity
		case BlockType::EndGateway:        Action = 8;  break;  // Update destination for a end gateway entity
		case BlockType::AcaciaSign:
		case BlockType::BirchSign:
		case BlockType::CrimsonSign:
		case BlockType::DarkOakSign:
		case BlockType::JungleSign:
		case BlockType::OakSign:
		case BlockType::SpruceSign:
		case BlockType::WarpedSign:        Action = 9;  break;  // Update sign entity
		// case BlockType::SHULKER_BOX:    Action = 10; break;  // sets shulker box - not used just here if anyone is confused from reading the protocol wiki
		case BlockType::BlackBed:
		case BlockType::BlueBed:
		case BlockType::BrownBed:
		case BlockType::CyanBed:
		case BlockType::GrayBed:
		case BlockType::GreenBed:
		case BlockType::LightBlueBed:
		case BlockType::LightGrayBed:
		case BlockType::LimeBed:
		case BlockType::MagentaBed:
		case BlockType::OrangeBed:
		case BlockType::PinkBed:
		case BlockType::PurpleBed:
		case BlockType::RedBed:
		case BlockType::WhiteBed:
		case BlockType::YellowBed:          Action = 11; break;  // Update bed color

		default: return;  // Block entities change between versions
	}

	cPacketizer Pkt(*this, pktUpdateBlockEntity);
	Pkt.WriteXYZPosition64(a_BlockEntity.GetPosX(), a_BlockEntity.GetPosY(), a_BlockEntity.GetPosZ());
	Pkt.WriteBEUInt8(Action);

	cFastNBTWriter Writer;
	WriteBlockEntity(Writer, a_BlockEntity);
	Writer.Finish();
	Pkt.WriteBuf(Writer.GetResult());
}





UInt8 cProtocol_1_13::GetEntityMetadataID(EntityMetadata a_Metadata) const
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

		case EntityMetadata::EntityPose:
		case EntityMetadata::AreaEffectCloudParticleParameter1:
		case EntityMetadata::AreaEffectCloudParticleParameter2:
		case EntityMetadata::ZombieUnusedWasType: break;

		default:
			break;
	}
	UNREACHABLE("Retrieved invalid metadata for protocol");
}





UInt8 cProtocol_1_13::GetEntityMetadataID(EntityMetadataType a_FieldType) const
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





Item cProtocol_1_13::GetItemFromProtocolID(UInt32 a_ProtocolID) const
{
	return Palette_1_13::ToItem(a_ProtocolID);
}





UInt32 cProtocol_1_13::GetPacketID(ePacketType a_PacketType) const
{
	switch (a_PacketType)
	{
		case pktAttachEntity:           return 0x46;
		case pktBlockChanges:           return 0x0f;
		case pktCameraSetTo:            return 0x3c;
		case pktChatRaw:                return 0x0e;
		case pktCollectEntity:          return 0x4f;
		case pktCommnadTree:            return 0x11;
		case pktDestroyEntity:          return 0x35;
		case pktDisconnectDuringGame:   return 0x1b;
		case pktEditSign:               return 0x2c;
		case pktEntityEffect:           return 0x53;
		case pktEntityEquipment:        return 0x42;
		case pktEntityHeadLook:         return 0x39;
		case pktEntityLook:             return 0x2a;
		case pktEntityMeta:             return 0x3f;
		case pktEntityProperties:       return 0x52;
		case pktEntityRelMove:          return 0x28;
		case pktEntityRelMoveLook:      return 0x29;
		case pktEntityStatus:           return 0x1c;
		case pktEntityVelocity:         return 0x41;
		case pktExperience:             return 0x43;
		case pktExplosion:              return 0x1e;
		case pktGameMode:               return 0x20;
		case pktHeldItemChange:         return 0x3d;
		case pktInventorySlot:          return 0x17;
		case pktJoinGame:               return 0x25;
		case pktKeepAlive:              return 0x21;
		case pktLeashEntity:            return 0x40;
		case pktMapData:                return 0x26;
		case pktParticleEffect:         return 0x24;
		case pktPlayerAbilities:        return 0x2e;
		case pktPlayerList:             return 0x30;
		case pktPlayerListHeaderFooter: return 0x4e;
		case pktPlayerMoveLook:         return 0x32;
		case pktPluginMessage:          return 0x19;
		case pktRemoveEntityEffect:     return 0x36;
		case pktRespawn:                return 0x38;
		case pktScoreboardObjective:    return 0x45;
		case pktSoundEffect:            return 0x1a;
		case pktSoundParticleEffect:    return 0x23;
		case pktSpawnPosition:          return 0x49;
		case pktTabCompletionResults:   return 0x10;
		case pktTeleportEntity:         return 0x50;
		case pktTimeUpdate:             return 0x4a;
		case pktTitle:                  return 0x4b;
		case pktUnloadChunk:            return 0x1f;
		case pktUnlockRecipe:           return 0x32;
		case pktUpdateHealth:           return 0x44;
		case pktUpdateScore:            return 0x48;
		case pktUpdateSign:             return GetPacketID(pktUpdateBlockEntity);
		case pktUseBed:                 return 0x33;
		case pktWeather:                return 0x20;
		case pktWindowClose:            return 0x13;
		case pktWindowItems:            return 0x15;
		case pktWindowOpen:             return 0x14;
		case pktWindowProperty:         return 0x16;
		default: return Super::GetPacketID(a_PacketType);
	}
}





UInt32 cProtocol_1_13::GetProtocolBlockType(BlockState a_Block) const
{
	return Palette_1_13::From(a_Block);
}





signed char cProtocol_1_13::GetProtocolEntityStatus(const EntityAnimation a_Animation) const
{
	switch (a_Animation)
	{
		case EntityAnimation::DolphinShowsHappiness: return 38;
		default: return Super::GetProtocolEntityStatus(a_Animation);
	}
}





UInt32 cProtocol_1_13::GetProtocolItemType(Item a_ItemID) const
{
	return Palette_1_13::From(a_ItemID);
}





UInt32 cProtocol_1_13::GetProtocolMobType(eMonsterType a_MobType) const
{
	switch (a_MobType)
	{
		// Map invalid type to Giant for easy debugging (if this ever spawns, something has gone very wrong)
		case mtInvalidType:           return 27;
		case mtBat:                   return 3;
		case mtCat:                   return 48;
		case mtBlaze:                 return 4;
		case mtCaveSpider:            return 6;
		case mtChicken:               return 7;
		case mtCod:                   return 8;
		case mtCow:                   return 9;
		case mtCreeper:               return 10;
		case mtDonkey:                return 11;
		case mtDolphin:               return 12;
		case mtDrowned:               return 14;
		case mtElderGuardian:         return 15;
		case mtEnderDragon:           return 17;
		case mtEnderman:              return 18;
		case mtEndermite:             return 19;
		case mtEvoker:                return 21;
		case mtGhast:                 return 26;
		case mtGiant:                 return 27;
		case mtGuardian:              return 28;
		case mtHorse:                 return 29;
		case mtHusk:                  return 30;
		case mtIllusioner:            return 31;
		case mtIronGolem:             return 80;
		case mtLlama:                 return 36;
		case mtMagmaCube:             return 38;
		case mtMule:                  return 46;
		case mtMooshroom:             return 47;
		case mtOcelot:                return 48;
		case mtParrot:                return 50;
		case mtPhantom:               return 90;
		case mtPig:                   return 51;
		case mtPufferfish:            return 52;
		case mtPolarBear:             return 54;
		case mtRabbit:                return 56;
		case mtSalmon:                return 57;
		case mtSheep:                 return 58;
		case mtShulker:               return 59;
		case mtSilverfish:            return 61;
		case mtSkeleton:              return 62;
		case mtSkeletonHorse:         return 63;
		case mtSlime:                 return 64;
		case mtSnowGolem:             return 66;
		case mtSpider:                return 69;
		case mtSquid:                 return 70;
		case mtStray:                 return 71;
		case mtTropicalFish:          return 72;
		case mtTurtle:                return 73;
		case mtVex:                   return 78;
		case mtVillager:              return 79;
		case mtVindicator:            return 81;
		case mtWitch:                 return 82;
		case mtWither:                return 83;
		case mtWitherSkeleton:        return 84;
		case mtWolf:                  return 86;
		case mtZombie:                return 87;
		case mtZombiePigman:          return 53;
		case mtZombieHorse:           return 88;
		case mtZombieVillager:        return 89;
		default:                      return 0;
	}
}





UInt32 cProtocol_1_13::GetProtocolStatisticType(const CustomStatistic a_Statistic) const
{
	return Palette_1_13::From(a_Statistic);
}





cProtocol::Version cProtocol_1_13::GetProtocolVersion() const
{
	return Version::v1_13;
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
		case 0x1c: HandlePacketNameItem(a_ByteBuffer); return true;
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





void cProtocol_1_13::HandlePacketNameItem(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, NewItemName);

	LOGD("New item name : %s", NewItemName);
}





void cProtocol_1_13::HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer)
{
	ContiguousByteBuffer Data;

	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, NamespacedChannel);

	const auto & [Namespace, Channel] = NamespaceSerializer::SplitNamespacedID(NamespacedChannel);

	// If the plugin channel is recognized vanilla, handle it directly:
	if (Namespace == NamespaceSerializer::Namespace::Minecraft)
	{
		HandleVanillaPluginMessage(a_ByteBuffer, Channel);
		return;
	}

	// Read the plugin message and relay to clienthandle:
	a_ByteBuffer.ReadSome(Data, a_ByteBuffer.GetReadableSpace());
	m_Client->HandlePluginMessage(NamespacedChannel, Data);
}





void cProtocol_1_13::HandlePacketSetBeaconEffect(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, Effect1);
	HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, Effect2);
	m_Client->HandleBeaconSelection(Effect1, Effect2);
}





void cProtocol_1_13::HandleVanillaPluginMessage(cByteBuffer & a_ByteBuffer, const std::string_view a_Channel)
{
	if (a_Channel == "brand")
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Brand);

		m_Client->SetClientBrand(Brand);
		m_Client->SendPluginMessage("brand", "\x08""Cuberite");  // Send back our brand, including the length.
	}
	else if (a_Channel == "register")
	{
		ContiguousByteBuffer Data;

		a_ByteBuffer.ReadSome(Data, a_ByteBuffer.GetReadableSpace());

		AString tempstring = std::string{a_Channel};
		if (m_Client->HasPluginChannel(tempstring))
		{
			m_Client->SendPluginMessage("unregister", tempstring);
			return;  // Can't register again if already taken - kinda defeats the point of plugin messaging!
		}

		m_Client->RegisterPluginChannels(m_Client->BreakApartPluginChannels(Data));
	}
	else if (a_Channel == "unregister")
	{
		ContiguousByteBuffer Data;

		// Read the plugin message and relay to clienthandle:
		a_ByteBuffer.ReadSome(Data, a_ByteBuffer.GetReadableSpace());
		m_Client->UnregisterPluginChannels(m_Client->BreakApartPluginChannels(Data));
	}
}





bool cProtocol_1_13::ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes) const
{
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt16, Int16, ItemID);
	if (ItemID == -1)
	{
		// The item is empty, no more data follows
		a_Item.Empty();
		return true;
	}

	a_Item.m_ItemType = GetItemFromProtocolID(ToUnsigned(ItemID));

	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt8, Int8, ItemCount);
	a_Item.m_ItemCount = ItemCount;
	if (ItemCount <= 0)
	{
		a_Item.Empty();
	}

	ContiguousByteBuffer Metadata;
	if (!a_ByteBuffer.ReadSome(Metadata, a_ByteBuffer.GetReadableSpace() - a_KeepRemainingBytes) || Metadata.empty() || (Metadata[0] == std::byte(0)))
	{
		// No metadata
		return true;
	}

	ParseItemMetadata(a_Item, Metadata);
	return true;
}





void cProtocol_1_13::WriteEntityMetadata(cPacketizer & a_Pkt, const EntityMetadata a_Metadata, const EntityMetadataType a_FieldType) const
{
	a_Pkt.WriteBEUInt8(GetEntityMetadataID(a_Metadata));  // Index
	a_Pkt.WriteBEUInt8(GetEntityMetadataID(a_FieldType));  // Type
}





void cProtocol_1_13::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const
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
					int Content = PaletteUpgrade::ToItem(MinecartContent.m_ItemType).first;
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
			const auto & Frame = static_cast<const cItemFrame &>(a_Entity);
			WriteEntityMetadata(a_Pkt, EntityMetadata::ItemFrameItem, EntityMetadataType::Item);
			WriteItem(a_Pkt, Frame.GetItem());
			WriteEntityMetadata(a_Pkt, EntityMetadata::ItemFrameRotation, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(Frame.GetItemRotation());
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





void cProtocol_1_13::WriteItem(cPacketizer & a_Pkt, const cItem & a_Item) const
{
	if (a_Item.IsEmpty())
	{
		a_Pkt.WriteBEInt16(-1);
		return;
	}

	// Normal item
	a_Pkt.WriteBEInt16(static_cast<Int16>(GetProtocolItemType(a_Item.m_ItemType)));
	a_Pkt.WriteBEInt8(a_Item.m_ItemCount);

	// TODO: NBT
	a_Pkt.WriteBEInt8(0);
}





void cProtocol_1_13::WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) const
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
			auto NumericBlock = PaletteUpgrade::ToBlock(Enderman.GetCarriedBlock());
			UInt32 Carried = 0;
			Carried |= static_cast<UInt32>(NumericBlock.first << 4);
			Carried |= static_cast<UInt32>(NumericBlock.second);
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

		case mtSkeleton:
		{
			auto & Skeleton = static_cast<const cSkeleton &>(a_Mob);
			WriteEntityMetadata(a_Pkt, EntityMetadata::LivingActiveHand, EntityMetadataType::Byte);
			a_Pkt.WriteBEUInt8(Skeleton.IsChargingBow() ? 0x01 : 0x00);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AbstractSkeletonArmsSwinging, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Skeleton.IsChargingBow());
			break;
		}  // case mtSkeleton

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
		case mtIronGolem:
		case mtSnowGolem:
		case mtSpider:
		case mtZombieVillager:

		case mtElderGuardian:
		case mtGuardian:
		{
			// TODO: Mobs with extra fields that aren't implemented
			break;
		}

		case mtCat:

		case mtCod:

		case mtDolphin:

		case mtDonkey:

		case mtDrowned:

		case mtEvoker:

		case mtIllusioner:

		case mtLlama:

		case mtMule:

		case mtParrot:

		case mtPhantom:

		case mtPolarBear:

		case mtPufferfish:

		case mtSalmon:

		case mtShulker:

		case mtStray:

		case mtSkeletonHorse:
		case mtZombieHorse:

		case mtTropicalFish:

		case mtTurtle:

		case mtVex:

		case mtVindicator:

		case mtHusk:
		{
			// Todo: Mobs not added yet. Grouped ones have the same metadata
			ASSERT(!"cProtocol_1_13::WriteMobMetadata: received unimplemented type");
			break;
		}

		case mtMooshroom:
		case mtCaveSpider:
		{
			// Not mentioned on http://wiki.vg/Entities
			break;
		}

		case mtEndermite:
		case mtGiant:
		case mtSilverfish:
		case mtSquid:
		case mtWitherSkeleton:
		{
			// Mobs with no extra fields
			break;
		}

		default: UNREACHABLE("cProtocol_1_13::WriteMobMetadata: received mob of invalid type");
	}  // switch (a_Mob.GetType())
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_13_1:

void cProtocol_1_13_1::SendBossBarAdd(UInt32 a_UniqueID, const cCompositeChat & a_Title, float a_FractionFilled, BossBarColor a_Color, BossBarDivisionType a_DivisionType, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBossBar);
	// TODO: Bad way to write a UUID, and it's not a true UUID, but this is functional for now.
	Pkt.WriteBEUInt64(0);
	Pkt.WriteBEUInt64(a_UniqueID);
	Pkt.WriteVarInt32(0);  // Add
	Pkt.WriteString(a_Title.CreateJsonString());
	Pkt.WriteBEFloat(a_FractionFilled);
	Pkt.WriteVarInt32([a_Color]
	{
		switch (a_Color)
		{
			case BossBarColor::Pink: return 0U;
			case BossBarColor::Blue: return 1U;
			case BossBarColor::Red: return 2U;
			case BossBarColor::Green: return 3U;
			case BossBarColor::Yellow: return 4U;
			case BossBarColor::Purple: return 5U;
			case BossBarColor::White: return 6U;
		}
		UNREACHABLE("Unsupported boss bar property");
	}());
	Pkt.WriteVarInt32([a_DivisionType]
	{
		switch (a_DivisionType)
		{
			case BossBarDivisionType::None: return 0U;
			case BossBarDivisionType::SixNotches: return 1U;
			case BossBarDivisionType::TenNotches: return 2U;
			case BossBarDivisionType::TwelveNotches: return 3U;
			case BossBarDivisionType::TwentyNotches: return 4U;
		}
		UNREACHABLE("Unsupported boss bar property");
	}());
	{
		UInt8 Flags = 0x00;
		if (a_DarkenSky)
		{
			Flags |= 0x01;
		}
		if (a_PlayEndMusic)
		{
			Flags |= 0x02;
		}
		if (a_CreateFog)
		{
			Flags |= 0x04;  // Only difference to 1.9 is fog now a separate flag
		}
		Pkt.WriteBEUInt8(Flags);
	}
}





void cProtocol_1_13_1::SendBossBarUpdateFlags(UInt32 a_UniqueID, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBossBar);
	// TODO: Bad way to write a UUID, and it's not a true UUID, but this is functional for now.
	Pkt.WriteBEUInt64(0);
	Pkt.WriteBEUInt64(a_UniqueID);
	Pkt.WriteVarInt32(5);  // Update Flags
	{
		UInt8 Flags = 0x00;
		if (a_DarkenSky)
		{
			Flags |= 0x01;
		}
		if (a_PlayEndMusic)
		{
			Flags |= 0x02;
		}
		if (a_CreateFog)
		{
			Flags |= 0x04;  // Only difference to 1.9 is fog now a separate flag
		}
		Pkt.WriteBEUInt8(Flags);
	}
}





Item cProtocol_1_13_1::GetItemFromProtocolID(UInt32 a_ProtocolID) const
{
	return Palette_1_13_1::ToItem(a_ProtocolID);
}





UInt32 cProtocol_1_13_1::GetProtocolBlockType(BlockState a_Block) const
{
	return Palette_1_13_1::From(a_Block);
}





UInt32 cProtocol_1_13_1::GetProtocolItemType(Item a_ItemID) const
{
	return Palette_1_13_1::From(a_ItemID);
}





UInt32 cProtocol_1_13_1::GetProtocolStatisticType(const CustomStatistic a_Statistic) const
{
	return Palette_1_13_1::From(a_Statistic);
}





cProtocol::Version cProtocol_1_13_1::GetProtocolVersion() const
{
	return Version::v1_13_1;
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_13_2:

cProtocol::Version cProtocol_1_13_2::GetProtocolVersion() const
{
	return Version::v1_13_2;
}





bool cProtocol_1_13_2::ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes) const
{
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBool, bool, Present);
	if (!Present)
	{
		// The item is empty, no more data follows
		a_Item.Empty();
		return true;
	}

	HANDLE_PACKET_READ(a_ByteBuffer, ReadVarInt32, UInt32, ItemID);
	a_Item.m_ItemType = GetItemFromProtocolID(ItemID);

	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt8, Int8, ItemCount);
	a_Item.m_ItemCount = ItemCount;
	if (ItemCount <= 0)
	{
		a_Item.Empty();
	}

	ContiguousByteBuffer Metadata;
	if (!a_ByteBuffer.ReadSome(Metadata, a_ByteBuffer.GetReadableSpace() - a_KeepRemainingBytes) || Metadata.empty() || (Metadata[0] == std::byte(0)))
	{
		// No metadata
		return true;
	}

	ParseItemMetadata(a_Item, Metadata);
	return true;
}





void cProtocol_1_13_2::WriteItem(cPacketizer & a_Pkt, const cItem & a_Item) const
{

	if (a_Item.IsEmpty())
	{
		a_Pkt.WriteBool(false);
		return;
	}

	// Item present
	a_Pkt.WriteBool(true);

	// Normal item
	a_Pkt.WriteVarInt32(GetProtocolItemType(a_Item.m_ItemType));
	a_Pkt.WriteBEInt8(a_Item.m_ItemCount);

	cFastNBTWriter Writer;
	if (a_Item.m_ItemType == Item::Potion)
	{
		AString potionname;
		AString finalname = "minecraft:";
		int potion_dmg = a_Item.m_ItemDamage & 0x1F;
		switch (potion_dmg)
		{
			case 0: potionname = "water"; break;
			case 1: potionname = "regeneration"; break;
			case 2: potionname = "swiftness"; break;
			case 3: potionname = "fire_resistance"; break;
			case 4: potionname = "poison"; break;
			case 5: potionname = "healing"; break;
			case 6: potionname = "night_vision"; break;
			case 8: potionname = "weakness"; break;
			case 9: potionname = "strength"; break;
			case 10: potionname = "slowness"; break;
			case 11: potionname = "leaping"; break;
			case 12: potionname = "harming"; break;
			case 13: potionname = "water_breathing"; break;
			case 14: potionname = "invisibility"; break;
			case 15: potionname = "slow_falling"; break;
			case 16: potionname = "awkward"; break;
			case 17: potionname = "turtle_master"; break;
			case 32: potionname = "thick"; break;
			case 64: potionname = "mundane"; break;
		}
		if (((a_Item.m_ItemDamage & 32) == 32) && (potionname != ""))
		{
			potionname = "thick";
		}
		if (((a_Item.m_ItemDamage & 64) == 64) && (potionname != ""))
		{
			potionname = "mundane";
		}
		if ((cEntityEffect::GetPotionEffectIntensity(a_Item.m_ItemDamage) == 1) && (potionname != "thick"))
		{
			finalname += "strong_";
		}
		if (((a_Item.m_ItemDamage & 0x40) == 0x40) && (potionname != "mundane"))
		{
			finalname += "long_";
		}
		finalname += potionname;
		Writer.AddString("Potion", finalname);
	}
	if (a_Item.m_RepairCost != 0)
	{
		Writer.AddInt("RepairCost", a_Item.m_RepairCost);
	}
	if (!a_Item.m_Enchantments.IsEmpty())
	{
		const char * TagName = (a_Item.m_ItemType == Item::EnchantedBook) ? "StoredEnchantments" : "Enchantments";
		EnchantmentSerializer::WriteToNBTCompound(a_Item.m_Enchantments, Writer, TagName, true);
	}
	if ((a_Item.m_ItemType == Item::FireworkRocket) || (a_Item.m_ItemType == Item::FireworkStar))
	{
		cFireworkItem::WriteToNBTCompound(a_Item.m_FireworkItem, Writer, a_Item.m_ItemType);
	}

	if (!a_Item.IsBothNameAndLoreEmpty() || a_Item.m_ItemColor.IsValid())
	{
		Writer.BeginCompound("display");
		if (a_Item.m_ItemColor.IsValid())
		{
			Writer.AddInt("color", static_cast<Int32>(a_Item.m_ItemColor.m_Color));
		}

		if (!a_Item.IsCustomNameEmpty())
		{
			Writer.AddString("Name", a_Item.m_CustomName);
		}
		if (!a_Item.IsLoreEmpty())
		{
			Writer.BeginList("Lore", TAG_String);

			for (const auto & Line : a_Item.m_LoreTable)
			{
				Writer.AddString("", Line);
			}

			Writer.EndList();
		}
		Writer.EndCompound();
	}

	Writer.Finish();

	a_Pkt.WriteBuf(Writer.GetResult());
	// TODO: NBT
}
