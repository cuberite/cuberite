
// Protocol_1_11.cpp

/*
Implements the 1.11 protocol classes:
	- cProtocol_1_11_0
		- release 1.11 protocol (#315)
	- cProtocol_1_11_1
		- release 1.11.1 protocol (#316)
*/

#include "Globals.h"
#include "Protocol_1_11.h"
#include "Packetizer.h"

#include "../WorldStorage/FastNBT.h"

#include "../Entities/Boat.h"
#include "../Entities/EnderCrystal.h"
#include "../Entities/ExpOrb.h"
#include "../Entities/Minecart.h"
#include "../Entities/FallingBlock.h"
#include "../Entities/Painting.h"
#include "../Entities/Pickup.h"
#include "../Entities/Player.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/FireworkEntity.h"
#include "../Entities/SplashPotionEntity.h"

#include "../Mobs/IncludeAllMonsters.h"

#include "../BlockEntities/BedEntity.h"
#include "../BlockEntities/MobSpawnerEntity.h"

#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../CompositeChat.h"
#include "../JsonUtils.h"
#include "../Bindings/PluginManager.h"





// The disabled error is intended, since the Metadata have overlapping indexes
// based on the type of the Entity.
//
// IMPORTANT: The enum is used to automate the sequential counting of the
// Metadata indexes. Adding a new enum value causes the following values to
// increase their index. Therefore the ordering of the enum values is VERY important!
#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wduplicate-enum"
#endif

namespace Metadata_1_11
{
	enum MetadataIndex
	{
		// Entity
		ENTITY_FLAGS,
		ENTITY_AIR,
		ENTITY_CUSTOM_NAME,
		ENTITY_CUSTOM_NAME_VISIBLE,
		ENTITY_SILENT,
		ENTITY_NO_GRAVITY,
		_ENTITY_NEXT,  // Used by descendants

		// Potion
		POTION_THROWN = _ENTITY_NEXT,

		// FallingBlock
		FALLING_BLOCK_POSITION = _ENTITY_NEXT,

		// AreaEffectCloud
		AREA_EFFECT_CLOUD_RADIUS = _ENTITY_NEXT,
		AREA_EFFECT_CLOUD_COLOR,
		AREA_EFFECT_CLOUD_SINGLE_POINT_EFFECT,
		AREA_EFFECT_CLOUD_PARTICLE_ID,
		AREA_EFFECT_CLOUD_PARTICLE_PARAMETER1,
		AREA_EFFECT_CLOUD_PARTICLE_PARAMETER2,

		// Arrow
		ARROW_CRITICAL = _ENTITY_NEXT,
		_ARROW_NEXT,

		// TippedArrow
		TIPPED_ARROW_COLOR = _ARROW_NEXT,

		// Boat
		BOAT_LAST_HIT_TIME = _ENTITY_NEXT,
		BOAT_FORWARD_DIRECTION,
		BOAT_DAMAGE_TAKEN,
		BOAT_TYPE,
		BOAT_RIGHT_PADDLE_TURNING,
		BOAT_LEFT_PADDLE_TURNING,

		// EnderCrystal
		ENDER_CRYSTAL_BEAM_TARGET = _ENTITY_NEXT,
		ENDER_CRYSTAL_SHOW_BOTTOM,

		// Fireball
		_FIREBALL_NEXT = _ENTITY_NEXT,

		// WitherSkull
		WITHER_SKULL_INVULNERABLE = _FIREBALL_NEXT,

		// Fireworks
		FIREWORK_INFO = _ENTITY_NEXT,
		FIREWORK_BOOSTED_ENTITY_ID,  // 1.11.1 only

		// Hanging
		_HANGING_NEXT = _ENTITY_NEXT,

		// ItemFrame
		ITEM_FRAME_ITEM = _HANGING_NEXT,
		ITEM_FRAME_ROTATION,

		// Item
		ITEM_ITEM = _ENTITY_NEXT,

		// Living
		LIVING_ACTIVE_HAND = _ENTITY_NEXT,
		LIVING_HEALTH,
		LIVING_POTION_EFFECT_COLOR,
		LIVING_POTION_EFFECT_AMBIENT,
		LIVING_NUMBER_OF_ARROWS,
		_LIVING_NEXT,

		// Player
		PLAYER_ADDITIONAL_HEARTHS = _LIVING_NEXT,
		PLAYER_SCORE,
		PLAYER_DISPLAYED_SKIN_PARTS,
		PLAYER_MAIN_HAND,

		// ArmorStand
		ARMOR_STAND_STATUS = _LIVING_NEXT,
		ARMOR_STAND_HEAD_ROTATION,
		ARMOR_STAND_BODY_ROTATION,
		ARMOR_STAND_LEFT_ARM_ROTATION,
		ARMOR_STAND_RIGHT_ARM_ROTATION,
		ARMOR_STAND_LEFT_LEG_ROTATION,
		ARMOR_STAND_RIGHT_LEG_ROTATION,

