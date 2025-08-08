
// Protocol_1_10.cpp

/*
Implements the 1.10 protocol classes:
	- cProtocol_1_10_0
		- release 1.10 protocol (#210), also used by 1.10.1 and 1.10.2
*/

#include "Globals.h"
#include "Protocol_1_10.h"
#include "Packetizer.h"

#include "json/json.h"

#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../JsonUtils.h"

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

#include "../BlockEntities/BannerEntity.h"
#include "../BlockEntities/BeaconEntity.h"
#include "../BlockEntities/CommandBlockEntity.h"
#include "../BlockEntities/MobHeadEntity.h"
#include "../BlockEntities/MobSpawnerEntity.h"
#include "../BlockEntities/FlowerPotEntity.h"

#include "../Bindings/PluginManager.h"

#include "Palettes/Upgrade.h"





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

namespace Metadata
{
	enum Metadata_Index
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

		// Horse
		HORSE_STATUS = _ANIMAL_NEXT,
		HORSE_TYPE,
		HORSE_VARIANT,
		HORSE_OWNER,
		HORSE_ARMOR,

		// Pig
		PIG_HAS_SADDLE = _ANIMAL_NEXT,

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

		// Skeleton
		SKELETON_TYPE = _MONSTER_NEXT,
		SKELETON_ARMS_SWINGING,

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
		ZOMBIE_TYPE,
		ZOMBIE_CONVERTING,
		ZOMBIE_HANDS_RISED_UP,

		// Enderman
		ENDERMAN_CARRIED_BLOCK = _MONSTER_NEXT,
		ENDERMAN_SCREAMING,

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
// cProtocol_1_10_0:

void cProtocol_1_10_0::SendSoundEffect(const AString & a_SoundName, Vector3d a_Origin, float a_Volume, float a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSoundEffect);
	Pkt.WriteString(a_SoundName);
	Pkt.WriteVarInt32(0);  // Master sound category (may want to be changed to a parameter later)
	Pkt.WriteBEInt32(FloorC(a_Origin.x * 8.0));
	Pkt.WriteBEInt32(FloorC(a_Origin.y * 8.0));
	Pkt.WriteBEInt32(FloorC(a_Origin.z * 8.0));
	Pkt.WriteBEFloat(a_Volume);
	Pkt.WriteBEFloat(a_Pitch);
}





cProtocol::Version cProtocol_1_10_0::GetProtocolVersion() const
{
	return Version::v1_10_0;
}





void cProtocol_1_10_0::HandlePacketResourcePackStatus(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Status);
	m_Client->HandleResourcePack(Status);
}





void cProtocol_1_10_0::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity, bool a_WriteCommon) const
{
	using namespace Metadata;

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