		// Insentient
		INSENTIENT_STATUS = _LIVING_NEXT,
		_INSENTIENT_NEXT,

		// Ambient
		_AMBIENT_NEXT = _INSENTIENT_NEXT,

		// Bat
		BAT_HANGING = _AMBIENT_NEXT,

		// Creature
		_CREATURE_NEXT = _INSENTIENT_NEXT,

		// Ageable
		AGEABLE_BABY = _CREATURE_NEXT,
		_AGEABLE_NEXT,

		// PolarBear
		POLAR_BEAR_STANDING = _AGEABLE_NEXT,

		// Animal
		_ANIMAL_NEXT = _AGEABLE_NEXT,

		// Abstract horse
		ABSTRACT_HORSE_STATUS = _ANIMAL_NEXT,
		ABSTRACT_HORSE_OWNER,
		_ABSTRACT_HORSE_NEXT,

		// Horse
		HORSE_VARIANT = _ABSTRACT_HORSE_NEXT,
		HORSE_ARMOR,

		// Chested horse
		CHESTED_HORSE_CHESTED = _ABSTRACT_HORSE_NEXT,
		_CHESTED_HORSE_NEXT,

		// Llama
		LLAMA_STRENGTH = _CHESTED_HORSE_NEXT,
		LLAMA_CARPET_COLOR,
		LLAMA_VARIANT,

		// Pig
		PIG_HAS_SADDLE = _ANIMAL_NEXT,
		PIG_TOTAL_CARROT_ON_A_STICK_BOOST,  // 1.11.1 only

		// Rabbit
		RABBIT_TYPE = _ANIMAL_NEXT,

		// Sheep
		SHEEP_STATUS = _ANIMAL_NEXT,

		// TameableAnimal
		TAMEABLE_ANIMAL_STATUS = _ANIMAL_NEXT,
		TAMEABLE_ANIMAL_OWNER,
		_TAMEABLE_NEXT,

		// Ocelot
		OCELOT_TYPE = _TAMEABLE_NEXT,

		// Wolf
		WOLF_DAMAGE_TAKEN = _TAMEABLE_NEXT,
		WOLF_BEGGING,
		WOLF_COLLAR_COLOR,

		// Villager
		VILLAGER_PROFESSION = _AGEABLE_NEXT,

		// Golem
		_GOLEM_NEXT = _CREATURE_NEXT,

		// IronGolem
		IRON_GOLEM_PLAYER_CREATED = _GOLEM_NEXT,

		// Shulker
		SHULKER_FACING_DIRECTION = _GOLEM_NEXT,
		SHULKER_ATTACHMENT_FALLING_BLOCK_POSITION,
		SHULKER_SHIELD_HEIGHT,

		// Monster
		_MONSTER_NEXT = _CREATURE_NEXT,

		// Blaze
		BLAZE_ON_FIRE = _MONSTER_NEXT,

		// Creeper
		CREEPER_STATE = _MONSTER_NEXT,
		CREEPER_POWERED,
		CREEPER_IGNITED,

		// Guardian
		GUARDIAN_STATUS = _MONSTER_NEXT,
		GUARDIAN_TARGET,

		// Abstract Skeleton
		ABSTRACT_SKELETON_ARMS_SWINGING = _MONSTER_NEXT,

		// Spider
		SPIDER_CLIMBING = _MONSTER_NEXT,

		// Witch
		WITCH_AGGRESIVE = _MONSTER_NEXT,

		// Wither
		WITHER_FIRST_HEAD_TARGET = _MONSTER_NEXT,
		WITHER_SECOND_HEAD_TARGET,
		WITHER_THIRD_HEAD_TARGET,
		WITHER_INVULNERABLE_TIMER,

		// Zombie
		ZOMBIE_IS_BABY = _MONSTER_NEXT,
		ZOMBIE_UNUSED,  // Was type
		ZOMBIE_HANDS_RISED_UP,
		_ZOMBIE_NEXT,

		// Zombie villager
		ZOMBIE_VILLAGER_CONVERTING = _ZOMBIE_NEXT,
		ZOMBIE_VILLAGER_PROFESSION,

		// Enderman
		ENDERMAN_CARRIED_BLOCK = _MONSTER_NEXT,
		ENDERMAN_SCREAMING,

		// Evocation illager
		EVOKER_SPELL = _MONSTER_NEXT,

		// Vex
		VEX_FLAGS = _MONSTER_NEXT,

		// Vindication illager
		VINDICATOR_FLAGS = _MONSTER_NEXT,

		// EnderDragon
		ENDER_DRAGON_DRAGON_PHASE = _INSENTIENT_NEXT,

		// Flying
		_FLYING_NEXT = _INSENTIENT_NEXT,

		// Ghast
		GHAST_ATTACKING = _FLYING_NEXT,

		// Slime
		SLIME_SIZE = _INSENTIENT_NEXT,

		// Minecart
		MINECART_SHAKING_POWER = _ENTITY_NEXT,
		MINECART_SHAKING_DIRECTION,
		MINECART_SHAKING_MULTIPLIER,
		MINECART_BLOCK_ID_META,
		MINECART_BLOCK_Y,
		MINECART_SHOW_BLOCK,
		_MINECART_NEXT,

		// MinecartCommandBlock
		MINECART_COMMAND_BLOCK_COMMAND = _MINECART_NEXT,
		MINECART_COMMAND_BLOCK_LAST_OUTPUT,

		// MinecartFurnace
		MINECART_FURNACE_POWERED = _MINECART_NEXT,

		// TNTPrimed
		TNT_PRIMED_FUSE_TIME = _ENTITY_NEXT,
	};
}

#ifdef __clang__
	#pragma clang diagnostic pop  // Restore ignored clang errors
#endif





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_11_0:

void cProtocol_1_11_0::SendCollectEntity(const cEntity & a_Collected, const cEntity & a_Collector, unsigned a_Count)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktCollectEntity);
	Pkt.WriteVarInt32(a_Collected.GetUniqueID());
	Pkt.WriteVarInt32(a_Collector.GetUniqueID());
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Count));
}





void cProtocol_1_11_0::SendEntityAnimation(const cEntity & a_Entity, const EntityAnimation a_Animation)
{
	switch (a_Animation)
	{
		case EntityAnimation::EggCracks:
		case EntityAnimation::SnowballPoofs:
		{
			// Vanilla stopped doing clientside prediction for thrown projectile particle effects (for some reason).
			// But they're still doing motion prediction, and latency exists, hence re-send the server position to avoid particle effects happening inside a block:
			SendEntityPosition(a_Entity);
			break;
		}
		case EntityAnimation::PawnChestEquipmentBreaks:
		case EntityAnimation::PawnFeetEquipmentBreaks:
		case EntityAnimation::PawnHeadEquipmentBreaks:
		case EntityAnimation::PawnLegsEquipmentBreaks:
		case EntityAnimation::PawnMainHandEquipmentBreaks:
		case EntityAnimation::PawnOffHandEquipmentBreaks:
		{
			const auto Position = a_Entity.GetPosition();

			// 1.11 dropped the automatic particle effect + sound on item break. Emulate at least some of it:
			SendSoundEffect("entity.item.break", Position, 1, 0.75f + ((a_Entity.GetUniqueID() * 23) % 32) / 64.f);
			break;
		}
		default: break;
	}

	Super::SendEntityAnimation(a_Entity, a_Animation);
}





void cProtocol_1_11_0::SendHideTitle(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktTitle);
	Pkt.WriteVarInt32(4);  // Hide title
}





void cProtocol_1_11_0::SendResetTitle(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktTitle);
	Pkt.WriteVarInt32(5);  // Reset title
}





void cProtocol_1_11_0::SendSpawnMob(const cMonster & a_Mob)
{
	ASSERT(m_State == 3);  // In game mode?
	return;
	/*
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
	Pkt.WriteVarInt32(MobType);
	Vector3d LastSentPos = a_Mob.GetLastSentPosition();
	Pkt.WriteBEDouble(LastSentPos.x);
	Pkt.WriteBEDouble(LastSentPos.y);
	Pkt.WriteBEDouble(LastSentPos.z);
	Pkt.WriteByteAngle(a_Mob.GetHeadYaw());  // Doesn't seem to be used
	Pkt.WriteByteAngle(a_Mob.GetPitch());
	Pkt.WriteByteAngle(a_Mob.GetYaw());
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedZ() * 400));
	WriteEntityMetadata(Pkt, a_Mob);
	Pkt.WriteBEUInt8(0xff);  // Metadata terminator
	*/
}





void cProtocol_1_11_0::SendTitleTimes(int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktTitle);
	Pkt.WriteVarInt32(3);  // Set title display times
	Pkt.WriteBEInt32(a_FadeInTicks);
	Pkt.WriteBEInt32(a_DisplayTicks);
	Pkt.WriteBEInt32(a_FadeOutTicks);
}





void cProtocol_1_11_0::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	ASSERT(m_State == 3);  // In game mode?

	Byte Action;
	switch (a_BlockEntity.GetBlockType())
	{
		case BlockType::EnchantingTable:   Action = 0;  break;  // The ones with a action of 0 is just a workaround to send the block entities to a client.
		case BlockType::EndPortal:         Action = 0;  break;  // Todo: 18.09.2020 - remove this when block entities are transmitted in the ChunkData packet - 12xx12

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
		case BlockType::ZombieWallHead:    Action = 4;  break;  // Update mobhead entity
		case BlockType::PottedAcaciaSapling:
		case BlockType::PottedAzureBluet:
		case BlockType::PottedBamboo:
		case BlockType::PottedBirchSapling:
		case BlockType::PottedBlueOrchid:
		case BlockType::PottedBrownMushroom:
		case BlockType::PottedCactus:
		case BlockType::PottedCornflower:
		case BlockType::PottedCrimsonRoots:
		case BlockType::PottedCrimsonFungus:
		case BlockType::PottedDandelion:
		case BlockType::PottedDarkOakSapling:
		case BlockType::PottedDeadBush:
		case BlockType::PottedFern:
		case BlockType::PottedJungleSapling:
		case BlockType::PottedLilyOfTheValley:
		case BlockType::PottedOakSapling:
		case BlockType::PottedOrangeTulip:
		case BlockType::PottedOxeyeDaisy:
		case BlockType::PottedPinkTulip:
		case BlockType::PottedPoppy:
		case BlockType::PottedRedMushroom:
		case BlockType::PottedRedTulip:
		case BlockType::PottedSpruceSapling:
		case BlockType::PottedWarpedFungus:
		case BlockType::PottedWarpedRoots:
		case BlockType::PottedWhiteTulip:
		case BlockType::PottedWitherRose:
		case BlockType::PottedAllium:      Action = 5;  break;  // Update flower pot
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
		case BlockType::YellowWallBanner:  Action = 6;  break;  // Update banner
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
		case BlockType::YellowBed:          Action = 11; break;  // Update bed color (new!)

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





signed char cProtocol_1_11_0::GetProtocolEntityStatus(const EntityAnimation a_Animation) const
{
	switch (a_Animation)
	{
		case EntityAnimation::EggCracks: return 3;
		case EntityAnimation::EvokerFangsAttacks: return 4;
		case EntityAnimation::IronGolemStashesGift: return 34;
		case EntityAnimation::PawnTotemActivates: return 35;
		case EntityAnimation::SnowballPoofs: return 3;
		default: return Super::GetProtocolEntityStatus(a_Animation);
	}
}





cProtocol::Version cProtocol_1_11_0::GetProtocolVersion() const
{
	return Version::v1_11_0;
}





void cProtocol_1_11_0::HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer)
{
	Vector3i Position;
	if (!a_ByteBuffer.ReadXYZPosition64(Position))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Face);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Hand);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, CursorX);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, CursorY);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, CursorZ);

	m_Client->HandleRightClick(Position, FaceIntToBlockFace(Face), {FloorC(CursorX * 16), FloorC(CursorY * 16), FloorC(CursorZ * 16)}, Hand == 0);
}





void cProtocol_1_11_0::WriteBlockEntity(cFastNBTWriter & a_Writer, const cBlockEntity & a_BlockEntity) const
{
	switch (a_BlockEntity.GetBlockType())
	{
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
		case BlockType::YellowBed:
		{
			auto & BedEntity = static_cast<const cBedEntity &>(a_BlockEntity);
			a_Writer.AddInt("color", BedEntity.GetColor());  // New: multicoloured beds
			break;
		}
		case BlockType::Spawner:
		{
			auto & MobSpawnerEntity = static_cast<const cMobSpawnerEntity &>(a_BlockEntity);
			a_Writer.BeginCompound("SpawnData");
				a_Writer.AddString("id", cMonster::MobTypeToVanillaNBT(MobSpawnerEntity.GetEntity()));  // New: uses namespaced ID
			a_Writer.EndCompound();
			a_Writer.AddShort("Delay", MobSpawnerEntity.GetSpawnDelay());
			break;
		}
		default: return Super::WriteBlockEntity(a_Writer, a_BlockEntity);
	}

	a_Writer.AddInt("x", a_BlockEntity.GetPosX());
	a_Writer.AddInt("y", a_BlockEntity.GetPosY());
	a_Writer.AddInt("z", a_BlockEntity.GetPosZ());
}





void cProtocol_1_11_0::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity, bool a_WriteCommon) const
{
	using namespace Metadata_1_11;

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
	a_Pkt.WriteBEUInt8(ENTITY_FLAGS);  // Index
	a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);  // Type
	a_Pkt.WriteBEInt8(Flags);
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_11_1:

cProtocol::Version cProtocol_1_11_1::GetProtocolVersion() const
{
	return Version::v1_11_1;
}
